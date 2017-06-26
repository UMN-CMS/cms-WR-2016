#include "TH1F.h"
#include "TH2F.h"
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

void miniPlotter(){
  std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE,Selector::EMu};
  // for(auto c: channels)
  //   Plotter(c);
  Plotter(Selector::EE);
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
  
  switch (channel) {
  case Selector::EE:
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_lowdileptonsidebandEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandEE.root");
    //chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandEE.root");
    chain_data->Add("~/nobackup/selected/WRv07/selected_tree_data_lowdileptonsidebandEE.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_lowdileptonsidebandEE.root");    
    break;
  case Selector::MuMu:
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_lowdileptonsidebandMuMu.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandMuMu.root"); // 1 - Muons
    //chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandMuMu.root");
    chain_data->Add("~/nobackup/selected/WRv07/selected_tree_data_lowdileptonsidebandMuMu.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_lowdileptonsidebandMuMu.root");    
break;
  case Selector::EMu:
    chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_flavoursidebandEMu.root");
    chain_ttbar->Add("~/nobackup/selected/WRv06/selected_tree_TTAMC_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_Other_flavoursidebandEMu.root");
    chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_flavoursidebandEMu.root");
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

  myEvent_DY.SetBranchAddresses(chain_DY);
  myEvent_ttbar.SetBranchAddresses(chain_ttbar);
  myEvent_others.SetBranchAddresses(chain_others);
  myEvent_WJets.SetBranchAddresses(chain_WJets);
  myEvent_WZ.SetBranchAddresses(chain_WZ);
  myEvent_ZZ.SetBranchAddresses(chain_ZZ);
  myEvent_WW.SetBranchAddresses(chain_WW);
  myEvent_data.SetBranchAddresses(chain_data);

  std::vector<TH1*> hs_DY;
  MakeHistos(chain_DY, &myEvent_DY, &hs_DY, channel);
  std::vector<TH1*> hs_ttbar;
  MakeHistos(chain_ttbar, &myEvent_ttbar, &hs_ttbar, channel);
  std::vector<TH1*> hs_others;
  MakeHistos(chain_others, &myEvent_others, &hs_others, channel);
  std::vector<TH1*> hs_WJets;
  MakeHistos(chain_WJets, &myEvent_WJets, &hs_WJets, channel);
  std::vector<TH1*> hs_WZ;
  MakeHistos(chain_WZ, &myEvent_WZ, &hs_WZ, channel);
  std::vector<TH1*> hs_ZZ;
  MakeHistos(chain_ZZ, &myEvent_ZZ, &hs_ZZ, channel);
  std::vector<TH1*> hs_WW;
  MakeHistos(chain_WW, &myEvent_WW, &hs_WW, channel);

  std::vector<TH1*> hs_data;
  MakeHistos(chain_data, &myEvent_data, &hs_data, channel);

  unsigned int nPlots = hs_DY.size();

  // hs_data[13]->SetLineColor(kRed);
  // hs_data[13]->Draw();
  // hs_ttbar[13]->Draw("same");
  
  TString xtitles[] = {"leading lepton p_{T}","subleading lepton p_{T}","leading jet p_{T}","subleading jet p_{T}","leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi","Mlljj [GeV]","Mlljj [GeV]","Mlljj cumulative [GeV]","dilepton mass [GeV]","nPV","HT","dilepton p_{T}","Ml_1jj [GeV]","Ml_2jj [GeV]", "NJets", "S_{T}", "leading lepton dxy", "subleading lepton dxy", "leading lepton EmHad iso", "subleading lepton EmHad iso", "leading lepton trk iso", "subleading lepton trk iso", "leading lepton HoverEM", "subleading lepton HoverEM"};

  TString fnames[] = {"l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","Mlljj","Mlljj_binned","Mlljj_cum","Mll","nPV","HT","pT_ll","Ml1jj","Ml2jj","njets","ST", "l1_dxy", "l2_dxy", "l1_EmHad_iso", "l2_EmHad_iso", "l1_trk_iso", "l2_trk_iso", "l1_HoverEM", "l2_HoverEM"};
  
  int i = 0;
  for(unsigned int i = 0; i < nPlots; i++){
    std::string s = std::to_string(i);
    //drawPlots(hs_DY[i],hs_ttbar[i],hs_WJets[i],hs_WZ[i],hs_ZZ[i],hs_WW[i],hs_data[i],xtitles[i],fnames[i]);
    drawPlots(hs_DY[i],hs_ttbar[i],hs_others[i],hs_data[i],xtitles[i],fnames[i], channel);
  }
  
}

