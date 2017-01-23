#include "TChain.h"
#include "TString.h"

float efficiency(int mass, TString flavor);

void SignalTriggerEff(){

  gStyle->SetOptStat(0);
  
  TH1F *h1 = new TH1F("h1","",27,800,6000);
  TH1F *h2 = new TH1F("h2","",27,800,6000);
  for(int i=0;i<27;i++){
    h1->SetBinContent(i+1,efficiency(800+200*i,"mumu"));
    if(i!=8 && i!=19 && i!=23)
      h2->SetBinContent(i+1,efficiency(800+200*i,"ee"));
    else
      h2->SetBinContent(i+1,1.0);
  }
  h1->SetLineColor(kRed);
  h1->GetYaxis()->SetTitle("Trigger Efficiency/Offline selection");
  h1->GetXaxis()->SetTitle("WR mass [GeV]");
  h1->Draw();
  h1->GetYaxis()->SetRangeUser(0.95,1.02);
  h2->Draw("same");

   TLegend *leg = new TLegend( 0.52, 0.20, 0.78, 0.40 ) ; 
  leg->AddEntry( h1, "Muon channel" ) ; 
  leg->AddEntry( h2, "Electron channel" ) ;
  leg->SetFillColor( kWhite ) ;
  leg->Draw();

  
}

float efficiency(int mass, TString flavor){
  TChain *c1 = new TChain("miniTree_signal_"+flavor+"/t");
  TChain *c2 = new TChain("miniTree_signal_"+flavor+"/t");

  TString masses = std::to_string(mass)+"_"+std::to_string(mass/2);
  //cout<<masses<<endl;

  if(flavor=="mumu"){
    c1->Add("~/eos/ntuples/WRtoMuMuJJ_"+masses+"_WRv04/unmerged-allRange.root");
    c2->Add("~/eos/ntuples/WRtoMuMuJJ_"+masses+"_WRv04p5/unmerged-allRange.root");
  }
  else{
    c1->Add("~/eos/ntuples/WRtoEEJJ_"+masses+"_WRv04/unmerged-allRange.root");
    c2->Add("~/eos/ntuples/WRtoEEJJ_"+masses+"_WRv04p5/unmerged-allRange.root");
  }

  return (float(c2->GetEntries())/float(c1->GetEntries()));
  
}
