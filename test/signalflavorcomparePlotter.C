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
//void drawPlots(TH1* hs_DY,TH1* hs_ttbar,TH1* hs_WJets,TH1* hs_WZ,TH1* hs_ZZ,TH1* hs_WW,TH1* hs_data, TString xtitle, TString fname);
void drawPlots(TH1* hs_signal_ee_1,TH1* hs_signal_ee_2,TH1* hs_signal_ee_3,TH1* hs_signal_mumu_1,TH1* hs_signal_mumu_2,TH1* hs_signal_mumu_3, TString xtitle, TString fname, Selector::tag_t channel);
void Plotter(Selector::tag_t channel);

void signalflavorcomparePlotter(){
  std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE};
  // for(auto c: channels)
  //   Plotter(c);
  Plotter(Selector::EE);

}


void Plotter(Selector::tag_t channel){

  // Signal Points
  TChain * chain_signal_ee_1 = new TChain("Tree_Iter0");
  TChain * chain_signal_ee_2 = new TChain("Tree_Iter0");
  TChain * chain_signal_ee_3 = new TChain("Tree_Iter0");

  TChain * chain_signal_mumu_1 = new TChain("Tree_Iter0");
  TChain * chain_signal_mumu_2 = new TChain("Tree_Iter0");
  TChain * chain_signal_mumu_3 = new TChain("Tree_Iter0");

  
  switch (channel) {
  case Selector::EE:
    chain_signal_ee_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_2000_1000_signal_eeEE.root");
    chain_signal_ee_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_3000_1500_signal_eeEE.root");
    chain_signal_ee_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_4000_2000_signal_eeEE.root");

    chain_signal_mumu_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_2000_1000_signal_mumuMuMu.root");
    chain_signal_mumu_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_3000_1500_signal_mumuMuMu.root");
    chain_signal_mumu_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_4000_2000_signal_mumuMuMu.root");
    
    break;
  case Selector::MuMu:
    chain_signal_ee_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_2000_1000_signal_eeEE.root");
    chain_signal_ee_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_3000_1500_signal_eeEE.root");
    chain_signal_ee_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_4000_2000_signal_eeEE.root");

    chain_signal_mumu_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_2000_1000_signal_mumuMuMu.root");
    chain_signal_mumu_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_3000_1500_signal_mumuMuMu.root");
    chain_signal_mumu_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_4000_2000_signal_mumuMuMu.root");
    
    break;
  case Selector::EMu:
    chain_signal_ee_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_2000_1000_signal_eeEE.root");
    chain_signal_ee_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_3000_1500_signal_eeEE.root");
    chain_signal_ee_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_4000_2000_signal_eeEE.root");

    chain_signal_mumu_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_2000_1000_signal_mumuMuMu.root");
    chain_signal_mumu_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_3000_1500_signal_mumuMuMu.root");
    chain_signal_mumu_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_4000_2000_signal_mumuMuMu.root");

    break;
  default:
    std::cout << "Unknown tag" << std::endl;
  }

  Selector myEvent_signal_ee_1;
  Selector myEvent_signal_ee_2;
  Selector myEvent_signal_ee_3;

  myEvent_signal_ee_1.SetBranchAddresses(chain_signal_ee_1);
  myEvent_signal_ee_2.SetBranchAddresses(chain_signal_ee_2);
  myEvent_signal_ee_3.SetBranchAddresses(chain_signal_ee_3);

  std::vector<TH1*> hs_signal_ee_1;
  MakeHistos(chain_signal_ee_1, &myEvent_signal_ee_1, &hs_signal_ee_1, channel);
  std::vector<TH1*> hs_signal_ee_2;
  MakeHistos(chain_signal_ee_2, &myEvent_signal_ee_2, &hs_signal_ee_2, channel);
  std::vector<TH1*> hs_signal_ee_3;
  MakeHistos(chain_signal_ee_3, &myEvent_signal_ee_3, &hs_signal_ee_3, channel);

  Selector myEvent_signal_mumu_1;
  Selector myEvent_signal_mumu_2;
  Selector myEvent_signal_mumu_3;

  myEvent_signal_mumu_1.SetBranchAddresses(chain_signal_mumu_1);
  myEvent_signal_mumu_2.SetBranchAddresses(chain_signal_mumu_2);
  myEvent_signal_mumu_3.SetBranchAddresses(chain_signal_mumu_3);

  std::vector<TH1*> hs_signal_mumu_1;
  MakeHistos(chain_signal_mumu_1, &myEvent_signal_mumu_1, &hs_signal_mumu_1, channel);
  std::vector<TH1*> hs_signal_mumu_2;
  MakeHistos(chain_signal_mumu_2, &myEvent_signal_mumu_2, &hs_signal_mumu_2, channel);
  std::vector<TH1*> hs_signal_mumu_3;
  MakeHistos(chain_signal_mumu_3, &myEvent_signal_mumu_3, &hs_signal_mumu_3, channel);


  unsigned int nPlots = hs_signal_ee_1.size();

  // hs_data[13]->SetLineColor(kRed);
  // hs_data[13]->Draw();
  // hs_ttbar[13]->Draw("same");
  
  TString xtitles[] = {"leading lepton p_{T}","subleading lepton p_{T}","leading jet p_{T}","subleading jet p_{T}","leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi","Mlljj [GeV]","Mlljj cumulative [GeV]","dilepton mass [GeV]","nPV","HT","dilepton p_{T}","Ml_1jj [GeV]","Ml_2jj [GeV]", "NJets", "S_{T}"};

  TString fnames[] = {"l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","Mlljj","Mlljj_cum","Mll","nPV","HT","pT_ll","Ml1jj","Ml2jj","njets","ST"};

  int i = 0;
  for(unsigned int i = 0; i < nPlots; i++){
    std::string s = std::to_string(i);
    //drawPlots(hs_DY[i],hs_ttbar[i],hs_WJets[i],hs_WZ[i],hs_ZZ[i],hs_WW[i],hs_data[i],xtitles[i],fnames[i]);
    drawPlots(hs_signal_ee_1[i],hs_signal_ee_2[i],hs_signal_ee_3[i],hs_signal_mumu_1[i],hs_signal_mumu_2[i],hs_signal_mumu_3[i],xtitles[i],fnames[i], channel);
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

  Float_t bins[] = { 150,300, 450, 600, 750, 900, 1050, 1200, 1350, 1500, 1650,1800,1950,2100,2250,2400,2550,2700,2850,3000,3150,3300,3450,3600,3750,3900,4150,6000 };
  //Float_t bins[] = { 300, 500, 700, 1030, 1300, 4000,6000 };
  Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;
  //TH1F *h_WR_mass = new TH1F("h_WR_mass","",binnum,bins);
 
  TH1F *h_WR_mass = new TH1F("h_WR_mass","",40,0,6000);
  float dilepton_max = 110.;
  TH1F *h_dilepton_mass = new TH1F("h_dilepton_mass","",40,70,700);
  TH1F *h_nPV = new TH1F("h_nPV","",50,0,50);
  TH1F *h_HT = new TH1F("h_HT","",40,0,3000);
  TH1F *h_pT_ll = new TH1F("h_pT_ll","",40,50,1000);
  TH1F *h_Ml1jj = new TH1F("h_Ml1jj","",40,0,3000);
  TH1F *h_Ml2jj = new TH1F("h_Ml2jj","",40,0,3000);
  TH1F *h_njets = new TH1F("h_njets","",11,0,11);
  TH1F *h_ST = new TH1F("h_ST","",40,0,4000);

  //////
  // Float_t bins[] = { 50,110,130,150,170,190,220,250,290,340,450, 1300,4000 };
  // Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;
  
  TH1F * h_HT_DY = new TH1F("h_HT_DY","",binnum,bins);
  TH1F * h_HT_others = new TH1F("h_HT_others","",binnum,bins);
  TH1F * h_HT_data = new TH1F("h_HT_data","",binnum,bins);
  
  TH1F * h_WR_mass_data = new TH1F("h_WR_mass_data","",40,0,6000);

  TH1F * h_HT_ratio = new TH1F("h_HT_ratio","",binnum,bins);

  //////
  
  Long64_t nEntries = chain->GetEntries();

  cout<< nEntries << endl;

  for(int ev = 0; ev<nEntries; ++ev){
    chain->GetEntry(ev);

    if(myEvent->sublead_lepton_pt < 53) continue;
    //|| myEvent->WR_mass < 600) continue;
    
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
    h_ST->Fill(myEvent->lead_lepton_pt+myEvent->sublead_lepton_pt+myEvent->lead_jet_pt+myEvent->sublead_jet_pt,myEvent->weight);
  }

  TH1 *h_WR_massc = h_WR_mass->GetCumulative(kFALSE);
  h_WR_massc->GetYaxis()->SetRangeUser(1,100000);
  
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
  hs->push_back(h_WR_massc);
  hs->push_back(h_dilepton_mass);
  hs->push_back(h_nPV);
  hs->push_back(h_HT);
  hs->push_back(h_pT_ll);
  hs->push_back(h_Ml1jj);
  hs->push_back(h_Ml2jj);
  hs->push_back(h_njets);
  hs->push_back(h_ST);

}

void drawPlots(TH1* hs_signal_ee_1,TH1* hs_signal_ee_2,TH1* hs_signal_ee_3,TH1* hs_signal_mumu_1,TH1* hs_signal_mumu_2,TH1* hs_signal_mumu_3, TString xtitle, TString fname, Selector::tag_t channel){

  TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  leg->AddEntry( hs_signal_ee_1, "Electron M_{WR}=2 TeV" ) ; 
  leg->AddEntry( hs_signal_mumu_1, "Muon M_{WR}=2 TeV" ) ; 
  leg->AddEntry( hs_signal_ee_2, "Electron M_{WR}=3 TeV" ) ; 
  leg->AddEntry( hs_signal_mumu_2, "Muon M_{WR}=3 TeV" ) ;
  leg->AddEntry( hs_signal_ee_3, "Electron M_{WR}=4 TeV" ) ; 
  leg->AddEntry( hs_signal_mumu_3, "Muon M_{WR}=4 TeV" ) ; 

  //leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
  //leg->AddEntry( hs_data, "Data 2016");
  //leg->AddEntry( hs_data2, "Data 2015");
  leg->SetFillColor( kWhite ) ; 


  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  hs_signal_ee_1->SetLineColor(kBlue);
  hs_signal_mumu_1->SetLineColor(kRed);

  hs_signal_ee_2->SetLineColor(kBlack);
  hs_signal_mumu_2->SetLineColor(kGreen);

  hs_signal_ee_3->SetLineColor(kCyan);
  hs_signal_mumu_3->SetLineColor(kMagenta);

  
  
  Double_t eps = 0.001;
  TPad* p1 = new TPad("p1","p1",0,0.25,1,1,0); p1->Draw();
  TPad* p2 = new TPad("p2","p2",0,0.1,1,0.25+eps,0); p2->Draw();
  p1->SetBottomMargin(0);
  p2->SetTopMargin(0);   
  p1->cd();
  gPad->SetTickx();
  gPad->SetTicky();
  hs_signal_ee_1->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_signal_mumu_1->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_signal_ee_1->DrawNormalized("histo");
  hs_signal_mumu_1->DrawNormalized("histo same");
  hs_signal_ee_2->DrawNormalized("histo same");
  hs_signal_mumu_2->DrawNormalized("histo same");
  hs_signal_ee_3->DrawNormalized("histo same");
  hs_signal_mumu_3->DrawNormalized("histo same");
  // hs_data->Draw("epsame");
  //hs_data2->Draw("histo same");
  TH1F *errors = (TH1F*)hs_signal_ee_1->Clone();
  errors->SetLineColor(0);
  errors->SetFillColor(1);
  errors->SetFillStyle(3254);
  //errors->Draw("E2 same");
  TString ytitle = "Events/(";
  ytitle += (hs_signal_ee_1->GetXaxis()->GetNbins());
  ytitle += ")";
  hs_signal_ee_1->GetYaxis()->SetTitle(ytitle.Data());
  hs_signal_ee_1->GetXaxis()->SetTitle(xtitle.Data());
  hs_signal_mumu_1->GetYaxis()->SetTitle(ytitle.Data());
  hs_signal_mumu_1->GetXaxis()->SetTitle(xtitle.Data());

  TH1F *ratio = (TH1F*)hs_signal_ee_1->Clone();
  TH1F *ratio2 = (TH1F*)hs_signal_mumu_1->Clone();  
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
  ratio->SetLineColor(kBlue);
  ratio2->Sumw2();
  ratio2->SetStats(0);

  ratio2->Divide(hs_signal_ee_1);
  ratio->SetMarkerStyle(21);
  ratio->SetMarkerSize(0.5);
  ratio->Divide(hs_signal_ee_1);
  ratio2->SetMarkerStyle(22);
  ratio2->SetMarkerSize(0.5);
  ratio->SetLabelSize(0.1,"y");
  ratio->GetYaxis()->SetRangeUser(0.1,1.9);
  ratio->GetYaxis()->SetNdivisions(505);
  ratio->Draw("p");
  ratio2->Draw("psame");
  float xmax = ratio->GetXaxis()->GetXmax();
  float xmin = ratio->GetXaxis()->GetXmin();
  TF1 *f1 = new TF1("f1","1",xmin,xmax);
  ratio->Draw("p");
  //ratio2->Draw("psame");
  f1->Draw("same");
  mycanvas->cd();

  TString fn = "";

  if(channel == Selector::EE)
    fn = "~/www/WR/plots/miniTree/Selected/EESignalFlavorCompare/"+fname;
  //fn = "plots/EELowDilepton/"+fname;
  if(channel == Selector::MuMu)
    fn = "~/www/WR/plots/miniTree/Selected/MuMuSignalFlavorCompare/"+fname;
  //fn = "plots/MuMuLowDilepton/"+fname;

  mycanvas->Print((fn+".pdf").Data());
  mycanvas->Print((fn+".png").Data());
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());

  mycanvas->Close();
}
