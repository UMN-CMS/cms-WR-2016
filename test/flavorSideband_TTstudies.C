#include "TStyle.h"
#include "TPaveText.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLorentzVector.h"
#include <vector>
#include <iostream>
#include <string>
// #include "ExoAnalysis/cmsWR/src/Selector.cc"
// #include "ExoAnalysis/cmsWR/src/miniTreeEvent.cc"
#include "../src/Selector.cc"
#include "../src/miniTreeEvent.cc"
#include "CMS_lumi.C"
#include <cstdio>
#include <memory>

#define PRINTRATIOS

using namespace std;

std::string chiSquaredNdofString(TF1 * fit);
void fillHisto(TChain * chain, Selector *myEvent, TH1F * h);
void flavorSideband(TString fitType){
	
	// Float_t mumuEmuSF = 6.90682e-01, eeEmuSF = 4.18185e-01;
	// Float_t mumuEmuSF_err = 9.56973e-03, eeEmuSF_err = 6.70751e-03;

	Float_t mumuEmuSF = 7.19599e-01, eeEmuSF = 4.23142e-01;
	Float_t mumuEmuSF_err = 9.97339e-03, eeEmuSF_err = 6.78942e-03;

	// std::string longMuMuEmuSF = to_string(mumuEmuSF);
	// std::string shortMuMuEmuSF = longMuMuEmuSF.substr(0,5);
	// std::string longEEEmuSF = to_string(eeEmuSF);
	// std::string shortEEEmuSF = longEEEmuSF.substr(0,5);
	std::string shortMuMuEmuSF = (to_string(ceilf(mumuEmuSF*1000)/1000)).substr(0,5);
	std::string shortEEEmuSF = (to_string(ceilf(eeEmuSF*1000)/1000)).substr(0,5);

	// string shortMuMuEmuSF_err = (to_string(mumuEmuSF_err)).substr(0,5);
	// string shortEEEmuSF_err = (to_string(eeEmuSF_err)).substr(0,5);
	string shortMuMuEmuSF_err = (to_string(ceilf(mumuEmuSF_err*1000)/1000)).substr(0,5);
	string shortEEEmuSF_err = (to_string(ceilf(eeEmuSF_err*1000)/1000)).substr(0,5);

	//gStyle->SetOptFit(100);	///<only show chiSquared divided by nDOF
	gStyle->SetOptFit(0);	///<show nothing
	TChain * chain_EMu = new TChain("Tree_Iter0");
	TChain * chain_EE = new TChain("Tree_Iter0");
	TChain * chain_MuMu = new TChain("Tree_Iter0");
	TChain * chain_EMuData = new TChain("Tree_Iter0");
	// TChain * chain_EMuDataEE = new TChain("Tree_Iter0");
	// TChain * chain_EMuDataMuMu = new TChain("Tree_Iter0");

	// TString outputdir = "/afs/cern.ch/user/g/gnegro/www/cmsWR/TTstudies/";
	// TString outputdir = "/afs/cern.ch/user/g/gnegro/www/cmsWR/TTstudies/newPlots/";
	// TString outputdir = "/afs/cern.ch/user/g/gnegro/www/cmsWR/approval/TTstudies/";	
	TString outputdir = "/afs/cern.ch/user/g/gnegro/www/cmsWR/approval/TTstudies/newXs/";
	// TString outputdir = "/afs/cern.ch/user/g/gnegro/www/cmsWR/approval/TTstudies/old/";	

	// TString dirOld = "/afs/cern.ch/user/g/gnegro/work/NuAnalysis-cmsWR16_preApproval/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/TTstudies/WRv07/";
	// TString dirOld = "/afs/cern.ch/user/g/gnegro/work/NuAnalysis-cmsWR16_afterPreApproval/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/selectedTreesWRv07_TTstudies/";
	TString dir = "/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/selectedTrees_WRv07/selectedTreesWRv07/";

	chain_EMu->Add(dir+"selected_tree_TTAMC_flavoursidebandEMu.root");	
	chain_EE->Add(dir+"selected_tree_TTAMC_signal_eeEE.root");
	chain_MuMu->Add(dir+"selected_tree_TTAMC_signal_mumuMuMu.root");
	chain_EMuData->Add(dir+"selected_tree_data_flavoursidebandEMu.root");
	// // chain_EMuDataEE->Add(dir+"selected_tree_data_flavoursidebandEMuEE.root");
	// // chain_EMuDataMuMu->Add(dir+"selected_tree_data_flavoursidebandEMuMuMu.root");

	// chain_EMu->Add(dirOld+"selected_tree_TTAMC_flavoursidebandEMu.root");	
	// chain_EE->Add(dirOld+"selected_tree_TTAMC_signal_eeEE.root");
	// chain_MuMu->Add(dirOld+"selected_tree_TTAMC_signal_mumuMuMu.root");
	// chain_EMuData->Add(dirOld+"selected_tree_data_flavoursidebandEMu.root");

	Selector myEvent_EMu;
	Selector myEvent_EE;
	Selector myEvent_MuMu;
	Selector myEvent_EMuData;
	// Selector myEvent_EMuDataEE;
	// Selector myEvent_EMuDataMuMu;

	myEvent_EMu.SetBranchAddresses(chain_EMu);
	myEvent_EE.SetBranchAddresses(chain_EE);
	myEvent_MuMu.SetBranchAddresses(chain_MuMu);
	myEvent_EMuData.SetBranchAddresses(chain_EMuData);
	// myEvent_EMuDataEE.SetBranchAddresses(chain_EMuDataEE);
	// myEvent_EMuDataMuMu.SetBranchAddresses(chain_EMuDataMuMu);

	//Float_t bins[] = { 200, 400, 450, 500, 550, 600, 625, 652, 683, 718, 760, 812, 877, 975, 1160, 2000 };	//for xMax of 2000
	Float_t bins[] = { 200, 400, 450, 500, 550, 600, 620, 645, 678, 705, 740, 790, 850, 1030, 1400, 4000 };	//for xMax much greater than 2000 
	
	Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;

	//do not use variable bin widths on these histos with large domain, they are not needed
	// TH1F *h_WR_mass_EMu = new TH1F("h_WR_mass_EMu","",50,200,4000);
	// TH1F *h_WR_mass_EE = new TH1F("h_WR_mass_EE","",50,200,4000);
	// TH1F *h_WR_mass_MuMu = new TH1F("h_WR_mass_MuMu","",50,200,4000);
	// TH1F *h_WR_mass_EMuData = new TH1F("h_WR_mass_EMuData","",50,200,4000);

	//fixed bin width MLLJJ plots with standard domain
	//TH1F *h_WR_mass_EMu = new TH1F("h_WR_mass_EMu","",50,200,2000);
	//TH1F *h_WR_mass_EE = new TH1F("h_WR_mass_EE","",50,200,2000);
	//TH1F *h_WR_mass_MuMu = new TH1F("h_WR_mass_MuMu","",50,200,2000);
	//TH1F *h_WR_mass_EMuData = new TH1F("h_WR_mass_EMuData","",50,200,2000);
	
	//variable bin width MLLJJ plots with standard domain
	TH1F *h_WR_mass_EMu = new TH1F("h_WR_mass_EMu","",binnum, bins);
	TH1F *h_WR_mass_EE = new TH1F("h_WR_mass_EE","",binnum, bins);
	TH1F *h_WR_mass_MuMu = new TH1F("h_WR_mass_MuMu","",binnum, bins);
	TH1F *h_WR_mass_EMuData = new TH1F("h_WR_mass_EMuData","",binnum, bins);
	// TH1F *h_WR_mass_EMuDataEE = new TH1F("h_WR_mass_EMuDataEE","",binnum, bins);
	// TH1F *h_WR_mass_EMuDataMuMu = new TH1F("h_WR_mass_EMuDataMuMu","",binnum, bins);

	h_WR_mass_EMu->Sumw2();
	h_WR_mass_EE->Sumw2();
	h_WR_mass_MuMu->Sumw2();
	h_WR_mass_EMuData->Sumw2();
	// h_WR_mass_EMuDataEE->Sumw2();
	// h_WR_mass_EMuDataMuMu->Sumw2();

	fillHisto(chain_EMu, &myEvent_EMu, h_WR_mass_EMu);
	fillHisto(chain_EE, &myEvent_EE, h_WR_mass_EE);
	fillHisto(chain_MuMu, &myEvent_MuMu, h_WR_mass_MuMu);
	fillHisto(chain_EMuData, &myEvent_EMuData, h_WR_mass_EMuData);
	// fillHisto(chain_EMuDataEE, &myEvent_EMuDataEE, h_WR_mass_EMuDataEE);
	// fillHisto(chain_EMuDataMuMu, &myEvent_EMuDataMuMu, h_WR_mass_EMuDataMuMu);	


	TCanvas* mycanvas_EE = new TCanvas( "mycanvas_EE", "", 0, 0, 600, 600 ) ;
	h_WR_mass_EMu->GetXaxis()->SetTitle("m_{eejj} (GeV)");
	h_WR_mass_EMu->DrawNormalized();
	h_WR_mass_EE->SetLineColor(kRed);
	h_WR_mass_EE->DrawNormalized("same histo");
	TLegend *leg_EE = new TLegend( 0.72, 0.50, 0.98, 0.70 );
	leg_EE->AddEntry( h_WR_mass_EMu, "EMu" );
	leg_EE->AddEntry( h_WR_mass_EE, "EE" );
	leg_EE->Draw();
	CMS_lumi(mycanvas_EE, "Simulation");
	// mycanvas_EE->Print((outputdir+"flavor_EE_variablebinwidth.pdf"));
	// mycanvas_EE->Print((outputdir+"flavor_EE_variablebinwidth.png"));

	TCanvas* mycanvas_MuMu = new TCanvas( "mycanvas_MuMu", "", 0, 0, 600, 600 ) ;
	h_WR_mass_EMu->GetXaxis()->SetTitle("m_{#mu#mujj} (GeV)");
	h_WR_mass_EMu->DrawNormalized();
	h_WR_mass_MuMu->SetLineColor(kRed);
	h_WR_mass_MuMu->DrawNormalized("same histo");
	TLegend *leg_MuMu = new TLegend( 0.72, 0.50, 0.98, 0.70 );
	leg_MuMu->AddEntry( h_WR_mass_EMu, "EMu" );
	leg_MuMu->AddEntry( h_WR_mass_MuMu, "MuMu" );
	leg_MuMu->Draw();
	CMS_lumi(mycanvas_MuMu, "Simulation");
	// mycanvas_MuMu->Print((outputdir+"flavor_MuMu_variablebinwidth.pdf"));
	// mycanvas_MuMu->Print((outputdir+"flavor_MuMu_variablebinwidth.png"));

#ifdef PRINTRATIOS
	std::cout<<"number of overflow events in h_WR_mass_EE histo =\t"<< h_WR_mass_EE->GetBinContent(h_WR_mass_EE->GetNbinsX() + 1) <<std::endl;
	std::cout<<"number of overflow events in h_WR_mass_MuMu histo =\t"<< h_WR_mass_MuMu->GetBinContent(h_WR_mass_MuMu->GetNbinsX() + 1) <<std::endl;
	std::cout<<"number of overflow events in h_WR_mass_EMu histo =\t"<< h_WR_mass_EMu->GetBinContent(h_WR_mass_EMu->GetNbinsX() + 1) <<std::endl;
#endif


	///dont show anything in histo stats box for the two plots with TF1 curves
	gStyle->SetOptStat("");
	TH1F *h_ratio_EE = (TH1F*)h_WR_mass_EE->Clone();
	TH1F *h_ratio_MuMu = (TH1F*)h_WR_mass_MuMu->Clone();

	h_ratio_EE->Sumw2();
	h_ratio_MuMu->Sumw2();

	h_ratio_EE->Sumw2();
	h_ratio_EE->Divide(h_WR_mass_EMu);
	h_ratio_EE->GetXaxis()->SetTitle("#it{m_{eejj}} (GeV)");
	h_ratio_EE->GetXaxis()->SetTitleSize(0.045);
	h_ratio_EE->GetYaxis()->SetRangeUser(0.3,0.6);
	h_ratio_EE->GetYaxis()->SetTitle("#it{m_{eejj} / m_{e#mujj}}   ");
	h_ratio_EE->GetYaxis()->SetTitleSize(0.045);
	h_ratio_EE->SetTitleOffset(1.1,"Y");

	h_ratio_MuMu->Sumw2();
	h_ratio_MuMu->Divide(h_WR_mass_EMu);
	h_ratio_MuMu->GetXaxis()->SetTitle("#it{m_{#mu#mujj}} (GeV)");
	h_ratio_MuMu->GetXaxis()->SetTitleSize(0.045);
	h_ratio_MuMu->GetYaxis()->SetRangeUser(0.5,0.9);
	h_ratio_MuMu->GetYaxis()->SetTitle("#it{m_{#mu#mujj} / m_{e#mujj}}");
	h_ratio_MuMu->GetYaxis()->SetTitleSize(0.045);
	h_ratio_MuMu->SetTitleOffset(1.1,"Y");
	

	TCanvas* mycanvas_ratio_EE = new TCanvas( "mycanvas_ratio_EE", "", 0, 0, 600, 600 ) ;
	// TPaveText* chiSqdBoxEE = new TPaveText(1500.,0.5,2500.,0.55);
	// chiSqdBoxEE->SetFillColor(kWhite);
	// chiSqdBoxEE->SetTextSize(0.05);
	TLatex chiSqdBoxEE;
	chiSqdBoxEE.SetNDC();
	chiSqdBoxEE.SetTextAngle(0);
	chiSqdBoxEE.SetTextColor(kBlack);
	chiSqdBoxEE.SetTextFont(42);
	chiSqdBoxEE.SetTextAlign(11);
	chiSqdBoxEE.SetTextSize(0.04);

	TF1 *f_EE = new TF1("f_EE","[0]",200,4000);  

	if (fitType == "pol0") { 
		// f_EE->FixParameter(0,eeEmuSF);
		// chiSqdBoxEE->AddText( TString("ratio = " + shortEEEmuSF + "#pm " + shortEEEmuSF_err) );
	}

	if (fitType == "pol1" || fitType == "pol1UncBand") {
		f_EE = new TF1("f_EE","[0]*x+[1]",200,4000);
		// chiSqdBoxEE->AddText( TString("p0 = " + to_string(1.76436e-05) + "#pm " + to_string(1.69739e-05)) ); 
		// chiSqdBoxEE->AddText( TString("p1 = " + to_string(4.02728e-01) + "#pm " + to_string(1.63134e-02)) );	
	}

	if (fitType == "pol2") {
		f_EE = new TF1("f_EE","[0]*x*x+[1]*x+[2]",200,4000);
		// chiSqdBoxEE = new TPaveText(1000.,0.5,2500.,0.58);
		// chiSqdBoxEE->AddText( TString("p0 = " + to_string(9.89190e-09) + "#pm " + to_string(2.43133e-08)) );
		// chiSqdBoxEE->AddText( TString("p1 = " + to_string(-1.32382e-05) + "#pm " + to_string(7.77791e-05)) );	
		// chiSqdBoxEE->AddText( TString("p2 = " + to_string(4.20646e-01) + "#pm " + to_string(4.69654e-02)) );	
	}

	if (fitType == "pol3") {
		f_EE = new TF1("f_EE","[0]*x*x*x+[1]*x*x+[2]*x+[3]",200,4000);			
		// chiSqdBoxEE->AddText( TString("p0 = " + to_string(1.22525e-10) + "#pm " + to_string(1.13690e-10)) );
		// chiSqdBoxEE->AddText( TString("p1 = " + to_string(-5.38217e-07) + "#pm " + to_string(5.09170e-07)) );	
		// chiSqdBoxEE->AddText( TString("p2 = " + to_string(6.65966e-04) + "#pm " + to_string(6.35012e-04)) );	
		// chiSqdBoxEE->AddText( TString("p3 = " + to_string(1.73002e-01) + "#pm " + to_string(2.34537e-01)) );
	}

	if (fitType == "pol4") f_EE = new TF1("f_EE","[0]*x*x*x*x+[1]*x*x*x+[2]*x*x+[3]*x+[4]",200,4000);		

	if (fitType == "exp") f_EE = new TF1("f_EE","exp([0]+[1]*x)",200,4000);
	if (fitType == "inverseExp") f_EE = new TF1("f_EE","exp([0]+[1]/x)",200,4000);
	if (fitType == "simpleExp") f_EE = new TF1("f_EE","exp([0])",200,4000);

	if (fitType == "log") f_EE = new TF1("f_EE","log([0]+[1]*x)",200,4000);
	if (fitType == "inverseLog") f_EE = new TF1("f_EE","log([0]+[1]/x)",200,4000);
	if (fitType == "simpleLog") f_EE = new TF1("f_EE","log([0])",200,4000);

	h_ratio_EE->Fit("f_EE");
	h_ratio_EE->Draw();
	// chiSqdBoxEE->AddText( TString( chiSquaredNdofString(f_EE) ) );
	// chiSqdBoxEE->Draw("same");

	chiSqdBoxEE.DrawLatex(0.2,0.8,TString( chiSquaredNdofString(f_EE) ));
	// if (fitType == "pol0") chiSqdBoxEE.DrawLatex(0.2,0.75,TString( "ratio = " + shortEEEmuSF + "#pm " + shortEEEmuSF_err ));
	if (fitType == "pol0") chiSqdBoxEE.DrawLatex(0.2,0.75,TString( "ratio = 0.423 #pm " + shortEEEmuSF_err ));

	f_EE->SetLineColor(kBlue);
	f_EE->Draw("same");

	if (fitType == "pol1UncBand") {
		Double_t f_EE_max = f_EE->Eval(4000);
		Double_t f_EE_min = f_EE->Eval(200);	
		cout << "f_EE(4000) = " << f_EE_max << endl;
		cout << "f_EE(200) = " << f_EE_min << endl;
		Double_t unc_EE = f_EE_max - f_EE_min;
		cout << "unc_EE = " << unc_EE << endl;
		// chiSqdBoxEE->AddText( TString( "syst. unc. = " + to_string(unc_EE) ) );
		chiSqdBoxEE.DrawLatex(0.2,0.75,TString( "syst. unc. = " + to_string(unc_EE) ));

		TLine* unc_EE_min = new TLine(200, f_EE_min, 4000, f_EE_min);
		TLine* unc_EE_max = new TLine(200, f_EE_max, 4000, f_EE_max);
		unc_EE_min->SetLineWidth(2);
		unc_EE_min->SetLineStyle(2);
		unc_EE_max->SetLineWidth(2);
		unc_EE_max->SetLineStyle(2);
		unc_EE_max->Draw("same");
		unc_EE_min->Draw("same");
	}
	CMS_lumi(mycanvas_ratio_EE, "Simulation");


	TCanvas* mycanvas_ratio_MuMu = new TCanvas( "mycanvas_ratio_MuMu", "", 0, 0, 600, 600 ) ;	
	// TPaveText* chiSqdBoxMuMu = new TPaveText(1500.,0.81,2500.,0.86);
	// chiSqdBoxMuMu->SetFillColor(kWhite);
	// chiSqdBoxMuMu->SetTextSize(0.05);
	TLatex chiSqdBoxMuMu;
	chiSqdBoxMuMu.SetNDC();
	chiSqdBoxMuMu.SetTextAngle(0);
	chiSqdBoxMuMu.SetTextColor(kBlack);
	chiSqdBoxMuMu.SetTextFont(42);
	chiSqdBoxMuMu.SetTextAlign(11);
	chiSqdBoxMuMu.SetTextSize(0.04);

	TF1 *f_MuMu = new TF1("f_MuMu","[0]",200,4000);

	if (fitType == "pol0") {
		// //f_MuMu->FixParameter(0,mumuEmuSF);
		// chiSqdBoxMuMu->AddText( TString("ratio = " + shortMuMuEmuSF + "#pm " + shortMuMuEmuSF_err) );
		// chiSqdBoxMuMu->AddText( TString("ratio = 0.691 #pm " + shortMuMuEmuSF_err) );
	}

	if (fitType == "pol1" || fitType == "pol1UncBand") {
		f_MuMu = new TF1("f_MuMu","[0]*x+[1]",200,4000);
		// chiSqdBoxMuMu->AddText( TString("p0 = " + to_string(3.68076e-05) + "#pm " + to_string(2.45377e-05)) ); 
		// chiSqdBoxMuMu->AddText( TString("p1 = " + to_string(6.58590e-01) + "#pm " + to_string(2.34369e-02)) ); 
	}

	if (fitType == "pol2") {
		f_MuMu = new TF1("f_MuMu","[0]*x*x+[1]*x+[2]",200,4000);		
		// chiSqdBoxMuMu = new TPaveText(1000.,0.80,2500.,0.88);
		// chiSqdBoxMuMu->AddText( TString("p0 = " + to_string(-7.36314e-09) + "#pm " + to_string(3.49647e-08)) );
		// chiSqdBoxMuMu->AddText( TString("p1 = " + to_string(5.97535e-05) + "#pm " + to_string(1.11690e-04)) );
		// chiSqdBoxMuMu->AddText( TString("p2 = " + to_string(6.45301e-01) + "#pm " + to_string(6.73157e-02)) );
	}

	if (fitType == "pol3") {
		f_MuMu = new TF1("f_MuMu","[0]*x*x*x+[1]*x*x+[2]*x+[3]",200,4000);		
		// chiSqdBoxMuMu->AddText( TString("p0 = " + to_string(1.45775e-10) + "#pm " + to_string(1.62099e-10)) );
		// chiSqdBoxMuMu->AddText( TString("p1 = " + to_string(-6.58565e-07) + "#pm " + to_string(7.24966e-07)) );
		// chiSqdBoxMuMu->AddText( TString("p2 = " + to_string(8.64718e-04) + "#pm " + to_string(9.02044e-04)) );
		// chiSqdBoxMuMu->AddText( TString("p3 = " + to_string(3.52499e-01) + "#pm " + to_string(3.32476e-01)) );
	}

	if (fitType == "pol4") f_MuMu = new TF1("f_MuMu","[0]*x*x*x*x+[1]*x*x*x+[2]*x*x+[3]*x+[4]",200,4000);		

	if (fitType == "exp") f_MuMu = new TF1("f_MuMu","exp([0]+[1]*x)",200,4000);
	if (fitType == "inverseExp") f_MuMu = new TF1("f_MuMu","exp([0]+[1]/x)",200,4000);	
	if (fitType == "simpleExp") f_MuMu = new TF1("f_MuMu","exp([0])",200,4000);

	if (fitType == "log") f_MuMu = new TF1("f_MuMu","log([0]+[1]*x)",200,4000);
	if (fitType == "inverseLog") f_MuMu = new TF1("f_MuMu","log([0]+[1]/x)",200,4000);
	if (fitType == "simpleLog") f_MuMu = new TF1("f_MuMu","log([0])",200,4000);		

	h_ratio_MuMu->Fit("f_MuMu");
	h_ratio_MuMu->Draw();
	// chiSqdBoxMuMu->AddText( TString( chiSquaredNdofString(f_MuMu) ) );
	// chiSqdBoxMuMu->Draw("same");

	chiSqdBoxMuMu.DrawLatex(0.2,0.8,TString( chiSquaredNdofString(f_MuMu) ));
	// if (fitType == "pol0") chiSqdBoxMuMu.DrawLatex(0.2,0.75,TString( "ratio = 0.691 #pm " + shortMuMuEmuSF_err ));
	if (fitType == "pol0") chiSqdBoxMuMu.DrawLatex(0.2,0.75,TString( "ratio = " + shortMuMuEmuSF + "#pm " + shortMuMuEmuSF_err ));

	f_MuMu->SetLineColor(kBlue);
	f_MuMu->Draw("same");

	if (fitType == "pol1UncBand") {
		Double_t f_MuMu_max = f_MuMu->Eval(4000);
		Double_t f_MuMu_min = f_MuMu->Eval(200);	
		cout << "f_MuMu(4000) = " << f_MuMu_max << endl;
		cout << "f_MuMu(200) = " << f_MuMu_min << endl;
		Double_t unc_MuMu = f_MuMu_max - f_MuMu_min;
		cout << "unc_MuMu = " << unc_MuMu << endl;
		// chiSqdBoxMuMu = new TPaveText(1000.,0.82,2500.,0.87);
		// chiSqdBoxMuMu->AddText( TString( "syst. unc. = " + to_string(unc_MuMu) ) );
		chiSqdBoxMuMu.DrawLatex(0.2,0.75,TString( "syst. unc. = " + to_string(unc_MuMu) ));

		TLine* unc_MuMu_min = new TLine(200, f_MuMu_min, 4000, f_MuMu_min);
		TLine* unc_MuMu_max = new TLine(200, f_MuMu_max, 4000, f_MuMu_max);
		unc_MuMu_min->SetLineWidth(2);
		unc_MuMu_min->SetLineStyle(2);
		unc_MuMu_max->SetLineWidth(2);
		unc_MuMu_max->SetLineStyle(2);
		unc_MuMu_max->Draw("same");
		unc_MuMu_min->Draw("same");
	}
	CMS_lumi(mycanvas_ratio_MuMu, "Simulation");


	TString outname = "";
	if (fitType == "pol0") {
		// outname = "_SFfixed"; // = a not fixed
		outname = "_pol0-200-4000"; 
	}
	if (fitType == "pol1") outname = "_pol1-200-4000";
	if (fitType == "pol2") outname = "_pol2-200-4000";
	if (fitType == "pol3") outname = "_pol3-200-4000";
	if (fitType == "pol4") outname = "_pol4-200-4000";
	if (fitType == "exp") outname = "_exp-200-4000";
	if (fitType == "inverseExp") outname = "_inverseexp-200-4000";
	if (fitType == "simpleExp") outname = "_simpleExp-200-4000";
	if (fitType == "log") outname = "_log-200-4000";
	if (fitType == "inverseLog") outname = "_inverselog-200-4000";
	if (fitType == "simpleLog") outname = "_simpleLog-200-4000";
	if (fitType == "pol1UncBand") outname = "_pol1_uncBand";


	mycanvas_ratio_EE->Print((outputdir+"flavor_ratio_EE_variablebinwidth"+outname+".pdf")); 
	mycanvas_ratio_EE->Print((outputdir+"flavor_ratio_EE_variablebinwidth"+outname+".png"));
	// mycanvas_ratio_MuMu->Print((outputdir+"flavor_ratio_MuMu_variablebinwidth"+outname+".pdf")); 
	// mycanvas_ratio_MuMu->Print((outputdir+"flavor_ratio_MuMu_variablebinwidth"+outname+".png"));

 // 	outputdir = "/afs/cern.ch/user/g/gnegro/www/cmsWR/TTstudies/logXscale/";
	// mycanvas_ratio_EE->SetLogx(1);
	// mycanvas_ratio_EE->Print((outputdir+"flavor_ratio_EE_variablebinwidth"+outname+"_logx.pdf"));
	// mycanvas_ratio_EE->Print((outputdir+"flavor_ratio_EE_variablebinwidth"+outname+"_logx.png"));
	// mycanvas_ratio_MuMu->SetLogx(1);
	// mycanvas_ratio_MuMu->Print((outputdir+"flavor_ratio_MuMu_variablebinwidth"+outname+"_logx.pdf"));
	// mycanvas_ratio_MuMu->Print((outputdir+"flavor_ratio_MuMu_variablebinwidth"+outname+"_logx.png"));

	TFile f("flavor_fits.root","RECREATE");
	h_ratio_EE->Write();
	h_ratio_MuMu->Write();
	f_EE->Write();
	f_MuMu->Write();


// 
	// outputdir = "/afs/cern.ch/user/g/gnegro/www/cmsWR/TTstudies/comparison/";
	// gStyle->SetOptStat("nemr");
	// TCanvas* canvMuMuEMu = new TCanvas("canvMuMuEMu","",600,600);
	// canvMuMuEMu->cd();
	// TLegend * legMuMuEMu = new TLegend(0.72,0.6,0.98,0.8);
	// legMuMuEMu->AddEntry(h_WR_mass_EMu,"EMu");
	// legMuMuEMu->AddEntry(h_WR_mass_MuMu,"MuMu");
	// h_WR_mass_EMu->Draw("histo");
	// h_WR_mass_MuMu->Draw("Psame");
	// legMuMuEMu->Draw();
	// canvMuMuEMu->SaveAs(outputdir+"emujj_and_mumujj_signal_region_variablebinwidth.pdf","recreate");
	// canvMuMuEMu->SaveAs(outputdir+"emujj_and_mumujj_signal_region_variablebinwidth.png","recreate");

	// TCanvas* canvMuMuEMuData = new TCanvas("canvMuMuEMuData","",600,600);
	// canvMuMuEMuData->cd();
	// TLegend * legMuMuEMuData = new TLegend(0.72,0.6,0.98,0.8);
	// // legMuMuEMuData->AddEntry(h_WR_mass_EMuData,"Rescaled EMu Data");
	// legMuMuEMuData->AddEntry(h_WR_mass_EMuDataMuMu,"Rescaled EMu Data");
	// legMuMuEMuData->AddEntry(h_WR_mass_MuMu,"MuMu MC");
	// h_WR_mass_MuMu->Draw("histo");
	// // h_WR_mass_EMuData->Scale(mumuEmuSF);
	// // h_WR_mass_EMuData->SetMarkerStyle(2);
	// // h_WR_mass_EMuData->SetMarkerSize(2);
	// // h_WR_mass_EMuData->Draw("Psame");
	// h_WR_mass_EMuDataMuMu->Scale(mumuEmuSF);
	// h_WR_mass_EMuDataMuMu->SetMarkerStyle(2);
	// h_WR_mass_EMuDataMuMu->SetMarkerSize(2);
	// h_WR_mass_EMuDataMuMu->Draw("Psame");
	// legMuMuEMuData->Draw();
	// canvMuMuEMuData->SaveAs(outputdir+"rescaled_emujj_data_and_mumujj_MC_signal_region_variablebinwidth.pdf","recreate");
	// canvMuMuEMuData->SaveAs(outputdir+"rescaled_emujj_data_and_mumujj_MC_signal_region_variablebinwidth.png","recreate");


	// TCanvas* canvEEEMu = new TCanvas("canvEEEMu","",600,600);
	// canvEEEMu->cd();
	// TLegend * legEEEMu = new TLegend(0.72,0.6,0.98,0.8);
	// legEEEMu->AddEntry(h_WR_mass_EMu,"EMu");
	// legEEEMu->AddEntry(h_WR_mass_EE,"EE");
	// h_WR_mass_EMu->Draw("histo");
	// h_WR_mass_EE->Draw("Psame");
	// legEEEMu->Draw();
	// canvEEEMu->SaveAs(outputdir+"emujj_and_eejj_signal_region_variablebinwidth.pdf","recreate");
	// canvEEEMu->SaveAs(outputdir+"emujj_and_eejj_signal_region_variablebinwidth.png","recreate");

	// TCanvas* canvEEEMuData = new TCanvas("canvEEEMuData","",600,600);
	// canvEEEMuData->cd();
	// TLegend * legEEEMuData = new TLegend(0.72,0.6,0.98,0.8);
	// // legEEEMuData->AddEntry(h_WR_mass_EMuData,"Rescaled EMu Data");
	// legEEEMuData->AddEntry(h_WR_mass_EMuDataEE,"Rescaled EMu Data");
	// legEEEMuData->AddEntry(h_WR_mass_EE,"EE MC");
	// h_WR_mass_EE->Draw("histo");
	// // h_WR_mass_EMuData->Scale(1/mumuEmuSF);	///<undo the scaling which was done earlier
	// // h_WR_mass_EMuData->SetMarkerStyle(2);
	// // h_WR_mass_EMuData->SetMarkerSize(2);
	// // h_WR_mass_EMuData->Scale(eeEmuSF);
	// // h_WR_mass_EMuData->Draw("Psame");
	// h_WR_mass_EMuDataEE->SetMarkerStyle(2);
	// h_WR_mass_EMuDataEE->SetMarkerSize(2);
	// h_WR_mass_EMuDataEE->Scale(eeEmuSF);
	// h_WR_mass_EMuDataEE->Draw("Psame");
	// legEEEMuData->Draw();
	// canvEEEMuData->SaveAs(outputdir+"rescaled_emujj_data_and_eejj_MC_signal_region_variablebinwidth.pdf","recreate");
	// canvEEEMuData->SaveAs(outputdir+"rescaled_emujj_data_and_eejj_MC_signal_region_variablebinwidth.png","recreate");


	// ///rescale the EEJJ and MuMuJJ MC and compare it to the EMu data and EMuJJ MC
	// gStyle->SetOptStat("");
	// TCanvas* canvEMuDataTwoRescaledMCs = new TCanvas("canvEMuDataTwoRescaledMCs","",600,600);
	// canvEMuDataTwoRescaledMCs->cd();
	// TLegend * legEMuDataTwoRescaledMCs = new TLegend(0.6,0.15,0.98,0.4);
	// // legEMuDataTwoRescaledMCs->AddEntry(h_WR_mass_EMuData,"EMu Data");
	// legEMuDataTwoRescaledMCs->AddEntry(h_WR_mass_EMuDataEE,"EMu Data EE");
	// legEMuDataTwoRescaledMCs->AddEntry(h_WR_mass_EMuDataMuMu,"EMu Data MuMu");
	// legEMuDataTwoRescaledMCs->AddEntry(h_WR_mass_EE,"Rescaled TTBar EE MC");
	// legEMuDataTwoRescaledMCs->AddEntry(h_WR_mass_MuMu,"Rescaled TTBar MuMu MC");
	// legEMuDataTwoRescaledMCs->AddEntry(h_WR_mass_EMu,"TTBar EMu MC");
	// h_WR_mass_EE->Scale(1/eeEmuSF);
	// h_WR_mass_MuMu->Scale(1/mumuEmuSF);
	// // h_WR_mass_EMuData->Scale(1/eeEmuSF);	///<undo the scaling which was done earlier
	// h_WR_mass_EMuDataEE->Scale(1/eeEmuSF);	///<undo the scaling which was done earlier
	// h_WR_mass_EMuDataMuMu->Scale(1/mumuEmuSF);	///<undo the scaling which was done earlier
	// h_WR_mass_EE->SetLineColor(kRed);
	// h_WR_mass_EE->SetLineWidth(3);
	// h_WR_mass_EMu->SetLineColor(kBlue);
	// h_WR_mass_EMu->SetLineWidth(3);
	// h_WR_mass_MuMu->SetLineColor(kBlack);
	// h_WR_mass_MuMu->SetLineWidth(3);
	// // h_WR_mass_EMuData->SetMarkerStyle(20);
	// // h_WR_mass_EMuData->SetMarkerSize(1);
	// // h_WR_mass_EMuData->SetMarkerColor(kBlack);
	// h_WR_mass_EMuDataEE->SetMarkerStyle(20);
	// h_WR_mass_EMuDataEE->SetMarkerSize(1);
	// h_WR_mass_EMuDataEE->SetMarkerColor(kBlack);
	// h_WR_mass_EMuDataMuMu->SetMarkerStyle(22);
	// h_WR_mass_EMuDataMuMu->SetMarkerSize(1);
	// h_WR_mass_EMuDataMuMu->SetMarkerColor(kBlack);


	// Double_t eps = 0.001;
	// TPad* p1 = new TPad("p1", "p1", 0, 0.25, 1, 1, 0);
	// p1->Draw();
	// TPad* p2 = new TPad("p2", "p2", 0, 0.1, 1, 0.25 + eps, 0);
	// p2->Draw();
	// p1->SetBottomMargin(0);
	// p2->SetTopMargin(0);
	// p1->cd();
	// // h_WR_mass_EMuData->SetStats(1);
	// h_WR_mass_EMuDataMuMu->SetStats(1);
	// h_WR_mass_EMuDataEE->SetStats(1);
	// h_WR_mass_MuMu->SetStats(1);
	// // TH1F *ratio_Data_EMu = (TH1F*) h_WR_mass_EMuData->Clone();
	// // TH1F *ratio_Data_EE = (TH1F*) h_WR_mass_EMuData->Clone();
	// // TH1F *ratio_Data_MuMu = (TH1F*) h_WR_mass_EMuData->Clone();
	// TH1F *ratio_Data_EE = (TH1F*) h_WR_mass_EMuDataEE->Clone();
	// TH1F *ratio_Data_MuMu = (TH1F*) h_WR_mass_EMuDataMuMu->Clone();

	// // h_WR_mass_EMuData->Draw("ep");
	// h_WR_mass_EMuDataMuMu->Draw("ep");
	// h_WR_mass_EMuDataEE->Draw("ep same");
	// h_WR_mass_EMu->Draw("histo same");
	// h_WR_mass_EE->Draw("histo same");
	// h_WR_mass_MuMu->Draw("histo same");
	// // h_WR_mass_EMuData->Draw("epsame"); // ??
	// TString ytitle = "Events/(";
	// // ytitle += (h_WR_mass_EMuData->GetXaxis()->GetNbins());
	// ytitle += (h_WR_mass_EMuDataMuMu->GetXaxis()->GetNbins());
	// ytitle += ")";
	// h_WR_mass_MuMu->GetYaxis()->SetTitle(ytitle.Data());
	// legEMuDataTwoRescaledMCs->Draw();
	// canvEMuDataTwoRescaledMCs->cd();
	// p2->cd();	///<change to ratio TPad
	// ratio_Data_EE->Sumw2();
	// ratio_Data_EE->SetStats(0);
	// // ratio_Data_EMu->Sumw2();
	// // ratio_Data_EMu->SetStats(0);
	// ratio_Data_MuMu->Sumw2();
	// ratio_Data_MuMu->SetStats(0);

	// ratio_Data_EE->Divide(h_WR_mass_EE);
	// ratio_Data_EE->SetMarkerStyle(20);
	// ratio_Data_EE->SetMarkerColor(kRed);
	// ratio_Data_EE->SetLabelSize(0.1, "y");
	// ratio_Data_EE->GetYaxis()->SetRangeUser(0.60, 1.4);
	// ratio_Data_EE->GetYaxis()->SetNdivisions(505);
	// ratio_Data_EE->GetXaxis()->SetTitle("M_{LLJJ} (GeV)");

	// // ratio_Data_EMu->Divide(h_WR_mass_EMu);  //cosa metto?
	// // ratio_Data_EMu->SetMarkerStyle(22);
	// // ratio_Data_EMu->SetMarkerColor(kBlue);
	// // ratio_Data_EMu->SetLabelSize(0.1, "y");
	// // ratio_Data_EMu->GetYaxis()->SetRangeUser(0.60, 1.4);
	// // ratio_Data_EMu->GetYaxis()->SetNdivisions(505);
	// // ratio_Data_EMu->GetXaxis()->SetTitle("M_{LLJJ} (GeV)");

	// ratio_Data_MuMu->Divide(h_WR_mass_MuMu);
	// ratio_Data_MuMu->SetMarkerStyle(21);
	// ratio_Data_MuMu->SetMarkerColor(kBlack);
	// ratio_Data_MuMu->SetLabelSize(0.1, "y");
	// ratio_Data_MuMu->GetYaxis()->SetRangeUser(0.60, 1.4);
	// ratio_Data_MuMu->GetYaxis()->SetNdivisions(505);
	// ratio_Data_MuMu->GetXaxis()->SetTitle("M_{LLJJ} (GeV)");


	// ratio_Data_EE->Draw("p");
	// // ratio_Data_EMu->Draw("p");
	// ratio_Data_MuMu->Draw("p");
	// float xmax = ratio_Data_MuMu->GetXaxis()->GetXmax();
	// float xmin = ratio_Data_MuMu->GetXaxis()->GetXmin();
	// TF1 *f1 = new TF1("f1","1", xmin, xmax);
	// ratio_Data_EE->Draw("p");
	// // ratio_Data_EMu->Draw("psame");
	// ratio_Data_MuMu->Draw("psame");
	// f1->Draw("same");
	// canvEMuDataTwoRescaledMCs->cd();
	// canvEMuDataTwoRescaledMCs->Update();
	// canvEMuDataTwoRescaledMCs->SaveAs(outputdir+"emujj_data_and_MC_and_rescaled_eejj_and_mumujj_MC_signal_region_variablebinwidth.pdf","recreate");
	// canvEMuDataTwoRescaledMCs->SaveAs(outputdir+"emujj_data_and_MC_and_rescaled_eejj_and_mumujj_MC_signal_region_variablebinwidth.png","recreate");

	// p1->SetLogy();
	// canvEMuDataTwoRescaledMCs->SaveAs(outputdir+"emujj_data_and_MC_and_rescaled_eejj_and_mumujj_MC_signal_region_log_variablebinwidth.pdf","recreate");
	// canvEMuDataTwoRescaledMCs->SaveAs(outputdir+"emujj_data_and_MC_and_rescaled_eejj_and_mumujj_MC_signal_region_log_variablebinwidth.png","recreate");
	// canvEMuDataTwoRescaledMCs->Close();
	
}

void fillHisto(TChain * chain, Selector *myEvent, TH1F * h){

	Long64_t nEntries = chain->GetEntries();

	cout<< nEntries << endl;

	for(int ev = 0; ev<nEntries; ++ev){
		chain->GetEntry(ev);
		if(myEvent->WR_mass > 600. && myEvent->dilepton_mass > 200.) 
			h->Fill(myEvent->WR_mass,myEvent->weight);
	}
	std::cout<<"histo named\t"<< h->GetName() <<"\thas integral\t"<< h->Integral() <<"\thas entries\t"<< h->GetEntries() <<std::endl;
}

//call this fxn once TF1 is fitted to distribution
std::string chiSquaredNdofString(TF1 * fit){
	std::string tempchiSqd = "#chi^{2}/ndof  =  ";
	std::string chiSqdVal = to_string(fit->GetChisquare());
	std::string ndof = to_string(fit->GetNDF());
	tempchiSqd += chiSqdVal.substr(0,4);
	tempchiSqd += " / ";
	tempchiSqd += ndof.substr(0,2);
	tempchiSqd += " = ";
	tempchiSqd += to_string(fit->GetChisquare()/fit->GetNDF());
	return tempchiSqd;
}

