#include "TChain.h"
#include "TString.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "../test/CMS_lumi.C"
#include <iostream>

vector<float> efficiency(int mass, TString flavor);

void SignalTriggerEff(){

  gStyle->SetOptStat(0);
  
  TH1F *h1 = new TH1F("h1","",27,800,6000);
  TH1F *h2 = new TH1F("h2","",27,800,6000);
  h1->Sumw2();
  h2->Sumw2();
  for(int i=0;i<27;i++){
    float A = efficiency(800+200*i,"mumu")[0];
    float B = efficiency(800+200*i,"mumu")[1];
    h1->SetBinContent(i+1,A/B);
    h1->SetBinError(i+1,TMath::Sqrt(A+(B-A)*(B-A))/B);
    if(i!=8 && i!=19 && i!=23){  //no trigger trees
      A = efficiency(800+200*i,"ee")[0];
      B = efficiency(800+200*i,"ee")[1];
      h2->SetBinContent(i+1,A/B);
      h2->SetBinError(i+1,TMath::Sqrt(A+(B-A)*(B-A))/B);
    }
    else
      h2->SetBinContent(i+1,1.0);
  }

  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 700, 500) ;
  mycanvas->cd();

  h1->SetLineColor(kRed);
  h1->GetYaxis()->SetTitle("Trigger Efficiency/Offline selection");
  h1->GetYaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetLabelSize(0.04);
  h1->GetXaxis()->SetTitle("m_{W_{R}} (GeV)");
  h1->GetXaxis()->SetTitleOffset(0.9);
  h1->GetXaxis()->SetTitleSize(0.05);
  h1->GetXaxis()->SetLabelSize(0.04);
  h1->Draw();
  h1->GetYaxis()->SetRangeUser(0.95,1.02);
  
  h2->SetLineColor(kBlue);
  h2->Draw("same");

  gPad->SetTickx();
  gPad->SetTicky();

  TLegend *leg = new TLegend( 0.52, 0.20, 0.78, 0.40 ) ; 
  leg->AddEntry( h1, "Muon channel" ) ; 
  leg->AddEntry( h2, "Electron channel" ) ;
  leg->SetFillColor( kWhite ) ;
  leg->Draw();

  CMS_lumi(mycanvas);
  mycanvas->Print("triggerEff_signal.pdf");
  
}

vector<float> efficiency(int mass, TString flavor){
  TChain *c1 = new TChain("Tree_Iter0");
  TChain *c2 = new TChain("Tree_Iter0");

  TString masses = std::to_string(mass)+"_"+std::to_string(mass/2);
  //cout<<masses<<endl;

  TString inputDir = "/afs/cern.ch/work/g/gnegro/NuAnalysis-cmsWR16-approval_prova/CMSSW_8_0_26_patch1/src/ExoAnalysis/cmsWR/selectedTrees/";
  TString inputDirTrigger = "/afs/cern.ch/user/g/gnegro/trigger/";

  if(flavor=="mumu"){
    c1->Add(inputDir+"selected_tree_WRtoMuMuJJ_"+masses+"_signal_mumuMuMu.root");
    c2->Add(inputDirTrigger+"selected_tree_trig_WRtoMuMuJJ_"+masses+"_signal_mumuMuMu.root");
  }
  else{
    c1->Add(inputDir+"selected_tree_WRtoEEJJ_"+masses+"_signal_eeEE.root");
    c2->Add(inputDirTrigger+"selected_tree_trig_WRtoEEJJ_"+masses+"_signal_eeEE.root");
  }

  return {float(c2->GetEntries()),float(c1->GetEntries())};
  
}
