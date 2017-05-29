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

void MakeHistos(TChain* chain, Selector *myEvent, std::vector<TH1*> *hs);
void drawPlots(TH1* hs_DY,TH1* hs_DY2,TH1* hs_ttbar,TH1* hs_ttbar2,TH1* hs_others,TH1* hs_others2,TH1* hs_data,TH1* hs_data2, TString xtitle, TString fname, TString region);
void Plotter(TString region);

void flavorcomparePlotter(){
  std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE};
   // for(auto c: channels)
   //   Plotter(c);
  // Plotter("signalAMC");
  // Plotter("signalMAD");
  //Plotter("sidebandAMCAbs");
  //Plotter("sidebandMADAbs");
  Plotter("sidebandCOMB");
  Plotter("dataNorm");

}


void Plotter(TString region){
  TChain * chain_DY = new TChain("Tree_Iter0");
  TChain * chain_DY2 = new TChain("Tree_Iter0");
  TChain * chain_ttbar = new TChain("Tree_Iter0");
  TChain * chain_ttbar2 = new TChain("Tree_Iter0");
  TChain * chain_others = new TChain("Tree_Iter0");
  TChain * chain_others2 = new TChain("Tree_Iter0");
  TChain * chain_WJets = new TChain("Tree_Iter0");
  TChain * chain_WZ = new TChain("Tree_Iter0");
  TChain * chain_ZZ = new TChain("Tree_Iter0");
  TChain * chain_WW = new TChain("Tree_Iter0");
  TChain * chain_data = new TChain("Tree_Iter0");
  TChain * chain_data2 = new TChain("Tree_Iter0");
  
  if(region == "signalAMC"){
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_signal_eeEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_signal_eeEE.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_signal_eeEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_signal_mumuMuMu.root");
    chain_ttbar2->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    chain_others2->Add("~/nobackup/selected/WRv07/selected_tree_Other_signal_mumuMuMu.root");
  }
  else if(region == "signalMAD"){
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_signal_eeEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_signal_eeEE.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_signal_eeEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_signal_mumuMuMu.root");
    chain_ttbar2->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    chain_others2->Add("~/nobackup/selected/WRv07/selected_tree_Other_signal_mumuMuMu.root");
  }
  else if(region == "sidebandAMC" || region == "sidebandAMCAbs"){
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_lowdileptonsidebandEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandEE.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_lowdileptonsidebandMuMu.root");
    chain_ttbar2->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandMuMu.root");
    chain_others2->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandMuMu.root");
  }
  else if(region == "sidebandMAD" || region == "sidebandMADAbs"){
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_lowdileptonsidebandEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandEE.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_lowdileptonsidebandMuMu.root");
    chain_ttbar2->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandMuMu.root");
    chain_others2->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandMuMu.root");
  }
  else if(region == "sidebandCOMB" || region == "sidebandCOMBAbs"){
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_lowdileptonsidebandEE.root");
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_lowdileptonsidebandEE.root");
    chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandEE.root");
    chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_lowdileptonsidebandMuMu.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_lowdileptonsidebandMuMu.root");
    chain_ttbar2->Add("~/nobackup/selected/WRv07/selected_tree_TTAMC_lowdileptonsidebandMuMu.root");
    chain_others2->Add("~/nobackup/selected/WRv07/selected_tree_Other_lowdileptonsidebandMuMu.root");
  }
  else if(region == "dataNorm"){
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_data_lowdileptonsidebandEE.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_signal_eeEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_data_lowdileptonsidebandMuMu.root");
  }
  else if(region == "dataAbs"){
    chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_data_lowdileptonsidebandEE.root");
    //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_signal_eeEE.root");
    chain_DY2->Add("~/nobackup/selected/WRv07/selected_tree_data_lowdileptonsidebandMuMu.root");
  }
  
  Selector myEvent_DY;
  Selector myEvent_DY2;
  Selector myEvent_ttbar;
  Selector myEvent_ttbar2;
  Selector myEvent_others;
  Selector myEvent_others2;
  Selector myEvent_WJets;
  Selector myEvent_WZ;
  Selector myEvent_ZZ;
  Selector myEvent_WW;
  Selector myEvent_data;
  Selector myEvent_data2;

  myEvent_DY.SetBranchAddresses(chain_DY);
  myEvent_DY2.SetBranchAddresses(chain_DY2);
  myEvent_ttbar.SetBranchAddresses(chain_ttbar);
  myEvent_ttbar2.SetBranchAddresses(chain_ttbar2);
  myEvent_others.SetBranchAddresses(chain_others);
  myEvent_others2.SetBranchAddresses(chain_others2);
  myEvent_WJets.SetBranchAddresses(chain_WJets);
  myEvent_WZ.SetBranchAddresses(chain_WZ);
  myEvent_ZZ.SetBranchAddresses(chain_ZZ);
  myEvent_WW.SetBranchAddresses(chain_WW);
  myEvent_data.SetBranchAddresses(chain_data);
  myEvent_data2.SetBranchAddresses(chain_data2);

  std::vector<TH1*> hs_DY;
  MakeHistos(chain_DY, &myEvent_DY, &hs_DY);
  std::vector<TH1*> hs_DY2;
  MakeHistos(chain_DY2, &myEvent_DY2, &hs_DY2);
  std::vector<TH1*> hs_ttbar;
  MakeHistos(chain_ttbar, &myEvent_ttbar, &hs_ttbar);
  std::vector<TH1*> hs_ttbar2;
  MakeHistos(chain_ttbar2, &myEvent_ttbar2, &hs_ttbar2);
  std::vector<TH1*> hs_others;
  MakeHistos(chain_others, &myEvent_others, &hs_others);
  std::vector<TH1*> hs_others2;
  MakeHistos(chain_others2, &myEvent_others2, &hs_others2);

  std::vector<TH1*> hs_data;
  MakeHistos(chain_data, &myEvent_data, &hs_data);
  std::vector<TH1*> hs_data2;
  MakeHistos(chain_data2, &myEvent_data2, &hs_data2);

  unsigned int nPlots = hs_DY.size();

  // hs_data[13]->SetLineColor(kRed);
  // hs_data[13]->Draw();
  // hs_ttbar[13]->Draw("same");
  
  TString xtitles[] = {"leading lepton p_{T}","subleading lepton p_{T}","leading jet p_{T}","subleading jet p_{T}","leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi","Mlljj [GeV]","Mlljj cumulative [GeV]","dilepton mass [GeV]","nPV","HT","dilepton p_{T}","Ml_1jj [GeV]","Ml_2jj [GeV]", "NJets", "S_{T}","#Delta#phi"};

  TString fnames[] = {"l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","Mlljj","Mlljj_cum","Mll","nPV","HT","pT_ll","Ml1jj","Ml2jj","njets","ST","deltaPhi"};

  int i = 0;
  for(unsigned int i = 0; i < nPlots; i++){
    std::string s = std::to_string(i);
    //drawPlots(hs_DY[i],hs_ttbar[i],hs_WJets[i],hs_WZ[i],hs_ZZ[i],hs_WW[i],hs_data[i],xtitles[i],fnames[i]);
    drawPlots(hs_DY[i],hs_DY2[i],hs_ttbar[i],hs_ttbar2[i],hs_others[i],hs_others2[i],hs_data[i],hs_data2[i],xtitles[i],fnames[i], region);
  }
  
}

