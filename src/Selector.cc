#include <iostream>
#include <algorithm>
#include <vector>
// #include "ExoAnalysis/cmsWR/interface/Selector.h"
#include "../interface/Selector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "../interface/SelectorHist.h"
#include "TFile.h"
#include "TH2F.h"
#include "TMath.h"

//#define DEBUGG

TFile *TrigSF1 = new TFile("data/EfficienciesAndSF_TrigBF.root");
TFile *TrigSF2 = new TFile("data/EfficienciesAndSF_TrigGH.root");
TH2F *Trigh1 = (TH2F*)TrigSF1->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
TH2F *Trigh2 = (TH2F*)TrigSF2->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
TH2F *Trigg1 = (TH2F*)TrigSF1->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/abseta_pt_DATA");
TH2F *Trigg2 = (TH2F*)TrigSF2->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/abseta_pt_DATA");


float dR_TLV(TLorentzVector t1, TLorentzVector t2)
{
  return deltaR(t1.Eta(), t1.Phi(), t2.Eta(), t2.Phi());
}

void goodJets(myJetCollection *evJets, myJetCollection *selJets)
{
  for(auto j : *evJets) {
    if(j.p4.Pt() > 40 && fabs(j.p4.Eta()) < 2.4)
      selJets->push_back(j);
  }
}

void goodJetsLooseCuts(myJetCollection *evJets, myJetCollection *selJets)
{
  for(auto j : *evJets) {
    if(j.p4.Pt() > 10 && fabs(j.p4.Eta()) < 2.4 ) selJets->push_back(j);
  }
}

void goodEles(myElectronCollection *evEles, myElectronCollection *selEles)
{
  for(auto e : *evEles) {
    if(e.p4.Pt() > 40 && fabs(e.p4.Eta()) < 2.4 && (fabs(e.p4.Eta()) < 1.4442 || fabs(e.p4.Eta()) > 1.566) && e.heepId == 1)
      selEles->push_back(e);
  }
}
void goodQCDEles(myElectronCollection *evEles, myElectronCollection *selEles)
{
  for(auto e : *evEles) {
    if(e.p4.Pt() > 40 && fabs(e.p4.Eta()) < 2.4 && (fabs(e.p4.Eta()) < 1.4442 || fabs(e.p4.Eta()) > 1.566) && e.heepId == 0)
      selEles->push_back(e);
  }
}

void goodElesLooseCuts(myElectronCollection *evEles, myElectronCollection *selEles)
{
  for(auto e : *evEles) {
    if(fabs(e.p4.Eta()) < 2.4 && (fabs(e.p4.Eta()) < 1.4442 || fabs(e.p4.Eta()) > 1.566)) selEles->push_back(e);
  }
}

void goodMuons(myMuonCollection *evMuons, myMuonCollection *selMuons)
{
  for(auto m : *evMuons) {
    if(m.p4.Pt() > 40 && fabs(m.p4.Eta()) < 2.4 && m.trkIso < 0.1)
      selMuons->push_back(m);
  }
}

void goodMuonsLooseCuts(myMuonCollection *evMuons, myMuonCollection *selMuons)
{
  for(auto m : *evMuons) {
    if(fabs(m.p4.Eta()) < 2.4) selMuons->push_back(m);
  }
}

std::pair<float,float> TriggerSF2(float eta1, float pt1, float eta2, float pt2, TH2F *h1, TH2F *h2, TH2F *g1, TH2F *g2){
  Int_t eta_bin1 = h1->GetXaxis()->FindBin(eta1);
  Int_t pt_bin1 = h1->GetYaxis()->FindBin(pt1);
  
  Int_t eta_bin2 = h1->GetXaxis()->FindBin(eta2);
  Int_t pt_bin2 = h1->GetYaxis()->FindBin(pt2);
 
  float sf1 = h1->GetBinContent(eta_bin1,pt_bin1);
  float sf2 = h1->GetBinContent(eta_bin2,pt_bin2);
  
  float eff1 = g1->GetBinContent(eta_bin1,pt_bin1); // data efficiency
  float eff2 = g1->GetBinContent(eta_bin2,pt_bin2);

  float SF1 = (eff1+eff2 - eff1*eff2)/(eff1/sf1 + eff2/sf2 - (eff1*eff2)/(sf1*sf2));

  ////////////////////////////////////////////////////////////////////////////////
  
  eta_bin1 = h2->GetXaxis()->FindBin(eta1);
  pt_bin1 = h2->GetYaxis()->FindBin(pt1);
  
  eta_bin2 = h2->GetXaxis()->FindBin(eta2);
  pt_bin2 = h2->GetYaxis()->FindBin(pt2);
 
  sf1 = h2->GetBinContent(eta_bin1,pt_bin1);
  sf2 = h2->GetBinContent(eta_bin2,pt_bin2);
  
  eff1 = g2->GetBinContent(eta_bin1,pt_bin1); // data efficiency
  eff2 = g2->GetBinContent(eta_bin2,pt_bin2);

  float SF2 = (eff1+eff2 - eff1*eff2)/(eff1/sf1 + eff2/sf2 - (eff1*eff2)/(sf1*sf2));
  
  float e1 = h1->GetBinError(eta_bin1,pt_bin1);
  float e2 = h2->GetBinError(eta_bin2,pt_bin2);

  float w1 = (5.929+2.646+4.353+4.117+3.186)/(5.929+2.646+4.353+4.117+3.186+7.721+8.636+0.221);
  float w2 = 1.0 - w1;

  if(TMath::Finite(SF1) && TMath::Finite(SF2))
    return std::make_pair(SF1*w1+SF2*w2,TMath::Sqrt(e1*e1*w1*w1+e2*e2*w2*w2));
  else
    return
      std::make_pair(1,0);
}

std::pair<float,float> TriggerSF1(float eta1, float pt1, TH2F *h1, TH2F *h2){
  Int_t eta_bin1 = h1->GetXaxis()->FindBin(eta1);
  Int_t pt_bin1 = h1->GetYaxis()->FindBin(pt1);
   
  float sf1 = h1->GetBinContent(eta_bin1,pt_bin1);
  
  ////////////////////////////////////////////////////////////////////////////////
  
  eta_bin1 = h2->GetXaxis()->FindBin(eta1);
  pt_bin1 = h2->GetYaxis()->FindBin(pt1);
  
  float sf2 = h2->GetBinContent(eta_bin1,pt_bin1);
   
  float e1 = h1->GetBinError(eta_bin1,pt_bin1);
  float e2 = h2->GetBinError(eta_bin1,pt_bin1);

  float w1 = (5.929+2.646+4.353+4.117+3.186)/(5.929+2.646+4.353+4.117+3.186+7.721+8.636+0.221);
  float w2 = 1.0 - w1;
  
  return std::make_pair(sf1*w1+sf2*w2,TMath::Sqrt(e1*e1*w1*w1+e2*e2*w2*w2));
}