void MakeHistos(TChain * chain, Selector *myEvent, std::vector<TH1*> *hs, Selector::tag_t channel){

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
  TH1F *h_nPV = new TH1F("h_nPV","",50,0,50);
  TH1F *h_HT = new TH1F("h_HT","",40,0,3000);
  TH1F *h_pT_ll = new TH1F("h_pT_ll","",40,50,1000);
  TH1F *h_Ml1jj = new TH1F("h_Ml1jj","",40,0,3000);
  TH1F *h_Ml2jj = new TH1F("h_Ml2jj","",40,0,3000);
  TH1F *h_njets = new TH1F("h_njets","",11,0,11);
  TH1F *h_ST = new TH1F("h_ST","",40,0,4000);

  TH1F *h_lepton_dxy0 = new TH1F("h_lepton_dxy0","",40,-0.25,0.25);
  TH1F *h_lepton_EmHadIso0 = new TH1F("h_lepton_EmHadIso0","",40,0,20);
  TH1F *h_lepton_TrkIso0 = new TH1F("h_lepton_TrkIso0","",40,0,5);
  TH1F *h_lepton_HoverEM0 = new TH1F("h_lepton_HoverEM0","",40,0,0.1);
  
  TH1F *h_lepton_dxy1 = new TH1F("h_lepton_dxy1","",40,-0.25,0.25);
  TH1F *h_lepton_EmHadIso1 = new TH1F("h_lepton_EmHadIso1","",40,0,20);
  TH1F *h_lepton_TrkIso1 = new TH1F("h_lepton_TrkIso1","",40,0,5);
  TH1F *h_lepton_HoverEM1 = new TH1F("h_lepton_HoverEM1","",40,0,0.1);

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
    h_ST->Fill(myEvent->lead_lepton_pt+myEvent->sublead_lepton_pt+myEvent->lead_jet_pt+myEvent->sublead_jet_pt,myEvent->weight);

    h_lepton_dxy0->Fill(myEvent->lead_lepton_dxy,myEvent->weight);
    h_lepton_EmHadIso0->Fill(myEvent->lead_lepton_GsfEleEmHadD1IsoRhoCut,myEvent->weight);
    h_lepton_TrkIso0->Fill(myEvent->lead_lepton_trkIso,myEvent->weight);
    h_lepton_HoverEM0->Fill(myEvent->lead_lepton_HoverEM,myEvent->weight);
    h_lepton_dxy1->Fill(myEvent->sublead_lepton_dxy,myEvent->weight);
    h_lepton_EmHadIso1->Fill(myEvent->sublead_lepton_GsfEleEmHadD1IsoRhoCut,myEvent->weight);
    h_lepton_TrkIso1->Fill(myEvent->sublead_lepton_trkIso,myEvent->weight);
    h_lepton_HoverEM1->Fill(myEvent->sublead_lepton_HoverEM,myEvent->weight);
    
  }

  h_WR_mass->GetYaxis()->SetRangeUser(0.01,100000);
  h_WR_mass_binned->GetYaxis()->SetRangeUser(0.01,100000);
  TH1 *h_WR_massc = h_WR_mass->GetCumulative(kFALSE);
  h_WR_massc->GetYaxis()->SetRangeUser(0.01,200000);
  
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
  hs->push_back(h_WR_massc);
  hs->push_back(h_dilepton_mass);
  hs->push_back(h_nPV);
  hs->push_back(h_HT);
  hs->push_back(h_pT_ll);
  hs->push_back(h_Ml1jj);
  hs->push_back(h_Ml2jj);
  hs->push_back(h_njets);
  hs->push_back(h_ST);
  hs->push_back(h_lepton_dxy0);
  hs->push_back(h_lepton_dxy1);
  hs->push_back(h_lepton_EmHadIso0);
  hs->push_back(h_lepton_EmHadIso1);
  hs->push_back(h_lepton_TrkIso0);
  hs->push_back(h_lepton_TrkIso1);
  hs->push_back(h_lepton_HoverEM0);
  hs->push_back(h_lepton_HoverEM1);

}