void MakeHistos(TChain * chain, Selector *myEvent, std::vector<TH1*> *hs){

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
  TH1F *h_WR_mass = new TH1F("h_WR_mass","",binnum,bins);
 
  //TH1F *h_WR_mass = new TH1F("h_WR_mass","",40,0,6000);
  float dilepton_max = 110.;
  TH1F *h_dilepton_mass = new TH1F("h_dilepton_mass","",40,70,700);
  TH1F *h_nPV = new TH1F("h_nPV","",50,0,50);
  TH1F *h_HT = new TH1F("h_HT","",40,0,3000);
  TH1F *h_pT_ll = new TH1F("h_pT_ll","",40,50,1000);
  TH1F *h_Ml1jj = new TH1F("h_Ml1jj","",40,0,3000);
  TH1F *h_Ml2jj = new TH1F("h_Ml2jj","",40,0,3000);
  TH1F *h_njets = new TH1F("h_njets","",11,0,11);
  TH1F *h_ST = new TH1F("h_ST","",40,0,4000);
  TH1F *h_deltaphi = new TH1F("h_deltaphi","",40,-3,3);

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
    //if(myEvent->WR_mass < 200) continue;    
    //if(myEvent->nPV > 20) continue;    

    //if(fabs(myEvent->lead_lepton_eta) > 1.2 || fabs(myEvent->sublead_lepton_eta) > 1.2 ) continue;
    
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
    h_deltaphi->Fill(TVector2::Phi_mpi_pi(myEvent->lead_lepton_phi- myEvent->sublead_lepton_phi));
  }

  h_WR_mass->GetYaxis()->SetRangeUser(1e-8,1);
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
  hs->push_back(h_deltaphi);

}

