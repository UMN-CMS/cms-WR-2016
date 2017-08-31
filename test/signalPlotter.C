#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"
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

void MakeHistos(TChain* chain, Selector *myEvent, std::vector<TH1F*> *hs, Selector::tag_t channel);
// void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_others,TH1F* hs_data,TH1F* hs_signal_1,TH1F* hs_signal_2,TH1F* hs_signal_3,TString xtitle, TString ytitle, TString fname, Selector::tag_t channel);
void drawPlots(TH1F* hs_WJets, TH1F* hs_singleTop, TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_others,TH1F* hs_data,TH1F* hs_signal_1,TH1F* hs_signal_2,TH1F* hs_signal_3,TString xtitle, TString ytitle, TString fname, Selector::tag_t channel);
void Plotter(Selector::tag_t channel);

void signalPlotter(){
	// std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE};
	// for(auto c: channels)
	// 	Plotter(c);
	// Plotter(Selector::EE);
	Plotter(Selector::MuMu);
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
	TChain * chain_singleTop = new TChain("Tree_Iter0");

	// Signal Points
	TChain * chain_signal_1 = new TChain("Tree_Iter0");
	TChain * chain_signal_2 = new TChain("Tree_Iter0");
	TChain * chain_signal_3 = new TChain("Tree_Iter0");

	TString inputDirDY = "/afs/cern.ch/work/g/gnegro/NuAnalysis-cmsWR16_afterPreApproval/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/selectedTreesWRv07_DY-EWK/"; 
	TString inputDir = "/afs/cern.ch/work/g/gnegro/NuAnalysis-cmsWR16_afterPreApproval/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/selectedTreesWRv07/"; 
	TString inputDirData = "/afs/cern.ch/work/g/gnegro/NuAnalysis-cmsWR16_afterPreApproval/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/selectedTreesWRv07_signalRegion/"; 

	switch (channel) {
	case Selector::EE:
		chain_DY->Add(inputDirDY+"selected_tree_DYAMCPT_1_signal_eeEE.root");
		chain_ttbar->Add(inputDir+"selected_tree_data_flavoursidebandEMu.root");
		chain_others->Add(inputDir+"selected_tree_Other_signal_eeEE.root");
		chain_data->Add(inputDirData+"selected_tree_data_signal_eeEE.root"); 
		chain_signal_1->Add(inputDir+"selected_tree_WRtoEEJJ_2000_1000_signal_eeEE.root");
		chain_signal_2->Add(inputDir+"selected_tree_WRtoEEJJ_3000_1500_signal_eeEE.root");
		chain_signal_3->Add(inputDir+"selected_tree_WRtoEEJJ_4000_2000_signal_eeEE.root");
		chain_singleTop->Add(inputDirData+"selected_tree_SingleTop_signal_eeEE.root");
		chain_WJets->Add(inputDirData+"selected_tree_W_signal_eeEE.root");
		break;
	case Selector::MuMu:
		chain_DY->Add(inputDirDY+"selected_tree_DYAMCPT_1_signal_mumuMuMu.root");
		chain_ttbar->Add(inputDir+"selected_tree_data_flavoursidebandEMu.root");
		chain_others->Add(inputDir+"selected_tree_Other_signal_mumuMuMu.root");
		chain_data->Add(inputDirData+"selected_tree_data_signal_mumuMuMu.root");
		chain_signal_1->Add(inputDir+"selected_tree_WRtoMuMuJJ_2000_1000_signal_mumuMuMu.root");
		chain_signal_2->Add(inputDir+"selected_tree_WRtoMuMuJJ_3000_1500_signal_mumuMuMu.root");
		chain_signal_3->Add(inputDir+"selected_tree_WRtoMuMuJJ_4000_2000_signal_mumuMuMu.root");
		chain_singleTop->Add(inputDirData+"selected_tree_SingleTop_signal_mumuMuMu.root");
		// chain_WJets->Add(inputDirData+"selected_tree_W_signal_mumuMuMu.root");
		chain_WJets->Add(inputDirData+"selected_tree_WJets_HT_signal_mumuMuMu.root");
		break;
	default:
		std::cout << "Unknown tag" << std::endl;
	}

	Selector myEvent_DY;
	Selector myEvent_ttbar;
	Selector myEvent_others;
	Selector myEvent_WJets;
	Selector myEvent_WZ;
	Selector myEvent_ZZ;
	Selector myEvent_WW;
	Selector myEvent_data;
	Selector myEvent_singleTop;
	Selector myEvent_signal_1;
	Selector myEvent_signal_2;
	Selector myEvent_signal_3;

	myEvent_DY.SetBranchAddresses(chain_DY);
	myEvent_ttbar.SetBranchAddresses(chain_ttbar);
	myEvent_others.SetBranchAddresses(chain_others);
	myEvent_WJets.SetBranchAddresses(chain_WJets);
	myEvent_WZ.SetBranchAddresses(chain_WZ);
	myEvent_ZZ.SetBranchAddresses(chain_ZZ);
	myEvent_WW.SetBranchAddresses(chain_WW);
	myEvent_data.SetBranchAddresses(chain_data);
	myEvent_singleTop.SetBranchAddresses(chain_singleTop);
	myEvent_signal_1.SetBranchAddresses(chain_signal_1);
	myEvent_signal_2.SetBranchAddresses(chain_signal_2);
	myEvent_signal_3.SetBranchAddresses(chain_signal_3);

	std::vector<TH1F*> hs_DY;
	MakeHistos(chain_DY, &myEvent_DY, &hs_DY, channel);
	std::vector<TH1F*> hs_ttbar;
	MakeHistos(chain_ttbar, &myEvent_ttbar, &hs_ttbar, channel);
	std::vector<TH1F*> hs_others;
	MakeHistos(chain_others, &myEvent_others, &hs_others, channel);
	std::vector<TH1F*> hs_WJets;
	MakeHistos(chain_WJets, &myEvent_WJets, &hs_WJets, channel);
	std::vector<TH1F*> hs_WZ;
	MakeHistos(chain_WZ, &myEvent_WZ, &hs_WZ, channel);
	std::vector<TH1F*> hs_ZZ;
	MakeHistos(chain_ZZ, &myEvent_ZZ, &hs_ZZ, channel);
	std::vector<TH1F*> hs_WW;
	MakeHistos(chain_WW, &myEvent_WW, &hs_WW, channel);

	std::vector<TH1F*> hs_data;
	MakeHistos(chain_data, &myEvent_data, &hs_data, channel);
	std::vector<TH1F*> hs_singleTop;
	MakeHistos(chain_singleTop, &myEvent_singleTop, &hs_singleTop, channel);

	std::vector<TH1F*> hs_signal_1;
	MakeHistos(chain_signal_1, &myEvent_signal_1, &hs_signal_1, channel);
	std::vector<TH1F*> hs_signal_2;
	MakeHistos(chain_signal_2, &myEvent_signal_2, &hs_signal_2, channel);
	std::vector<TH1F*> hs_signal_3;
	MakeHistos(chain_signal_3, &myEvent_signal_3, &hs_signal_3, channel);

	unsigned int nPlots = hs_DY.size();

	// hs_data[13]->SetLineColor(kRed);
	// hs_data[13]->Draw();
	// hs_ttbar[13]->Draw("same");
	
	TString xtitles[] = {"leading lepton p_{T} (GeV)","subleading lepton p_{T} (GeV)","leading jet p_{T} (GeV)","subleading jet p_{T} (GeV)","leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi","Mlljj (GeV)","dilepton mass (GeV)","nPV","HT","dilepton p_{T} (GeV)","Ml_{1}jj (GeV)","Ml_{2}jj (GeV)", "NJets"};

	TString ytitles[] = {"dN/dp_{T} (GeV^{-1})","dN/dp_{T} (GeV^{-1})","dN/dp_{T} (GeV^{-1})","dN/dp_{T} (GeV^{-1})","dN/d#eta","dN/d#eta","dN/d#eta","dN/d#eta","dN/d#phi","dN/d#phi","dN/d#phi","dN/d#phi","dN/dMlljj (GeV^{-1})","dN/dmass (GeV^{-1})","dN/dnPV","dN/dHT","dN/dp_{T} (GeV^{-1})","dN/dMl_{1}jj (GeV^{-1})","dM/dMl_{2}jj (GeV^{-1})", "dN/dNJets"};

	TString fnames[] = {"l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","Mlljj","Mll","nPV","HT","pT_ll","Ml1jj","Ml2jj","njets"};

	int i = 0;
	for(unsigned int i = 0; i < nPlots; i++){
		std::string s = std::to_string(i);
		// drawPlots(hs_DY[i],hs_ttbar[i],hs_others[i],hs_data[i],hs_signal_1[i],hs_signal_2[i],hs_signal_3[i],xtitles[i],ytitles[i],fnames[i], channel);
		drawPlots(hs_WJets[i], hs_singleTop[i],hs_DY[i],hs_ttbar[i],hs_others[i],hs_data[i],hs_signal_1[i],hs_signal_2[i],hs_signal_3[i],xtitles[i],ytitles[i],fnames[i], channel);		
	}
	
}