void drawPlots(TH1* hs_DY,TH1* hs_ttbar,TH1* hs_others,TH1* hs_data, TString xtitle, TString fname, Selector::tag_t channel){

  TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  leg->AddEntry( hs_DY, "Z/#gamma* + jets" ) ; 
  leg->AddEntry( hs_ttbar, "ttbar" ) ;
  leg->AddEntry( hs_others, "Other background" ) ;  
  //leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
  leg->AddEntry( hs_data, "Data");
  leg->SetFillColor( kWhite ) ; 


  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  THStack* th = new THStack();
  hs_DY->SetFillColor(kYellow);
  hs_ttbar->SetFillColor(kGreen);
  hs_others->SetFillColor(kGreen+3);
  hs_DY->SetLineColor(kBlack);
  hs_ttbar->SetLineColor(kBlack);
  hs_others->SetLineColor(kBlack);


  //cout<<"BKG="<<hs_data->Integral()<<" "<<hs_DY->Integral()<<" "<<hs_ttbar<<" "<<hs_others<<endl;
  // if(channel == Selector::EE){
  //   hs_ttbar->Scale(0.439);
  // }
  // else if(channel == Selector::MuMu){
  //   hs_ttbar->Scale(0.667);
  // }
  
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
  th->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_data->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  //th->Draw("histo");
  //hs_data->Draw("epsame");

  hs_data->Draw("ep");
  th->Draw("histo same");
  hs_data->Draw("epsame");

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

  if(channel != Selector::EMu){
    TH1F *scale_error_hist = (TH1F*) scale_error_fn->Get(hname);
    TH1F *pdf_error_hist = (TH1F*) pdf_error_fn->Get(hname);
    TH1F *fact_renorm_error_hist = (TH1F*) fact_renorm_error_fn->Get(hname);

    TF1 * scale_error_funcEE = new TF1("scale_error_funcEE","2.44792e-05*x+1.45510e-01");
    TF1 * pdf_error_funcEE = new TF1("pdf_error_funcEE","1.05646e-06*x+1.33917e-01");
    TF1 * fact_renorm_error_funcEE = new TF1("fact_renorm_error_funcEE","2.97192e-06*x+4.24292e-02");
    
    TF1 * scale_error_funcMuMu = new TF1("scale_error_funcMuMu","5.01588e-05*x+5.44939e-02");
    TF1 * pdf_error_funcMuMu = new TF1("pdf_error_funcMuMu","1.21116e-06*x+1.33917e-01");
    TF1 * fact_renorm_error_funcMuMu = new TF1("fact_renorm_error_funcMuMu","5.54490e-06*x+1.13813e-01");
      
    for(int i = 0;i<errors->GetNbinsX()+1;i++){
      float errorSum = 0;
      float errorSyst = 0;
      if(channel == Selector::EE){
	errorSum = TMath::Sqrt(
			       (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       
			       // (scale_error_funcEE->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))*
			       // (scale_error_funcEE->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))+
			       // (pdf_error_funcEE->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))*
			       // (pdf_error_funcEE->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))+
			       // (fact_renorm_error_funcEE->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))*
			       // (fact_renorm_error_funcEE->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))+
			       
			       (errors->GetBinError(i)*errors->GetBinError(i)));

	errorSyst = TMath::Sqrt(
			       (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))));
      }
    else if(channel == Selector::MuMu){
	errorSum = TMath::Sqrt(
			       (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +

			       // (scale_error_funcMuMu->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))*
			       // (scale_error_funcMuMu->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))+
			       // (pdf_error_funcMuMu->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))*
			       // (pdf_error_funcMuMu->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))+
			       // (fact_renorm_error_funcMuMu->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))*
			       // (fact_renorm_error_funcMuMu->Eval(hs_DY->GetBinCenter(i))*hs_DY->GetBinContent(i))+

			       (errors->GetBinError(i)*errors->GetBinError(i)));

	errorSyst = TMath::Sqrt(
			       (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (scale_error_hist->GetBinContent(scale_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))) +
			       (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i)))*
			       (fact_renorm_error_hist->GetBinContent(fact_renorm_error_hist->FindBin(hs_DY->GetBinCenter(i)))*(hs_DY->GetBinContent(i))));

    }
      
      if(xtitle == "Mlljj [GeV]"){
	cout<<i<<" "<<hs_data->GetBinContent(i)<<" "<<errors->GetBinContent(i)<<" "<<errors->GetBinError(i)<<" "<<errorSyst<<" "<<errorSum<<endl;
	errors->SetBinError(i,errorSum);
	hs_DY->SetBinError(i,errorSum);
	//cout<<"Bin="<<i<<" "<<hs_data->GetBinContent(i)<<endl;
	//cout<<"Bin="<<i<<" "<<hs_DY->GetBinContent(i)<<endl;
      }
    }
  }
  
  errors->SetLineColor(0);
  errors->SetFillColor(1);
  errors->SetFillStyle(3254);
  if(xtitle != "Mlljj cumulative [GeV]")
    errors->Draw("E2 same");
  TString ytitle = "Events/(";
  ytitle += (th->GetXaxis()->GetNbins());
  ytitle += ")";
  th->GetYaxis()->SetTitle(ytitle.Data());
  th->GetXaxis()->SetTitle(xtitle.Data());
  hs_data->GetXaxis()->SetTitle(xtitle.Data());
  //hs_data->GetYaxis()->SetTitle(ytitle.Data());


  //cout<<"Bins1="<<((TH1*)th->GetStack()->Last())->FindBin(80)<<std::endl;
  //cout<<"Bins2="<<((TH1*)th->GetStack()->Last())->FindBin(100)<<std::endl;

  int mlljj_min = 3500;
  
  if(xtitle == "Mlljj [GeV]"){
    cout<<"Integral="<<((TH1*)th->GetStack()->Last())->Integral(((TH1*)th->GetStack()->Last())->FindBin(mlljj_min),((TH1*)th->GetStack()->Last())->FindBin(7000))<<" "<<hs_data->Integral(((TH1*)th->GetStack()->Last())->FindBin(mlljj_min),((TH1*)th->GetStack()->Last())->FindBin(7000))<<endl;
  }

  if(xtitle == "Mlljj [GeV]"){
     //th->GetYaxis()->SetRangeUser(0.1,500);
     std::cout<<"Mlljj="<<((TH1*)th->GetStack()->Last())->GetRMS()<<std::endl;
   }
  ratio->GetXaxis()->SetTitle(xtitle.Data());
  //ths[icanvas]->GetXaxis()->SetTickSize(1.0);
  //ths[icanvas]->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.40);
  //ratio->GetXaxis()->SetTitleSize(0.2);
  ratio->SetLabelSize(0.1,"x");
  leg->Draw(); 
  mycanvas->cd();
  p2->cd();
  p2->SetGridy(); 
  ratio->Sumw2();
  ratio->SetStats(0);

  hs_DY->Add(hs_ttbar);
  hs_DY->Add(hs_others);

   // if(xtitle == "Mlljj [GeV]"){
   //   for(int i = 0;i<hs_DY->GetNbinsX()+1;i++){
   //     //cout<<i<<" "<<errors->GetBinContent(i)<<" "<<errorSum<<" "<<errors->GetBinError(i)<<" "<< pdf_error_hist->GetBinContent(pdf_error_hist->FindBin(hs_DY->GetBinCenter(i)))<<endl;
   //     cout<<"Bin2="<<i<<" "<<hs_data->GetBinContent(i)<<endl;
   //     cout<<"Bin2="<<i<<" "<<hs_DY->GetBinContent(i)<<endl;
   //   }
   // }
  
  ratio->Divide(hs_DY);
  ratio->SetMarkerStyle(21);
  ratio->SetMarkerSize(0.5);
  ratio->SetLabelSize(0.1,"y");
  ratio->GetYaxis()->SetRangeUser(0.5,1.5);
  ratio->GetYaxis()->SetNdivisions(505);
  ratio->Draw("p");
  
  float xmax = ratio->GetXaxis()->GetXmax();
  float xmin = ratio->GetXaxis()->GetXmin();
  TF1 *f1 = new TF1("f1","1",xmin,xmax);
  ratio->Draw("p");
  f1->Draw("same");
  //std::cout<<"OFFSET="<<ratio->GetXaxis()->GetTitleOffset()<<std::endl;
  ratio->GetXaxis()->SetTitle(xtitle.Data());
  ratio->GetXaxis()->SetTitleSize(0.12);
  
  mycanvas->cd();

  TString fn = "";

  if(channel == Selector::EMu)
    fn = "~/www/WR/plots/miniTree/Selected/Flavor/"+fname;
    //fn = "plots/Flavor/"+fname;
  if(channel == Selector::EE)
    fn = "~/www/WR/plots/miniTree/Selected/EELowDilepton/"+fname;
  //fn = "plots/EELowDilepton/"+fname;
  if(channel == Selector::MuMu)
    fn = "~/www/WR/plots/miniTree/Selected/MuMuLowDilepton/"+fname;
    //fn = "plots/MuMuLowDilepton/"+fname;

  mycanvas->Print((fn+".pdf").Data());
  mycanvas->Print((fn+".png").Data());
  mycanvas->Print((fn+".root").Data());
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());

  mycanvas->Close();
}