Selector::Selector(const miniTreeEvent& myEvent) :
  WR_mass(-1),
  N1_mass(-1),
  N2_mass(-1),
  dilepton_pt(-1),
  dilepton_mass(-1)
{
  datasetName = myEvent.datasetName;

  int nele = myEvent.electrons_p4->size();
  for(int i = 0; i < nele; i++) {
    myElectron ele;
    ele.p4 = myEvent.electrons_p4->at(i);
    ele.scale_error = myEvent.electron_scale_error->at(i);
    ele.smearing_sigma = myEvent.electron_smearing_sigma->at(i);
    ele.smearing_sigma_phi_up = myEvent.electron_smearing_sigma_phi_up->at(i);
    ele.smearing_sigma_phi_down = myEvent.electron_smearing_sigma_phi_down->at(i);
    ele.smearing_sigma_rho_up = myEvent.electron_smearing_sigma_rho_up->at(i);
    ele.smearing_sigma_rho_down = myEvent.electron_smearing_sigma_rho_down->at(i);
    ele.charge = myEvent.electron_charge->at(i);
    ele.r9 = myEvent.electron_r9->at(i);
    ele.IDSF = myEvent.electron_IDSF_central->at(i);
    ele.IDSF_error = myEvent.electron_IDSF_error->at(i);
    ele.RecoSF = myEvent.electron_RecoSF_central->at(i);
    ele.RecoSF_error = myEvent.electron_RecoSF_error->at(i);
    ele.HltSF = myEvent.electron_HltSF_central->at(i);
    ele.HltSF_error = myEvent.electron_HltSF_error->at(i);
    ele.weight = (ele.IDSF) * (ele.RecoSF) * (ele.HltSF);

    ele.heepId = myEvent.electron_heepId->at(i);
    ele.HoverEM = myEvent.electron_HoverEM->at(i);
    ele.dxy = myEvent.electron_dxy->at(i);
    ele.dB = myEvent.electron_dB->at(i);
    ele.dxyError = myEvent.electron_dxyError->at(i);
    ele.pfIso = myEvent.electron_pfIso->at(i);
    ele.GsfEleEmHadD1IsoRhoCut = myEvent.electron_GsfEleEmHadD1IsoRhoCut->at(i);
    ele.GsftrkIso = myEvent.electron_GsftrkIso->at(i);
    ele.trkIso = myEvent.electron_trkIso->at(i);
    ele.satCrystal = myEvent.electron_satCrystal->at(i);
    ele.heepBitmap = myEvent.electron_heepBitmap->at(i);

    
    electrons.push_back(ele);
  }
  int nmu = myEvent.muons_p4->size();
  for(int i = 0; i < nmu; i++) {
    myMuon mu;
    mu.p4 = myEvent.muons_p4->at(i);
    mu.IDSF = myEvent.muon_IDSF_central2->at(i);
    mu.IsoSF = myEvent.muon_IsoSF_central2->at(i);
    mu.TrigSF = myEvent.muon_TrigSF_central->at(i);
    mu.IDSF_error = myEvent.muon_IDSF_error2->at(i);
    mu.IsoSF_error = myEvent.muon_IsoSF_error2->at(i);
    mu.TrigSF_error = myEvent.muon_TrigSF_error->at(i);
    mu.charge = myEvent.muon_charge->at(i);
    mu.weight = mu.IDSF * mu.IsoSF;
    mu.trkIso = myEvent.muon_trkIso->at(i);
    muons.push_back(mu);
  }
  int njet = myEvent.jets_p4->size();
  for(int i = 0; i < njet; i++) {
    myJet jet;
    jet.p4 = myEvent.jets_p4->at(i);
    jet.jec_uncertainty = myEvent.jec_uncertainty->at(i);
    jet.weight = 1.;
    jets.push_back(jet);
  }

  run = myEvent.run;
  event = myEvent.event;
  lumi = myEvent.lumi;
	
  nPV = myEvent.nPV;
  nPU = myEvent.nPU;
  global_event_weight = (myEvent.weight > 0 ? 1 : -1);
  // * myEvent.PU_reweight;
#ifdef DEBUGG
  std::cout << "global_event_weight=\t" << global_event_weight << std::endl;
#endif

  //	Clear();
}

Selector::Selector()
{
  Clear();
}

void Selector::Clear()
{
  WR_mass = N1_mass = N2_mass = dilepton_pt = dilepton_mass = weight = HT = 0.0;
}

