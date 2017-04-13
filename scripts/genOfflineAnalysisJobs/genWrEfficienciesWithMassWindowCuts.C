#include <TFile.h>
#include <TList.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TProfile.h>
#include <TROOT.h>
#include <TBranch.h>
#include <TTree.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TObjArray.h>
#include "TCollection.h"
#include <TCut.h>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <map>
#include <utility>

//#define DEBUG

using namespace std;

//given the WR mass, return a string in the form
//  brName > XX && brName < YY
//where XX is the lower bound cut, brName is the branch to which the cut is applied, and YY is the upper bound cut
//channel can be EE or MuMu
string getMassWindowCuts(string branchNameForCut, string desiredWrMass, string desiredChannel, string inputMassCutsFilePath){
	string cutStringToReturn = "";
	ifstream massWindowReader(inputMassCutsFilePath.c_str());
	int mass = std::stoi(desiredWrMass);
	bool wrMassIsNotEven = ((mass % 200) != 0);
	int increment = 100;
	float incrementFloat = 100;
	
	//loop over the mass cuts file
	while(massWindowReader.peek() != EOF && massWindowReader.good() ){
		if(massWindowReader.peek() == 35){
			massWindowReader.ignore(1000, '\n');
			continue;
		}//end discard of lines which start with a number sign  ???

		//declare strings, and read a line from the file and write the line contents to the strings
		//the file should be formatted such that the channel is first, then the wr mass, then the lower bound mass, and finally the upper bound mass
		string channelFromFile, wrMassForMassWindow, massWindowLowerBound, massWindowUpperBound;
		massWindowReader >> channelFromFile >> wrMassForMassWindow >> massWindowLowerBound >> massWindowUpperBound;
		if(wrMassForMassWindow == "0") continue;	//skip any lines in which the wrMass is zero
		if(!wrMassIsNotEven){
			//currently the WR mass points are generated in increments of 100 GeV, so if the WR mass is even then it is an integer multiple of 200
			//once a match is found between desiredChannel and channelFromFile, and desiredWrMass and wrMassForMassWindow then the cut
			//string can be generated and returned
			if(desiredChannel == channelFromFile && desiredWrMass == wrMassForMassWindow){
				cutStringToReturn = branchNameForCut + " > " + massWindowLowerBound + " && " + branchNameForCut + " < " + massWindowUpperBound;
				return cutStringToReturn;
			}

		}//end if desired wr mass is an integer multiple of 200

		if(wrMassIsNotEven){
			//make another file reader and a second set of four strings to read another line from the mass cuts file
			string channelFromFileTwo, wrMassForMassWindowTwo, massWindowLowerBoundTwo, massWindowUpperBoundTwo;
			ifstream massWindowReaderTwo(inputMassCutsFilePath.c_str());
			while(massWindowReaderTwo.peek() != EOF && massWindowReaderTwo.good() ){
				if(massWindowReaderTwo.peek() == 35){
					massWindowReaderTwo.ignore(1000, '\n');
					continue;
				}//end discard of lines which start with a number sign

				massWindowReaderTwo >> channelFromFileTwo >> wrMassForMassWindowTwo >> massWindowLowerBoundTwo >> massWindowUpperBoundTwo;
				if(wrMassForMassWindowTwo == "0") continue;	//skip any lines in which the wrMass is zero
#ifdef DEBUG
				cout<<"desired WR mass=\t"<< desiredWrMass << endl;
				cout<<"first line read in says"<<endl;
				cout<< channelFromFile << wrMassForMassWindow << massWindowLowerBound << massWindowUpperBound << endl;
				cout<<"second line read in says"<<endl;
				cout<< channelFromFileTwo << wrMassForMassWindowTwo << massWindowLowerBoundTwo << massWindowUpperBoundTwo << endl;
#endif

				//use the two sets of wrMassForWindow, massWindowLowerBound, and massWindowUpperBound values to interpolate the correct mass window size
				int desiredWrMassInt = stoi(desiredWrMass), wrMassForMassWindowInt = stoi(wrMassForMassWindow), wrMassForMassWindowTwoInt = stoi(wrMassForMassWindowTwo);
				if(desiredChannel == channelFromFile && desiredWrMassInt == (wrMassForMassWindowInt+increment) && desiredWrMassInt == (wrMassForMassWindowTwoInt-increment) ){
					//the mass_cuts.txt file is ordered from low to high masses, so lowMassTwo will always be greater than lowMass, similarly for highMassTwo and highMass
					float lowMass = stof(massWindowLowerBound), lowMassTwo = stof(massWindowLowerBoundTwo), highMass = stof(massWindowUpperBound), highMassTwo = stof(massWindowUpperBoundTwo);
					cutStringToReturn = branchNameForCut + " > " + to_string(lowMass + ((lowMassTwo-lowMass)/(2*incrementFloat))*incrementFloat)  + " && " + branchNameForCut + " < " + to_string(highMass + ((highMassTwo-highMass)/(2*incrementFloat))*incrementFloat );
					// cout << "lowMass = " << lowMass + ((lowMassTwo-lowMass)/(2*incrementFloat))*incrementFloat << ", highMass = " << highMass + ((highMassTwo-highMass)/(2*incrementFloat))*incrementFloat << endl;
					return cutStringToReturn;
				}

			}//end second loop over mass cuts file

		}//end if desired wr mass minus 100 is an integer multiple of 200


	}//end loop over lines in mass cuts file

	return cutStringToReturn;
	
}//end getMassWindowCuts()


