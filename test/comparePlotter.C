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
void drawPlots(TH1F* hs_DY,TH1F* hs_DY2,TH1F* hs_ttbar,TH1F* hs_others,TH1F* hs_data, TString xtitle, TString fname, Selector::tag_t channel);
void Plotter(Selector::tag_t channel);

void comparePlotter(){
  std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE};
  for(auto c: channels)
    Plotter(c);
  //Plotter(Selector::MuMu);

}


void Plotter(Selector::tag_t channel){
  TChain * chain_DY = new TChain("Tree_Iter0");
  TChain * chain_DY2 = new TChain("Tree_Iter0");
  TChain * chain_ttbar = new TChain("Tree_Iter0");
  TChain * chain_others = new TChain("Tree_Iter0");
  TChain * chain_WJets = new TChain("Tree_Iter0");
  TChain * chain_WZ = new TChain("Tree_Iter0");
  TChain * chain_ZZ = new TChain("Tree_Iter0");
  TChain * chain_WW = new TChain("Tree_Iter0");
  TChain * chain_data = new TChain("Tree_Iter0");
  
  switch (channel) {
  case Selector::EE:
    //chain_DY->Add("~/nobackup/selected/selected_tree_DYAMC_lowdileptonsidebandEE.root");
    //chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYMADHT_lowdileptonsidebandEE.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_W_lowdileptonsidebandEE.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_WZ_lowdileptonsidebandEE.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_ZZ_lowdileptonsidebandEE.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_WW_lowdileptonsidebandEE.root");
    // chain_WJets->Add("~/nobackup/selected/selected_tree_W_lowdileptonsidebandEE.root");
    // chain_WZ->Add("~/nobackup/selected/selected_tree_WZ_lowdileptonsidebandEE.root");
    // chain_ZZ->Add("~/nobackup/selected/selected_tree_ZZ_lowdileptonsidebandEE.root");
    // chain_WW->Add("~/nobackup/selected/selected_tree_WW_lowdileptonsidebandEE.root");

    chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_lowdileptonsidebandEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv06/selected_tree_DYMADHT_lowdileptonsidebandEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv06/selected_tree_TTAMC_lowdileptonsidebandEE.root");
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_Other_lowdileptonsidebandEE.root");
    chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_lowdileptonsidebandEE.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_lowdileptonsidebandEE.root");


    
    break;
  case Selector::MuMu:
    //chain_DY->Add("~/nobackup/selected/selected_tree_DYAMC_lowdileptonsidebandMuMu.root");
    //chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYMADHT_lowdileptonsidebandMuMu.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_W_lowdileptonsidebandMuMu.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_WZ_lowdileptonsidebandMuMu.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_ZZ_lowdileptonsidebandMuMu.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_WW_lowdileptonsidebandMuMu.root");
    // chain_WJets->Add("~/nobackup/selected/selected_tree_W_lowdileptonsidebandMuMu.root");
    // chain_WZ->Add("~/nobackup/selected/selected_tree_WZ_lowdileptonsidebandMuMu.root");
    // chain_ZZ->Add("~/nobackup/selected/selected_tree_ZZ_lowdileptonsidebandMuMu.root");
    // chain_WW->Add("~/nobackup/selected/selected_tree_WW_lowdileptonsidebandMuMu.root");

    chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_lowdileptonsidebandMuMu.root");
    chain_DY2->Add("~/nobackup/selected/WRv06/selected_tree_DYMADHT_lowdileptonsidebandMuMu.root");
    chain_ttbar->Add("~/nobackup/selected/WRv06/selected_tree_TTAMC_lowdileptonsidebandMuMu.root"); // 1 - Muons
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_Other_lowdileptonsidebandMuMu.root");
    chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_lowdileptonsidebandMuMu.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_lowdileptonsidebandMuMu.root");

    
break;
  case Selector::EMu:
    //chain_others->Add("~/nobackup/selected/selected_tree_W_flavoursidebandEMu.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_WZ_flavoursidebandEMu.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_ZZ_flavoursidebandEMu.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_WW_flavoursidebandEMu.root");
    // chain_others->Add("~/nobackup/selected/selected_tree_SingleTop_flavoursidebandEMu.root");
    // chain_WJets->Add("~/nobackup/selected/selected_tree_W_flavoursidebandEMu.root");
    // chain_WZ->Add("~/nobackup/selected/selected_tree_WZ_flavoursidebandEMu.root");
    // chain_ZZ->Add("~/nobackup/selected/selected_tree_ZZ_flavoursidebandEMu.root");
    // chain_WW->Add("~/nobackup/selected/selected_tree_WW_flavoursidebandEMu.root");
    
    chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_flavoursidebandEMu.root");
    chain_ttbar->Add("~/nobackup/selected/WRv06/selected_tree_TTAMC_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_Other_flavoursidebandEMu.root");
    chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_flavoursidebandEMu.root");
    break;
  default:
    std::cout << "Unknown tag" << std::endl;
  }

  Selector myEvent_DY;
  Selector myEvent_DY2;
  Selector myEvent_ttbar;
  Selector myEvent_others;
  Selector myEvent_WJets;
  Selector myEvent_WZ;
  Selector myEvent_ZZ;
  Selector myEvent_WW;
  Selector myEvent_data;

  myEvent_DY.SetBranchAddresses(chain_DY);
  myEvent_DY2.SetBranchAddresses(chain_DY2);
  myEvent_ttbar.SetBranchAddresses(chain_ttbar);
  myEvent_others.SetBranchAddresses(chain_others);
  myEvent_WJets.SetBranchAddresses(chain_WJets);
  myEvent_WZ.SetBranchAddresses(chain_WZ);
  myEvent_ZZ.SetBranchAddresses(chain_ZZ);
  myEvent_WW.SetBranchAddresses(chain_WW);
  myEvent_data.SetBranchAddresses(chain_data);

  std::vector<TH1F*> hs_DY;
  MakeHistos(chain_DY, &myEvent_DY, &hs_DY, channel);
  std::vector<TH1F*> hs_DY2;
  MakeHistos(chain_DY2, &myEvent_DY2, &hs_DY2, channel);
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
    drawPlots(hs_DY[i],hs_DY2[i],hs_ttbar[i],hs_others[i],hs_data[i],xtitles[i],fnames[i], channel);
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
  float dilepton_max = 110.;
  if(channel == Selector::EMu)
    dilepton_max = 1000;
  TH1F *h_dilepton_mass = new TH1F("h_dilepton_mass","",40,70,dilepton_max);
  TH1F *h_nPV = new TH1F("h_nPV","",40,0,50);
  TH1F *h_HT = new TH1F("h_HT","",40,0,3000);
  TH1F *h_pT_ll = new TH1F("h_pT_ll","",40,50,1000);
  TH1F *h_Ml1jj = new TH1F("h_Ml1jj","",40,0,3000);
  TH1F *h_Ml2jj = new TH1F("h_Ml2jj","",40,0,3000);
  TH1F *h_njets = new TH1F("h_njets","",11,0,11);

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

void drawPlots(TH1F* hs_DY,TH1F* hs_DY2,TH1F* hs_ttbar,TH1F* hs_others,TH1F* hs_data, TString xtitle, TString fname, Selector::tag_t channel){

  TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  leg->AddEntry( hs_DY, "Z/#gamma* + jets AMC@NLO" ) ; 
  leg->AddEntry( hs_DY2, "Z/#gamma* + jets Madgraph" ) ; 
  //leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
  leg->AddEntry( hs_data, "Data");
  leg->SetFillColor( kWhite ) ; 


  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  hs_DY->SetLineColor(kRed);
  hs_DY2->SetLineColor(kBlue);

  //hs_DY->Scale(1.0/1.3);
  // hs_ttbar->Scale(2.6/35.8);
  // hs_others->Scale(2.6/35.8);

  hs_DY->Add(hs_others);
  hs_DY->Add(hs_ttbar);

  hs_DY2->Add(hs_others);
  hs_DY2->Add(hs_ttbar);

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
  TH1F *ratio2 = (TH1F*)hs_data->Clone();
  hs_DY->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_DY2->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_data->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  //th->Draw("histo");
  //hs_data->Draw("epsame");
  hs_data->Draw("ep");
  hs_DY->Draw("histo same");
  hs_DY2->Draw("histo same");
  hs_data->Draw("epsame");
  TH1F *errors = (TH1F*)hs_DY->Clone();
  errors->SetLineColor(0);
  errors->SetFillColor(1);
  errors->SetFillStyle(3254);
  //errors->Draw("E2 same");
  TString ytitle = "Events/(";
  ytitle += (hs_DY->GetXaxis()->GetNbins());
  ytitle += ")";
  hs_DY->GetYaxis()->SetTitle(ytitle.Data());
  hs_DY->GetXaxis()->SetTitle(xtitle.Data());
  hs_DY2->GetYaxis()->SetTitle(ytitle.Data());
  hs_DY2->GetXaxis()->SetTitle(xtitle.Data());

  //cout<<"Bins1="<<((TH1*)th->GetStack()->Last())->FindBin(80)<<std::endl;
  //cout<<"Bins2="<<((TH1*)th->GetStack()->Last())->FindBin(100)<<std::endl;
  
  cout<<"Integral="<<hs_DY->Integral(11,31)<<" "<<hs_data->Integral(11,31)<<endl;
  
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
  ratio->SetLineColor(kRed);
  ratio2->Sumw2();
  ratio2->SetStats(0);

  ratio->Divide(hs_DY);
  ratio->SetMarkerStyle(21);
  ratio->SetMarkerSize(0.5);
  ratio2->Divide(hs_DY2);
  ratio2->SetMarkerStyle(22);
  ratio2->SetMarkerSize(0.5);
  ratio->SetLabelSize(0.1,"y");
  ratio->GetYaxis()->SetRangeUser(0.5,1.5);
  ratio->GetYaxis()->SetNdivisions(505);
  ratio->Draw("p");
  ratio2->Draw("psame");
  float xmax = ratio->GetXaxis()->GetXmax();
  float xmin = ratio->GetXaxis()->GetXmin();
  TF1 *f1 = new TF1("f1","1",xmin,xmax);
  ratio->Draw("p");
  ratio2->Draw("psame");
  f1->Draw("same");
  mycanvas->cd();

  TString fn = "";

  if(channel == Selector::EE)
    fn = "~/www/WR/plots/miniTree/Selected/EELowDileptonCompare/"+fname;
  //fn = "plots/EELowDilepton/"+fname;
  if(channel == Selector::MuMu)
    fn = "~/www/WR/plots/miniTree/Selected/MuMuLowDileptonCompare/"+fname;
    //fn = "plots/MuMuLowDilepton/"+fname;

  mycanvas->Print((fn+".pdf").Data());
  mycanvas->Print((fn+".png").Data());
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());

  mycanvas->Close();
}