bool Selector::isPassingLooseCuts(tag_t tag)
{
  _isPassingLooseCuts = false;
  WR_mass = -1, N1_mass = -1, N2_mass = -1, lead_lepton_r9 = -1, sublead_lepton_r9 = -1, HT = -1;
  TLorentzVector lead_lepton_p4, sublead_lepton_p4, lead_jet_p4, sublead_jet_p4;
  lead_lepton_IDSF_error = -9;
  lead_lepton_RecoSF_error = -9;
  lead_lepton_HltSF_error = -9;
  lead_lepton_EScaling_error = -9;
  lead_lepton_ESmearing_sigma = -9;
  lead_lepton_ESmearing_sigma_phi_up = -9;
  lead_lepton_ESmearing_sigma_phi_down = -9;
  lead_lepton_ESmearing_sigma_rho_up = -9;
  lead_lepton_ESmearing_sigma_rho_down = -9;
  sublead_lepton_IDSF_error = -9;
  sublead_lepton_RecoSF_error = -9;
  sublead_lepton_HltSF_error = -9;
  sublead_lepton_EScaling_error = -9;
  sublead_lepton_ESmearing_sigma_phi_up = -9;
  sublead_lepton_ESmearing_sigma_phi_down = -9;
  sublead_lepton_ESmearing_sigma_rho_up = -9;
  sublead_lepton_ESmearing_sigma_rho_down = -9;
  lead_lepton_IsoSF_error = -9;
  sublead_lepton_IsoSF_error = -9;
  
  lead_lepton_HoverEM = -9;
  lead_lepton_dxy = -9;
  lead_lepton_dB = -9;
  lead_lepton_dxyError = -9;
  lead_lepton_pfIso = -9;
  lead_lepton_GsfEleEmHadD1IsoRhoCut = -9;
  lead_lepton_GsftrkIso = -9;
  lead_lepton_trkIso = -9;
  lead_lepton_satCrystal = -9;
  lead_lepton_heepBitmap = -9;
  sublead_lepton_HoverEM = -9;
  sublead_lepton_dxy = -9;
  sublead_lepton_dB = -9;
  sublead_lepton_dxyError = -9;
  sublead_lepton_pfIso = -9;
  sublead_lepton_GsfEleEmHadD1IsoRhoCut = -9;
  sublead_lepton_GsftrkIso = -9;
  sublead_lepton_trkIso = -9;
  sublead_lepton_satCrystal = -9;
  sublead_lepton_heepBitmap = -9;

  myJetCollection gJets;
  myElectronCollection gEles;
  myMuonCollection gMuons;
	
  // Basic Kinematic cuts (only eta cuts applied in good...LooseCuts() )
  goodJetsLooseCuts(&jets, &gJets);
  goodElesLooseCuts(&electrons, &gEles);
  goodMuonsLooseCuts(&muons, &gMuons);
	
  std::sort(gJets.begin(), gJets.end(),
	    [](myJet const & a, myJet const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });
  std::sort(gEles.begin(), gEles.end(),
	    [](myElectron const & a, myElectron const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });
  std::sort(gMuons.begin(), gMuons.end(),
	    [](myMuon const & a, myMuon const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });

  jets = gJets;
  electrons = gEles;
  muons = gMuons;

  float ht  = 0.0;
  for(auto j:jets){
    ht += j.p4.Pt();
  }
  HT = ht;
	
  if(tag == EE) { // EEJJ Channel
    // Assert at least 2 good leptons
    if(electrons.size() < 2) {
      return false;
    }

    lead_lepton_p4 = electrons[0].p4;
    sublead_lepton_p4 = electrons[1].p4;

    lead_lepton_weight = electrons[0].weight;
    sublead_lepton_weight = electrons[1].weight;

    lead_lepton_IDSF_error = electrons[0].IDSF_error;
    lead_lepton_RecoSF_error = electrons[0].RecoSF_error;
    lead_lepton_HltSF_error = electrons[0].HltSF_error;
    lead_lepton_EScaling_error = electrons[0].scale_error;
    lead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
    lead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
    lead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
    lead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
    lead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;

    sublead_lepton_IDSF_error = electrons[1].IDSF_error;
    sublead_lepton_RecoSF_error = electrons[1].RecoSF_error;
    sublead_lepton_HltSF_error = electrons[1].HltSF_error;
    sublead_lepton_EScaling_error = electrons[1].scale_error;
    sublead_lepton_ESmearing_sigma = electrons[1].smearing_sigma;
    sublead_lepton_ESmearing_sigma_phi_up = electrons[1].smearing_sigma_phi_up;
    sublead_lepton_ESmearing_sigma_phi_down = electrons[1].smearing_sigma_phi_down;
    sublead_lepton_ESmearing_sigma_rho_up = electrons[1].smearing_sigma_rho_up;
    sublead_lepton_ESmearing_sigma_rho_down = electrons[1].smearing_sigma_rho_down;

    lead_lepton_HoverEM = electrons[0].HoverEM;
    lead_lepton_dxy = electrons[0].dxy;
    lead_lepton_dB = electrons[0].dB;
    lead_lepton_dxyError = electrons[0].dxyError;
    lead_lepton_pfIso = electrons[0].pfIso;
    lead_lepton_GsfEleEmHadD1IsoRhoCut = electrons[0].GsfEleEmHadD1IsoRhoCut;
    lead_lepton_GsftrkIso = electrons[0].GsftrkIso;
    lead_lepton_trkIso = electrons[0].trkIso;
    lead_lepton_satCrystal = electrons[0].satCrystal;
    lead_lepton_heepBitmap = electrons[0].heepBitmap;
    sublead_lepton_HoverEM = electrons[1].HoverEM;
    sublead_lepton_dxy = electrons[1].dxy;
    sublead_lepton_dB = electrons[1].dB;
    sublead_lepton_dxyError = electrons[1].dxyError;
    sublead_lepton_pfIso = electrons[1].pfIso;
    sublead_lepton_GsfEleEmHadD1IsoRhoCut = electrons[1].GsfEleEmHadD1IsoRhoCut;
    sublead_lepton_GsftrkIso = electrons[1].GsftrkIso;
    sublead_lepton_trkIso = electrons[1].trkIso;
    sublead_lepton_satCrystal = electrons[1].satCrystal;
    sublead_lepton_heepBitmap = electrons[1].heepBitmap;
    
    lead_lepton_r9 = electrons[0].r9;
    sublead_lepton_r9 = electrons[1].r9;

    lead_lepton_charge = electrons[0].charge;
    sublead_lepton_charge = electrons[1].charge;
  }
  else if(tag == MuMu) { // MuMuJJ Channel
    // Assert at least 2 good leptons
    if(muons.size() < 2) {
      return false;
    }

    lead_lepton_p4 = muons[0].p4;
    sublead_lepton_p4 = muons[1].p4;
    
    lead_lepton_charge = muons[0].charge;
    sublead_lepton_charge = muons[1].charge;

    lead_lepton_IDSF_error = muons[0].IDSF_error;
    lead_lepton_IsoSF_error = muons[0].IsoSF_error;

    sublead_lepton_IDSF_error = muons[1].IDSF_error;
    sublead_lepton_IsoSF_error = muons[1].IsoSF_error;

    lead_lepton_weight = muons[0].weight * TriggerSF2(fabs(lead_lepton_p4.Eta()), lead_lepton_p4.Pt(),fabs(sublead_lepton_p4.Eta()), sublead_lepton_p4.Pt(),Trigh1,Trigh2,Trigg1,Trigg2).first;
    sublead_lepton_weight = muons[1].weight;
  }
  else if(tag == EMu) { // EMuJJ Channel
    // Assert at least 2 good leptons
    if(electrons.size() < 1 || muons.size() < 1) {
      return false;
    }
    // check which is the leading, which the subleading
    if(electrons[0].p4.Pt() > muons[0].p4.Pt()) { // e > mu
      lead_lepton_p4 = electrons[0].p4;
      sublead_lepton_p4 = muons[0].p4;

      lead_lepton_IDSF_error = electrons[0].IDSF_error;
      lead_lepton_RecoSF_error = electrons[0].RecoSF_error;
      lead_lepton_HltSF_error = electrons[0].HltSF_error;
      lead_lepton_EScaling_error = electrons[0].scale_error;
      lead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
      lead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
      lead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
      lead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
      lead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;

      sublead_lepton_IDSF_error = muons[0].IDSF_error;
      sublead_lepton_IsoSF_error = muons[0].IsoSF_error;

      lead_lepton_weight = electrons[0].weight;
      sublead_lepton_weight = muons[0].weight * TriggerSF1(fabs(sublead_lepton_p4.Eta()), sublead_lepton_p4.Pt(),Trigh1,Trigh2).first;
      lead_lepton_r9 = electrons[0].r9;
      lead_lepton_charge = electrons[0].charge;
      sublead_lepton_charge = muons[0].charge;
    } else {

      sublead_lepton_p4 = electrons[0].p4;
      sublead_lepton_weight = electrons[0].weight;
      sublead_lepton_IDSF_error = electrons[0].IDSF_error;
      sublead_lepton_RecoSF_error = electrons[0].RecoSF_error;
      sublead_lepton_HltSF_error = electrons[0].HltSF_error;
      sublead_lepton_EScaling_error = electrons[0].scale_error;
      sublead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
      sublead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
      sublead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
      sublead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
      sublead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;


      lead_lepton_IDSF_error = muons[0].IDSF_error;
      lead_lepton_IsoSF_error = muons[0].IsoSF_error;
      lead_lepton_p4 = muons[0].p4;
      lead_lepton_weight = muons[0].weight * TriggerSF1(fabs(lead_lepton_p4.Eta()), lead_lepton_p4.Pt(),Trigh1,Trigh2).first;
      sublead_lepton_r9 = electrons[0].r9;
      lead_lepton_charge = muons[0].charge;
      sublead_lepton_charge = electrons[0].charge;
    }
  }

  //lepton pt cuts necessitated by dytagandprobe triggers
  if(lead_lepton_p4.Pt() < 35) return false;
  if(sublead_lepton_p4.Pt() < 35) return false;


  //defaults if no jets are found in the event
  lead_jet_jec_unc = -10;
  sublead_jet_jec_unc = -10;
  lead_jet_pt = -9;
  lead_jet_eta = -6;
  lead_jet_phi = -6;
  lead_jet_weight = 1.0;
  sublead_jet_weight = 1.0;
  sublead_jet_pt = -9;
  sublead_jet_eta = -6;
  sublead_jet_phi = -6;
  dR_leadlepton_leadjet = 9, dR_leadlepton_subleadjet = 9, dR_subleadlepton_leadjet = 9, dR_subleadlepton_subleadjet = 9;

  njets = jets.size();
	
  if(jets.size() == 1) {
    lead_jet_pt = jets[0].p4.Pt();
    lead_jet_eta = jets[0].p4.Eta();
    lead_jet_phi = jets[0].p4.Phi();
    lead_jet_weight = 1.0;
    lead_jet_jec_unc = jets[0].jec_uncertainty;
    dR_leadlepton_leadjet = dR_TLV(lead_lepton_p4, jets[0].p4);
    dR_subleadlepton_leadjet = dR_TLV(sublead_lepton_p4, jets[0].p4);
  }//one jet in event

  if(jets.size() > 1) {
    lead_jet_pt = jets[0].p4.Pt();
    lead_jet_eta = jets[0].p4.Eta();
    lead_jet_phi = jets[0].p4.Phi();
    lead_jet_weight = 1.0;
    sublead_jet_weight = 1.0;
    lead_jet_jec_unc = jets[0].jec_uncertainty;
    sublead_jet_jec_unc = jets[1].jec_uncertainty;

    sublead_jet_pt = jets[1].p4.Pt();
    sublead_jet_eta = jets[1].p4.Eta();
    sublead_jet_phi = jets[1].p4.Phi();

    dR_leadlepton_leadjet = dR_TLV(lead_lepton_p4, jets[0].p4);
    dR_subleadlepton_leadjet = dR_TLV(sublead_lepton_p4, jets[0].p4);
    dR_leadlepton_subleadjet = dR_TLV(lead_lepton_p4, jets[1].p4);
    dR_subleadlepton_subleadjet = dR_TLV(sublead_lepton_p4, jets[1].p4);
  }//two or more jets in the event


  lead_lepton_pt = lead_lepton_p4.Pt();
  lead_lepton_eta = lead_lepton_p4.Eta();
  lead_lepton_phi = lead_lepton_p4.Phi();

  sublead_lepton_pt = sublead_lepton_p4.Pt();
  sublead_lepton_eta = sublead_lepton_p4.Eta();
  sublead_lepton_phi = sublead_lepton_p4.Phi();


  // Build the WR mass and dilepton mass with the 2 highest pT jets and 2 highest pT leptons
  //WR_mass = (lead_lepton_p4 + sublead_lepton_p4 + gJets[0].p4 + gJets[1].p4).M();
  //weight = lead_lepton_weight * sublead_lepton_weight * lead_jet_weight * sublead_jet_weight * global_event_weight;
  WR_mass = -1;
  N1_mass = -1;
  N2_mass = -1;
  weight = lead_lepton_weight * sublead_lepton_weight * global_event_weight;
  //weight = global_event_weight;
  pu_weight = fabs(global_event_weight);


#ifdef DEBUGG
  std::cout << "weight (global_event_weight times single object weights)=\t" << weight << std::endl;
#endif

  dilepton_mass = (lead_lepton_p4 + sublead_lepton_p4).M();
  dilepton_pt = (lead_lepton_p4 + sublead_lepton_p4).Pt();
  if(dilepton_mass < 60.0 || dilepton_mass > 120.0) return false;

  _isPassingLooseCuts = true;
  return _isPassingLooseCuts;

}