void MakeHistos(TChain * chain, Selector *myEvent, std::vector<TH1F*> *hs, Selector::tag_t channel){

	TH1F *h_lepton_pt0 = new TH1F("h_lepton_pt0","",40,0,700);
	TH1F *h_lepton_eta0 = new TH1F("h_lepton_eta0","",40,-3,3);
	TH1F *h_lepton_phi0 = new TH1F("h_lepton_phi0","",40,-3.15,3.15);
	TH1F *h_lepton_pt1 = new TH1F("h_lepton_pt1","",40,0,700);
	TH1F *h_lepton_eta1 = new TH1F("h_lepton_eta1","",40,-3,3);
	TH1F *h_lepton_phi1 = new TH1F("h_lepton_phi1","",40,-3.15,3.15);

	TH1F *h_jet_pt0 = new TH1F("h_jet_pt0","",40,0,700);
	TH1F *h_jet_eta0 = new TH1F("h_jet_eta0","",40,-3,3);
	TH1F *h_jet_phi0 = new TH1F("h_jet_phi0","",40,-3.15,3.15);
	TH1F *h_jet_pt1 = new TH1F("h_jet_pt1","",40,0,700);
	TH1F *h_jet_eta1 = new TH1F("h_jet_eta1","",40,-3,3);
	TH1F *h_jet_phi1 = new TH1F("h_jet_phi1","",40,-3.15,3.15);

	//TH1F *h_WR_mass = new TH1F("h_WR_mass","",40,0,6000);

	Float_t bins[] = { 150,300, 450, 600, 750, 900, 1050, 1200, 1350, 1500, 1650,1800,1950,2100,2250,2400,2550,2700,2850,3000,3150,3300,3450,3600,3750,3900,4150,7000 };
	Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;
	TH1F *h_WR_mass = new TH1F("h_WR_mass","",binnum,bins);

	
	float dilepton_max = 1000.;
	if(channel == Selector::EMu)
		dilepton_max = 1000;
	TH1F *h_dilepton_mass = new TH1F("h_dilepton_mass","",40,200,dilepton_max);
	TH1F *h_nPV = new TH1F("h_nPV","",40,0,50);
	TH1F *h_HT = new TH1F("h_HT","",40,0,3000);
	TH1F *h_pT_ll = new TH1F("h_pT_ll","",40,0,1000);
	TH1F *h_Ml1jj = new TH1F("h_Ml1jj","",40,0,3000);
	TH1F *h_Ml2jj = new TH1F("h_Ml2jj","",40,0,3000);
	TH1F *h_njets = new TH1F("h_njets","",10,1,10);

	Long64_t nEntries = chain->GetEntries();

	cout<< nEntries << endl;

	for(int ev = 0; ev<nEntries; ++ev){
		chain->GetEntry(ev);

		h_lepton_pt0->Fill(myEvent->lead_lepton_pt,myEvent->weight);
		h_lepton_pt1->Fill(myEvent->sublead_lepton_pt,myEvent->weight);
		h_lepton_eta0->Fill(myEvent->lead_lepton_eta,myEvent->weight);
		h_lepton_eta1->Fill(myEvent->sublead_lepton_eta,myEvent->weight);
		h_lepton_phi0->Fill(myEvent->lead_lepton_phi,myEvent->weight);
		h_lepton_phi1->Fill(myEvent->sublead_lepton_phi,myEvent->weight);

		h_jet_pt0->Fill(myEvent->lead_jet_pt,myEvent->weight);
		h_jet_pt1->Fill(myEvent->sublead_jet_pt,myEvent->weight);
		h_jet_eta0->Fill(myEvent->lead_jet_eta,myEvent->weight);
		h_jet_eta1->Fill(myEvent->sublead_jet_eta,myEvent->weight);
		h_jet_phi0->Fill(myEvent->lead_jet_phi,myEvent->weight);
		h_jet_phi1->Fill(myEvent->sublead_jet_phi,myEvent->weight);
			
		h_WR_mass->Fill(myEvent->WR_mass,myEvent->weight);
		h_dilepton_mass->Fill(myEvent->dilepton_mass,myEvent->weight);
		h_nPV->Fill(myEvent->nPV,myEvent->weight);
		h_HT->Fill(myEvent->HT,myEvent->weight);
		h_pT_ll->Fill(myEvent->dilepton_pt,myEvent->weight);
		h_Ml1jj->Fill(myEvent->N1_mass,myEvent->weight);
		h_Ml2jj->Fill(myEvent->N2_mass,myEvent->weight);
		h_njets->Fill(myEvent->njets,myEvent->weight);
	}

	h_WR_mass->GetYaxis()->SetRangeUser(0.001,500);
	
	hs->push_back(h_lepton_pt0);
	hs->push_back(h_lepton_pt1);
	hs->push_back(h_jet_pt0);
	hs->push_back(h_jet_pt1);
	hs->push_back(h_lepton_eta0);
	hs->push_back(h_lepton_eta1);
	hs->push_back(h_jet_eta0);
	hs->push_back(h_jet_eta1);
	hs->push_back(h_lepton_phi0);
	hs->push_back(h_lepton_phi1);
	hs->push_back(h_jet_phi0);
	hs->push_back(h_jet_phi1);
	hs->push_back(h_WR_mass);
	hs->push_back(h_dilepton_mass);
	hs->push_back(h_nPV);
	hs->push_back(h_HT);
	hs->push_back(h_pT_ll);
	hs->push_back(h_Ml1jj);
	hs->push_back(h_Ml2jj);
	hs->push_back(h_njets);

}


