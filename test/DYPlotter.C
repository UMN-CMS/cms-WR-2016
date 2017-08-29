#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
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

void MakeHistos(TChain* chain, Selector *myEvent, std::vector<TH1F*> *hs, Selector::tag_t channel);
//void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_WJets,TH1F* hs_WZ,TH1F* hs_ZZ,TH1F* hs_WW,TH1F* hs_data, TString xtitle, TString fname);
void drawPlots(TH1F* hs_signal,TH1F* hs_lowdilepton, TString xtitle, TString fname, Selector::tag_t channel);
void Plotter(Selector::tag_t channel);

void DYPlotter(){
  std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE};
  for(auto c: channels)
    Plotter(c);
  //Plotter(Selector::MuMu);

}


void Plotter(Selector::tag_t channel){
  TChain * chain_signal = new TChain("Tree_Iter0");
  TChain * chain_lowdilepton = new TChain("Tree_Iter0");

  TString inputDir = "/afs/cern.ch/work/g/gnegro/NuAnalysis-cmsWR16_afterPreApproval/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/selectedTreesWRv07_DY-EWK/";  

  switch (channel) {
  case Selector::EE:
    chain_signal->Add(inputDir+"selected_tree_DYAMCPT_1_signal_eeEE.root");
    chain_lowdilepton->Add(inputDir+"selected_tree_DYAMCPT_lowdileptonsidebandEE.root");
    break;
  case Selector::MuMu:
    chain_signal->Add(inputDir+"selected_tree_DYAMCPT_1_signal_mumuMuMu.root");
    chain_lowdilepton->Add(inputDir+"selected_tree_DYAMCPT_lowdileptonsidebandMuMu.root");    
    break;
  default:
    std::cout << "Unknown tag" << std::endl;
  }

  Selector myEvent_signal;
  Selector myEvent_lowdilepton;

  myEvent_signal.SetBranchAddresses(chain_signal);
  myEvent_lowdilepton.SetBranchAddresses(chain_lowdilepton);

  std::vector<TH1F*> hs_signal;
  MakeHistos(chain_signal, &myEvent_signal, &hs_signal, channel);
  std::vector<TH1F*> hs_lowdilepton;
  MakeHistos(chain_lowdilepton, &myEvent_lowdilepton, &hs_lowdilepton, channel);


  unsigned int nPlots = hs_signal.size();

  // hs_data[13]->SetLineColor(kRed);
  // hs_data[13]->Draw();
  // hs_ttbar[13]->Draw("same");
  
  TString xtitles[] = {"leading lepton p_{T}","subleading lepton p_{T}","leading jet p_{T}","subleading jet p_{T}","leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi","Mlljj [GeV]","Mlljj [GeV]","dilepton mass [GeV]","nPV","HT","dilepton p_{T}","Ml_1jj [GeV]","Ml_2jj [GeV]", "NJets"};

  TString fnames[] = {"l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","Mlljj","Mlljj_binned","Mll","nPV","HT","pT_ll","Ml1jj","Ml2jj","njets"};

  int i = 0;
  for(unsigned int i = 0; i < nPlots; i++){
    std::string s = std::to_string(i);
    drawPlots(hs_signal[i],hs_lowdilepton[i],xtitles[i],fnames[i], channel);
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

  Float_t bins[] = { 150,300, 450, 600, 750, 900, 1050, 1200, 1350, 1500, 1650,1800,1950,2100,2250,2400,2550,2700,2850,3000,3150,3300,3450,3600,3750,3900,4150,7000 };
  //Float_t bins[] = { 300, 500, 700, 1030, 1300, 4000,6000 };
  Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;
  TH1F *h_WR_mass_binned = new TH1F("h_WR_mass_binned","",binnum,bins);
  
  TH1F *h_WR_mass = new TH1F("h_WR_mass","",40,0,7000);
  float dilepton_max = 110.;
  if(channel == Selector::EMu)
    dilepton_max = 1000;
  TH1F *h_dilepton_mass = new TH1F("h_dilepton_mass","",40,70,dilepton_max);
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
    h_WR_mass_binned->Fill(myEvent->WR_mass,myEvent->weight);    
    h_dilepton_mass->Fill(myEvent->dilepton_mass,myEvent->weight);
    h_nPV->Fill(myEvent->nPV,myEvent->weight);
    h_HT->Fill(myEvent->HT,myEvent->weight);
    h_pT_ll->Fill(myEvent->dilepton_pt,myEvent->weight);
    h_Ml1jj->Fill(myEvent->N1_mass,myEvent->weight);
    h_Ml2jj->Fill(myEvent->N2_mass,myEvent->weight);
    h_njets->Fill(myEvent->njets,myEvent->weight);
  }

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
  hs->push_back(h_WR_mass_binned);
  hs->push_back(h_dilepton_mass);
  hs->push_back(h_nPV);
  hs->push_back(h_HT);
  hs->push_back(h_pT_ll);
  hs->push_back(h_Ml1jj);
  hs->push_back(h_Ml2jj);
  hs->push_back(h_njets);
}

