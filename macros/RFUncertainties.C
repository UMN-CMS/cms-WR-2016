void RFUncertainties(){

  TChain *c1 = new TChain("Tree_Iter0");
  c1->Add("~/nobackup/selected/RF/selected_tree_DYAMCPT_signal_eeEE0.root");
		     
  TH1F * hWR_mass = new TH1F("hWR_mass", "hWR_mass", 140, 0, 7000);
  hWR_mass->Sumw2();
  c1->Draw("WR_mass>>hWR_mass", "weight", "goff");
  float e_0 = hWR_mass->Integral();
  hWR_mass->Delete();
  c1->Delete();

  float e = 0.0;

  for(int i=1; i<9; i++){
    TChain *c = new TChain("Tree_Iter0");
    TString fname = "selected_tree_DYAMCPT_signal_eeEE"+std::to_string(i)+".root";
    c->Add("~/nobackup/selected/RF/"+fname);
    TH1F * hWR_mass = new TH1F("hWR_mass", "hWR_mass", 140, 0, 7000);
    hWR_mass->Sumw2();
    c->Draw("WR_mass>>hWR_mass", "weight", "goff");
    //cout<<i<<" "<<hWR_mass->Integral()<<endl;
    float e_i = hWR_mass->Integral();
    hWR_mass->Delete();
    c->Delete();
    if(fabs(e_0-e_i)/e_0 > e)
      e = fabs(e_0-e_i)/e_0;	
  }

  cout<<e<<endl;

  e = 0.0;

  for(int i=9; i<111; i++){
    TChain *c = new TChain("Tree_Iter0");
    TString fname = "selected_tree_DYAMCPT_signal_eeEE"+std::to_string(i)+".root";
    c->Add("~/nobackup/selected/RF/"+fname);
    TH1F * hWR_mass = new TH1F("hWR_mass", "hWR_mass", 140, 0, 7000);
    hWR_mass->Sumw2();
    c->Draw("WR_mass>>hWR_mass", "weight", "goff");
    //cout<<i<<" "<<hWR_mass->Integral()<<endl;
    float e_i = hWR_mass->Integral();
    hWR_mass->Delete();
    c->Delete();
    if(fabs(e_0-e_i)/e_0 > e)
      e = fabs(e_0-e_i)/e_0;	
  }

  cout<<e<<endl;
      
	     
  cout<<"Muons"<<endl;
  TChain *c2 = new TChain("Tree_Iter0");
  c2->Add("~/nobackup/selected/RF/selected_tree_DYAMCPT_signal_mumuMuMu0.root");

  TH1F * hWR_mass2 = new TH1F("hWR_mass", "hWR_mass", 140, 0, 7000);
  hWR_mass2->Sumw2();
  c2->Draw("WR_mass>>hWR_mass", "weight", "goff");
  float m_0 = hWR_mass2->Integral();
  hWR_mass2->Delete();
  c2->Delete();

  float m = 0.0;
  
  for(int i=1; i<9; i++){
    TChain *c = new TChain("Tree_Iter0");
    TString fname = "selected_tree_DYAMCPT_signal_mumuMuMu"+std::to_string(i)+".root";
    c->Add("~/nobackup/selected/RF/"+fname);
    TH1F * hWR_mass = new TH1F("hWR_mass", "hWR_mass", 140, 0, 7000);
    hWR_mass->Sumw2();
    c->Draw("WR_mass>>hWR_mass", "weight", "goff");
    //cout<<i<<" "<<hWR_mass->Integral()<<endl;
    float m_i = hWR_mass->Integral();
    hWR_mass->Delete();
    c->Delete();
    if(fabs(m_0-m_i)/m_0 > m)
      m = fabs(m_0-m_i)/m_0;	
  }
  
  cout<<m<<endl;
  
  m = 0.0;
  
  for(int i=9; i<111; i++){
    TChain *c = new TChain("Tree_Iter0");
    TString fname = "selected_tree_DYAMCPT_signal_mumuMuMu"+std::to_string(i)+".root";
    c->Add("~/nobackup/selected/RF/"+fname);
    TH1F * hWR_mass = new TH1F("hWR_mass", "hWR_mass", 140, 0, 7000);
    hWR_mass->Sumw2();
    c->Draw("WR_mass>>hWR_mass", "weight", "goff");
    //cout<<i<<" "<<hWR_mass->Integral()<<endl;
    float m_i = hWR_mass->Integral();
    hWR_mass->Delete();
    c->Delete();
    if(fabs(m_0-m_i)/m_0 > m)
      m = fabs(m_0-m_i)/m_0;	
  }
  
  cout<<m<<endl;

}