// void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_others,TH1F* hs_data,TH1F* hs_signal_1,TH1F* hs_signal_2,TH1F* hs_signal_3, TString xtitle, TString ytitle, TString fname, Selector::tag_t channel){
void drawPlots(TH1F* hs_WJets, TH1F* hs_singleTop,TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_others,TH1F* hs_data,TH1F* hs_signal_1,TH1F* hs_signal_2,TH1F* hs_signal_3, TString xtitle, TString ytitle, TString fname, Selector::tag_t channel){

	TLegend *leg = new TLegend( 0.52, 0.60, 0.98, 0.90 ) ; 
	leg->AddEntry( hs_DY, "Z/#gamma* + jets" ) ; 
	leg->AddEntry( hs_ttbar, "t#bar{t}" ) ;
	leg->AddEntry( hs_others, "Other background" ) ;  
	leg->AddEntry( hs_signal_1, "WR M=2000 GeV, NR M=1000 GeV" ) ;  
	leg->AddEntry( hs_signal_2, "WR M=3000 GeV, NR M=1500 GeV" ) ;  
	leg->AddEntry( hs_signal_3, "WR M=4000 GeV, NR M=2000 GeV" ) ;  
	//leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
	//leg->AddEntry( hs_data, "Data");
	leg->SetFillColor( kWhite ) ; 


	TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
	THStack* th = new THStack();
	hs_DY->SetFillColor(kYellow);
	hs_ttbar->SetFillColor(kGreen);
	hs_others->SetFillColor(kGreen+3);
	hs_DY->SetLineColor(kBlack);
	hs_ttbar->SetLineColor(kBlack);
	hs_others->SetLineColor(kBlack);
	hs_signal_1->SetLineColor(kMagenta);
	hs_signal_1->SetFillColor(0);
	hs_signal_2->SetLineColor(kCyan);
	hs_signal_2->SetFillColor(0);
	hs_signal_3->SetLineColor(kBlue);
	hs_signal_3->SetFillColor(0);

	// hs_ttbar->Scale(2.6/35.8);
	// hs_DY->Scale(1.2*2.6/35.8);
	// hs_others->Scale(2.6/35.8);

	if(channel == Selector::EE)
		hs_ttbar->Scale(0.418);                 
	else if(channel == Selector::MuMu)
		hs_ttbar->Scale(0.691);
	 
	th->Add(hs_others);
	th->Add(hs_DY);
	th->Add(hs_ttbar);
	hs_data->SetMarkerStyle(20);

	Double_t eps = 0.001;
	TPad* p1 = new TPad("p1","p1",0,0.25,1,1,0); p1->Draw();
	TPad* p2 = new TPad("p2","p2",0,0,1,0.25+eps,0); p2->Draw();
	p1->SetBottomMargin(0);
	p2->SetTopMargin(0);   
	p2->SetBottomMargin(0.5); 
	p1->cd();
	gPad->SetTickx();
	gPad->SetTicky();
	hs_data->SetStats(0);
	TH1F *ratio = (TH1F*)hs_data->Clone();
	th->SetTitle("CMS Preliminary            35.9 fb^{-1} (13 TeV)");
	
		if(xtitle == "Mlljj [GeV]"){
			//th->GetYaxis()->SetRangeUser(0.1,500);
			std::cout<<"Mlljj="<<((TH1*)th->GetStack()->Last())->GetRMS()<<std::endl;
		}
	
	th->Draw("histo");
	//hs_data->Draw();
	 th->Draw("histo same");
	// hs_data->Draw("epsame");


	
	hs_data->SetTitle("CMS Preliminary            35.9 fb^{-1} (13 TeV)");
	gStyle->SetTitleFont(62,"");
	
	hs_signal_1->Draw("histo same");
	hs_signal_2->Draw("histo same");
	hs_signal_3->Draw("histo same");

	TH1F *errors = (TH1F*)(th->GetStack()->Last())->Clone();
	TFile *scale_error_fn = new TFile("data/scale_syst.root");
	TFile *pdf_error_fn = new TFile("data/pdf_syst.root");
	TFile *fact_renorm_error_fn = new TFile("data/fact_renorm_syst.root");

	TString hname = "";

	if(fname == "Mlljj") {
		if(channel == Selector::EE) hname = "hWR_mass_binned_EE";
		if(channel == Selector::MuMu) hname = "hWR_mass_binned_MuMu";

		if(channel != Selector::EMu){
			TH1F *scale_error_hist = (TH1F*) scale_error_fn->Get(hname);
			TH1F *pdf_error_hist = (TH1F*) pdf_error_fn->Get(hname);
			TH1F *fact_renorm_error_hist = (TH1F*) fact_renorm_error_fn->Get(hname);
			
			for(int i = 0;i<errors->GetNbinsX()+1;i++){
				float errorSum = TMath::Sqrt(
					(scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
					+ 
					(pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
					+ 
					(fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
					+ 
					(errors->GetBinError(i)*errors->GetBinError(i))
					);  

				// float systErrorSum = TMath::Sqrt(
				//   (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
				//   + 
				//   (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
				//   + 
				//   (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*(fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) 
				//   );  

				// cout<<i<<" errorSum = "<<errorSum<<endl;
				// cout<<i<<" systErrorSum = "<<systErrorSum<<endl;
				errors->SetBinError(i,errorSum);
				// systErrors->SetBinError(i,systErrorSum);
			}
		}
	}


	errors->SetLineColor(0);
	errors->SetFillColor(1);
	errors->SetFillStyle(3254);
	errors->Draw("E2 same");
	// TString ytitle = "Events/(";
	// ytitle += (th->GetXaxis()->GetNbins());
	// ytitle += ")";
	th->GetYaxis()->SetTitle(ytitle.Data());
	th->GetXaxis()->SetTitle(xtitle.Data());
	th->GetYaxis()->SetTitleSize(0.05);
	th->GetYaxis()->SetTitleOffset(0.9); 
	th->GetYaxis()->SetLabelSize(0.04);	


	// TString ytitle = "Events/(";
	// ytitle += (hs_data->GetXaxis()->GetNbins());
	// ytitle += ")";
	// hs_data->GetYaxis()->SetTitle(ytitle);
	// hs_data->GetYaxis()->SetTitleSize(0.05);
	// hs_data->GetYaxis()->SetTitleOffset(0.9); 
	// hs_data->GetYaxis()->SetLabelSize(0.04);	

	//cout<<"Bins1="<<((TH1*)th->GetStack()->Last())->FindBin(80)<<std::endl;
	//cout<<"Bins2="<<((TH1*)th->GetStack()->Last())->FindBin(100)<<std::endl;
	
	// cout<<"Integral="<<((TH1*)th->GetStack()->Last())->Integral(11,31)<<" "<<hs_data->Integral(11,31)<<endl;
	
	ratio->GetXaxis()->SetTitle(xtitle.Data());
	ratio->GetXaxis()->SetTickSize(0.40);
	ratio->GetXaxis()->SetTitleSize(0.18);
	ratio->SetLabelSize(0.15,"x");
	leg->Draw(); 
	mycanvas->cd();
	p2->cd();
	p2->SetGridy(); 
	ratio->Sumw2();
	ratio->SetStats(0);

	if(xtitle == "Mlljj (GeV)"){
		if(channel == Selector::EE) ratio->GetXaxis()->SetTitle("M_{eejj} (GeV)"); 
		if(channel == Selector::MuMu) ratio->GetXaxis()->SetTitle("M_{#mu#mujj} (GeV)");


		// // float sumBinContent = 0.;
		// for(int i = 0;i<hs_singleTop->GetNbinsX()+1;i++){	
		// 	cout << "bin " << hs_singleTop->GetBinCenter(i) << ": histo singleTop =  " << hs_singleTop->GetBinContent(i) << " +/- " << hs_singleTop->GetBinError(i) << endl;
		// 	// sumBinContent += hs_singleTop->GetBinContent(i);
		// 	if (hs_singleTop->GetBinCenter(i)==2025) cout << "bin centered in 2025: " << i << endl;
		// 	if (hs_singleTop->GetBinCenter(i)==2625) cout << "bin centered in 2625: " << i << endl;
		// }

		// float integral = sumBinContent/hs_singleTop->GetNbinsX();
		// cout<<"Integral singleTop="<<hs_singleTop->Integral()<<", sumBinContent="<<sumBinContent<<", nBins="<<hs_singleTop->GetNbinsX()<<", integral="<<integral<<endl;

		cout<<"Integrals: data="<<hs_data->Integral()<<", DY="<<hs_DY->Integral()<<", ttbar="<<hs_ttbar->Integral()<<", Other="<<hs_others->Integral()<<", singleTop="<<hs_singleTop->Integral()<<endl;
		cout<<"Integral totalBkg="<<(hs_DY->Integral()+hs_ttbar->Integral()+hs_others->Integral())<<endl;

		cout<<"Integral singleTop: for mlljj>2TeV="<<hs_singleTop->Integral(13,hs_singleTop->GetNbinsX())<<", for mlljj>2.5TeV="<<hs_singleTop->Integral(17,hs_singleTop->GetNbinsX())<<endl;

		// for(int i = 0;i<hs_WJets->GetNbinsX()+1;i++){	
		// 	cout << "bin " << hs_WJets->GetBinCenter(i) << ": histo WJets =  " << hs_WJets->GetBinContent(i) << " +/- " << hs_WJets->GetBinError(i) << endl;
		// }

		cout<<"Integral WJets="<<hs_WJets->Integral()<<endl;


		TFile *f = new TFile("/afs/cern.ch/user/g/gnegro/work/NuAnalysis-cmsWR16_afterPreApproval/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/selected_data.root"); 
		TH1F *hs_QCD1 = (TH1F*)f->Get("h_wrmass1"); 
		TH1F *hs_QCD2 = (TH1F*)f->Get("h_wrmass2"); 
		TH1F *hs_QCD3 = (TH1F*)f->Get("h_wrmass3"); 
		TH1F *hs_QCD4 = (TH1F*)f->Get("h_wrmass4"); 
		TH1F *hs_QCD5 = (TH1F*)f->Get("h_wrmass5"); 

		cout<<"Integral QCD 1 = "<<hs_QCD1->Integral()<<endl;
		cout<<"Integral QCD 2 = "<<hs_QCD2->Integral()<<endl;
		cout<<"Integral QCD 3 = "<<hs_QCD3->Integral()<<endl;
		cout<<"Integral QCD 4 = "<<hs_QCD4->Integral()<<endl;
		cout<<"Integral QCD 5 = "<<hs_QCD5->Integral()<<endl;

	}

	hs_DY->Add(hs_ttbar);
	hs_DY->Add(hs_others);

	ratio->Divide(hs_DY);
	ratio->SetMarkerStyle(21);
	ratio->SetMarkerSize(0.5);
	ratio->SetLabelSize(0.12,"y");
	ratio->GetYaxis()->SetRangeUser(0.5,1.5);
	ratio->GetYaxis()->SetNdivisions(505);
	ratio->Draw("p");
	float xmax = ratio->GetXaxis()->GetXmax();
	float xmin = ratio->GetXaxis()->GetXmin();
	TF1 *f1 = new TF1("f1","1",xmin,xmax);
	ratio->Draw("p");
	f1->Draw("same");
	mycanvas->cd();

	TString fn = "";

	// if(channel == Selector::EMu)
	//   fn = "~/www/WR/plots/miniTree/Selected/Flavor/"+fname;
		//fn = "plots/Flavor/"+fname;
	if(channel == Selector::EE)
		fn = "/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/EWK-NLO_corrections/EESignalRegion/"+fname;
	//fn = "plots/EELowDilepton/"+fname;
	if(channel == Selector::MuMu)
		fn = "/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/EWK-NLO_corrections/MuMuSignalRegion/"+fname;
		//fn = "plots/MuMuLowDilepton/"+fname;

	// mycanvas->Print((fn+".pdf").Data());
	// mycanvas->Print((fn+".png").Data());
	// p1->SetLogy();
	// mycanvas->Print((fn+"_log.pdf").Data());
	// mycanvas->Print((fn+"_log.png").Data());

	mycanvas->Close();
}
