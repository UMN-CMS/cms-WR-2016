#include "TChain.h"
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
#include <fstream>
#include <string>
// #include "ExoAnalysis/cmsWR/src/Selector.cc"
// #include "ExoAnalysis/cmsWR/src/miniTreeEvent.cc"
#include "../src/Selector.cc"
#include "../src/miniTreeEvent.cc"
#include <cstdio>
#include <memory>


Float_t CalculateSF(Selector::tag_t channel);
void DYNormalization(){

  std::ifstream  src("configs/2016-v1.conf", std::ios::binary);
  std::ofstream  dst("configs/2016-v2.conf",   std::ios::binary);
  dst << src.rdbuf();

  Selector::tag_t channel = Selector::MuMu;
  dst<<"DYScale_MuMu="<<CalculateSF(channel)<<std::endl;
  channel = Selector::EE;
  dst<<"DYScale_EE="<<CalculateSF(channel);
    
}

Float_t CalculateSF(Selector::tag_t channel){
  
  TChain * chain_DY = new TChain("Tree_Iter0");
  TChain * chain_others = new TChain("Tree_Iter0");
  TChain * chain_data = new TChain("Tree_Iter0");

  
  switch (channel) {
  case Selector::EE:
    //chain_DY->Add("selected_tree_DYAMCPT_lowdileptonsidebandEE.root");
    chain_DY->Add("selected_tree_DYAMC_lowdileptonsidebandEE.root");
    chain_others->Add("selected_tree_TT_lowdileptonsidebandEE.root");
    chain_others->Add("selected_tree_W_lowdileptonsidebandEE.root");
    chain_others->Add("selected_tree_WZ_lowdileptonsidebandEE.root");
    chain_others->Add("selected_tree_ZZ_lowdileptonsidebandEE.root");
    chain_others->Add("selected_tree_WW_lowdileptonsidebandEE.root");
    chain_data->Add("selected_tree_data_lowdileptonsidebandEE.root");
    break;
  case Selector::MuMu:
    //chain_DY->Add("selected_tree_DYAMCPT_lowdileptonsidebandMuMu.root");
    chain_DY->Add("selected_tree_DYAMC_lowdileptonsidebandMuMu.root");
    //chain_DY->Add("selected_tree_DYMADHT_lowdileptonsidebandMuMu.root");
    chain_others->Add("selected_tree_TT_lowdileptonsidebandMuMu.root"); // 1 - Muons
    chain_others->Add("selected_tree_W_lowdileptonsidebandMuMu.root");
    chain_others->Add("selected_tree_WZ_lowdileptonsidebandMuMu.root");
    chain_others->Add("selected_tree_ZZ_lowdileptonsidebandMuMu.root");
    chain_others->Add("selected_tree_WW_lowdileptonsidebandMuMu.root");
    chain_data->Add("selected_tree_data_lowdileptonsidebandMuMu.root");
    break;
  case Selector::EMu:
    std::cout << "Not calculated for EMu" << std::endl;
  default:
    std::cout << "Unknown tag" << std::endl;

  }

  Selector myEvent_DY;
  Selector myEvent_others;
  Selector myEvent_data;

  myEvent_DY.SetBranchAddresses(chain_DY);
  myEvent_others.SetBranchAddresses(chain_others);
  myEvent_data.SetBranchAddresses(chain_data);

  TH1F * h_Mll_DY = new TH1F("h_Mll_DY","",20,80,100);
  TH1F * h_Mll_others = new TH1F("h_Mll_others","",20,80,100);
  TH1F * h_Mll_data = new TH1F("h_Mll_data","",20,80,100);

  Long64_t nEntries_DY = chain_DY->GetEntries();
  for(int ev = 0; ev<nEntries_DY; ++ev){
    chain_DY->GetEntry(ev);
    h_Mll_DY->Fill(myEvent_DY.dilepton_mass,myEvent_DY.weight);
  }
  Long64_t nEntries_others = chain_others->GetEntries();
  for(int ev = 0; ev<nEntries_others; ++ev){
    chain_others->GetEntry(ev);
    h_Mll_others->Fill(myEvent_others.dilepton_mass,myEvent_others.weight);
  }
  Long64_t nEntries_data = chain_data->GetEntries();
  for(int ev = 0; ev<nEntries_data; ++ev){
    chain_data->GetEntry(ev);
    h_Mll_data->Fill(myEvent_data.dilepton_mass,myEvent_data.weight);
  }

  std::cout<< "Integrals"<<std::endl;
  std::cout<<"DY="<< h_Mll_DY->Integral()<<std::endl;
  std::cout<<"others="<< h_Mll_others->Integral()<<std::endl;
  std::cout<<"data="<< h_Mll_data->Integral()<<std::endl;

  Float_t SF = 1.0;

  SF = (h_Mll_data->Integral() - h_Mll_others->Integral())/h_Mll_DY->Integral();


  return SF;
  
}
