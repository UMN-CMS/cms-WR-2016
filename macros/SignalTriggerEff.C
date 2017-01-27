#include "TChain.h"
#include "TString.h"

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
    if(i!=8 && i!=19 && i!=23){
      A = efficiency(800+200*i,"ee")[0];
      B = efficiency(800+200*i,"ee")[1];
      h2->SetBinContent(i+1,A/B);
      h2->SetBinError(i+1,TMath::Sqrt(A+(B-A)*(B-A))/B);
    }
    else
      h2->SetBinContent(i+1,1.0);
  }
  h1->SetLineColor(kRed);
  h2->GetYaxis()->SetTitle("Trigger Efficiency/Offline selection");
  h2->GetXaxis()->SetTitle("WR mass [GeV]");
  h2->Draw();
  h2->GetYaxis()->SetRangeUser(0.95,1.02);
  h1->Draw("same");

   TLegend *leg = new TLegend( 0.52, 0.20, 0.78, 0.40 ) ; 
  leg->AddEntry( h1, "Muon channel" ) ; 
  leg->AddEntry( h2, "Electron channel" ) ;
  leg->SetFillColor( kWhite ) ;
  leg->Draw();

  
}

vector<float> efficiency(int mass, TString flavor){
  TChain *c1 = new TChain("Tree_Iter0");
  TChain *c2 = new TChain("Tree_Iter0");

  TString masses = std::to_string(mass)+"_"+std::to_string(mass/2);
  //cout<<masses<<endl;

  if(flavor=="mumu"){
    c1->Add("~/nobackup/selected/selected_tree_WRtoMuMuJJ_"+masses+"_signal_mumuMuMu.root");
    c2->Add("~/nobackup/selected/selected_tree_trig_WRtoMuMuJJ_"+masses+"_signal_mumuMuMu.root");
  }
  else{
    c1->Add("~/nobackup/selected/selected_tree_WRtoEEJJ_"+masses+"_signal_eeEE.root");
    c2->Add("~/nobackup/selected/selected_tree_trig_WRtoEEJJ_"+masses+"_signal_eeEE.root");
  }

  return {float(c2->GetEntries()),float(c1->GetEntries())};
  
}
