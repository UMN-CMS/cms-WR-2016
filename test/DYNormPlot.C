

void MakePlot(TString hname,Float_t sf, Float_t step,TString xname);
void DYNormPlot(){
  MakePlot("h_Mll_data_MWR_AMCPT_MuMu",1.02,120,"WR mass [GeV]");
  MakePlot("h_Mll_data_njets_AMCPT_MuMu",1.02,0.14,"Njets");
  MakePlot("h_Mll_data_Zpt_AMCPT_MuMu",1.02,10,"dilepton pT [GeV]");
  MakePlot("h_Mll_data_Mll_AMCPT_MuMu",1.02,10,"dilepton mass [GeV]");
  MakePlot("h_Mll_data_MWR_MAD_MuMu",1.33,120,"WR mass [GeV]");
  MakePlot("h_Mll_data_njets_MAD_MuMu",1.33,0.14,"Njets");
  MakePlot("h_Mll_data_Zpt_MAD_MuMu",1.33,10,"dilepton pT [GeV]");
  MakePlot("h_Mll_data_Mll_MAD_MuMu",1.33,10,"dilepton mass [GeV]");
  MakePlot("h_Mll_data_MWR_AMCPT_EE",1.02,120,"WR mass [GeV]");
  MakePlot("h_Mll_data_njets_AMCPT_EE",1.02,0.14,"Njets");
  MakePlot("h_Mll_data_Zpt_AMCPT_EE",1.02,10,"dilepton pT [GeV]");
  MakePlot("h_Mll_data_Mll_AMCPT_EE",1.02,10,"dilepton mass [GeV]");
  MakePlot("h_Mll_data_MWR_MAD_EE",1.37,120,"WR mass [GeV]");
  MakePlot("h_Mll_data_njets_MAD_EE",1.37,0.14,"Njets");
  MakePlot("h_Mll_data_Zpt_MAD_EE",1.37,10,"dilepton pT [GeV]");
  MakePlot("h_Mll_data_Mll_MAD_EE",1.37,10,"dilepton mass [GeV]");
}

void MakePlot(TString hname,Float_t sf, Float_t step,TString xname){
  gStyle->SetOptStat(0);

  TCanvas* mycanvas = new TCanvas( "mycanvas", "", 0, 0, 600, 600 ) ;
  TFile *f = new TFile("../DYweights_lt600Mlljj.root");
  
  TH1F *h = (TH1F*)f->Get(hname);
  h->GetYaxis()->SetRangeUser(-10,10);
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
  //mg->Add(gr2);
  //mg->Add(gr3);
  //mg->Draw("AC");
  //mg->GetYaxis()->SetRangeUser(0.8,5.0);
  h->GetYaxis()->SetTitle("DY Normalization");
  h->GetXaxis()->SetTitle(xname);

  h->Draw("same");
  
  func->Draw("same");

  mycanvas->Print("plots/DYSF/"+hname+"_lt600mlljj.pdf");
  mycanvas->Print("plots/DYSF/"+hname+"_lt600mlljj.png");
  delete mycanvas;
  delete h;
  delete mg;
}
