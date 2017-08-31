#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLorentzVector.h"
#include <vector>
#include <iostream>
#include <string>
// #include "ExoAnalysis/cmsWR/src/Selector.cc"
// #include "ExoAnalysis/cmsWR/src/miniTreeEvent.cc"
#include "../src/Selector.cc"
#include "../src/miniTreeEvent.cc"
#include <cstdio>
#include <memory>


#ifdef __CINT__
#pragma link C++ class std::vector<TLorentzVector>+;
#endif

void MakeHistos(TChain* chain, Selector *myEvent, std::vector<TH1*> *hs, Selector::tag_t channel);
//void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_WJets,TH1F* hs_WZ,TH1F* hs_ZZ,TH1F* hs_WW,TH1F* hs_data, TString xtitle, TString fname);
void drawPlots(TH1* hs_DY,TH1* hs_ttbar,TH1* hs_others,TH1* hs_data, TString xtitle, TString fname, Selector::tag_t channel);
void Plotter(Selector::tag_t channel);

void systVsMass(){
	// std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE,Selector::EMu};
	// std::vector<Selector::tag_t> channels = {Selector::EMu};  
	// for(auto c: channels)
	// 	Plotter(c);
	Plotter(Selector::EE);
	// Plotter(Selector::MuMu);

}

void Plotter(Selector::tag_t channel){
	TChain * chain_DY = new TChain("Tree_Iter0");
	TChain * chain_ttbar = new TChain("Tree_Iter0");
	TChain * chain_others = new TChain("Tree_Iter0");
	TChain * chain_WJets = new TChain("Tree_Iter0");
	TChain * chain_WZ = new TChain("Tree_Iter0");
	TChain * chain_ZZ = new TChain("Tree_Iter0");
	TChain * chain_WW = new TChain("Tree_Iter0");
	TChain * chain_data = new TChain("Tree_Iter0");

	TString inputDir = "/afs/cern.ch/work/g/gnegro/NuAnalysis-cmsWR16_afterPreApproval/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/selectedTreesWRv07/";

	switch (channel) {
		case Selector::EE:
			chain_DY->Add(inputDir+"selected_tree_DYAMCPT_lowdileptonsidebandEE.root");
			chain_ttbar->Add(inputDir+"selected_tree_TTAMC_lowdileptonsidebandEE.root");
			chain_others->Add(inputDir+"selected_tree_Other_lowdileptonsidebandEE.root");
			chain_data->Add(inputDir+"selected_tree_data_lowdileptonsidebandEE.root");
			break;
		case Selector::MuMu:
			chain_DY->Add(inputDir+"selected_tree_DYAMCPT_lowdileptonsidebandMuMu.root");
			chain_ttbar->Add(inputDir+"selected_tree_TTAMC_lowdileptonsidebandMuMu.root"); 
			chain_others->Add(inputDir+"selected_tree_Other_lowdileptonsidebandMuMu.root");
			chain_data->Add(inputDir+"selected_tree_data_lowdileptonsidebandMuMu.root"); 
			break;
		case Selector::EMu:
			chain_DY->Add(inputDir+"selected_tree_DYAMCPT_flavoursidebandEMu.root");
			chain_ttbar->Add(inputDir+"selected_tree_TTAMC_flavoursidebandEMu.root");
			chain_others->Add(inputDir+"selected_tree_Other_flavoursidebandEMu.root");
			chain_data->Add(inputDir+"selected_tree_data_flavoursidebandEMu.root");
			break;
		// default:
		// 	std::cout << "Unknown tag" << std::endl;
	}

	Selector myEvent_DY;
	Selector myEvent_ttbar;
	Selector myEvent_others;
	Selector myEvent_data;

	myEvent_DY.SetBranchAddresses(chain_DY);
	myEvent_ttbar.SetBranchAddresses(chain_ttbar);
	myEvent_others.SetBranchAddresses(chain_others);
	myEvent_data.SetBranchAddresses(chain_data);

	std::vector<TH1*> hs_DY;
	MakeHistos(chain_DY, &myEvent_DY, &hs_DY, channel);
	std::vector<TH1*> hs_ttbar;
	MakeHistos(chain_ttbar, &myEvent_ttbar, &hs_ttbar, channel);
	std::vector<TH1*> hs_others;
	MakeHistos(chain_others, &myEvent_others, &hs_others, channel);
	std::vector<TH1*> hs_data;
	MakeHistos(chain_data, &myEvent_data, &hs_data, channel);

	unsigned int nPlots = hs_DY.size();

	// hs_data[13]->SetLineColor(kRed);
	// hs_data[13]->Draw();
	// hs_ttbar[13]->Draw("same");
	
	TString xtitles[] = {"Mlljj [GeV]","Mlljj [GeV]","dilepton mass [GeV]"};

	TString fnames[] = {"Mlljj","Mlljj_binned","Mll"};
	
	int i = 0;
	for(unsigned int i = 0; i < nPlots; i++){
		std::string s = std::to_string(i);
		if(xtitles[i] == "Mlljj [GeV]")
			drawPlots(hs_DY[i],hs_ttbar[i],hs_others[i],hs_data[i],xtitles[i],fnames[i], channel);
	}
	
}