bool Selector::isPassing(tag_t tag, bool makeHists)
{

  /*
    enum det_t {
    DET_ENDCAP,
    DET_BARREL,
    DET_GAP,
    } lead_det, sublead_det;

    enum pair_t {
    P_EE,
    P_BB,
    P_EB,
    P_GAP,
    } pair;
  */

  _isPassing = false;
  WR_mass = -1, N1_mass = -1, N2_mass = -1, lead_lepton_r9 = -1, sublead_lepton_r9 = -1, HT = -1;
  TLorentzVector lead_lepton_p4, sublead_lepton_p4, lead_jet_p4, sublead_jet_p4;
  lead_lepton_IDSF_error = -9;
  lead_lepton_RecoSF_error = -9;
  lead_lepton_HltSF_error = -9;
  lead_lepton_EScaling_error = -9;
  lead_lepton_ESmearing_sigma = -9;
  lead_lepton_ESmearing_sigma_phi_up = -9;
  lead_lepton_ESmearing_sigma_phi_down = -9;
  lead_lepton_ESmearing_sigma_rho_up = -9;
  lead_lepton_ESmearing_sigma_rho_down = -9;
  sublead_lepton_IDSF_error = -9;
  sublead_lepton_RecoSF_error = -9;
  sublead_lepton_HltSF_error = -9;
  sublead_lepton_EScaling_error = -9;
  sublead_lepton_ESmearing_sigma = -9;
  sublead_lepton_ESmearing_sigma_phi_up = -9;
  sublead_lepton_ESmearing_sigma_phi_down = -9;
  sublead_lepton_ESmearing_sigma_rho_up = -9;
  sublead_lepton_ESmearing_sigma_rho_down = -9;
  lead_lepton_IsoSF_error = -9;
  sublead_lepton_IsoSF_error = -9;

  lead_lepton_HoverEM = -9;
  lead_lepton_dxy = -9;
  lead_lepton_dB = -9;
  lead_lepton_dxyError = -9;
  lead_lepton_pfIso = -9;
  lead_lepton_GsfEleEmHadD1IsoRhoCut = -9;
  lead_lepton_GsftrkIso = -9;
  lead_lepton_trkIso = -9;
  lead_lepton_satCrystal = -9;
  lead_lepton_heepBitmap = -9;
  sublead_lepton_HoverEM = -9;
  sublead_lepton_dxy = -9;
  sublead_lepton_dB = -9;
  sublead_lepton_dxyError = -9;
  sublead_lepton_pfIso = -9;
  sublead_lepton_GsfEleEmHadD1IsoRhoCut = -9;
  sublead_lepton_GsftrkIso = -9;
  sublead_lepton_trkIso = -9;
  sublead_lepton_satCrystal = -9;
  sublead_lepton_heepBitmap = -9;

  myJetCollection gJets;
  myElectronCollection gEles;
  myMuonCollection gMuons;

  // Basic Kinematic cuts
  goodJets(&jets, &gJets);
  goodEles(&electrons, &gEles);
  goodMuons(&muons, &gMuons);

  std::sort(gJets.begin(), gJets.end(),
	    [](myJet const & a, myJet const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });
  std::sort(gEles.begin(), gEles.end(),
	    [](myElectron const & a, myElectron const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });
  std::sort(gMuons.begin(), gMuons.end(),
	    [](myMuon const & a, myMuon const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });

  jets = gJets;
  electrons = gEles;
  muons = gMuons;

  //if (makeHists) sel::hists("njets", 10, 0, 10)->Fill(jets.size());

  float ht  = 0.0;
  for(auto j:jets){
    ht += j.p4.Pt();
  }
  HT = ht;
  // Assert at least 2 good jets
  if(jets.size() < 2) {
    return false;
  }
  // //if (makeHists) sel::hists("njets_cut", 10, 0, 10)->Fill(jets.size());

  njets = jets.size();

  lead_jet_pt = jets[0].p4.Pt();
  lead_jet_eta = jets[0].p4.Eta();
  lead_jet_phi = jets[0].p4.Phi();
  lead_jet_weight = 1.0;
  sublead_jet_weight = 1.0;
  lead_jet_jec_unc = jets[0].jec_uncertainty;
  sublead_jet_jec_unc = jets[1].jec_uncertainty;

  sublead_jet_pt = jets[1].p4.Pt();
  sublead_jet_eta = jets[1].p4.Eta();
  sublead_jet_phi = jets[1].p4.Phi();

  if(tag == EE) { // EEJJ Channel
    // Assert at least 2 good leptons
    //if (makeHists) sel::hists("nlep", 10, 0, 10)->Fill(electrons.size());
    if(electrons.size() < 2) {
      return false;
    }
    //if (makeHists) sel::hists("nlep_cut", 10, 0, 10)->Fill(electrons.size());

    lead_lepton_p4 = electrons[0].p4;
    sublead_lepton_p4 = electrons[1].p4;

    lead_lepton_weight = electrons[0].weight;
    sublead_lepton_weight = electrons[1].weight;

    lead_lepton_IDSF_error = electrons[0].IDSF_error;
    lead_lepton_RecoSF_error = electrons[0].RecoSF_error;
    lead_lepton_HltSF_error = electrons[0].HltSF_error;
    lead_lepton_EScaling_error = electrons[0].scale_error;
    lead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
    lead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
    lead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
    lead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
    lead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;

    sublead_lepton_IDSF_error = electrons[1].IDSF_error;
    sublead_lepton_RecoSF_error = electrons[1].RecoSF_error;
    sublead_lepton_HltSF_error = electrons[1].HltSF_error;
    sublead_lepton_EScaling_error = electrons[1].scale_error;
    sublead_lepton_ESmearing_sigma = electrons[1].smearing_sigma;
    sublead_lepton_ESmearing_sigma_phi_up = electrons[1].smearing_sigma_phi_up;
    sublead_lepton_ESmearing_sigma_phi_down = electrons[1].smearing_sigma_phi_down;
    sublead_lepton_ESmearing_sigma_rho_up = electrons[1].smearing_sigma_rho_up;
    sublead_lepton_ESmearing_sigma_rho_down = electrons[1].smearing_sigma_rho_down;

    lead_lepton_HoverEM = electrons[0].HoverEM;
    lead_lepton_dxy = electrons[0].dxy;
    lead_lepton_dB = electrons[0].dB;
    lead_lepton_dxyError = electrons[0].dxyError;
    lead_lepton_pfIso = electrons[0].pfIso;
    lead_lepton_GsfEleEmHadD1IsoRhoCut = electrons[0].GsfEleEmHadD1IsoRhoCut;
    lead_lepton_GsftrkIso = electrons[0].GsftrkIso;
    lead_lepton_trkIso = electrons[0].trkIso;
    lead_lepton_satCrystal = electrons[0].satCrystal;
    lead_lepton_heepBitmap = electrons[0].heepBitmap;
    sublead_lepton_HoverEM = electrons[1].HoverEM;
    sublead_lepton_dxy = electrons[1].dxy;
    sublead_lepton_dB = electrons[1].dB;
    sublead_lepton_dxyError = electrons[1].dxyError;
    sublead_lepton_pfIso = electrons[1].pfIso;
    sublead_lepton_GsfEleEmHadD1IsoRhoCut = electrons[1].GsfEleEmHadD1IsoRhoCut;
    sublead_lepton_GsftrkIso = electrons[1].GsftrkIso;
    sublead_lepton_trkIso = electrons[1].trkIso;
    sublead_lepton_satCrystal = electrons[1].satCrystal;
    sublead_lepton_heepBitmap = electrons[1].heepBitmap;

    lead_lepton_r9 = electrons[0].r9;
    sublead_lepton_r9 = electrons[1].r9;
    lead_lepton_charge = electrons[0].charge;
    sublead_lepton_charge = electrons[1].charge;
  }
  else if(tag == MuMu) { // MuMuJJ Channel
    // Assert at least 2 good leptons
    //if (makeHists) sel::hists("nlep", 10, 0, 10)->Fill(muons.size());
    if(muons.size() < 2) {
      return false;
    }
    //if (makeHists) sel::hists("nlep_cut", 10, 0, 10)->Fill(muons.size());

    lead_lepton_p4 = muons[0].p4;
    sublead_lepton_p4 = muons[1].p4;

    lead_lepton_charge = muons[0].charge;
    sublead_lepton_charge = muons[1].charge;

    lead_lepton_IDSF_error = muons[0].IDSF_error;
    lead_lepton_IsoSF_error = muons[0].IsoSF_error;

    sublead_lepton_IDSF_error = muons[1].IDSF_error;
    sublead_lepton_IsoSF_error = muons[1].IsoSF_error;

    lead_lepton_weight = muons[0].weight * TriggerSF2(fabs(lead_lepton_p4.Eta()), lead_lepton_p4.Pt(),fabs(sublead_lepton_p4.Eta()), sublead_lepton_p4.Pt(),Trigh1,Trigh2,Trigg1,Trigg2).first;
    sublead_lepton_weight = muons[1].weight;

  }
  else if(tag == EMu) { // EMuJJ Channel
    // Assert at least 2 good leptons
    //if (makeHists) sel::hists("nlep", 10, 0, 10)->Fill(muons.size() + electrons.size());
    if(electrons.size() < 1 || muons.size() < 1) {
      return false;
    }
    //if (makeHists) sel::hists("nlep_cut", 10, 0, 10)->Fill(muons.size() + electrons.size());

    // check which is the leading, which the subleading
    if(electrons[0].p4.Pt() > muons[0].p4.Pt()) { // e > mu
      lead_lepton_p4 = electrons[0].p4;
      sublead_lepton_p4 = muons[0].p4;

      lead_lepton_IDSF_error = electrons[0].IDSF_error;
      lead_lepton_RecoSF_error = electrons[0].RecoSF_error;
      lead_lepton_HltSF_error = electrons[0].HltSF_error;
      lead_lepton_EScaling_error = electrons[0].scale_error;
      lead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
      lead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
      lead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
      lead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
      lead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;

      sublead_lepton_IDSF_error = muons[0].IDSF_error;
      sublead_lepton_IsoSF_error = muons[0].IsoSF_error;

      lead_lepton_weight = electrons[0].weight;
      sublead_lepton_weight = muons[0].weight * TriggerSF1(fabs(sublead_lepton_p4.Eta()), sublead_lepton_p4.Pt(),Trigh1,Trigh2).first;

      lead_lepton_r9 = electrons[0].r9;

      lead_lepton_charge = electrons[0].charge;
      sublead_lepton_charge = muons[0].charge;
      
    } else {

      sublead_lepton_p4 = electrons[0].p4;
      sublead_lepton_weight = electrons[0].weight;
      sublead_lepton_IDSF_error = electrons[0].IDSF_error;
      sublead_lepton_RecoSF_error = electrons[0].RecoSF_error;
      sublead_lepton_HltSF_error = electrons[0].HltSF_error;
      sublead_lepton_EScaling_error = electrons[0].scale_error;
      sublead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
      sublead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
      sublead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
      sublead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
      sublead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;

      lead_lepton_IDSF_error = muons[0].IDSF_error;
      lead_lepton_IsoSF_error = muons[0].IsoSF_error;

      lead_lepton_p4 = muons[0].p4;
      lead_lepton_weight = muons[0].weight * TriggerSF1(fabs(lead_lepton_p4.Eta()), lead_lepton_p4.Pt(),Trigh1,Trigh2).first;

      sublead_lepton_r9 = electrons[0].r9;

      lead_lepton_charge = muons[0].charge;
      sublead_lepton_charge = electrons[0].charge;

    }
  }

  // check eta and pt cuts
  //if (makeHists) sel::hists("lead_lepton_pt", 100, 0, 200)->Fill(lead_lepton_p4.Pt());
  if(lead_lepton_p4.Pt() < 60) return false;
  //if (makeHists) sel::hists("lead_lepton_pt_cut", 100, 0, 200)->Fill(lead_lepton_p4.Pt());
  //if (makeHists) sel::hists("sublead_lepton_pt", 100, 0, 200)->Fill(sublead_lepton_p4.Pt());
  if(sublead_lepton_p4.Pt() < 53) return false;
  //if (makeHists) sel::hists("sublead_lepton_pt_cut", 100, 0, 200)->Fill(sublead_lepton_p4.Pt());

  //if (makeHists) sel::hists("dr", 100, 0, 5)->Fill(dR_TLV(lead_lepton_p4, gJets[0].p4));
  //if (makeHists) sel::hists("dr", 100, 0, 5)->Fill(dR_TLV(lead_lepton_p4, gJets[1].p4));
  //if (makeHists) sel::hists("dr", 100, 0, 5)->Fill(dR_TLV(sublead_lepton_p4, gJets[0].p4));
  //if (makeHists) sel::hists("dr", 100, 0, 5)->Fill(dR_TLV(sublead_lepton_p4, gJets[1].p4));
  //if (makeHists) sel::hists("dr_count", 1, 0, 1)->Fill(0);

  if(dR_TLV(lead_lepton_p4, sublead_lepton_p4) < 0.4) return false;
  if(dR_TLV(lead_lepton_p4, gJets[0].p4) < 0.4) return false;
  if(dR_TLV(lead_lepton_p4, gJets[1].p4) < 0.4) return false;
  if(dR_TLV(sublead_lepton_p4, gJets[0].p4) < 0.4) return false;
  if(dR_TLV(sublead_lepton_p4, gJets[1].p4) < 0.4) return false;

  //if (makeHists) sel::hists("dr_cut", 100, 0, 5)->Fill(dR_TLV(lead_lepton_p4, gJets[0].p4));
  //if (makeHists) sel::hists("dr_cut", 100, 0, 5)->Fill(dR_TLV(lead_lepton_p4, gJets[1].p4));
  //if (makeHists) sel::hists("dr_cut", 100, 0, 5)->Fill(dR_TLV(sublead_lepton_p4, gJets[0].p4));
  //if (makeHists) sel::hists("dr_cut", 100, 0, 5)->Fill(dR_TLV(sublead_lepton_p4, gJets[1].p4));
  //if (makeHists) sel::hists("dr_count_cut", 1, 0, 1)->Fill(0);

  dR_leadlepton_leadjet = dR_TLV(lead_lepton_p4, jets[0].p4);
  dR_subleadlepton_leadjet = dR_TLV(sublead_lepton_p4, jets[0].p4);
  dR_leadlepton_subleadjet = dR_TLV(lead_lepton_p4, jets[1].p4);
  dR_subleadlepton_subleadjet = dR_TLV(sublead_lepton_p4, jets[1].p4);

  lead_lepton_pt = lead_lepton_p4.Pt();
  lead_lepton_eta = lead_lepton_p4.Eta();
  lead_lepton_phi = lead_lepton_p4.Phi();

  sublead_lepton_pt = sublead_lepton_p4.Pt();
  sublead_lepton_eta = sublead_lepton_p4.Eta();
  sublead_lepton_phi = sublead_lepton_p4.Phi();

  // Build the WR mass and dilepton mass with the 2 highest pT jets and 2 highest pT leptons
  WR_mass = (lead_lepton_p4 + sublead_lepton_p4 + gJets[0].p4 + gJets[1].p4).M();
  N1_mass = (lead_lepton_p4 + gJets[0].p4 + gJets[1].p4).M();
  N2_mass = (sublead_lepton_p4 + gJets[0].p4 + gJets[1].p4).M();
  weight = global_event_weight * lead_lepton_weight * sublead_lepton_weight * lead_jet_weight * sublead_jet_weight;
  //weight = global_event_weight;
	
#ifdef DEBUGG
  std::cout << "weight (global_event_weight times single object weights)=\t" << weight << std::endl;
#endif

  pu_weight = fabs(global_event_weight);

  dilepton_mass = (lead_lepton_p4 + sublead_lepton_p4).M();
  dilepton_pt = (lead_lepton_p4 + sublead_lepton_p4).Pt();
	
  /*
    if (makeHists) {
    sel::hists("global", 4, 0, 4)->Fill(int(pair));
    if(pair == P_EE) sel::hists("global_EE", 1, 0, 1)->Fill(0);
    if(pair == P_BB) sel::hists("global_BB", 1, 0, 1)->Fill(0);
    if(pair == P_EB) sel::hists("global_EB", 1, 0, 1)->Fill(0);
    if(pair == P_GAP)  sel::hists("global_GAP", 1, 0, 1)->Fill(0);
    }
  */
  _isPassing = true;
  return _isPassing;

}
bool Selector::isPassingQCD(tag_t tag, bool makeHists)
{

  _isPassing = false;
  WR_mass = -1, N1_mass = -1, N2_mass = -1, lead_lepton_r9 = -1, sublead_lepton_r9 = -1, HT = -1;
  TLorentzVector lead_lepton_p4, sublead_lepton_p4, lead_jet_p4, sublead_jet_p4;
  lead_lepton_IDSF_error = -9;
  lead_lepton_RecoSF_error = -9;
  lead_lepton_HltSF_error = -9;
  lead_lepton_EScaling_error = -9;
  lead_lepton_ESmearing_sigma = -9;
  lead_lepton_ESmearing_sigma_phi_up = -9;
  lead_lepton_ESmearing_sigma_phi_down = -9;
  lead_lepton_ESmearing_sigma_rho_up = -9;
  lead_lepton_ESmearing_sigma_rho_down = -9;
  sublead_lepton_IDSF_error = -9;
  sublead_lepton_RecoSF_error = -9;
  sublead_lepton_HltSF_error = -9;
  sublead_lepton_EScaling_error = -9;
  sublead_lepton_ESmearing_sigma = -9;
  sublead_lepton_ESmearing_sigma_phi_up = -9;
  sublead_lepton_ESmearing_sigma_phi_down = -9;
  sublead_lepton_ESmearing_sigma_rho_up = -9;
  sublead_lepton_ESmearing_sigma_rho_down = -9;
  lead_lepton_IsoSF_error = -9;
  sublead_lepton_IsoSF_error = -9;

  lead_lepton_HoverEM = -9;
  lead_lepton_dxy = -9;
  lead_lepton_dB = -9;
  lead_lepton_dxyError = -9;
  lead_lepton_pfIso = -9;
  lead_lepton_GsfEleEmHadD1IsoRhoCut = -9;
  lead_lepton_GsftrkIso = -9;
  lead_lepton_trkIso = -9;
  lead_lepton_satCrystal = -9;
  lead_lepton_heepBitmap = -9;
  sublead_lepton_HoverEM = -9;
  sublead_lepton_dxy = -9;
  sublead_lepton_dB = -9;
  sublead_lepton_dxyError = -9;
  sublead_lepton_pfIso = -9;
  sublead_lepton_GsfEleEmHadD1IsoRhoCut = -9;
  sublead_lepton_GsftrkIso = -9;
  sublead_lepton_trkIso = -9;
  sublead_lepton_satCrystal = -9;
  sublead_lepton_heepBitmap = -9;

  myJetCollection gJets;
  myElectronCollection gEles;
  myMuonCollection gMuons;

  // Basic Kinematic cuts
  goodJets(&jets, &gJets);
  goodQCDEles(&electrons, &gEles);
  goodMuons(&muons, &gMuons);

  std::sort(gJets.begin(), gJets.end(),
	    [](myJet const & a, myJet const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });
  std::sort(gEles.begin(), gEles.end(),
	    [](myElectron const & a, myElectron const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });
  std::sort(gMuons.begin(), gMuons.end(),
	    [](myMuon const & a, myMuon const & b) {
	      return a.p4.Pt() > b.p4.Pt();
	    });

  jets = gJets;
  electrons = gEles;
  muons = gMuons;

  float ht  = 0.0;
  for(auto j:jets){
    ht += j.p4.Pt();
  }
  HT = ht;
  // Assert at least 2 good jets
  if(jets.size() < 2) {
    return false;
  }
  
  njets = jets.size();

  lead_jet_pt = jets[0].p4.Pt();
  lead_jet_eta = jets[0].p4.Eta();
  lead_jet_phi = jets[0].p4.Phi();
  lead_jet_weight = 1.0;
  sublead_jet_weight = 1.0;
  lead_jet_jec_unc = jets[0].jec_uncertainty;
  sublead_jet_jec_unc = jets[1].jec_uncertainty;

  sublead_jet_pt = jets[1].p4.Pt();
  sublead_jet_eta = jets[1].p4.Eta();
  sublead_jet_phi = jets[1].p4.Phi();

  if(tag == EE) { // EEJJ Channel
    // Assert at least 2 good leptons
    if(electrons.size() < 2) {
      return false;
    }

    lead_lepton_p4 = electrons[0].p4;
    sublead_lepton_p4 = electrons[1].p4;

    lead_lepton_weight = electrons[0].weight;
    sublead_lepton_weight = electrons[1].weight;

    lead_lepton_IDSF_error = electrons[0].IDSF_error;
    lead_lepton_RecoSF_error = electrons[0].RecoSF_error;
    lead_lepton_HltSF_error = electrons[0].HltSF_error;
    lead_lepton_EScaling_error = electrons[0].scale_error;
    lead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
    lead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
    lead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
    lead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
    lead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;

    sublead_lepton_IDSF_error = electrons[1].IDSF_error;
    sublead_lepton_RecoSF_error = electrons[1].RecoSF_error;
    sublead_lepton_HltSF_error = electrons[1].HltSF_error;
    sublead_lepton_EScaling_error = electrons[1].scale_error;
    sublead_lepton_ESmearing_sigma = electrons[1].smearing_sigma;
    sublead_lepton_ESmearing_sigma_phi_up = electrons[1].smearing_sigma_phi_up;
    sublead_lepton_ESmearing_sigma_phi_down = electrons[1].smearing_sigma_phi_down;
    sublead_lepton_ESmearing_sigma_rho_up = electrons[1].smearing_sigma_rho_up;
    sublead_lepton_ESmearing_sigma_rho_down = electrons[1].smearing_sigma_rho_down;

    lead_lepton_HoverEM = electrons[0].HoverEM;
    lead_lepton_dxy = electrons[0].dxy;
    lead_lepton_dB = electrons[0].dB;
    lead_lepton_dxyError = electrons[0].dxyError;
    lead_lepton_pfIso = electrons[0].pfIso;
    lead_lepton_GsfEleEmHadD1IsoRhoCut = electrons[0].GsfEleEmHadD1IsoRhoCut;
    lead_lepton_GsftrkIso = electrons[0].GsftrkIso;
    lead_lepton_trkIso = electrons[0].trkIso;
    lead_lepton_satCrystal = electrons[0].satCrystal;
    lead_lepton_heepBitmap = electrons[0].heepBitmap;
    sublead_lepton_HoverEM = electrons[1].HoverEM;
    sublead_lepton_dxy = electrons[1].dxy;
    sublead_lepton_dB = electrons[1].dB;
    sublead_lepton_dxyError = electrons[1].dxyError;
    sublead_lepton_pfIso = electrons[1].pfIso;
    sublead_lepton_GsfEleEmHadD1IsoRhoCut = electrons[1].GsfEleEmHadD1IsoRhoCut;
    sublead_lepton_GsftrkIso = electrons[1].GsftrkIso;
    sublead_lepton_trkIso = electrons[1].trkIso;
    sublead_lepton_satCrystal = electrons[1].satCrystal;
    sublead_lepton_heepBitmap = electrons[1].heepBitmap;

    lead_lepton_r9 = electrons[0].r9;
    sublead_lepton_r9 = electrons[1].r9;

    lead_lepton_charge = electrons[0].charge;
    sublead_lepton_charge = electrons[1].charge;

  }
  else if(tag == MuMu) { // MuMuJJ Channel
    // Assert at least 2 good leptons
    if(muons.size() < 2) {
      return false;
    }

    lead_lepton_p4 = muons[0].p4;
    sublead_lepton_p4 = muons[1].p4;

    lead_lepton_charge = muons[0].charge;
    sublead_lepton_charge = muons[1].charge;

    
    lead_lepton_IDSF_error = muons[0].IDSF_error;
    lead_lepton_IsoSF_error = muons[0].IsoSF_error;

    sublead_lepton_IDSF_error = muons[1].IDSF_error;
    sublead_lepton_IsoSF_error = muons[1].IsoSF_error;

    lead_lepton_weight = muons[0].weight * TriggerSF2(fabs(lead_lepton_p4.Eta()), lead_lepton_p4.Pt(),fabs(sublead_lepton_p4.Eta()), sublead_lepton_p4.Pt(),Trigh1,Trigh2,Trigg1,Trigg2).first;
    sublead_lepton_weight = muons[1].weight;

  }
  else if(tag == EMu) { // EMuJJ Channel
    // Assert at least 2 good leptons
    if(electrons.size() < 1 || muons.size() < 1) {
      return false;
    }

    // check which is the leading, which the subleading
    if(electrons[0].p4.Pt() > muons[0].p4.Pt()) { // e > mu
      lead_lepton_p4 = electrons[0].p4;
      sublead_lepton_p4 = muons[0].p4;

      lead_lepton_IDSF_error = electrons[0].IDSF_error;
      lead_lepton_RecoSF_error = electrons[0].RecoSF_error;
      lead_lepton_HltSF_error = electrons[0].HltSF_error;
      lead_lepton_EScaling_error = electrons[0].scale_error;
      lead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
      lead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
      lead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
      lead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
      lead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;

      sublead_lepton_IDSF_error = muons[0].IDSF_error;
      sublead_lepton_IsoSF_error = muons[0].IsoSF_error;

      lead_lepton_weight = electrons[0].weight;
      sublead_lepton_weight = muons[0].weight * TriggerSF1(fabs(sublead_lepton_p4.Eta()), sublead_lepton_p4.Pt(),Trigh1,Trigh2).first;

      lead_lepton_r9 = electrons[0].r9;
      lead_lepton_charge = electrons[0].charge;
      sublead_lepton_charge = muons[0].charge;

    } else {

      sublead_lepton_p4 = electrons[0].p4;
      sublead_lepton_weight = electrons[0].weight;
      sublead_lepton_IDSF_error = electrons[0].IDSF_error;
      sublead_lepton_RecoSF_error = electrons[0].RecoSF_error;
      sublead_lepton_HltSF_error = electrons[0].HltSF_error;
      sublead_lepton_EScaling_error = electrons[0].scale_error;
      sublead_lepton_ESmearing_sigma = electrons[0].smearing_sigma;
      sublead_lepton_ESmearing_sigma_phi_up = electrons[0].smearing_sigma_phi_up;
      sublead_lepton_ESmearing_sigma_phi_down = electrons[0].smearing_sigma_phi_down;
      sublead_lepton_ESmearing_sigma_rho_up = electrons[0].smearing_sigma_rho_up;
      sublead_lepton_ESmearing_sigma_rho_down = electrons[0].smearing_sigma_rho_down;

      lead_lepton_IDSF_error = muons[0].IDSF_error;
      lead_lepton_IsoSF_error = muons[0].IsoSF_error;

      lead_lepton_p4 = muons[0].p4;
      lead_lepton_weight = muons[0].weight * TriggerSF1(fabs(lead_lepton_p4.Eta()), lead_lepton_p4.Pt(),Trigh1,Trigh2).first;

      sublead_lepton_r9 = electrons[0].r9;

      lead_lepton_charge = muons[0].charge;
      sublead_lepton_charge = electrons[0].charge;

    }
  }

  // check eta and pt cuts
  if(lead_lepton_p4.Pt() < 60) return false;
  if(sublead_lepton_p4.Pt() < 53) return false;

  if(dR_TLV(lead_lepton_p4, sublead_lepton_p4) < 0.4) return false;
  if(dR_TLV(lead_lepton_p4, gJets[0].p4) < 0.4) return false;
  if(dR_TLV(lead_lepton_p4, gJets[1].p4) < 0.4) return false;
  if(dR_TLV(sublead_lepton_p4, gJets[0].p4) < 0.4) return false;
  if(dR_TLV(sublead_lepton_p4, gJets[1].p4) < 0.4) return false;

  dR_leadlepton_leadjet = dR_TLV(lead_lepton_p4, jets[0].p4);
  dR_subleadlepton_leadjet = dR_TLV(sublead_lepton_p4, jets[0].p4);
  dR_leadlepton_subleadjet = dR_TLV(lead_lepton_p4, jets[1].p4);
  dR_subleadlepton_subleadjet = dR_TLV(sublead_lepton_p4, jets[1].p4);

  lead_lepton_pt = lead_lepton_p4.Pt();
  lead_lepton_eta = lead_lepton_p4.Eta();
  lead_lepton_phi = lead_lepton_p4.Phi();

  sublead_lepton_pt = sublead_lepton_p4.Pt();
  sublead_lepton_eta = sublead_lepton_p4.Eta();
  sublead_lepton_phi = sublead_lepton_p4.Phi();

  // Build the WR mass and dilepton mass with the 2 highest pT jets and 2 highest pT leptons
  WR_mass = (lead_lepton_p4 + sublead_lepton_p4 + gJets[0].p4 + gJets[1].p4).M();
  N1_mass = (lead_lepton_p4 + gJets[0].p4 + gJets[1].p4).M();
  N2_mass = (sublead_lepton_p4 + gJets[0].p4 + gJets[1].p4).M();
  weight = global_event_weight * lead_lepton_weight * sublead_lepton_weight * lead_jet_weight * sublead_jet_weight;
  //weight = global_event_weight;
	
  pu_weight = fabs(global_event_weight);

  dilepton_mass = (lead_lepton_p4 + sublead_lepton_p4).M();
  dilepton_pt = (lead_lepton_p4 + sublead_lepton_p4).Pt();

  _isPassing = true;
  return _isPassing;

}

