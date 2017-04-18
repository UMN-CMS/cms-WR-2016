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


Float_t CalculateSF(Selector::tag_t channel, TString sample);
void DYNormalization(){

  std::ifstream  src("configs/2016-v1.conf", std::ios::binary);
  std::ofstream  dst("configs/2016-v2.conf",   std::ios::binary);
  dst << src.rdbuf();

  TFile f1("DYweights.root","RECREATE");
    
  Selector::tag_t channel = Selector::MuMu;
  //dst<<"DYScale_MuMu="<<CalculateSF(channel,"AMC")<<std::endl;
  dst<<"DYScale_MuMu_PT="<<CalculateSF(channel,"AMCPT")<<std::endl;
  dst<<"DYScale_MuMu_MAD="<<CalculateSF(channel,"MAD")<<std::endl;
  channel = Selector::EE;
  //dst<<"DYScale_EE="<<CalculateSF(channel,"AMC")<<std::endl;
  dst<<"DYScale_EE_PT="<<CalculateSF(channel,"AMCPT")<<std::endl;
  dst<<"DYScale_EE_MAD="<<CalculateSF(channel,"MAD");
    
}

Float_t CalculateSF(Selector::tag_t channel, TString sample){
  
  TChain * chain_DY = new TChain("Tree_Iter0");
  TChain * chain_others = new TChain("Tree_Iter0");
  TChain * chain_data = new TChain("Tree_Iter0");
  // TChain * chain_DY = new TChain("treeDyCheck");
  // TChain * chain_others = new TChain("treeDyCheck");
  // TChain * chain_data = new TChain("treeDyCheck");
  TString flavor = "";
  
  switch (channel) {
  case Selector::EE:
    if(sample == "AMC")
      chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMC_lowdileptonsidebandEE_withoutMllWeight.root");
    else if(sample == "AMCPT")
      chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_lowdileptonsidebandEE_withoutMllWeight.root");
      //chain_DY->Add("~/nobackup/selected/selected_tree_DYAMCPT_lowdileptonsidebandEE_withoutMllWeight.root");
    else if(sample == "MAD")
      chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYMADHT_lowdileptonsidebandEE_withoutMllWeight.root");
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_TTAMC_lowdileptonsidebandEE.root");
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_Other_lowdileptonsidebandEE.root");
    chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_lowdileptonsidebandEE.root");
    flavor = "EE";
    break;
  case Selector::MuMu:
    if(sample == "AMC")
      chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMC_lowdileptonsidebandMuMu_withoutMllWeight.root");
    else if(sample == "AMCPT")
      //      chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_lowdileptonsidebandMuMu_withoutMllWeight.root");
      chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYAMCPT_lowdileptonsidebandMuMu_withoutMllWeight.root");
    else if(sample == "MAD")
      chain_DY->Add("~/nobackup/selected/WRv06/selected_tree_DYMADHT_lowdileptonsidebandMuMu_withoutMllWeight.root");
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_TTAMC_lowdileptonsidebandMuMu.root"); // 1 - Muons
    chain_others->Add("~/nobackup/selected/WRv06/selected_tree_Other_lowdileptonsidebandMuMu.root");
    chain_data->Add("~/nobackup/selected/WRv06/selected_tree_data_lowdileptonsidebandMuMu.root");
    flavor = "MuMu";
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
  // WR MASS

  Float_t bins[] = { 300, 500, 700, 1030, 1300, 4000,6000 };
  //Float_t bins[] = { 150,300, 450, 600, 750, 900, 1050, 1200, 1350, 1500, 1650,1800,1950,2100,2250,2400,2550,2700,2850,3000,3150,3300,3450,3600,3750,3900,4150,5000,7000 };
  Int_t  binnum = sizeof(bins)/sizeof(Float_t) - 1;
  
  // TH1F * h_Mll_DY_MWR = new TH1F("h_Mll_DY_MWR_"+sample+"_"+flavor,"",5,200,4000);
  // TH1F * h_Mll_others_MWR = new TH1F("h_Mll_others_MWR_"+sample+"_"+flavor,"",5,200,4000);
  // TH1F * h_Mll_data_MWR = new TH1F("h_Mll_data_MWR_"+sample+"_"+flavor,"",5,200,4000);
  TH1F * h_Mll_DY_MWR = new TH1F("h_Mll_DY_MWR_"+sample+"_"+flavor,"",binnum,bins);
  TH1F * h_Mll_others_MWR = new TH1F("h_Mll_others_MWR_"+sample+"_"+flavor,"",binnum,bins);
  TH1F * h_Mll_data_MWR = new TH1F("h_Mll_data_MWR_"+sample+"_"+flavor,"",binnum,bins);
  // NJets
  TH1F * h_Mll_DY_njets = new TH1F("h_Mll_DY_njets_"+sample+"_"+flavor,"",5,2,7);
  TH1F * h_Mll_others_njets = new TH1F("h_Mll_others_njets_"+sample+"_"+flavor,"",5,2,7);
  TH1F * h_Mll_data_njets = new TH1F("h_Mll_data_njets_"+sample+"_"+flavor,"",5,2,7);
  // Zpt
  TH1F * h_Mll_DY_Zpt = new TH1F("h_Mll_DY_Zpt_"+sample+"_"+flavor,"",10,100,1000);
  TH1F * h_Mll_others_Zpt = new TH1F("h_Mll_others_Zpt_"+sample+"_"+flavor,"",10,100,1000);
  TH1F * h_Mll_data_Zpt = new TH1F("h_Mll_data_Zpt_"+sample+"_"+flavor,"",10,100,1000);
  // L1_pt
  TH1F * h_Mll_DY_l1pt = new TH1F("h_Mll_DY_l1pt_"+sample+"_"+flavor,"",20,0,700);
  TH1F * h_Mll_others_l1pt = new TH1F("h_Mll_others_l1pt_"+sample+"_"+flavor,"",20,0,700);
  TH1F * h_Mll_data_l1pt = new TH1F("h_Mll_data_l1pt_"+sample+"_"+flavor,"",20,0,700);
  // St
  Float_t bins2[] = { 300, 500, 700, 1300, 2000,4000 };
  Int_t  binnum2 = sizeof(bins2)/sizeof(Float_t) - 1;
  TH1F * h_Mll_DY_St = new TH1F("h_Mll_DY_St_"+sample+"_"+flavor,"",binnum2,bins2);
  TH1F * h_Mll_others_St = new TH1F("h_Mll_others_St_"+sample+"_"+flavor,"",binnum2,bins2);
  TH1F * h_Mll_data_St = new TH1F("h_Mll_data_St_"+sample+"_"+flavor,"",binnum2,bins2);

  Long64_t nEntries_DY = chain_DY->GetEntries();
  for(int ev = 0; ev<nEntries_DY; ++ev){
    chain_DY->GetEntry(ev);
    h_Mll_DY->Fill(myEvent_DY.dilepton_mass,myEvent_DY.weight);
    if(myEvent_DY.dilepton_mass>80 && myEvent_DY.dilepton_mass<100){
      h_Mll_DY_MWR->Fill(myEvent_DY.WR_mass,myEvent_DY.weight);
      h_Mll_DY_njets->Fill(myEvent_DY.njets,myEvent_DY.weight);
      h_Mll_DY_Zpt->Fill(myEvent_DY.dilepton_pt,myEvent_DY.weight);
      h_Mll_DY_l1pt->Fill(myEvent_DY.lead_lepton_pt,myEvent_DY.weight);
      h_Mll_DY_St->Fill(myEvent_DY.lead_lepton_pt+myEvent_DY.sublead_lepton_pt+myEvent_DY.lead_jet_pt+myEvent_DY.sublead_jet_pt,myEvent_DY.weight);
    }
  }

  h_Mll_DY->Draw();
  
  
  Long64_t nEntries_others = chain_others->GetEntries();
  for(int ev = 0; ev<nEntries_others; ++ev){
    chain_others->GetEntry(ev);
    h_Mll_others->Fill(myEvent_others.dilepton_mass,myEvent_others.weight);
    if(myEvent_others.dilepton_mass>80 && myEvent_others.dilepton_mass<100){
      h_Mll_others_MWR->Fill(myEvent_others.WR_mass,myEvent_others.weight);
      h_Mll_others_njets->Fill(myEvent_others.njets,myEvent_others.weight);
      h_Mll_others_Zpt->Fill(myEvent_others.dilepton_pt,myEvent_others.weight);
      h_Mll_others_l1pt->Fill(myEvent_others.lead_lepton_pt,myEvent_others.weight);
      h_Mll_others_St->Fill(myEvent_others.lead_lepton_pt+myEvent_others.sublead_lepton_pt+myEvent_others.lead_jet_pt+myEvent_others.sublead_jet_pt,myEvent_others.weight);
    }
  }
  Long64_t nEntries_data = chain_data->GetEntries();
  for(int ev = 0; ev<nEntries_data; ++ev){
    chain_data->GetEntry(ev);
    h_Mll_data->Fill(myEvent_data.dilepton_mass,myEvent_data.weight);
    if(myEvent_data.dilepton_mass>80 && myEvent_data.dilepton_mass<100){
      h_Mll_data_MWR->Fill(myEvent_data.WR_mass,myEvent_data.weight);
      h_Mll_data_njets->Fill(myEvent_data.njets,myEvent_data.weight);
      h_Mll_data_Zpt->Fill(myEvent_data.dilepton_pt,myEvent_data.weight);
      h_Mll_data_l1pt->Fill(myEvent_data.lead_lepton_pt,myEvent_data.weight);
      h_Mll_data_St->Fill(myEvent_data.lead_lepton_pt+myEvent_data.sublead_lepton_pt+myEvent_data.lead_jet_pt+myEvent_data.sublead_jet_pt,myEvent_data.weight);    }
  }

  std::cout<< "Integrals"<<std::endl;
  std::cout<<"DY="<< h_Mll_DY->Integral()<<std::endl;
  std::cout<<"others="<< h_Mll_others->Integral()<<std::endl;
  std::cout<<"data="<< h_Mll_data->Integral()<<std::endl;

  Float_t SF = 1.0;
  Float_t E_SF = 1.0;

  SF = (h_Mll_data->Integral() - h_Mll_others->Integral())/h_Mll_DY->Integral();
  E_SF = TMath::Sqrt( (1./h_Mll_DY->Integral())*(1./h_Mll_DY->Integral())*h_Mll_data->Integral() +
		      (1./h_Mll_DY->Integral())*(1./h_Mll_DY->Integral())*h_Mll_others->Integral() +
		      ((h_Mll_data->Integral() - h_Mll_others->Integral())/(h_Mll_DY->Integral()*h_Mll_DY->Integral()))*((h_Mll_data->Integral() - h_Mll_others->Integral())/(h_Mll_DY->Integral()*h_Mll_DY->Integral()))*h_Mll_DY->Integral());

  cout<<"SF="<<SF<<" E_SF="<< E_SF<<endl;
  
  h_Mll_others_MWR->Scale(-1.0);
  h_Mll_data_MWR->Add(h_Mll_others_MWR);
  h_Mll_data_MWR->Divide(h_Mll_DY_MWR);

  h_Mll_others_njets->Scale(-1.0);
  h_Mll_data_njets->Add(h_Mll_others_njets);
  h_Mll_data_njets->Divide(h_Mll_DY_njets);
    
  h_Mll_others_Zpt->Scale(-1.0);
  h_Mll_data_Zpt->Add(h_Mll_others_Zpt);
  h_Mll_data_Zpt->Divide(h_Mll_DY_Zpt);

  h_Mll_others_l1pt->Scale(-1.0);
  h_Mll_data_l1pt->Add(h_Mll_others_l1pt);
  h_Mll_data_l1pt->Divide(h_Mll_DY_l1pt);
  
  h_Mll_others_St->Scale(-1.0);
  h_Mll_data_St->Add(h_Mll_others_St);
  h_Mll_data_St->Divide(h_Mll_DY_St);
  
  h_Mll_data_MWR->Draw();
  h_Mll_data_MWR->Write();
  h_Mll_data_njets->Draw();
  h_Mll_data_njets->Write();
  h_Mll_data_Zpt->Draw();
  h_Mll_data_Zpt->Write();
  h_Mll_data_l1pt->Draw();
  h_Mll_data_l1pt->Write();
  h_Mll_data_St->Draw();
  h_Mll_data_St->Write();
  
  
  return SF;
  
}