void drawPlots(TH1* hs_DY,TH1* hs_DY2,TH1* hs_ttbar,TH1* hs_ttbar2,TH1* hs_others,TH1* hs_others2,TH1* hs_data,TH1* hs_data2, TString xtitle, TString fname, TString region){

  TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  leg->AddEntry( hs_DY, "Electron background" ) ; 
  leg->AddEntry( hs_DY2, "Muon background" ) ; 
  //leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
  //leg->AddEntry( hs_data, "Data 2016");
  //leg->AddEntry( hs_data2, "Data 2015");
  leg->SetFillColor( kWhite ) ; 

  hs_DY->Sumw2();
  hs_DY2->Sumw2();

  
  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  hs_DY->SetLineColor(kBlack);
  hs_DY2->SetLineColor(kRed);
  if (region =="sidebandCOMBAbs" || region =="sidebandCOMB" ){
    hs_DY->Scale(0.5);
    hs_DY2->Scale(0.5);
  }
  
  //hs_ttbar->Scale(0.423);
  //hs_ttbar2->Scale(0.676);
  
  hs_DY->Add(hs_others);
  hs_DY->Add(hs_ttbar);

  hs_DY2->Add(hs_others2);
  hs_DY2->Add(hs_ttbar2); 

  if(!region.Contains("Abs")){
    hs_DY->Scale(1/hs_DY->Integral());    
    hs_DY2->Scale(1/hs_DY2->Integral());
  }
  if(fname=="Mlljj"){
    hs_DY->GetYaxis()->SetRangeUser(10e-6,0.5);
    hs_DY2->GetYaxis()->SetRangeUser(10e-6,0.5);
  }

  if(fname=="Mlljj_cum"){
    hs_DY->GetYaxis()->SetRangeUser(10e-7,0.5);
    hs_DY2->GetYaxis()->SetRangeUser(10e-7,0.5);
  }
  hs_data->SetMarkerStyle(21);
  hs_data2->SetMarkerStyle(22);
  hs_data->SetLineColor(kRed);

  Double_t eps = 0.001;
  TPad* p1 = new TPad("p1","p1",0,0.25,1,1,0); p1->Draw();
  TPad* p2 = new TPad("p2","p2",0,0.1,1,0.25+eps,0); p2->Draw();
  p1->SetBottomMargin(0);
  p2->SetTopMargin(0);   
  p1->cd();
  gPad->SetTickx();
  gPad->SetTicky();
  hs_data->SetStats(0);
  TH1F *ratio = (TH1F*)hs_DY->Clone();
  hs_DY->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_DY2->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  hs_data->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  //th->Draw("histo");
  //hs_data->Draw("epsame");
  //hs_data->Draw("ephisto");
  TH1F *sum = (TH1F*)hs_DY->Clone();
  sum->Add(hs_DY2);
  sum->SetLineColor(kBlue);
  //leg->AddEntry( sum, "Sum of backgrounds" ) ;
  hs_DY->DrawNormalized("histo");
  hs_DY2->DrawNormalized("histo same");
  // sum->DrawNormalized("histo same");
  // hs_DY2->Draw("");
  // hs_DY->Draw("same");
  if(xtitle == "Mlljj [GeV]"){
    // hs_DY->GetYaxis()->SetRangeUser(1e-8,1);
    // hs_DY2->GetYaxis()->SetRangeUser(1e-8,1);
    // sum->GetYaxis()->SetRangeUser(1e-8,1);
    //sum->Fit("expo","","",1000,6000);
  }
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

  // std::cout<<"Means="<<hs_DY->GetMean()<<" "<<hs_DY2->GetMean()<<std::endl;
  // std::cout<<"MeansE="<<hs_DY->GetMeanError()<<" "<<hs_DY2->GetMeanError()<<std::endl;
  // std::cout<<"Nevs="<<hs_DY->GetEntries()<<" "<<hs_DY2->GetEntries()<<std::endl;
  

  //cout<<"Bins1="<<((TH1*)th->GetStack()->Last())->FindBin(80)<<std::endl;
  //cout<<"Bins2="<<((TH1*)th->GetStack()->Last())->FindBin(100)<<std::endl;
  
  cout<<"Integral="<<hs_DY->Integral()<<" "<<hs_DY2->Integral()<<endl;
  
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
  ratio->SetLineColor(kBlack);
  ratio->SetMarkerStyle(21);
  ratio->SetMarkerSize(0.5);
  ratio->Divide(hs_DY2);
  //ratio->Add(hs_DY2,-1);
  ratio->SetLabelSize(0.1,"y");
  ratio->GetYaxis()->SetRangeUser(0.0,1.9);
  ratio->GetYaxis()->SetNdivisions(505);
  ratio->Draw("p");
  float xmax = ratio->GetXaxis()->GetXmax();
  float xmin = ratio->GetXaxis()->GetXmin();
  TF1 *f1 = new TF1("f1","1",xmin,xmax);
  ratio->Draw("p");
  // if(xtitle == "Mlljj [GeV]"){
    // ratio->Fit("pol0");
  //   ratio->Fit("pol1");
  // }
  f1->Draw("same");
  mycanvas->cd();

  if(xtitle == "Mlljj [GeV]"){
    TFile *rs = new TFile("ratios_"+region+".root","recreate");
    rs->cd();
    ratio->Write();
    
  }
  if(xtitle == "Mlljj cumulative [GeV]"){
    TFile *rs = new TFile("ratios_cum_"+region+".root","recreate");
    rs->cd();
    ratio->Write();

    std::cout<<"Cum[0]="<<hs_DY->GetBinContent(1)<<" "<<hs_DY2->GetBinContent(1)<<" "<<ratio->GetBinContent(1) <<std::endl;
    
  }
  
  TString fn = "";

  fn = "~/www/WR/plots/miniTree/Selected/"+region+"FlavorCompare/"+fname;
  
  mycanvas->Print((fn+".pdf").Data());
  mycanvas->Print((fn+".png").Data());
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());

  mycanvas->Close();
}