bool Selector::isPassingPreselect(bool makeHists)
{
  int l54 = 0;
  int l44 = 0;
  int j30 = 0;
  for(auto ele : electrons) {
    //if (makeHists) sel::hists("preselect_ele_pt", 100, 0, 200)->Fill(ele.p4.Pt());
    if(ele.p4.Pt() > 54) l54 += 1;
    if(ele.p4.Pt() > 44) l44 += 1;
    //if(!makeHists && l54 >= 1 && l44 >= 2) break;
  }
  for(auto mu : muons) {
    //if (makeHists) sel::hists("preselect_mu_pt", 100, 0, 200)->Fill(mu.p4.Pt());
    if(mu.trkIso > 0.1) continue;
    if(mu.p4.Pt() > 54) l54 += 1;
    if(mu.p4.Pt() > 44) l44 += 1;
    //if(!makeHists && l54 >= 1 && l44 >= 2) break;
  }
  for(auto jet : jets) {
    //if (makeHists) sel::hists("preselect_jet_pt", 100, 0, 200)->Fill(jet.p4.Pt());
    if(jet.p4.Pt() > 30) j30 += 1;
    //if(!makeHists && j30 >= 2) break;
  }
  //if (makeHists) sel::hists("preselect_count", 1, 0, 1)->Fill(0);
  bool passes = ( l54 >= 1 && l44 >= 2 && j30 >= 2);
  /*
    if (passes && makeHists) {
    sel::hists("preselect_count_cut", 1, 0, 1)->Fill(0);
    for(auto ele : electrons)
    sel::hists("preselect_ele_pt_cut", 100, 0, 200)->Fill(ele.p4.Pt());
    for(auto mu : muons)
    sel::hists("preselect_mu_pt_cut", 100, 0, 200)->Fill(mu.p4.Pt());
    for(auto jet : jets)
    sel::hists("preselect_jet_pt_cut", 100, 0, 200)->Fill(jet.p4.Pt());
    }
  */
  return passes;
}