void drawPlots(TH1F* hs_signal,TH1F* hs_lowdilepton, TString xtitle, TString fname, Selector::tag_t channel){

  TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  leg->AddEntry( hs_signal, "Signal region" ) ; 
  leg->AddEntry( hs_lowdilepton, "Low dilepton SB" ) ;
  leg->SetFillColor( kWhite ) ; 


  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  hs_signal->SetLineColor(kBlack);
  hs_lowdilepton->SetLineColor(kRed);

  Double_t eps = 0.001;
  TPad* p1 = new TPad("p1","p1",0,0.25,1,1,0); p1->Draw();
  TPad* p2 = new TPad("p2","p2",0,0.1,1,0.25+eps,0); p2->Draw();
  p1->SetBottomMargin(0);
  p2->SetTopMargin(0);   
  p1->cd();
  gPad->SetTickx();
  gPad->SetTicky();
  hs_signal->SetStats(0);

  hs_lowdilepton->Scale(1/hs_lowdilepton->Integral());
  hs_signal->Scale(1/hs_signal->Integral());
  
  TH1F *ratio = (TH1F*)hs_lowdilepton->Clone();
  
  hs_signal->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_lowdilepton->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  // hs_signal->DrawNormalized("histo");
  // hs_lowdilepton->DrawNormalized("histo same");
  hs_signal->DrawNormalized("ep");
  hs_lowdilepton->DrawNormalized("ep same");  
  //hs_data->Draw("epsame");
  //TH1F *errors = (TH1F*)(th->GetStack()->Last())->Clone();
  //errors->SetLineColor(0);
  // errors->SetFillColor(1);
  // errors->SetFillStyle(3254);
  // errors->Draw("E2 same");
  TString ytitle = "Events/(";
  ytitle += (hs_signal->GetXaxis()->GetNbins());
  ytitle += ")";
  hs_signal->GetYaxis()->SetTitle(ytitle.Data());
  hs_signal->GetXaxis()->SetTitle(xtitle.Data());
  
  ratio->GetXaxis()->SetTitle(xtitle.Data());
  //ths[icanvas]->GetXaxis()->SetTickSize(1.0);
  //ths[icanvas]->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.40);
  ratio->GetXaxis()->SetTitleSize(0.3);
  ratio->SetLabelSize(0.2,"x");
  leg->Draw(); 
  mycanvas->cd();
  p2->cd();
  p2->SetGridy(); 
  ratio->Sumw2();
  ratio->SetStats(0);
  
  ratio->Divide(hs_signal);
  ratio->SetMarkerStyle(21);
  ratio->SetMarkerSize(0.5);
  ratio->SetLabelSize(0.15,"y");
  ratio->GetYaxis()->SetRangeUser(0.,1.5);
  ratio->GetYaxis()->SetNdivisions(505);
  ratio->Draw("p");
  float xmax = ratio->GetXaxis()->GetXmax();
  float xmin = ratio->GetXaxis()->GetXmin();
  TF1 *f1 = new TF1("f1","1",xmin,xmax);
  ratio->Draw("p");
  f1->Draw("same");

  TString fn = "";

  if(channel == Selector::EE)
    fn = "/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/EWK-NLO_corrections/EEDYComparison_signal-lowDileptonSideband/"+fname;
  if(channel == Selector::MuMu)
    fn = "/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/EWK-NLO_corrections/MuMuDYComparison_signal-lowDileptonSideband/"+fname;

  mycanvas->Print((fn+".pdf").Data());
  mycanvas->Print((fn+".png").Data());
  mycanvas->Print((fn+".root").Data());
  // mycanvas->SetLogy();
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());

  mycanvas->Close();
}
