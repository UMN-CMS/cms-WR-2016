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
#include "CMS_lumi.C"
#include <cstdio>
#include <memory>


#ifdef __CINT__
#pragma link C++ class std::vector<TLorentzVector>+;
#endif

void MakeHistos(TChain* chain, Selector *myEvent, std::vector<TH1*> *hs, Selector::tag_t channel);
//void drawPlots(TH1F* hs_DY,TH1F* hs_ttbar,TH1F* hs_WJets,TH1F* hs_WZ,TH1F* hs_ZZ,TH1F* hs_WW,TH1F* hs_data, TString xtitle, TString fname);
void drawPlots(TH1* hs_DY,TH1* hs_DY2,TH1* hs_ttbar,TH1* hs_ttbar2,TH1* hs_others,TH1* hs_others2,TH1* hs_data, TString xtitle, TString fname, Selector::tag_t channel);
void Plotter(Selector::tag_t channel);

void signalcomparePlotter(){
  std::vector<Selector::tag_t> channels = {Selector::MuMu,Selector::EE};
  // for(auto c: channels)
    // Plotter(c);
   Plotter(Selector::EE);
  // Plotter(Selector::MuMu);
}


void Plotter(Selector::tag_t channel){
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
  
  TString inputDir = "/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/selectedTrees_WRv07/selectedTreesWRv07/";
  TString inputDirNewXs = "/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/selectedTrees_WRv07/selectedTrees_WRv07_newXsections/";
  TString inputDir2015 = "/afs/cern.ch/work/s/skalafut/public/WR_starting2015/forJorge2015BackgroundEstimates/";

  switch (channel) {
  case Selector::EE:
    // chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_signal_eeEE.root");
    // chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_signal_eeEE.root");
    // chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    // chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_signal_eeEE.root");
    // //chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_signal_eeEE.root");
    // //chain_data->Add("/afs/cern.ch/work/s/skalafut/public/WR_starting2015/wrDevelopment/CMSSW_7_4_15_patch1/src/ExoAnalysis/cmsWR/analysisCppOutputRootFiles/selected_tree_data_signal_eeEE.root");
    // // chain_signal_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_2000_1000_signal_eeEE.root");
    // // chain_signal_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_3000_1500_signal_eeEE.root");
    // // chain_signal_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_4000_2000_signal_eeEE.root");
    chain_DY->Add(inputDirNewXs+"selected_tree_DYAMCPT_1_signal_eeEE.root");
    chain_ttbar->Add(inputDir+"selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add(inputDirNewXs+"selected_tree_Other_signal_eeEE.root");

    chain_DY2->Add(inputDir2015+"selected_tree_DYMadInclAndHT_signal_eeEE_withMllWeight.root");
    chain_ttbar2->Add(inputDir2015+"selected_tree_data_flavoursidebandEMu.root");
    chain_others2->Add(inputDir2015+"selected_tree_WInclAndHT_signal_eeEE.root");
    chain_others2->Add(inputDir2015+"selected_tree_WZ_signal_eeEE.root");
    chain_others2->Add(inputDir2015+"selected_tree_ZZ_signal_eeEE.root");
    chain_others2->Add(inputDir2015+"selected_tree_qcdData_signal_eeEE.root");    
    break;

  case Selector::MuMu:
    // chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYAMCPT_signal_mumuMuMu.root");
    // chain_DY->Add("~/nobackup/selected/WRv07/selected_tree_DYMADHT_signal_mumuMuMu.root");
    // chain_ttbar->Add("~/nobackup/selected/WRv07/selected_tree_data_flavoursidebandEMu.root");
    // chain_others->Add("~/nobackup/selected/WRv07/selected_tree_Other_signal_mumuMuMu.root");
    // //chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_signal_mumuMuMu.root");
    // //chain_signal_1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_2000_1000_signal_mumuMuMu.root");
    // //chain_signal_2->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_3000_1500_signal_mumuMuMu.root");
    // //chain_signal_3->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_4000_2000_signal_mumuMuMu.root");
    chain_DY->Add(inputDirNewXs+"selected_tree_DYAMCPT_1_signal_mumuMuMu.root");
    chain_ttbar->Add(inputDir+"selected_tree_data_flavoursidebandEMu.root");
    chain_others->Add(inputDirNewXs+"selected_tree_Other_signal_mumuMuMu.root");  

    chain_DY2->Add(inputDir2015+"selected_tree_DYMadInclAndHT_signal_mumuMuMu_withMllWeight.root");
    chain_ttbar2->Add(inputDir2015+"selected_tree_data_flavoursidebandEMu.root");
    chain_others2->Add(inputDir2015+"selected_tree_WInclAndHT_signal_mumuMuMu.root");
    chain_others2->Add(inputDir2015+"selected_tree_WZ_signal_mumuMuMu.root");
    chain_others2->Add(inputDir2015+"selected_tree_ZZ_signal_mumuMuMu.root");
    chain_others2->Add(inputDir2015+"selected_tree_qcdData_signal_mumuMuMu.root");
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

  std::vector<TH1*> hs_DY;
  MakeHistos(chain_DY, &myEvent_DY, &hs_DY, channel);
  std::vector<TH1*> hs_DY2;
  MakeHistos(chain_DY2, &myEvent_DY2, &hs_DY2, channel);
  std::vector<TH1*> hs_ttbar;
  MakeHistos(chain_ttbar, &myEvent_ttbar, &hs_ttbar, channel);
  std::vector<TH1*> hs_ttbar2;
  MakeHistos(chain_ttbar2, &myEvent_ttbar2, &hs_ttbar2, channel);
  std::vector<TH1*> hs_others;
  MakeHistos(chain_others, &myEvent_others, &hs_others, channel);
  std::vector<TH1*> hs_others2;
  MakeHistos(chain_others2, &myEvent_others2, &hs_others2, channel);

  std::vector<TH1*> hs_data;
  MakeHistos(chain_data, &myEvent_data, &hs_data, channel);

  unsigned int nPlots = hs_DY.size();

  // hs_data[13]->SetLineColor(kRed);
  // hs_data[13]->Draw();
  // hs_ttbar[13]->Draw("same");
  
  TString xtitles[] = {"leading lepton p_{T} (GeV)","subleading lepton p_{T} (GeV)","leading jet p_{T} (GeV)","subleading jet p_{T} (GeV)",
  "leading lepton #eta","subleading lepton #eta","leading jet #eta","subleading jet #eta","leading lepton #phi","subleading lepton #phi","leading jet #phi","subleading jet #phi", 
  "m_{lljj} (GeV)","m_{lljj} (GeV)","m_{ll} (GeV)","nPV","#Sigma #it{p}_{T}^{jets} (GeV)","dilepton p_{T} (GeV)","m_{l_{1}jj} (GeV)","m_{l_{2}jj} (GeV)","n_{jets}","S_{T}"};

  TString fnames[] = {"l1_pt","l2_pt","j1_pt","j2_pt","l1_eta","l2_eta","j1_eta","j2_eta","l1_phi","l2_phi","j1_phi","j2_phi","Mlljj","Mlljj_cum","Mll","nPV","HT","pT_ll","Ml1jj","Ml2jj","njets","ST"};
  
  int i = 0;
  for(unsigned int i = 0; i < nPlots; i++){
    std::string s = std::to_string(i);
    //drawPlots(hs_DY[i],hs_ttbar[i],hs_WJets[i],hs_WZ[i],hs_ZZ[i],hs_WW[i],hs_data[i],xtitles[i],fnames[i]);
    drawPlots(hs_DY[i],hs_DY2[i],hs_ttbar[i],hs_ttbar2[i],hs_others[i],hs_others2[i],hs_data[i],xtitles[i],fnames[i], channel);
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

  float dilepton_max = 110.;
  TH1F *h_dilepton_mass = new TH1F("h_dilepton_mass","",40,70,1000);
  TH1F *h_nPV = new TH1F("h_nPV","",50,0,50);
  TH1F *h_HT = new TH1F("h_HT","",40,0,3000);
  TH1F *h_pT_ll = new TH1F("h_pT_ll","",40,50,1000);
  TH1F *h_Ml1jj = new TH1F("h_Ml1jj","",40,0,3000);
  TH1F *h_Ml2jj = new TH1F("h_Ml2jj","",40,0,3000);
  TH1F *h_njets = new TH1F("h_njets","",11,0,11);

  //////
  //Float_t bins[] = { 50,110,130,150,170,190,220,250,290,340,450, 1300,4000 };
  //Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;

  Float_t bins[] = { 150,300, 450, 600, 750, 900, 1050, 1200, 1350, 1500, 1650,1800,1950,2100,2250,2400,2550,2700,2850,3000,3150,3300,3450,3600,3750,3900,4150,7000 };
  //Float_t bins[] = { 300, 500, 700, 1030, 1300, 4000,6000 };
  Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;
  TH1F *h_WR_mass = new TH1F("h_WR_mass","",binnum,bins);
  //TH1F *h_WR_mass = new TH1F("h_WR_mass","",40,0,6000);

  
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

    if(myEvent->sublead_lepton_pt < 53 || myEvent->WR_mass < 600) continue;
    
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

  TH1 *h_WR_massc = h_WR_mass->GetCumulative(kFALSE);
  //h_WR_massc->GetYaxis()->SetRangeUser(0.01,200000);


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


}

void drawPlots(TH1* hs_DY,TH1* hs_DY2,TH1* hs_ttbar,TH1* hs_ttbar2,TH1* hs_others,TH1* hs_others2,TH1* hs_data, TString xtitle, TString fname, Selector::tag_t channel){

  if(channel == Selector::EE) {
    if(xtitle == "dilepton mass (GeV)") {
      xtitle = "#it{m}_{ee} (GeV)";
    }
    if(xtitle == "dilepton p_{T} (GeV)") {
      xtitle = "#it{p}_{T}^{ee} (GeV)";
    }
    if(xtitle == "m_{lljj} (GeV)") {
      xtitle = "#it{m}_{eejj} (GeV)";
    }   
  }

  if(channel == Selector::MuMu) {
    if(xtitle == "dilepton mass (GeV)") {
      xtitle = "#it{m}_{#mu#mu} (GeV)";
    }
    if(xtitle == "dilepton p_{T} (GeV)") {
      xtitle = "#it{p}_{T}^{#mu#mu} (GeV)";
    }
    if(xtitle == "m_{lljj} (GeV)") {
      xtitle = "#it{m}_{#mu#mujj} (GeV)";
    }   
  }

  // TLegend *leg = new TLegend( 0.72, 0.50, 0.98, 0.70 ) ; 
  TLegend *leg = new TLegend( 0.62, 0.7, 0.9, 0.9 ) ;
  leg->AddEntry( hs_DY, "2016" ) ; 
  leg->AddEntry( hs_DY2, "2015" ) ; 
  //leg->AddEntry( histos[2][0], "10 x WR 2600" ) ; 
  //leg->AddEntry( hs_data, "Data");
  leg->SetFillColor( kWhite ) ; 


  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  hs_DY->SetLineColor(kRed);
  hs_DY2->SetLineColor(kBlue);

  //hs_DY->Scale(1.0/1.3);
  // hs_ttbar->Scale(2.6/35.8);
  // hs_others->Scale(2.6/35.8);

  if(channel == Selector::EE){
    hs_ttbar->Scale(0.423);
    hs_ttbar2->Scale(0.419);
  }
  else if(channel == Selector::MuMu){
    hs_ttbar->Scale(0.676);
    hs_ttbar2->Scale(0.656);
  }

  
  // hs_DY->Scale(0.5);
  
  hs_DY->Add(hs_others);
  hs_DY->Add(hs_ttbar);

  hs_DY2->Add(hs_others2);
  hs_DY2->Add(hs_ttbar2);

  hs_DY2->Scale(35.9/2.6);


  hs_data->SetMarkerStyle(20);


  Double_t eps = 0.001;
  // TPad* p1 = new TPad("p1","p1",0,0.25,1,1,0); p1->Draw();
  // TPad* p2 = new TPad("p2","p2",0,0.1,1,0.25+eps,0); p2->Draw();
  TPad* p1 = new TPad("p1","p1",0,0.26,1,1,0); p1->Draw();
  TPad* p2 = new TPad("p2","p2",0,0,1,0.24,0); p2->Draw();
  p1->SetBottomMargin(0);
  p2->SetTopMargin(0);   
  p2->SetBottomMargin(0.5); 
  p1->cd();
  gPad->SetTickx();
  gPad->SetTicky();
  hs_DY->SetStats(0);
  hs_data->SetStats(0);
  TH1F *ratio = (TH1F*)hs_DY->Clone();
  TH1F *ratio2 = (TH1F*)hs_data->Clone();
  // hs_DY->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  // hs_DY2->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");
  // hs_data->SetTitle("CMS Preliminary            35.87 fb^{-1} (13 TeV)");

  //th->Draw("histo");
  //hs_data->Draw("epsame");
  //hs_data->Draw("ep");
  hs_DY->Draw("");
  hs_DY2->Draw("same");
  //hs_data->Draw("epsame");
  TH1F *errors = (TH1F*)hs_DY->Clone();
  errors->SetLineColor(0);
  errors->SetFillColor(1);
  errors->SetFillStyle(3254);
  //errors->Draw("E2 same");

  TString ytitle = "Events/";
  ytitle += round(hs_DY->GetXaxis()->GetBinWidth(5));
  ytitle += " GeV";

  if (fname == "Mlljj") ytitle = "Events/bin";
  if ( (string(fname).find("eta") != string::npos) || (string(fname).find("phi") != string::npos) || (string(fname).find("nPV") != string::npos) || 
    (string(fname).find("njets") != string::npos) || (string(fname).find("delta") != string::npos) ) ytitle = "Events/bin";
  if (fname == "njets") ytitle = "Events/1 unit";

  hs_DY->GetYaxis()->SetTitle(ytitle.Data());
  // hs_DY->GetXaxis()->SetTitle(xtitle.Data());
  hs_DY2->GetYaxis()->SetTitle(ytitle.Data());
  // hs_DY2->GetXaxis()->SetTitle(xtitle.Data());

  hs_DY->GetYaxis()->SetTitleSize(0.05);
  hs_DY->GetYaxis()->SetTitleOffset(0.9); 
  hs_DY->GetYaxis()->SetLabelSize(0.04);  
  hs_DY->GetXaxis()->SetLabelSize(0); 

  //cout<<"Bins1="<<((TH1*)th->GetStack()->Last())->FindBin(80)<<std::endl;
  //cout<<"Bins2="<<((TH1*)th->GetStack()->Last())->FindBin(100)<<std::endl;
  
  cout<<"Integral="<<hs_DY->Integral()/hs_DY2->Integral()<<endl;
  
  ratio->GetXaxis()->SetTitle(xtitle.Data());
  //ths[icanvas]->GetXaxis()->SetTickSize(1.0);
  //ths[icanvas]->GetXaxis()->SetTitleSize(0.1);
  ratio->GetXaxis()->SetTickSize(0.40);
  ratio->GetXaxis()->SetTitleSize(0.18);
  ratio->SetLabelSize(0.15,"x");

  ratio->GetYaxis()->SetTitle("ratio");
  ratio->GetYaxis()->SetTitleSize(0.15);
  ratio->GetYaxis()->SetTitleOffset(0.3); 
  ratio->SetLabelSize(0.12,"y");  
  ratio->GetYaxis()->SetRangeUser(0.5,1.5);
  ratio->GetYaxis()->SetNdivisions(505);

  leg->Draw(); 
  mycanvas->cd();
  p2->cd();
  p2->SetGridy(); 
  ratio->Sumw2();
  ratio->SetStats(0);
  ratio->SetLineColor(kRed);
  ratio2->Sumw2();
  ratio2->SetStats(0);

  ratio->Divide(hs_DY2);
  ratio->SetMarkerStyle(21);
  ratio->SetMarkerSize(0.5);

  ratio2->Divide(hs_DY2);
  ratio2->SetMarkerStyle(22);
  ratio2->SetMarkerSize(0.5);

  ratio->Draw("p");
  //ratio2->Draw("psame");
  float xmax = ratio->GetXaxis()->GetXmax();
  float xmin = ratio->GetXaxis()->GetXmin();
  TF1 *f1 = new TF1("f1","1",xmin,xmax);
  ratio->Draw("p");
  //ratio2->Draw("psame");
  f1->Draw("same");
  mycanvas->cd();

  // CMS_lumi(mycanvas,"Preliminary");  
  CMS_lumi(mycanvas,"");

  TString fn = "";

  TString outputdir = "/afs/cern.ch/user/g/gnegro/www/cmsWR/thesis/";

  TString dir = "";

  if(channel == Selector::EE)
    // fn = "~/www/WR/plots/miniTree/Selected/EESignalCompare/"+fname;
    //fn = "plots/EESignalCompare/"+fname;
    fn = outputdir+"EESignalCompare/"+dir+fname;
  if(channel == Selector::MuMu)
    // fn = "~/www/WR/plots/miniTree/Selected/MuMuSignalCompare/"+fname;
  //fn = "plots/MuMuSignalCompare/"+fname;
    fn = outputdir+"MuMuSignalCompare/"+dir+fname;

  mycanvas->Print((fn+".pdf").Data());
  mycanvas->Print((fn+".png").Data());
  mycanvas->Print((fn+".root").Data());
  p1->SetLogy();
  mycanvas->Print((fn+"_log.pdf").Data());
  mycanvas->Print((fn+"_log.png").Data());
  mycanvas->Print((fn+"_log.root").Data());

  mycanvas->Close();
}
