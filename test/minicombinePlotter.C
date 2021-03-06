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
void drawPlots(TH1* hs_DY1,TH1* hs_DY2,TH1* hs_ttbar,TH1* hs_others,TH1* hs_data, TString xtitle, TString fname, Selector::tag_t channel);
void MakeHistos2D(TChain* chain, Selector *myEvent, std::vector<TH2*> *hs, Selector::tag_t channel);
void drawPlots2D(TH2* hs_DY,TH2* hs_ttbar,TH2* hs_others,TH2* hs_data, TString xtitle,  TString ytitle, TString fname, Selector::tag_t channel);
void Plotter(Selector::tag_t channel);

void minicombinePlotter(){
  std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE};//,Selector::EMu};
   // for(auto c: channels)
   // Plotter(c);
  Plotter(Selector::EE);
  Plotter(Selector::MuMu);
  Plotter(Selector::EMu);

}

void Plotter(Selector::tag_t channel){
  TChain * chain_DY1 = new TChain("Tree_Iter0");
  TChain * chain_DY2 = new TChain("Tree_Iter0");
  TChain * chain_ttbar = new TChain("Tree_Iter0");
  TChain * chain_others = new TChain("Tree_Iter0");
  TChain * chain_data = new TChain("Tree_Iter0");
  
  switch (channel) {
  case Selector::EE:
    chain_DY1->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_lowdileptonsidebandEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_lowdileptonsidebandEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandEE.root");
    //chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandEE.root");
    chain_data->Add("~/nobackup/selected/WRv07/selected_tree_data_lowdileptonsidebandEE.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_lowdileptonsidebandEE.root");    
    break;
  case Selector::MuMu:
    chain_DY1->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_lowdileptonsidebandMuMu.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_lowdileptonsidebandMuMu.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandMuMu.root"); // 1 - Muons
    //chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandMuMu.root");
    chain_data->Add("~/nobackup/selected/WRv07/selected_tree_data_lowdileptonsidebandMuMu.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_lowdileptonsidebandMuMu.root");    
break;
  case Selector::EMu:
    chain_DY1->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_flavoursidebandEMu.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_flavoursidebandEMu.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_flavoursidebandEMu.root");
    chain_data->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    break;
  default:
    std::cout << "Unknown tag" << std::endl;
  }

  Selector myEvent_DY1;
  Selector myEvent_DY2;
  Selector myEvent_ttbar;
  Selector myEvent_others;
  Selector myEvent_data;

  myEvent_DY1.SetBranchAddresses(chain_DY1);
  myEvent_DY2.SetBranchAddresses(chain_DY2);
  myEvent_ttbar.SetBranchAddresses(chain_ttbar);
  myEvent_others.SetBranchAddresses(chain_others);
  myEvent_data.SetBranchAddresses(chain_data);

  std::vector<TH1*> hs_DY1;
  MakeHistos(chain_DY1, &myEvent_DY1, &hs_DY1, channel);
  std::vector<TH1*> hs_DY2;
  MakeHistos(chain_DY2, &myEvent_DY2, &hs_DY2, channel);
  std::vector<TH1*> hs_ttbar;
  MakeHistos(chain_ttbar, &myEvent_ttbar, &hs_ttbar, channel);
  std::vector<TH1*> hs_others;
  MakeHistos(chain_others, &myEvent_others, &hs_others, channel);

  std::vector<TH1*> hs_data;
  MakeHistos(chain_data, &myEvent_data, &hs_data, channel);

  // std::vector<TH2*> hs_2D_DY;
  // MakeHistos2D(chain_DY, &myEvent_DY, &hs_2D_DY, channel);
  // std::vector<TH2*> hs_2D_ttbar;
  // MakeHistos2D(chain_ttbar, &myEvent_ttbar, &hs_2D_ttbar, channel);
  // std::vector<TH2*> hs_2D_others;
  // MakeHistos2D(chain_others, &myEvent_others, &hs_2D_others, channel);

  // std::vector<TH2*> hs_2D_data;
  // MakeHistos2D(chain_data, &myEvent_data, &hs_2D_data, channel);
  
  unsigned int nPlots = hs_DY1.size();
  // unsigned int nPlots2 = hs_2D_DY.size();
  
  TString xtitles[] = {"leading lepton p_{T}","subleading lepton p_{T}","leading jet p_{T}","subleading jet p_{T}","leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi","Mlljj [GeV]","Mlljj [GeV]","Mlljj cumulative [GeV]","dilepton mass [GeV]","nPV","HT","dilepton p_{T}","Ml_1jj [GeV]","Ml_2jj [GeV]", "NJets", "S_{T}", "leading lepton dxy", "subleading lepton dxy", "leading lepton EmHad iso", "subleading lepton EmHad iso", "leading lepton trk iso", "subleading lepton trk iso", "leading lepton HoverEM", "subleading lepton HoverEM"};

  TString fnames[] = {"l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","Mlljj","Mlljj_binned","Mlljj_cum","Mll","nPV","HT","pT_ll","Ml1jj","Ml2jj","njets","ST", "l1_dxy", "l2_dxy", "l1_EmHad_iso", "l2_EmHad_iso", "l1_trk_iso", "l2_trk_iso", "l1_HoverEM", "l2_HoverEM"};
  
  int i = 0;
  for(unsigned int i = 0; i < nPlots; i++){
    std::string s = std::to_string(i);
    drawPlots(hs_DY1[i],hs_DY2[i],hs_ttbar[i],hs_others[i],hs_data[i],xtitles[i],fnames[i], channel);
  }

  TString xtitles2D[] = {"leading lepton p_{T}","leading lepton p_{T}","leading lepton #eta","subleading lepton p_{T}","subleading lepton p_{T}","subleading lepton #eta"};
  TString ytitles2D[] = {"leading lepton #eta","leading lepton #phi","leading lepton #phi","subleading lepton #eta","subleading lepton #phi","subleading lepton #phi"};
 
  TString fnames2D[] = {"l1_pt_l1_eta","l1_pt_l1_phi","l1_eta_l1_phi","l2_pt_l2_eta","l2_pt_l2_phi","l2_eta_l2_phi"};
  
 
  // for(unsigned int i = 0; i < nPlots2; i++){
  //   std::string s = std::to_string(i);
  //   drawPlots2D(hs_2D_DY[i],hs_2D_ttbar[i],hs_2D_others[i],hs_2D_data[i],xtitles2D[i],ytitles2D[i],fnames2D[i], channel);
  // }
  
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

  h_WR_mass->GetYaxis()->SetRangeUser(0.1,10000);
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

void drawPlots(TH1* hs_DY1,TH1* hs_DY2,TH1* hs_ttbar,TH1* hs_others,TH1* hs_data, TString xtitle, TString fname, Selector::tag_t channel){

  TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  leg->AddEntry( hs_DY1, "Z/#gamma* + jets" ) ; 
  leg->AddEntry( hs_ttbar, "ttbar" ) ;
  leg->AddEntry( hs_others, "Other background" ) ;  
  //leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
  leg->AddEntry( hs_data, "Data");
  leg->SetFillColor( kWhite ) ; 


  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  THStack* th = new THStack();
  hs_DY1->SetFillColor(kYellow);
  hs_DY1->SetLineColor(kBlack);
  hs_ttbar->SetFillColor(kGreen);
  hs_ttbar->SetLineColor(kBlack);
  hs_others->SetFillColor(kGreen+3);
  hs_others->SetLineColor(kBlack);

  // if(channel == Selector::EE){
  //   hs_ttbar->Scale(0.439);
  // }
  // else if(channel == Selector::MuMu){
  //   hs_ttbar->Scale(0.667);
  // }

  TH1F *hs_DY1_clone = (TH1F*)hs_DY1->Clone();
  hs_DY1->Add(hs_DY2);
  hs_DY1->Scale(0.5);
  
  if(channel == Selector::EMu){
    th->Add(hs_DY1);
    th->Add(hs_others);
    th->Add(hs_ttbar);
  }
  else{
    th->Add(hs_others);
    th->Add(hs_ttbar);
    th->Add(hs_DY1);
  }

  // cout<<"Histo="<<fname<<endl;
  // cout<<"BKG="<<hs_data->Integral()<<" "<<hs_DY1->Integral()<<" "<<hs_ttbar->Integral()<<" "<<hs_others->Integral()<<endl;
  
  hs_data->SetMarkerStyle(20);

  Double_t eps = 0.001;
  TPad* p1 = new TPad("p1","p1",0,0.25,1,1,0); p1->Draw();
  TPad* p2 = new TPad("p2","p2",0,0.1,1,0.25+eps,0); p2->Draw();
  p1->SetBottomMargin(0);
  p2->SetTopMargin(0);   
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
  for(int i = 0;i<errors->GetNbinsX()+1;i++){
    float errorSum = TMath::Sqrt((hs_DY1_clone->GetBinContent(i)-hs_DY2->GetBinContent(i))*(hs_DY1_clone->GetBinContent(i)-hs_DY2->GetBinContent(i)) + (errors->GetBinError(i)*errors->GetBinError(i)));
    //cout<<i<<" "<<errors->GetBinContent(i)<<" "<<errorSum<<" "<<errors->GetBinError(i)<<endl;
    errors->SetBinError(i,errorSum);
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
  hs_data->GetYaxis()->SetTitle(ytitle.Data());
  hs_data->GetXaxis()->SetTitle(ytitle.Data());
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
  ratio->GetXaxis()->SetTitleSize(0.2);
  ratio->SetLabelSize(0.1,"x");
  leg->Draw(); 
  mycanvas->cd();
  p2->cd();
  p2->SetGridy(); 
  ratio->Sumw2();
  ratio->SetStats(0);

  hs_DY1->Add(hs_ttbar);
  hs_DY1->Add(hs_others);

  ratio->Divide(errors);
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
  mycanvas->cd();

  TString fn = "";

  if(channel == Selector::EMu)
    fn = "~/www/WR/plots/miniTree/Selected/Flavor/"+fname;
  //fn = "plots/Flavor/"+fname;
  if(channel == Selector::EE)
    fn = "~/www/WR/plots/miniTree/Selected/EELowDileptonComb/"+fname;
  //fn = "plots/EELowDilepton/"+fname;
  if(channel == Selector::MuMu)
    fn = "~/www/WR/plots/miniTree/Selected/MuMuLowDileptonComb/"+fname;
  //fn = "plots/MuMuLowDilepton/"+fname;

  mycanvas->Print((fn+".pdf").Data());
  mycanvas->Print((fn+".png").Data());
  mycanvas->Print((fn+".root").Data());
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());

  mycanvas->Close();
}


void MakeHistos2D(TChain * chain, Selector *myEvent, std::vector<TH2*> *hs, Selector::tag_t channel){

  TH2F *h_lepton_pt0_lepton_eta0 = new TH2F("h_lepton_pt0_lepton_eta0","",20,0,700,20,-3,3);
  TH2F *h_lepton_pt0_lepton_phi0 = new TH2F("h_lepton_pt0_lepton_phi0","",20,0,700,20,-3.15,3.15);
  TH2F *h_lepton_eta0_lepton_phi0 = new TH2F("h_lepton_eta0_lepton_phi0","",20,-3,3,20,-3.15,3.15);

  TH2F *h_lepton_pt1_lepton_eta1 = new TH2F("h_lepton_pt1_lepton_eta1","",20,0,700,20,-3,3);
  TH2F *h_lepton_pt1_lepton_phi1 = new TH2F("h_lepton_pt1_lepton_phi1","",20,0,700,20,-3.15,3.15);
  TH2F *h_lepton_eta1_lepton_phi1 = new TH2F("h_lepton_eta1_lepton_phi1","",20,-3,3,20,-3.15,3.15);
  
  Long64_t nEntries = chain->GetEntries();

  cout<< nEntries << endl;

  for(int ev = 0; ev<nEntries; ++ev){
    chain->GetEntry(ev);

    //if(myEvent->WR_mass<2500) continue;
    //if(myEvent->lead_lepton_charge*myEvent->sublead_lepton_charge == 1) continue;
    
    h_lepton_pt0_lepton_eta0->Fill(myEvent->lead_lepton_pt,myEvent->lead_lepton_eta,myEvent->weight);
    h_lepton_pt0_lepton_phi0->Fill(myEvent->lead_lepton_pt,myEvent->lead_lepton_phi,myEvent->weight);
    h_lepton_eta0_lepton_phi0->Fill(myEvent->lead_lepton_eta,myEvent->lead_lepton_phi,myEvent->weight);

    h_lepton_pt1_lepton_eta1->Fill(myEvent->sublead_lepton_pt,myEvent->sublead_lepton_eta,myEvent->weight);
    h_lepton_pt1_lepton_phi1->Fill(myEvent->sublead_lepton_pt,myEvent->sublead_lepton_phi,myEvent->weight);
    h_lepton_eta1_lepton_phi1->Fill(myEvent->sublead_lepton_eta,myEvent->sublead_lepton_phi,myEvent->weight);

  }
  
  hs->push_back(h_lepton_pt0_lepton_eta0);
  hs->push_back(h_lepton_pt0_lepton_phi0);
  hs->push_back(h_lepton_eta0_lepton_phi0);
  hs->push_back(h_lepton_pt1_lepton_eta1);
  hs->push_back(h_lepton_pt1_lepton_phi1);
  hs->push_back(h_lepton_eta1_lepton_phi1);
}


void drawPlots2D(TH2* hs_DY,TH2* hs_ttbar,TH2* hs_others,TH2* hs_data, TString xtitle, TString ytitle, TString fname, Selector::tag_t channel){

  TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  leg->AddEntry( hs_DY, "Z/#gamma* + jets" ) ; 
  leg->AddEntry( hs_ttbar, "ttbar" ) ;
  leg->AddEntry( hs_others, "Other background" ) ;  
  //leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
  leg->AddEntry( hs_data, "Data");
  leg->SetFillColor( kWhite ) ; 


  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  THStack* th = new THStack();
  hs_DY->Scale(0.5);
  hs_DY->Add(hs_others);
  hs_DY->Add(hs_ttbar);
  
  gPad->SetTickx();
  gPad->SetTicky();
  hs_data->SetStats(0);
  hs_DY->SetStats(0);
  TH1F *ratio = (TH1F*)hs_data->Clone();
  hs_DY->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_data->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  
  hs_data->GetYaxis()->SetTitle(ytitle.Data());
  hs_data->GetXaxis()->SetTitle(xtitle.Data());  
  hs_data->Draw("colz");

  TCanvas* mycanvas2 = new TCanvas( "mycanvas2", "", 0, 0, 600, 600 ) ;
  mycanvas2->cd();
  hs_DY->GetYaxis()->SetTitle(ytitle.Data());
  hs_DY->GetXaxis()->SetTitle(xtitle.Data());
  hs_DY->Draw("colz");
  
  TString fn = "";

  // if(channel == Selector::EMu)
  //   fn = "~/www/WR/plots/miniTree/Selected/FlavorHighMass/"+fname;
  //   //fn = "plots/Flavor/"+fname;
  if(channel == Selector::EE)
    fn = "~/www/WR/plots/miniTree/Selected/EELowDileptonComb/"+fname;
  //fn = "plots/EELowDilepton/"+fname;
  if(channel == Selector::MuMu)
    fn = "~/www/WR/plots/miniTree/Selected/MuMuLowDileptonComb/"+fname;
    //fn = "plots/MuMuLowDilepton/"+fname;

  // mycanvas->Print((fn+".pdf").Data());
  // mycanvas->Print((fn+".png").Data());
  // mycanvas2->Print((fn+"_MC.pdf").Data());
  // mycanvas2->Print((fn+"_MC.png").Data());

  mycanvas->Close();
}