void MakeHistos(TChain * chain, Selector *myEvent, std::vector<TH1*> *hs, Selector::tag_t channel){

	TH1F *h_WR_mass = new TH1F("h_WR_mass","",40,0,7000);

	Float_t bins[] = { 150,300, 450, 600, 750, 900, 1050, 1200, 1350, 1500, 1650,1800,1950,2100,2250,2400,2550,2700,2850,3000,3150,3300,3450,3600,3750,3900,4150,7000 };
	Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;
	TH1F *h_WR_mass_binned = new TH1F("h_WR_mass_binned","",binnum,bins);
	
	float dilepton_max = 110.;
	if(channel == Selector::EMu)
		dilepton_max = 1000;
	TH1F *h_dilepton_mass = new TH1F("h_dilepton_mass","",40,70,dilepton_max);

	Long64_t nEntries = chain->GetEntries();
	cout<< nEntries << endl;

	for(int ev = 0; ev<nEntries; ++ev){
		chain->GetEntry(ev);
		h_WR_mass->Fill(myEvent->WR_mass,myEvent->weight);    
		h_WR_mass_binned->Fill(myEvent->WR_mass,myEvent->weight);    
		h_dilepton_mass->Fill(myEvent->dilepton_mass,myEvent->weight);		
	}

	h_WR_mass->GetYaxis()->SetRangeUser(0.01,100000);
	h_WR_mass_binned->GetYaxis()->SetRangeUser(0.01,100000);
	
	hs->push_back(h_WR_mass);
	hs->push_back(h_WR_mass_binned);
	hs->push_back(h_dilepton_mass);
}

