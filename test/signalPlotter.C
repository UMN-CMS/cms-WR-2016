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

void MakeHistos(TChain* chain, Selector *myEvent, std::vector<TH1F*> *hs, Selector::tag_t channel);
//void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_WJets,TH1F* hs_WZ,TH1F* hs_ZZ,TH1F* hs_WW,TH1F* hs_data, TString xtitle, TString fname);
void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_others,TH1F* hs_data,TH1F* hs_signal_1,TH1F* hs_signal_2,TH1F* hs_signal_3,TString xtitle, TString fname, Selector::tag_t channel);
void Plotter(Selector::tag_t channel);

void signalPlotter(){
  std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE};
  for(auto c: channels)
    Plotter(c);
  //Plotter(Selector::EE);
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

  // Signal Points
  TChain * chain_signal_1 = new TChain("Tree_Iter0");
  TChain * chain_signal_2 = new TChain("Tree_Iter0");
  TChain * chain_signal_3 = new TChain("Tree_Iter0");

  
  switch (channel) {
  case Selector::EE:
    //chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMC_signal_eeEE.root");
    //chain_ttbar->Add("~/nobackup/selected/WRv06/selected_tree_TTAMC_signal_eeEE.root");
    // chain_others->Add("~/nobackup/selected/WRv06/selected_tree_W_signal_eeEE.root");
    // chain_others->Add("~/nobackup/selected/WRv06/selected_tree_WZ_signal_eeEE.root");
    // chain_others->Add("~/nobackup/selected/WRv06/selected_tree_ZZ_signal_eeEE.root");
    // chain_others->Add("~/nobackup/selected/WRv06/selected_tree_WW_signal_eeEE.root");
    // chain_WJets->Add("~/nobackup/selected/WRv06/selected_tree_W_signal_eeEE.root");
    // chain_WZ->Add("~/nobackup/selected/WRv06/selected_tree_WZ_signal_eeEE.root");
    // chain_ZZ->Add("~/nobackup/selected/WRv06/selected_tree_ZZ_signal_eeEE.root");
    // chain_WW->Add("~/nobackup/selected/WRv06/selected_tree_WW_signal_eeEE.root");

    chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_signal_eeEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv06/selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_Other_signal_eeEE.root");
    //chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_signal_eeEE.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_signal_eeEE.root");
    chain_signal_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_2000_1000_signal_eeEE.root");
    chain_signal_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_3000_1500_signal_eeEE.root");
    chain_signal_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_4000_2000_signal_eeEE.root");
       break;
  case Selector::MuMu:
    //chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMC_signal_mumuMuMu.root");
    //chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYMADHT_signal_mumuMuMu.root");
    //chain_ttbar->Add("~/nobackup/selected/WRv06/selected_tree_TTAMC_signal_mumuMuMu.root"); // 1 - Muons
    // chain_others->Add("~/nobackup/selected/WRv06/selected_tree_W_signal_mumuMuMu.root");
    // chain_others->Add("~/nobackup/selected/WRv06/selected_tree_WZ_signal_mumuMuMu.root");
    // chain_others->Add("~/nobackup/selected/WRv06/selected_tree_ZZ_signal_mumuMuMu.root");
    // chain_others->Add("~/nobackup/selected/WRv06/selected_tree_WW_signal_mumuMuMu.root");
    // chain_WJets->Add("~/nobackup/selected/WRv06/selected_tree_W_signal_mumuMuMu.root");
    // chain_WZ->Add("~/nobackup/selected/WRv06/selected_tree_WZ_signal_mumuMuMu.root");
    // chain_ZZ->Add("~/nobackup/selected/WRv06/selected_tree_ZZ_signal_mumuMuMu.root");
    // chain_WW->Add("~/nobackup/selected/WRv06/selected_tree_WW_signal_mumuMuMu.root");
    chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_signal_mumuMuMu.root");
    chain_ttbar->Add("~/nobackup/selected/WRv06/selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_Other_signal_mumuMuMu.root");
    //chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_signal_mumuMuMu.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_signal_mumuMuMu.root");
    chain_signal_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_2000_1000_signal_mumuMuMu.root");
    chain_signal_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_3000_1500_signal_mumuMuMu.root");
    chain_signal_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_4000_2000_signal_mumuMuMu.root");
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
  
  TString xtitles[] = {"leading lepton p_{T}","subleading lepton p_{T}","leading jet p_{T}","subleading jet p_{T}","leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi","Mlljj [GeV]","dilepton mass [GeV]","nPV","HT","dilepton p_{T}","Ml_1jj [GeV]","Ml_2jj [GeV]", "NJets"};

  TString fnames[] = {"l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","Mlljj","Mll","nPV","HT","pT_ll","Ml1jj","Ml2jj","njets"};

  int i = 0;
  for(unsigned int i = 0; i < nPlots; i++){
    std::string s = std::to_string(i);
    //drawPlots(hs_DY[i],hs_ttbar[i],hs_WJets[i],hs_WZ[i],hs_ZZ[i],hs_WW[i],hs_data[i],xtitles[i],fnames[i]);
    drawPlots(hs_DY[i],hs_ttbar[i],hs_others[i],hs_data[i],hs_signal_1[i],hs_signal_2[i],hs_signal_3[i],xtitles[i],fnames[i], channel);
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

  TH1F *h_WR_mass = new TH1F("h_WR_mass","",40,0,6000);
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
/*
void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_WJets,TH1F* hs_WZ,TH1F* hs_ZZ,TH1F* hs_WW,TH1F* hs_data, TString xtitle, TString fname){

  TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  leg->AddEntry( hs_DY, "Z/#gamma* + jets" ) ; 
  leg->AddEntry( hs_ttbar, "ttbar" ) ;
  leg->AddEntry( hs_WJets, "WJets" ) ; 
  leg->AddEntry( hs_ZZ, "ZZ" ) ;
  leg->AddEntry( hs_WZ, "WZ" ) ;
  leg->AddEntry( hs_WW, "WW" ) ;  
  //leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
  leg->AddEntry( hs_data, "Data");
  leg->SetFillColor( kWhite ) ; 


  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  THStack* th = new THStack();
  hs_DY->SetFillColor(kYellow);
  hs_ttbar->SetFillColor(kGreen);
  hs_WJets->SetFillColor(kBlue);
  hs_ZZ->SetFillColor(kMagenta);
  hs_WW->SetFillColor(kOrange);
  hs_WZ->SetFillColor(kCyan);
  th->Add(hs_WW);
  th->Add(hs_WZ);
  th->Add(hs_ZZ);
  th->Add(hs_WJets);
  th->Add(hs_ttbar);
  th->Add(hs_DY);
  hs_data->SetMarkerStyle(20);

  //cout<<"Integrals="<<((TH1*)(th->GetStack()->Last()))->Integral()<<" "<<hs_data->Integral()<<endl;

  Double_t eps = 0.001;
  TPad* p1 = new TPad("p1","p1",0,0.25,1,1,0); p1->Draw();
  TPad* p2 = new TPad("p2","p2",0,0.1,1,0.25+eps,0); p2->Draw();
  p1->SetBottomMargin(0);
  p2->SetTopMargin(0);   
  p1->cd();
  hs_data->SetStats(0);
  TH1F *ratio = (TH1F*)hs_data->Clone();
  th->SetTitle("CMS Preliminary");
  hs_data->SetTitle("CMS Preliminary");
  //th->Draw("histo");
  //hs_data->Draw("epsame");
  hs_data->Draw("ep");
  th->Draw("histo same");
  hs_data->Draw("epsame");
  TH1F *errors = (TH1F*)(th->GetStack()->Last())->Clone();
  errors->SetLineColor(0);
  errors->SetFillColor(1);
  errors->SetFillStyle(3254);
  errors->Draw("E2 same");
  TString ytitle = "Events/(";
  ytitle += (th->GetXaxis()->GetNbins());
  ytitle += ")";
  th->GetYaxis()->SetTitle(ytitle.Data());
  th->GetXaxis()->SetTitle(xtitle.Data());

  cout<<"Integral="<<((TH1*)th->GetStack()->Last())->Integral()<<" "<<hs_data->Integral()<<endl;
  
  ratio->GetXaxis()->SetTitle(xtitle.Data());
  //ths[icanvas]->GetXaxis()->SetTickSize(1.0);
  //ths[icanvas]->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.40);
  ratio->GetXaxis()->SetTitleSize(0.2);
  ratio->SetLabelSize(0.1,"x");
  leg->Draw(); 
  mycanvas->cd();
  p2->cd();
  ratio->Sumw2();
  ratio->SetStats(0);

  hs_DY->Add(hs_ttbar);
  hs_DY->Add(hs_WJets);
  hs_DY->Add(hs_WZ);
  hs_DY->Add(hs_ZZ);
  hs_DY->Add(hs_WW);

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
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());

  mycanvas->Close();
}*/
void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_others,TH1F* hs_data,TH1F* hs_signal_1,TH1F* hs_signal_2,TH1F* hs_signal_3, TString xtitle, TString fname, Selector::tag_t channel){

  TLegend *leg = new TLegend( 0.52, 0.60, 0.98, 0.90 ) ; 
  leg->AddEntry( hs_DY, "Z/#gamma* + jets" ) ; 
  leg->AddEntry( hs_ttbar, "ttbar" ) ;
  leg->AddEntry( hs_others, "Other background" ) ;  
  leg->AddEntry( hs_signal_1, "WR M=2000 GeV, NR M=1000 GeV" ) ;  
  leg->AddEntry( hs_signal_2, "WR M=3000 GeV, NR M=1500 GeV" ) ;  
  leg->AddEntry( hs_signal_3, "WR M=4000 GeV, NR M=2000 GeV" ) ;  
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
    hs_ttbar->Scale(0.486);
  else if(channel == Selector::MuMu)
    hs_ttbar->Scale(0.662);
   
  th->Add(hs_others);
  th->Add(hs_DY);
  th->Add(hs_ttbar);
  hs_data->SetMarkerStyle(20);

  Double_t eps = 0.001;
  TPad* p1 = new TPad("p1","p1",0,0.25,1,1,0); p1->Draw();
  TPad* p2 = new TPad("p2","p2",0,0.1,1,0.25+eps,0); p2->Draw();
  p1->SetBottomMargin(0);
  p2->SetTopMargin(0);   
  p1->cd();
  hs_data->SetStats(0);
  TH1F *ratio = (TH1F*)hs_data->Clone();
  th->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_data->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  th->Draw("histo");
  hs_signal_1->Draw("histo same");
  hs_signal_2->Draw("histo same");
  hs_signal_3->Draw("histo same");
  //hs_data->Draw("epsame");
  // hs_data->Draw("ep");
  // th->Draw("histo same");
  // hs_data->Draw("epsame");
  TH1F *errors = (TH1F*)(th->GetStack()->Last())->Clone();
  errors->SetLineColor(0);
  errors->SetFillColor(1);
  errors->SetFillStyle(3254);
  errors->Draw("E2 same");
  TString ytitle = "Events/(";
  ytitle += (th->GetXaxis()->GetNbins());
  ytitle += ")";
  th->GetYaxis()->SetTitle(ytitle.Data());
  th->GetXaxis()->SetTitle(xtitle.Data());

  //cout<<"Bins1="<<((TH1*)th->GetStack()->Last())->FindBin(80)<<std::endl;
  //cout<<"Bins2="<<((TH1*)th->GetStack()->Last())->FindBin(100)<<std::endl;
  
  cout<<"Integral="<<((TH1*)th->GetStack()->Last())->Integral(11,31)<<" "<<hs_data->Integral(11,31)<<endl;
  
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

  hs_DY->Add(hs_ttbar);
  hs_DY->Add(hs_others);

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
  mycanvas->cd();

  TString fn = "";

  if(channel == Selector::EMu)
    fn = "~/www/WR/plots/miniTree/Selected/Flavor/"+fname;
    //fn = "plots/Flavor/"+fname;
  if(channel == Selector::EE)
    fn = "~/www/WR/plots/miniTree/Selected/EESignal/"+fname;
  //fn = "plots/EELowDilepton/"+fname;
  if(channel == Selector::MuMu)
    fn = "~/www/WR/plots/miniTree/Selected/MuMuSignal/"+fname;
    //fn = "plots/MuMuLowDilepton/"+fname;

  mycanvas->Print((fn+".pdf").Data());
  mycanvas->Print((fn+".png").Data());
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());

  mycanvas->Close();
}
