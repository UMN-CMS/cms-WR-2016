#include "TChain.h"
#include "TString.h"

float efficiency(int mass, TString flavor);

void SignalTotalEff(){

  gStyle->SetOptStat(0);
  
  TH1F *h1 = new TH1F("h1","",27,800,6000);
  TH1F *h2 = new TH1F("h2","",27,800,6000);
  h1->Sumw2();
  h2->Sumw2();
  for(int i=0;i<27;i++){
    h1->SetBinContent(i+1,efficiency(800+200*i,"mumu"));
    h1->SetBinError(i+1,TMath::Sqrt(efficiency(800+200*i,"mumu")/50000.0));
    if(i!=98){
      h2->SetBinContent(i+1,efficiency(800+200*i,"ee"));
      h2->SetBinError(i+1,TMath::Sqrt(efficiency(800+200*i,"ee")/50000.0));
    }
    else
      h2->SetBinContent(i+1,0.5);
  }
  h1->SetLineColor(kRed);
  h1->GetYaxis()->SetTitle("Efficiency*Acceptance");
  h1->GetXaxis()->SetTitle("WR mass [GeV]");
  h1->Draw("ep");
  h1->GetYaxis()->SetRangeUser(0.0,1.0);
  h2->Draw("epsame");

  gPad->SetTickx();
  gPad->SetTicky();

  
  TLegend *leg = new TLegend( 0.52, 0.20, 0.78, 0.40 ) ; 
  leg->AddEntry( h1, "Muon channel" ) ; 
  leg->AddEntry( h2, "Electron channel" ) ;
  leg->SetFillColor( kWhite ) ;
  leg->Draw();

  
}

float efficiency(int mass, TString flavor){
  TChain *c1 = new TChain("Tree_Iter0");

  TString masses = std::to_string(mass)+"_"+std::to_string(mass/2);
  //cout<<masses<<endl;

  if(flavor=="mumu"){
    c1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoMuMuJJ_"+masses+"_signal_mumuMuMu.root");
  }
  else{
    c1->Add("~/nobackup/selected/WRv06/selected_tree_WRtoEEJJ_"+masses+"_signal_eeEE.root");
  }

  TH1F *h = new TH1F("h","",140,0,7000);
  //c1->Draw("WR_mass>>h","(abs(lead_lepton_eta)<1.4442) && (abs(sublead_lepton_eta)<1.4442)","goff");

  //std::cout<<c1->GetEntries()<<" "<<h->GetEntries()<<std::endl;
  float r = float(c1->GetEntries())/50000.0;
  
  delete h;
  
  return (r);
  
}