void genWrEfficienciesWithMassWindowCuts(bool isEE){

	///make a 2D plot with MNu as the vertical axis, MWR as the horizontal axis, and the fraction of GEN WR->eejj evts which
	///pass all offline cuts at each point in the (MWR, MNu) space
	///also print a table of these values
	///also apply the mass window cuts before calculating the efficiency

	///all input .root files should be in the same directory, and have file names which differ only in the WR and Nu mass values
	string dir= "/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/genSamplesFor2Dplots/";  //"/path/name/to/wrEvents/after/cuts/";
	string fileBegin = "analyzed_genWrToMuMuJJFullOfflineAnalysis_WR_";
	if (isEE) { fileBegin = "analyzed_genWrToEEJJFullOfflineAnalysis_WR_"; }
	string fileEnd = "_1.root";
	string fileMiddle = "_NU_";
	gStyle->SetTitleOffset(1.6,"Y");
	gStyle->SetOptStat("");

	string cutEfficiencyVsMassFile = "offlineMuMuEfficienciesVsMassesWithMassWindowCuts.txt";
	if (isEE) { cutEfficiencyVsMassFile = "offlineEEefficienciesVsMassesWithMassWindowCuts.txt"; }
	ofstream writeToEfficiencyFile(cutEfficiencyVsMassFile.c_str(),ofstream::trunc);
	writeToEfficiencyFile <<"#WR mass\tNu mass\tefficiencyFraction"<< std::endl;
	Float_t passingPercentage=-1;
	Float_t passingAllGenCuts=-1;
	TH2F * twoDimAcceptanceHist = new TH2F("twoDimAccHist","Percentage of events passing all GEN and mass window cuts",52,800,6000,59,100,6000);
	TH2F * twoDimAccHistNoWindows = new TH2F("twoDimAccHistNoWindows","Percentage of events passing all GEN cuts without mass windows",52,800,6000,59,100,6000);

	int maxWrMass = 6000, increment = 100, minWrMass = 800, minNuMass = 100;
	//starting value for wrMass equals the minimum wr mass
	for(int wrMass=minWrMass; wrMass<=maxWrMass ; wrMass+=increment){  ///loop over WR mass values
		
		for(int nuMass=minNuMass; nuMass<wrMass ; nuMass+=increment){  ///loop over Nu mass values

			///define input root file name
			string pfn = dir+fileBegin+to_string(wrMass)+fileMiddle+to_string(nuMass)+fileEnd;
			
			///define one TChain to count the number of events generated, and record the desired WR and Nu masses
			///define another TChain to count the number of evts passing all offline cuts at GEN lvl (no HLT or ID)
			TChain * genInfo = new TChain("genNuAnalyzerOne/genNu");
			genInfo->Add(pfn.c_str());
			TChain * afterOfflineCuts = new TChain("genMatchedParticleAnalyzerFive/genLeptonsAndJetsWithAllCuts");
			afterOfflineCuts->Add(pfn.c_str());

			string channel = "MuMu";
			if (isEE) { channel = "EE"; }

			///calculate percentage of evts which pass GEN cuts and mass window cuts, and store this percentage along with the nu and wr mass values in a txt file
			passingPercentage = (100)*((Float_t) afterOfflineCuts->GetEntries( (getMassWindowCuts("fourObjectMass",to_string(wrMass), channel,"../../configs/mass_cuts.txt")).c_str() )/genInfo->GetEntries());

			///calculate percentage of evts which pass all GEN cuts before mass window cut
			passingAllGenCuts = (100)*((Float_t) afterOfflineCuts->GetEntries()/genInfo->GetEntries());

			//write the efficiency of all GEN cuts + mass window cuts to a txt file
			//this txt file will be used for the 2D limit exclusion plot
			writeToEfficiencyFile << wrMass <<"\t"<< nuMass <<"\t"<< passingPercentage << endl;
	
			///fill a bin in the 2D histo with a weight equal to passingPercentage
			twoDimAcceptanceHist->Fill((Double_t) wrMass, (Double_t) nuMass,passingPercentage);
			twoDimAccHistNoWindows->Fill((Double_t) wrMass, (Double_t) nuMass, passingAllGenCuts);
			genInfo->Delete();
			afterOfflineCuts->Delete();

		}///end loop over Nu mass values

	}///end loop over WR mass values	
	
	writeToEfficiencyFile.close();


	///set axis labels and draw the histo, save an image of it (both pdf and png formats), and close the txt file
	twoDimAcceptanceHist->GetXaxis()->SetTitle("WR Mass [GeV]");
	twoDimAcceptanceHist->GetYaxis()->SetTitle("Nu Mass [GeV]");
	TCanvas * r1 = new TCanvas("r1","r1",900,700);
	r1->cd();
	twoDimAcceptanceHist->Draw("COLZ");

	string twoDimAcceptanceName = "twoDimGenWrAcceptancesMuMu_afterAllCutsIncludingMassWindows_13TeV";
	if (isEE) { twoDimAcceptanceName = "twoDimGenWrAcceptancesEE_afterAllCutsIncludingMassWindows_13TeV"; }
	r1->SaveAs((twoDimAcceptanceName+".png").c_str(),"recreate");
	r1->SaveAs((twoDimAcceptanceName+".pdf").c_str(),"recreate");
	r1->SaveAs((twoDimAcceptanceName+".C").c_str(),"recreate");

	twoDimAccHistNoWindows->GetXaxis()->SetTitle("WR Mass [GeV]");
	twoDimAccHistNoWindows->GetYaxis()->SetTitle("Nu Mass [GeV]");
	TCanvas * c1 = new TCanvas("c1","c1",900,700);
	c1->cd();
	twoDimAccHistNoWindows->Draw("COLZ");

	string twoDimAccNoWindowsName = "twoDimGenWrAcceptancesMuMu_afterAllCutsWithoutMassWindows_13TeV";
	if (isEE) { twoDimAccNoWindowsName = "twoDimGenWrAcceptancesEE_afterAllCutsWithoutMassWindows_13TeV"; }
	c1->SaveAs((twoDimAccNoWindowsName+".png").c_str(),"recreate");
	c1->SaveAs((twoDimAccNoWindowsName+".pdf").c_str(),"recreate");
	c1->SaveAs((twoDimAccNoWindowsName+".C").c_str(),"recreate");


}///end genWrEfficienciesWithMassWindowCuts()