void drawPlots(TH1* hs_DY,TH1* hs_ttbar,TH1* hs_others,TH1* hs_data, TString xtitle, TString fname, Selector::tag_t channel){

	THStack* th = new THStack();

	if(channel == Selector::EMu){
		th->Add(hs_DY);
		th->Add(hs_others);
		th->Add(hs_ttbar);
	}
	else{
		th->Add(hs_others);
		th->Add(hs_ttbar);
		th->Add(hs_DY);
	}

	TH1F *errors = (TH1F*)(th->GetStack()->Last())->Clone();
	TFile *scale_error_fn = new TFile("data/scale_syst.root");
	TFile *pdf_error_fn = new TFile("data/pdf_syst.root");
	TFile *fact_renorm_error_fn = new TFile("data/fact_renorm_syst.root");

	TString hname = "";
	if(channel == Selector::EE){
		if(fname == "Mlljj_binned")
			hname = "hWR_mass_binned_EE";
		else 
			hname = "hWR_mass_EE";
	}
	if(channel == Selector::MuMu){
    	if(fname == "Mlljj_binned")
		    hname = "hWR_mass_binned_MuMu";
    	else
			hname = "hWR_mass_MuMu";
	}

	TH1F *scale_error_hist = (TH1F*) scale_error_fn->Get(hname);
	TH1F *pdf_error_hist = (TH1F*) pdf_error_fn->Get(hname);
	TH1F *fact_renorm_error_hist = (TH1F*) fact_renorm_error_fn->Get(hname);
		

	Int_t nbins = hs_DY->GetNbinsX();

	Double_t WRmass[nbins+1];
	Double_t WRmass_err[nbins+1];

	Double_t pdfSyst[nbins+1];
	Double_t pdfSyst_err[nbins+1];
	Double_t scaleSyst[nbins+1];
	Double_t scaleSyst_err[nbins+1];
	Double_t factRenormSyst[nbins+1];
	Double_t factRenormSyst_err[nbins+1];


	for(int i = 0;i<hs_DY->GetNbinsX()+1;i++){
		WRmass[i] = hs_DY->GetBinCenter(i);
		WRmass_err[i] = hs_DY->GetBinWidth(i)/2.;
		cout << "WRmass = " << WRmass[i];// << endl;

		pdfSyst[i] = pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)));
		pdfSyst_err[i] = pdf_error_hist->GetBinError(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)));
		cout << ": pdfSyst = " << pdfSyst[i] << " +/- " << pdfSyst_err[i];// << endl;

		scaleSyst[i] = scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)));
		scaleSyst_err[i] = scale_error_hist->GetBinError(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)));
		cout << ", scaleSyst = " << scaleSyst[i] << " +/- " << scaleSyst_err[i];// << endl;

		factRenormSyst[i] = fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)));
		factRenormSyst_err[i] = fact_renorm_error_hist->GetBinError(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)));
		cout << ", factRenormSyst = " << factRenormSyst[i] << " +/- " << factRenormSyst_err[i] << endl;
	}


	TCanvas* pdfSystCanvas = new TCanvas( "pdfSystCanvas", "", 0, 0, 600, 600 ) ;
	pdfSystCanvas->cd();

	TGraphErrors *pdfGraph = new TGraphErrors(nbins+1,WRmass,pdfSyst,WRmass_err,pdfSyst_err);
	pdfGraph->GetXaxis()->SetTitle("Mlljj [GeV]");
	pdfGraph->GetYaxis()->SetTitle("pdfSyst");
	pdfGraph->SetMarkerColor(4);
	pdfGraph->SetMarkerStyle(21);
	pdfGraph->Draw("ALP");


	TCanvas* scaleSystCanvas = new TCanvas( "scaleSystCanvas", "", 0, 0, 600, 600 ) ;
	scaleSystCanvas->cd();

	TGraphErrors *scaleGraph = new TGraphErrors(nbins+1,WRmass,scaleSyst,WRmass_err,scaleSyst_err);
	scaleGraph->GetXaxis()->SetTitle("Mlljj [GeV]");
	scaleGraph->GetYaxis()->SetTitle("scaleSyst");
	scaleGraph->SetMarkerColor(4);
	scaleGraph->SetMarkerStyle(21);
	scaleGraph->Draw("ALP");


	TCanvas* factRenormSystCanvas = new TCanvas( "factRenormSystCanvas", "", 0, 0, 600, 600 ) ;
	factRenormSystCanvas->cd();

	TGraphErrors *factRenormGraph = new TGraphErrors(nbins+1,WRmass,factRenormSyst,WRmass_err,factRenormSyst_err);
	factRenormGraph->GetXaxis()->SetTitle("Mlljj [GeV]");
	factRenormGraph->GetYaxis()->SetTitle("factRenormSyst");
	factRenormGraph->SetMarkerColor(4);
	factRenormGraph->SetMarkerStyle(21);
	factRenormGraph->Draw("ALP");


		// for(int i = 0;i<errors->GetNbinsX()+1;i++){
		// 	if(xtitle == "Mlljj [GeV]"){
		// 		cout<<i<<" "<<errors->GetBinContent(i)<<" "<<errors->GetBinError(i)<<" "<< pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))<<" "<< scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))<<" "<< fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))<<endl;
		// 	}
		// }


		// for(int i = 0;i<errors->GetNbinsX()+1;i++){
		// 	float errorSum = TMath::Sqrt(
		// 			(scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
		// 			+ 
		// 			(pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
		// 			+ 
		// 			(fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
		// 			+ 
		// 			(errors->GetBinError(i)*errors->GetBinError(i))
		// 			);	

		// 	float systErrorSum = TMath::Sqrt(
		// 			(scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
		// 			+ 
		// 			(pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
		// 			+ 
		// 			(fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
		// 			);	

		// 	if(xtitle == "Mlljj [GeV]"){
		// 		// cout<<i<<" "<<errors->GetBinContent(i)<<" "<<errorSum<<" "<<errors->GetBinError(i)<<" "<< pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))<<endl;
		// 		// cout<<i<<" "<<errors->GetBinContent(i)<<" "<<errors->GetBinError(i)<<" "<< pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))<<" "<< scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))<<" "<< fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))<<endl;
		// 		// cout<<i<<" errorSum = "<<errorSum<<endl;
		// 		// cout<<i<<" systErrorSum = "<<systErrorSum<<endl;
		// 		errors->SetBinError(i,errorSum);
		// 		// hs_DY->SetBinError(i,errorSum);

		// 		// systErrors->SetBinContent(i,1.);
		// 		systErrors->SetBinError(i,systErrorSum);
		// 		// systErrors->SetBinError(i,errorSum);
		// 	}
		// }


	TString fn = "";
	TString outputdir = "SystVsMass_new/";

	if(channel == Selector::EMu)
		fn = "/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/comparisonFlavorSideband/"+outputdir;
	if(channel == Selector::EE)
		fn = "/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/comparisonEELowDileptonSideband/"+outputdir;
	if(channel == Selector::MuMu)
		fn = "/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/comparisonMuMuLowDileptonSideband/"+outputdir;

	pdfSystCanvas->SaveAs(fn+"pdfSyst_vs_"+fname+".pdf");
	pdfSystCanvas->SaveAs(fn+"pdfSyst_vs_"+fname+".png");
	pdfSystCanvas->SaveAs(fn+"pdfSyst_vs_"+fname+".root");
	pdfSystCanvas->Close();

	scaleSystCanvas->SaveAs(fn+"scaleSyst_vs_"+fname+".pdf");
	scaleSystCanvas->SaveAs(fn+"scaleSyst_vs_"+fname+".png");
	scaleSystCanvas->SaveAs(fn+"scaleSyst_vs_"+fname+".root");
	scaleSystCanvas->Close();

	factRenormSystCanvas->SaveAs(fn+"factRenormSyst_vs_"+fname+".pdf");
	factRenormSystCanvas->SaveAs(fn+"factRenormSyst_vs_"+fname+".png");
	factRenormSystCanvas->SaveAs(fn+"factRenormSyst_vs_"+fname+".root");
	factRenormSystCanvas->Close();

}
