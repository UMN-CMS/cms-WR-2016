

void MakePlot(TString hname,Float_t sf, Float_t step,TString xname);
void DYNormPlot(){
  MakePlot("h_Mll_data_MWR_AMCPT_MuMu",1.001,120,"WR mass [GeV]");
  MakePlot("h_Mll_data_njets_AMCPT_MuMu",1.001,0.14,"Njets");
  MakePlot("h_Mll_data_Zpt_AMCPT_MuMu",1.001,10,"dilepton pT [GeV]");
  MakePlot("h_Mll_data_MWR_AMCPT_EE",1.01,120,"WR mass [GeV]");
  MakePlot("h_Mll_data_njets_AMCPT_EE",1.01,0.14,"Njets");
  MakePlot("h_Mll_data_Zpt_AMCPT_EE",1.01,10,"dilepton pT [GeV]");
}

void MakePlot(TString hname,Float_t sf, Float_t step,TString xname){
  gStyle->SetOptStat(0);

  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  TFile *f = new TFile("../DYweights.root");
  
  TH1F *h = (TH1F*)f->Get(hname);
  TString SF = std::to_string(sf);
  TF1 *func = new TF1("func",SF,0,6000);

  TMultiGraph *mg = new TMultiGraph();
  //mg->SetTitle("Exclusion graphs");

  const Int_t n = 50;
  Double_t x1[n], x2[n], x3[n], y1[n], y2[n], y3[n];
  for (Int_t i=0;i<n;i++) {
    x1[i]  = i;
    x2[i]  = step*x1[i];
    x3[i]  = step*x1[i];
    y1[i] = 10*sin(x1[i]);
    y2[i] = sf;
    y3[i] = sf;
  }

  TGraph *gr1 = new TGraph(n,x1,y1);
  gr1->SetLineColor(2);
  gr1->SetLineWidth(1504);
  gr1->SetFillStyle(3005);

  TGraph *gr2 = new TGraph(n,x2,y2);
  gr2->SetLineColor(4);
  gr2->SetLineWidth(-2500);
  gr2->SetFillStyle(3004);
  gr2->SetFillColor(2);

  TGraph *gr3 = new TGraph(n,x3,y3);
  gr3->SetLineColor(4);
  gr3->SetLineWidth(2500);
  gr3->SetFillStyle(3004);
  gr3->SetFillColor(2);

  //mg->Add(gr1);
  mg->Add(gr2);
  mg->Add(gr3);
  mg->Draw("AC");
  mg->GetYaxis()->SetRangeUser(0.8,5.0);
  mg->GetYaxis()->SetTitle("DY Normalization");
  mg->GetXaxis()->SetTitle(xname);

  h->Draw("same");
  
  func->Draw("same");

  mycanvas->Print("plots/"+hname+"_2.pdf");
  mycanvas->Print("plots/"+hname+"_2.png");
  delete mycanvas;
  delete h;
  delete mg;
}