void Selector::SetBranches(TTree* tree)
{
  tree->Branch("run", &run);
  tree->Branch("event", &event);
  tree->Branch("lumi", &lumi);

  tree->Branch("lead_lepton_pt", &lead_lepton_pt);
  tree->Branch("sublead_lepton_pt", &sublead_lepton_pt);
  tree->Branch("lead_lepton_eta", &lead_lepton_eta);
  tree->Branch("sublead_lepton_eta", &sublead_lepton_eta);
  tree->Branch("lead_lepton_phi", &lead_lepton_phi);
  tree->Branch("sublead_lepton_phi", &sublead_lepton_phi);
  tree->Branch("lead_jet_pt", &lead_jet_pt);
  tree->Branch("sublead_jet_pt", &sublead_jet_pt);
  tree->Branch("lead_jet_eta", &lead_jet_eta);
  tree->Branch("sublead_jet_eta", &sublead_jet_eta);
  tree->Branch("lead_jet_phi", &lead_jet_phi);
  tree->Branch("sublead_jet_phi", &sublead_jet_phi);
  tree->Branch("nPV", &nPV);
  tree->Branch("nPU", &nPU);
  tree->Branch("dR_leadlepton_leadjet", &dR_leadlepton_leadjet);
  tree->Branch("dR_leadlepton_subleadjet", &dR_leadlepton_subleadjet);
  tree->Branch("dR_subleadlepton_leadjet", &dR_subleadlepton_leadjet);
  tree->Branch("dR_subleadlepton_subleadjet", &dR_subleadlepton_subleadjet);
  tree->Branch("lead_lepton_r9", &lead_lepton_r9);
  tree->Branch("sublead_lepton_r9", &sublead_lepton_r9);
  tree->Branch("lead_lepton_charge", &lead_lepton_charge);
  tree->Branch("sublead_lepton_charge", &sublead_lepton_charge);

  tree->Branch("lead_lepton_HoverEM",&lead_lepton_HoverEM);
  tree->Branch("lead_lepton_dxy",&lead_lepton_dxy);
  tree->Branch("lead_lepton_dB",&lead_lepton_dB);
  tree->Branch("lead_lepton_dxyError",&lead_lepton_dxyError);
  tree->Branch("lead_lepton_pfIso",&lead_lepton_pfIso);
  tree->Branch("lead_lepton_GsfEleEmHadD1IsoRhoCut",&lead_lepton_GsfEleEmHadD1IsoRhoCut);
  tree->Branch("lead_lepton_GsftrkIso",&lead_lepton_GsftrkIso);
  tree->Branch("lead_lepton_trkIso",&lead_lepton_trkIso);
  tree->Branch("lead_lepton_satCrystal",&lead_lepton_satCrystal);
  tree->Branch("lead_lepton_heepBitmap",&lead_lepton_heepBitmap);

  tree->Branch("sublead_lepton_HoverEM",&sublead_lepton_HoverEM);
  tree->Branch("sublead_lepton_dxy",&sublead_lepton_dxy);
  tree->Branch("sublead_lepton_dB",&sublead_lepton_dB);
  tree->Branch("sublead_lepton_dxyError",&sublead_lepton_dxyError);
  tree->Branch("sublead_lepton_pfIso",&sublead_lepton_pfIso);
  tree->Branch("sublead_lepton_GsfEleEmHadD1IsoRhoCut",&sublead_lepton_GsfEleEmHadD1IsoRhoCut);
  tree->Branch("sublead_lepton_GsftrkIso",&sublead_lepton_GsftrkIso);
  tree->Branch("sublead_lepton_trkIso",&sublead_lepton_trkIso);
  tree->Branch("sublead_lepton_satCrystal",&sublead_lepton_satCrystal);
  tree->Branch("sublead_lepton_heepBitmap",&sublead_lepton_heepBitmap);
  
  tree->Branch("weight", &weight);
  tree->Branch("WR_mass", &WR_mass);
  tree->Branch("N1_mass", &N1_mass);
  tree->Branch("N2_mass", &N2_mass);
  tree->Branch("dilepton_mass", &dilepton_mass);
  tree->Branch("dilepton_pt", &dilepton_pt);
  tree->Branch("pu_weight", &pu_weight);
  tree->Branch("HT", &HT);
  tree->Branch("njets", &njets);
  tree->Branch("lead_jet_jec_unc", &lead_jet_jec_unc);
  tree->Branch("sublead_jet_jec_unc", &sublead_jet_jec_unc);

  tree->Branch("lead_lepton_IDSF_error", &lead_lepton_IDSF_error);
  tree->Branch("lead_lepton_IsoSF_error", &lead_lepton_IsoSF_error);
  tree->Branch("lead_lepton_RecoSF_error", &lead_lepton_RecoSF_error);
  tree->Branch("lead_lepton_HltSF_error", &lead_lepton_HltSF_error);
  tree->Branch("lead_lepton_EScaling_error", &lead_lepton_EScaling_error);
  tree->Branch("lead_lepton_ESmearing_sigma", &lead_lepton_ESmearing_sigma);
  tree->Branch("lead_lepton_ESmearing_sigma_phi_up", &lead_lepton_ESmearing_sigma_phi_up);
  tree->Branch("lead_lepton_ESmearing_sigma_phi_down", &lead_lepton_ESmearing_sigma_phi_down);
  tree->Branch("lead_lepton_ESmearing_sigma_rho_up", &lead_lepton_ESmearing_sigma_rho_up);
  tree->Branch("lead_lepton_ESmearing_sigma_rho_down", &lead_lepton_ESmearing_sigma_rho_down);
  tree->Branch("sublead_lepton_IDSF_error", &sublead_lepton_IDSF_error);
  tree->Branch("sublead_lepton_IsoSF_error", &sublead_lepton_IsoSF_error);
  tree->Branch("sublead_lepton_RecoSF_error", &sublead_lepton_RecoSF_error);
  tree->Branch("sublead_lepton_HltSF_error", &sublead_lepton_HltSF_error);
  tree->Branch("sublead_lepton_EScaling_error", &sublead_lepton_EScaling_error);
  tree->Branch("sublead_lepton_ESmearing_sigma", &sublead_lepton_ESmearing_sigma);
  tree->Branch("sublead_lepton_ESmearing_sigma_phi_up", &sublead_lepton_ESmearing_sigma_phi_up);
  tree->Branch("sublead_lepton_ESmearing_sigma_phi_down", &sublead_lepton_ESmearing_sigma_phi_down);
  tree->Branch("sublead_lepton_ESmearing_sigma_rho_up", &sublead_lepton_ESmearing_sigma_rho_up);
  tree->Branch("sublead_lepton_ESmearing_sigma_rho_down", &sublead_lepton_ESmearing_sigma_rho_down);

}

void Selector::SetBranchAddresses(TTree* tree)
{
  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("event", &event);
  tree->SetBranchAddress("lumi", &lumi);
  
  tree->SetBranchAddress("lead_lepton_pt", &lead_lepton_pt);
  tree->SetBranchAddress("lead_lepton_eta", &lead_lepton_eta);
  tree->SetBranchAddress("lead_lepton_phi", &lead_lepton_phi);
  tree->SetBranchAddress("sublead_lepton_pt", &sublead_lepton_pt);
  tree->SetBranchAddress("sublead_lepton_eta", &sublead_lepton_eta);
  tree->SetBranchAddress("sublead_lepton_phi", &sublead_lepton_phi);

  tree->SetBranchAddress("lead_jet_pt", &lead_jet_pt);
  tree->SetBranchAddress("lead_jet_eta", &lead_jet_eta);
  tree->SetBranchAddress("lead_jet_phi", &lead_jet_phi);
  tree->SetBranchAddress("sublead_jet_pt", &sublead_jet_pt);
  tree->SetBranchAddress("sublead_jet_eta", &sublead_jet_eta);
  tree->SetBranchAddress("sublead_jet_phi", &sublead_jet_phi);
  tree->SetBranchAddress("nPV", &nPV);
  tree->SetBranchAddress("nPU", &nPU);

  tree->SetBranchAddress("dR_leadlepton_leadjet", &dR_leadlepton_leadjet);
  tree->SetBranchAddress("dR_leadlepton_subleadjet", &dR_leadlepton_subleadjet);
  tree->SetBranchAddress("dR_subleadlepton_leadjet", &dR_subleadlepton_leadjet);
  tree->SetBranchAddress("dR_subleadlepton_subleadjet", &dR_subleadlepton_subleadjet);

  tree->SetBranchAddress("lead_lepton_r9", &lead_lepton_r9);
  tree->SetBranchAddress("sublead_lepton_r9", &sublead_lepton_r9);

  tree->SetBranchAddress("lead_lepton_charge", &lead_lepton_charge);
  tree->SetBranchAddress("sublead_lepton_charge", &sublead_lepton_charge);

  tree->SetBranchAddress("lead_lepton_HoverEM",&lead_lepton_HoverEM);
  tree->SetBranchAddress("lead_lepton_dxy",&lead_lepton_dxy);
  tree->SetBranchAddress("lead_lepton_dB",&lead_lepton_dB);
  tree->SetBranchAddress("lead_lepton_dxyError",&lead_lepton_dxyError);
  tree->SetBranchAddress("lead_lepton_pfIso",&lead_lepton_pfIso);
  tree->SetBranchAddress("lead_lepton_GsfEleEmHadD1IsoRhoCut",&lead_lepton_GsfEleEmHadD1IsoRhoCut);
  tree->SetBranchAddress("lead_lepton_GsftrkIso",&lead_lepton_GsftrkIso);
  tree->SetBranchAddress("lead_lepton_trkIso",&lead_lepton_trkIso);
  tree->SetBranchAddress("lead_lepton_satCrystal",&lead_lepton_satCrystal);
  tree->SetBranchAddress("lead_lepton_heepBitmap",&lead_lepton_heepBitmap);

  tree->SetBranchAddress("sublead_lepton_HoverEM",&sublead_lepton_HoverEM);
  tree->SetBranchAddress("sublead_lepton_dxy",&sublead_lepton_dxy);
  tree->SetBranchAddress("sublead_lepton_dB",&sublead_lepton_dB);
  tree->SetBranchAddress("sublead_lepton_dxyError",&sublead_lepton_dxyError);
  tree->SetBranchAddress("sublead_lepton_pfIso",&sublead_lepton_pfIso);
  tree->SetBranchAddress("sublead_lepton_GsfEleEmHadD1IsoRhoCut",&sublead_lepton_GsfEleEmHadD1IsoRhoCut);
  tree->SetBranchAddress("sublead_lepton_GsftrkIso",&sublead_lepton_GsftrkIso);
  tree->SetBranchAddress("sublead_lepton_trkIso",&sublead_lepton_trkIso);
  tree->SetBranchAddress("sublead_lepton_satCrystal",&sublead_lepton_satCrystal);
  tree->SetBranchAddress("sublead_lepton_heepBitmap",&sublead_lepton_heepBitmap);
  
  
  tree->SetBranchAddress("weight", &weight);
  tree->SetBranchAddress("WR_mass", &WR_mass);
  tree->SetBranchAddress("N1_mass", &N1_mass);
  tree->SetBranchAddress("N2_mass", &N2_mass);
  tree->SetBranchAddress("dilepton_mass", &dilepton_mass);
  tree->SetBranchAddress("dilepton_pt", &dilepton_pt);
  tree->SetBranchAddress("pu_weight", &pu_weight);
  tree->SetBranchAddress("njets", &njets);
  tree->SetBranchAddress("HT", &HT);
  tree->SetBranchAddress("lead_jet_jec_unc", &lead_jet_jec_unc);
  tree->SetBranchAddress("sublead_jet_jec_unc", &sublead_jet_jec_unc);

  tree->SetBranchAddress("lead_lepton_IDSF_error", &lead_lepton_IDSF_error);
  tree->SetBranchAddress("lead_lepton_IsoSF_error", &lead_lepton_IsoSF_error);
  tree->SetBranchAddress("lead_lepton_RecoSF_error", &lead_lepton_RecoSF_error);
  tree->SetBranchAddress("lead_lepton_HltSF_error", &lead_lepton_HltSF_error);
  tree->SetBranchAddress("lead_lepton_EScaling_error", &lead_lepton_EScaling_error);
  tree->SetBranchAddress("lead_lepton_ESmearing_sigma", &lead_lepton_ESmearing_sigma);
  tree->SetBranchAddress("lead_lepton_ESmearing_sigma_phi_up", &lead_lepton_ESmearing_sigma_phi_up);
  tree->SetBranchAddress("lead_lepton_ESmearing_sigma_phi_down", &lead_lepton_ESmearing_sigma_phi_down);
  tree->SetBranchAddress("lead_lepton_ESmearing_sigma_rho_up", &lead_lepton_ESmearing_sigma_rho_up);
  tree->SetBranchAddress("lead_lepton_ESmearing_sigma_rho_down", &lead_lepton_ESmearing_sigma_rho_down);
  tree->SetBranchAddress("sublead_lepton_IDSF_error", &sublead_lepton_IDSF_error);
  tree->SetBranchAddress("sublead_lepton_IsoSF_error", &sublead_lepton_IsoSF_error);
  tree->SetBranchAddress("sublead_lepton_RecoSF_error", &sublead_lepton_RecoSF_error);
  tree->SetBranchAddress("sublead_lepton_HltSF_error", &sublead_lepton_HltSF_error);
  tree->SetBranchAddress("sublead_lepton_EScaling_error", &sublead_lepton_EScaling_error);
  tree->SetBranchAddress("sublead_lepton_ESmearing_sigma", &sublead_lepton_ESmearing_sigma);
  tree->SetBranchAddress("sublead_lepton_ESmearing_sigma_phi_up", &sublead_lepton_ESmearing_sigma_phi_up);
  tree->SetBranchAddress("sublead_lepton_ESmearing_sigma_phi_down", &sublead_lepton_ESmearing_sigma_phi_down);
  tree->SetBranchAddress("sublead_lepton_ESmearing_sigma_rho_up", &sublead_lepton_ESmearing_sigma_rho_up);
  tree->SetBranchAddress("sublead_lepton_ESmearing_sigma_rho_down", &sublead_lepton_ESmearing_sigma_rho_down);

}
