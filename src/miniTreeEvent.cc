// #include "ExoAnalysis/cmsWR/interface/miniTreeEvent.h"
#include "../interface/miniTreeEvent.h"

miniTreeEvent::miniTreeEvent():
  electrons_p4(new std::vector<TLorentzVector>),
  electron_scale_error(new std::vector<Float_t>),
  electron_smearing_sigma(new std::vector<Float_t>),
  electron_smearing_sigma_phi_up(new std::vector<Float_t>),
  electron_smearing_sigma_phi_down(new std::vector<Float_t>),
  electron_smearing_sigma_rho_up(new std::vector<Float_t>),
  electron_smearing_sigma_rho_down(new std::vector<Float_t>),
  electron_r9(new std::vector<Float_t>),
  electron_SC_eta(new std::vector<Float_t>),
  electron_charge(new std::vector<Int_t>),
  electron_IDSF_central(new std::vector<Float_t>),
  electron_IDSF_error(new std::vector<Float_t>),
  electron_RecoSF_central(new std::vector<Float_t>),
  electron_RecoSF_error(new std::vector<Float_t>),
  electron_HltSF_central(new std::vector<Float_t>),
  electron_HltSF_error(new std::vector<Float_t>),
  electron_HltDiEle33CaloIdLMWPMS2UnseededFilter(new std::vector<bool>),
  electron_heepId(new std::vector<bool>),
  electron_HoverEM(new std::vector<Float_t>),
  electron_dxy(new std::vector<Float_t>),
  electron_dB(new std::vector<Float_t>),
  electron_dxyError(new std::vector<Float_t>),
  electron_pfIso(new std::vector<Float_t>),
  electron_GsfEleEmHadD1IsoRhoCut(new std::vector<Float_t>),
  electron_GsftrkIso(new std::vector<Float_t>),
  electron_trkIso(new std::vector<Float_t>),
  electron_satCrystal(new std::vector<Int_t>),
  electron_heepBitmap(new std::vector<Int_t>),

  muons_p4(new std::vector<TLorentzVector>),
  muon_charge(new std::vector<Int_t>),
  muon_IDSF_central(new std::vector<Float_t>),
  muon_IsoSF_central(new std::vector<Float_t>),
  muon_TrigSF_central(new std::vector<Float_t>),
  muon_IDSF_error(new std::vector<Float_t>),
  muon_IsoSF_error(new std::vector<Float_t>),
  muon_TrigSF_error(new std::vector<Float_t>),
  muon_IDSF_central2(new std::vector<Float_t>),
  muon_IsoSF_central2(new std::vector<Float_t>),
  muon_TrigSF_central2(new std::vector<Float_t>),
  muon_IDSF_error2(new std::vector<Float_t>),
  muon_IsoSF_error2(new std::vector<Float_t>),
  muon_TrigSF_error2(new std::vector<Float_t>),
  muon_trackerLayersWithMeasurement(new std::vector<Float_t>),
  muon_trkIso(new std::vector<Float_t>),
  muon_ptRel(new std::vector<Float_t>),
  muon_muonChambers(new std::vector<Float_t>),
  muon_matchedStations(new std::vector<Float_t>),
  muon_dxy(new std::vector<Float_t>),
  muon_dxyError(new std::vector<Float_t>),
  muon_pixelHits(new std::vector<Float_t>),
  muon_stripHits(new std::vector<Float_t>),
  ////////
  jets_p4(new std::vector<TLorentzVector>),
  jec_uncertainty(new std::vector<Float_t>),
  jetResolution(new std::vector<Float_t>),
  JER_sf(new std::vector<Float_t>),
  JER_sf_up(new std::vector<Float_t>),
  JER_sf_down(new std::vector<Float_t>),
  genJetPt(new std::vector<Float_t>),
  genJetMatch(new std::vector<bool>),
  genps_p4(new std::vector<TLorentzVector>),
  genps_pdgId(new std::vector<Int_t>),
  genps_status(new std::vector<Int_t>),
  genps_motherpdgId(new std::vector<Int_t>),
  RF_weights(new std::vector<Float_t>),
	
  _owningMembers(true)
{

  clear();
}


miniTreeEvent::miniTreeEvent(const miniTreeEvent& otherEvent):
  electrons_p4(new std::vector<TLorentzVector>),
  electron_scale_error(new std::vector<Float_t>),
  electron_smearing_sigma(new std::vector<Float_t>),
  electron_smearing_sigma_phi_up(new std::vector<Float_t>),
  electron_smearing_sigma_phi_down(new std::vector<Float_t>),
  electron_smearing_sigma_rho_up(new std::vector<Float_t>),
  electron_smearing_sigma_rho_down(new std::vector<Float_t>),
  electron_r9(new std::vector<Float_t>),
  electron_SC_eta(new std::vector<Float_t>),
  electron_charge(new std::vector<Int_t>),
  electron_IDSF_central(new std::vector<Float_t>),
  electron_IDSF_error(new std::vector<Float_t>),
  electron_RecoSF_central(new std::vector<Float_t>),
  electron_RecoSF_error(new std::vector<Float_t>),
  electron_HltSF_central(new std::vector<Float_t>),
  electron_HltSF_error(new std::vector<Float_t>),
  electron_HltDiEle33CaloIdLMWPMS2UnseededFilter(new std::vector<bool>),
  electron_heepId(new std::vector<bool>),
  electron_HoverEM(new std::vector<Float_t>),
  electron_dxy(new std::vector<Float_t>),
  electron_dB(new std::vector<Float_t>),
  electron_dxyError(new std::vector<Float_t>),
  electron_pfIso(new std::vector<Float_t>),
  electron_GsfEleEmHadD1IsoRhoCut(new std::vector<Float_t>),
  electron_GsftrkIso(new std::vector<Float_t>),
  electron_trkIso(new std::vector<Float_t>),
  electron_satCrystal(new std::vector<Int_t>),
  electron_heepBitmap(new std::vector<Int_t>),
  ///////
  muons_p4(new std::vector<TLorentzVector>),
  muon_charge(new std::vector<Int_t>),
  muon_IDSF_central(new std::vector<Float_t>),
  muon_IsoSF_central(new std::vector<Float_t>),
  muon_TrigSF_central(new std::vector<Float_t>),
  muon_IDSF_error(new std::vector<Float_t>),
  muon_IsoSF_error(new std::vector<Float_t>),
  muon_TrigSF_error(new std::vector<Float_t>),
  muon_IDSF_central2(new std::vector<Float_t>),
  muon_IsoSF_central2(new std::vector<Float_t>),
  muon_TrigSF_central2(new std::vector<Float_t>),
  muon_IDSF_error2(new std::vector<Float_t>),
  muon_IsoSF_error2(new std::vector<Float_t>),
  muon_TrigSF_error2(new std::vector<Float_t>),
  muon_trackerLayersWithMeasurement(new std::vector<Float_t>),
  muon_trkIso(new std::vector<Float_t>),
  muon_ptRel(new std::vector<Float_t>),
  muon_muonChambers(new std::vector<Float_t>),
  muon_matchedStations(new std::vector<Float_t>),
  muon_dxy(new std::vector<Float_t>),
  muon_dxyError(new std::vector<Float_t>),
  muon_pixelHits(new std::vector<Float_t>),
  muon_stripHits(new std::vector<Float_t>),
  /////////
  jets_p4(new std::vector<TLorentzVector>),
  jec_uncertainty(new std::vector<Float_t>),
  jetResolution(new std::vector<Float_t>),
  JER_sf(new std::vector<Float_t>),
  JER_sf_up(new std::vector<Float_t>),
  JER_sf_down(new std::vector<Float_t>),
  genJetPt(new std::vector<Float_t>),
  genJetMatch(new std::vector<bool>),
  genps_p4(new std::vector<TLorentzVector>),
  genps_pdgId(new std::vector<Int_t>),
  genps_status(new std::vector<Int_t>),
  genps_motherpdgId(new std::vector<Int_t>),
  RF_weights(new std::vector<Float_t>),
	
  _owningMembers(true)
{
  clear();
  *electrons_p4 = *(otherEvent.electrons_p4);
  *electron_scale_error = *(otherEvent.electron_scale_error);
  *electron_smearing_sigma = *(otherEvent.electron_smearing_sigma);
  *electron_smearing_sigma_phi_up = *(otherEvent.electron_smearing_sigma_phi_up);
  *electron_smearing_sigma_phi_down = *(otherEvent.electron_smearing_sigma_phi_down);
  *electron_smearing_sigma_rho_up = *(otherEvent.electron_smearing_sigma_rho_up);
  *electron_smearing_sigma_rho_down = *(otherEvent.electron_smearing_sigma_rho_down);
  *electron_r9 = *(otherEvent.electron_r9);
  *electron_SC_eta = *(otherEvent.electron_SC_eta);
  *electron_charge = *(otherEvent.electron_charge);
  *electron_IDSF_central = *(otherEvent.electron_IDSF_central);
  *electron_IDSF_error = *(otherEvent.electron_IDSF_error);
  *electron_RecoSF_central = *(otherEvent.electron_RecoSF_central);
  *electron_RecoSF_error = *(otherEvent.electron_RecoSF_error);
  *electron_HltSF_central = *(otherEvent.electron_HltSF_central);
  *electron_HltSF_error = *(otherEvent.electron_HltSF_error);
  *electron_HltDiEle33CaloIdLMWPMS2UnseededFilter = *(otherEvent.electron_HltDiEle33CaloIdLMWPMS2UnseededFilter),
  *electron_heepId = *(otherEvent.electron_heepId);
  *electron_HoverEM = *(otherEvent.electron_HoverEM);
  *electron_dxy = *(otherEvent.electron_dxy);
  *electron_dB = *(otherEvent.electron_dB);
  *electron_dxyError = *(otherEvent.electron_dxyError);
  *electron_pfIso = *(otherEvent.electron_pfIso);
  *electron_GsfEleEmHadD1IsoRhoCut = *(otherEvent.electron_GsfEleEmHadD1IsoRhoCut);
  *electron_GsftrkIso = *(otherEvent.electron_GsftrkIso);
  *electron_trkIso = *(otherEvent.electron_trkIso);
  *electron_satCrystal = *(otherEvent.electron_satCrystal);
  *electron_heepBitmap = *(otherEvent.electron_heepBitmap);
  ///////////
  *muons_p4 = *(otherEvent.muons_p4);
  *jets_p4 = *(otherEvent.jets_p4);
  *jec_uncertainty = *(otherEvent.jec_uncertainty);
  *jetResolution = *(otherEvent.jetResolution);
  *JER_sf = *(otherEvent.JER_sf);
  *JER_sf_up = *(otherEvent.JER_sf_up);
  *JER_sf_down = *(otherEvent.JER_sf_down);
  *genJetPt = *(otherEvent.genJetPt);
  *genJetMatch = *(otherEvent.genJetMatch);
  *muon_charge = *(otherEvent.muon_charge);
  *muon_IDSF_central = *(otherEvent.muon_IDSF_central);
  *muon_IsoSF_central = *(otherEvent.muon_IsoSF_central);
  *muon_TrigSF_central = *(otherEvent.muon_TrigSF_central);
  *muon_IDSF_error = *(otherEvent.muon_IDSF_error);
  *muon_IsoSF_error = *(otherEvent.muon_IsoSF_error);
  *muon_TrigSF_error = *(otherEvent.muon_TrigSF_error);
  *muon_IDSF_central2 = *(otherEvent.muon_IDSF_central2);
  *muon_IsoSF_central2 = *(otherEvent.muon_IsoSF_central2);
  *muon_TrigSF_central2 = *(otherEvent.muon_TrigSF_central2);
  *muon_IDSF_error2 = *(otherEvent.muon_IDSF_error2);
  *muon_IsoSF_error2 = *(otherEvent.muon_IsoSF_error2);
  *muon_TrigSF_error2 = *(otherEvent.muon_TrigSF_error2);
  *muon_trackerLayersWithMeasurement = *(otherEvent.muon_trackerLayersWithMeasurement );
  *muon_trkIso = *(otherEvent.muon_trkIso);
  *muon_ptRel = *(otherEvent.muon_ptRel);
  *muon_muonChambers = *(otherEvent.muon_muonChambers);
  *muon_matchedStations = *(otherEvent.muon_matchedStations);
  *muon_dxy = *(otherEvent.muon_dxy);
  *muon_dxyError = *(otherEvent.muon_dxyError);
  *muon_pixelHits = *(otherEvent.muon_pixelHits);
  *muon_stripHits = *(otherEvent.muon_stripHits);
  ///////////
  *genps_p4 = *(otherEvent.genps_p4);
  *genps_pdgId = *(otherEvent.genps_pdgId);
  *genps_status = *(otherEvent.genps_status);
  *genps_motherpdgId = *(otherEvent.genps_motherpdgId);
	
  _owningMembers = false;

  run = otherEvent.run;
  lumi = otherEvent.lumi;
  event = otherEvent.event;

  nPU = otherEvent.nPU;
  nPV = otherEvent.nPV;
  weight = otherEvent.weight;

  *RF_weights = *(otherEvent.RF_weights);
	
  PU_reweight = otherEvent.PU_reweight;

  sprintf(datasetName, "%s", otherEvent.datasetName);
}


void miniTreeEvent::clear()
{
  run = lumi = event = 0;

  electrons_p4->clear();
  electron_scale_error->clear();
  electron_smearing_sigma->clear();
  electron_smearing_sigma_phi_up->clear();
  electron_smearing_sigma_phi_down->clear();
  electron_smearing_sigma_rho_up->clear();
  electron_smearing_sigma_rho_down->clear();
  electron_r9->clear();
  electron_SC_eta->clear();
  electron_charge->clear();
  electron_IDSF_central->clear();
  electron_IDSF_error->clear();
  electron_RecoSF_central->clear();
  electron_RecoSF_error->clear();
  electron_HltSF_central->clear();
  electron_HltSF_error->clear();
  electron_HltDiEle33CaloIdLMWPMS2UnseededFilter->clear();
  electron_heepId->clear();
  electron_HoverEM->clear();
  electron_dxy->clear();
  electron_dB->clear();
  electron_dxyError->clear();
  electron_pfIso->clear();
  electron_GsfEleEmHadD1IsoRhoCut->clear();
  electron_GsftrkIso->clear();
  electron_trkIso->clear();
  electron_satCrystal->clear();
  electron_heepBitmap->clear();

  muons_p4->clear();
  jets_p4->clear();
  genps_p4->clear();

  jec_uncertainty->clear();
  jetResolution->clear();
  JER_sf->clear();
  JER_sf_up->clear();
  JER_sf_down->clear();
  genJetPt->clear();
  genJetMatch->clear();
  
  muon_charge->clear();
  muon_IDSF_central->clear();
  muon_IsoSF_central->clear();
  muon_TrigSF_central->clear();
  muon_IDSF_error->clear();
  muon_IsoSF_error->clear();
  muon_TrigSF_error->clear();
  muon_IDSF_central2->clear();
  muon_IsoSF_central2->clear();
  muon_TrigSF_central2->clear();
  muon_IDSF_error2->clear();
  muon_IsoSF_error2->clear();
  muon_TrigSF_error2->clear();
  muon_trackerLayersWithMeasurement->clear();
  muon_trkIso->clear();
  muon_ptRel->clear();
  muon_muonChambers->clear();
  muon_matchedStations->clear();
  muon_dxy->clear();
  muon_dxyError->clear();
  muon_pixelHits->clear();
  muon_stripHits->clear();

  genps_pdgId->clear();
  genps_status->clear();
  genps_motherpdgId->clear();

  nPU = -999.;
  nPV = 0.;
  weight = 0.0;
  RF_weights->clear();
  PU_reweight = 0.0;

}
miniTreeEvent::~miniTreeEvent()
{
  clear();
  delete electrons_p4;
  delete electron_scale_error;
  delete electron_smearing_sigma;
  delete electron_smearing_sigma_phi_up;
  delete electron_smearing_sigma_phi_down;
  delete electron_smearing_sigma_rho_up;
  delete electron_smearing_sigma_rho_down;
  delete electron_r9;
  delete electron_SC_eta;
  delete electron_charge;
  delete electron_IDSF_central;
  delete electron_IDSF_error;
  delete electron_RecoSF_central;
  delete electron_RecoSF_error;
  delete electron_HltSF_central;
  delete electron_HltSF_error;
  delete electron_HltDiEle33CaloIdLMWPMS2UnseededFilter;
  delete electron_heepId;
  delete electron_HoverEM;
  delete electron_dxy;
  delete electron_dB;
  delete electron_dxyError;
  delete electron_pfIso;
  delete electron_GsfEleEmHadD1IsoRhoCut;
  delete electron_GsftrkIso;
  delete electron_trkIso;
  delete electron_satCrystal;
  delete electron_heepBitmap;

  delete muons_p4;
  delete muon_charge;
  delete muon_IDSF_central;
  delete muon_IsoSF_central;
  delete muon_TrigSF_central;
  delete muon_IDSF_error;
  delete muon_IsoSF_error;
  delete muon_TrigSF_error;
  ///////
  delete muon_IDSF_central2;
  delete muon_IsoSF_central2;
  delete muon_TrigSF_central2;
  delete muon_IDSF_error2;
  delete muon_IsoSF_error2;
  delete muon_TrigSF_error2;
  delete muon_trackerLayersWithMeasurement;
  delete muon_trkIso;
  delete muon_ptRel;
  delete muon_muonChambers;
  delete muon_matchedStations;
  delete muon_dxy;
  delete muon_dxyError;
  delete muon_pixelHits;
  delete muon_stripHits;
  ///////
  delete jets_p4;
  delete jec_uncertainty;
  delete jetResolution;
  delete JER_sf;
  delete JER_sf_up;
  delete JER_sf_down;
  delete genJetPt;
  delete genJetMatch;
  delete genps_p4;
  delete genps_pdgId;
  delete genps_status;
  delete genps_motherpdgId;
  delete RF_weights;
}

void miniTreeEvent::SetBranches(TTree* tree)
{
  tree->Branch("run", &run);
  tree->Branch("lumi", &lumi);
  tree->Branch("event", &event);
  tree->Branch("datasetName", datasetName, "datasetName/C");

  tree->Branch("electrons_p4", electrons_p4, 32000, -1);
  tree->Branch("electron_scale_error", electron_scale_error);
  tree->Branch("electron_smearing_sigma", electron_smearing_sigma);
  tree->Branch("electron_smearing_sigma_phi_up", electron_smearing_sigma_phi_up);
  tree->Branch("electron_smearing_sigma_phi_down", electron_smearing_sigma_phi_down);
  tree->Branch("electron_smearing_sigma_rho_up", electron_smearing_sigma_rho_up);
  tree->Branch("electron_smearing_sigma_rho_down", electron_smearing_sigma_rho_down);
  tree->Branch("electron_r9", electron_r9);
  tree->Branch("electron_SC_eta", electron_SC_eta);
  tree->Branch("electron_charge", electron_charge);
  tree->Branch("electron_HltDiEle33CaloIdLMWPMS2UnseededFilter", electron_HltDiEle33CaloIdLMWPMS2UnseededFilter);
  tree->Branch("electron_heepId", electron_heepId);
  tree->Branch("electron_HoverEM",electron_HoverEM);
  tree->Branch("electron_dxy",electron_dxy);
  tree->Branch("electron_dB",electron_dB);
  tree->Branch("electron_dxyError",electron_dxyError);
  tree->Branch("electron_pfIso",electron_pfIso);
  tree->Branch("electron_GsfEleEmHadD1IsoRhoCut",electron_GsfEleEmHadD1IsoRhoCut);
  tree->Branch("electron_GsftrkIso",electron_GsftrkIso);
  tree->Branch("electron_trkIso",electron_trkIso);
  tree->Branch("electron_satCrystal",electron_satCrystal);
  tree->Branch("electron_heepBitmap",electron_heepBitmap);

  tree->Branch("muons_p4", muons_p4, 32000, -1);
  tree->Branch("jets_p4", jets_p4, 32000, -1);
  tree->Branch("genps_p4", genps_p4, 32000, -1);

  tree->Branch("jec_uncertainty", jec_uncertainty);
  tree->Branch("jetResolution", jetResolution);
  tree->Branch("JER_sf", JER_sf);
  tree->Branch("JER_sf_up", JER_sf_up);
  tree->Branch("JER_sf_down", JER_sf_down);
  tree->Branch("genJetPt", genJetPt);
  tree->Branch("genJetMatch", genJetMatch);
  tree->Branch("muon_charge", muon_charge);
  tree->Branch("muon_IDSF_central", muon_IDSF_central);
  tree->Branch("muon_IsoSF_central", muon_IsoSF_central);
  tree->Branch("muon_TrigSF_central", muon_TrigSF_central);
  tree->Branch("muon_IDSF_error", muon_IDSF_error);
  tree->Branch("muon_IsoSF_error", muon_IsoSF_error);
  tree->Branch("muon_TrigSF_error", muon_TrigSF_error);
  tree->Branch("muon_trackerLayersWithMeasurement", muon_trackerLayersWithMeasurement);
  tree->Branch("muon_trkIso",muon_trkIso);
  tree->Branch("muon_ptRel",muon_ptRel);
  tree->Branch("muon_muonChambers",muon_muonChambers);
  tree->Branch("muon_matchedStations",muon_matchedStations);
  tree->Branch("muon_dxy",muon_dxy);
  tree->Branch("muon_dxyError",muon_dxyError);
  tree->Branch("muon_pixelHits",muon_pixelHits);
  tree->Branch("muon_stripHits",muon_stripHits);

  tree->Branch("genps_pdgId", genps_pdgId);
  tree->Branch("genps_status", genps_status);
  tree->Branch("genps_motherpdgId", genps_motherpdgId);

  tree->Branch("nPU", &nPU);
  tree->Branch("nPV", &nPV);
  tree->Branch("weight", &weight);
  tree->Branch("RF_weights", RF_weights);
	
  tree->Branch("PU_reweight", &PU_reweight);

}

void miniTreeEvent::SetBranchAddresses(TChain* tree)
{
  /*
  delete electrons_p4;
  delete muons_p4;
  delete jets_p4;

  delete jec_uncertainty;
  delete jetResolution;
  delete JER_sf;
  delete JER_sf_up;
  delete JER_sf_down;
  delete genJetPt;
  delete genJetMatch;
  delete electron_scale_error;
  delete electron_smearing_sigma;
  delete electron_smearing_sigma_phi_up;
  delete electron_smearing_sigma_phi_down;
  delete electron_smearing_sigma_rho_up;
  delete electron_smearing_sigma_rho_down;
  delete electron_r9;
  delete electron_SC_eta;
  delete electron_charge;
  delete electron_HltDiEle33CaloIdLMWPMS2UnseededFilter;
  delete electron_heepId;
  delete electron_HoverEM;
  delete electron_dxy;
  delete electron_dB;
  delete electron_dxyError;
  delete electron_pfIso;
  delete electron_GsfEleEmHadD1IsoRhoCut;
  delete electron_GsftrkIso;
  delete electron_trkIso;
  delete electron_satCrystal;
  delete electron_heepBitmap;
  ///////
  delete muons_p4;
  delete muon_charge;
  delete muon_IDSF_central;
  delete muon_IsoSF_central;
  delete muon_TrigSF_central;
  delete muon_IDSF_error;
  delete muon_IsoSF_error;
  delete muon_TrigSF_error;
  delete muon_IDSF_central2;
  delete muon_IsoSF_central2;
  delete muon_TrigSF_central2;
  delete muon_IDSF_error2;
  delete muon_IsoSF_error2;
  delete muon_TrigSF_error2;
  delete muon_trackerLayersWithMeasurement;
  delete muon_trkIso;
  delete muon_ptRel;
  delete muon_muonChambers;
  delete muon_matchedStations;
  delete muon_dxy;
  delete muon_dxyError;
  delete muon_pixelHits;
  delete muon_stripHits;
  delete genps_p4;
  delete genps_pdgId;
  delete genps_status;
  delete genps_motherpdgId;
  delete RF_weights;

  */
  _owningMembers = false;

  electrons_p4 = 0;
  electron_scale_error = 0;
  electron_smearing_sigma = 0;
  electron_smearing_sigma_phi_up = 0;
  electron_smearing_sigma_phi_down = 0;
  electron_smearing_sigma_rho_up = 0;
  electron_smearing_sigma_rho_down = 0;
  electron_r9 = 0;
  electron_SC_eta = 0;
  electron_charge = 0;
  electron_HltDiEle33CaloIdLMWPMS2UnseededFilter = 0;
  electron_heepId = 0;
  electron_HoverEM = 0;
  electron_dxy = 0;
  electron_dB = 0;
  electron_dxyError = 0;
  electron_pfIso = 0;
  electron_GsfEleEmHadD1IsoRhoCut = 0;
  electron_GsftrkIso = 0;
  electron_trkIso = 0;
  electron_satCrystal = 0;
  electron_heepBitmap = 0;

  muons_p4 = 0;
  jets_p4 = 0;
  genps_p4 = 0;
  
  jec_uncertainty = 0;
  jetResolution = 0;
  JER_sf = 0;
  JER_sf_up = 0;
  JER_sf_down = 0;
  genJetPt = 0;
  genJetMatch = 0;
  muon_charge = 0;
  muon_IDSF_central = 0;
  muon_IsoSF_central = 0;
  muon_TrigSF_central = 0;
  muon_IDSF_error = 0;
  muon_IsoSF_error = 0;
  muon_TrigSF_error = 0;
  muon_trackerLayersWithMeasurement = 0;
  muon_trkIso = 0;
  muon_ptRel = 0;
  muon_muonChambers = 0;
  muon_matchedStations = 0;
  muon_dxy = 0;
  muon_dxyError = 0;
  muon_pixelHits = 0;
  muon_stripHits = 0;

  genps_pdgId = 0;
  genps_status = 0;
  genps_motherpdgId = 0;

  RF_weights = 0;
	
  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("lumi", &lumi);
  tree->SetBranchAddress("event", &event);
  tree->SetBranchAddress("datasetName", datasetName);

  tree->SetBranchAddress("electrons_p4", &electrons_p4);
  tree->SetBranchAddress("electron_scale_error", &electron_scale_error);
  tree->SetBranchAddress("electron_smearing_sigma", &electron_smearing_sigma);
  tree->SetBranchAddress("electron_smearing_sigma_phi_up", &electron_smearing_sigma_phi_up);
  tree->SetBranchAddress("electron_smearing_sigma_phi_down", &electron_smearing_sigma_phi_down);
  tree->SetBranchAddress("electron_smearing_sigma_rho_up", &electron_smearing_sigma_rho_up);
  tree->SetBranchAddress("electron_smearing_sigma_rho_down", &electron_smearing_sigma_rho_down);
  tree->SetBranchAddress("electron_r9", &electron_r9);
  tree->SetBranchAddress("electron_SC_eta", &electron_SC_eta);
  tree->SetBranchAddress("electron_charge", &electron_charge);
  tree->SetBranchAddress("electron_HltDiEle33CaloIdLMWPMS2UnseededFilter", &electron_HltDiEle33CaloIdLMWPMS2UnseededFilter);
  tree->SetBranchAddress("electron_heepId", &electron_heepId);
  tree->SetBranchAddress("electron_HoverEM",&electron_HoverEM);
  tree->SetBranchAddress("electron_dxy",&electron_dxy);
  tree->SetBranchAddress("electron_dB",&electron_dB);
  tree->SetBranchAddress("electron_dxyError",&electron_dxyError);
  tree->SetBranchAddress("electron_pfIso",&electron_pfIso);
  tree->SetBranchAddress("electron_GsfEleEmHadD1IsoRhoCut",&electron_GsfEleEmHadD1IsoRhoCut);
  tree->SetBranchAddress("electron_GsftrkIso",&electron_GsftrkIso);
  tree->SetBranchAddress("electron_trkIso",&electron_trkIso);
  tree->SetBranchAddress("electron_satCrystal",&electron_satCrystal);
  tree->SetBranchAddress("electron_heepBitmap",&electron_heepBitmap);

  tree->SetBranchAddress("muons_p4", &muons_p4);
  tree->SetBranchAddress("jets_p4", &jets_p4);
  tree->SetBranchAddress("genps_p4", &genps_p4);

  tree->SetBranchAddress("jec_uncertainty", &jec_uncertainty);
  tree->SetBranchAddress("jetResolution", &jetResolution);
  tree->SetBranchAddress("JER_sf", &JER_sf);
  tree->SetBranchAddress("JER_sf_up", &JER_sf_up);
  tree->SetBranchAddress("JER_sf_down", &JER_sf_down);
  tree->SetBranchAddress("genJetPt", &genJetPt);
  tree->SetBranchAddress("genJetMatch", &genJetMatch);

  tree->SetBranchAddress("muon_charge", &muon_charge);
  tree->SetBranchAddress("muon_IDSF_central", &muon_IDSF_central);
  tree->SetBranchAddress("muon_IsoSF_central", &muon_IsoSF_central);
  tree->SetBranchAddress("muon_TrigSF_central", &muon_TrigSF_central);
  tree->SetBranchAddress("muon_IDSF_error", &muon_IDSF_error);
  tree->SetBranchAddress("muon_IsoSF_error", &muon_IsoSF_error);
  tree->SetBranchAddress("muon_TrigSF_error", &muon_TrigSF_error);
  tree->SetBranchAddress("muon_trackerLayersWithMeasurement", &muon_trackerLayersWithMeasurement);
  tree->SetBranchAddress("muon_trkIso",&muon_trkIso);
  tree->SetBranchAddress("muon_ptRel",&muon_ptRel);
  tree->SetBranchAddress("muon_muonChambers",&muon_muonChambers);
  tree->SetBranchAddress("muon_matchedStations",&muon_matchedStations);
  tree->SetBranchAddress("muon_dxy",&muon_dxy);
  tree->SetBranchAddress("muon_dxyError",&muon_dxyError);
  tree->SetBranchAddress("muon_pixelHits",&muon_pixelHits);
  tree->SetBranchAddress("muon_stripHits",&muon_stripHits);

  tree->SetBranchAddress("genps_pdgId", &genps_pdgId);
  tree->SetBranchAddress("genps_status", &genps_status);
  tree->SetBranchAddress("genps_motherpdgId", &genps_motherpdgId);


  tree->SetBranchAddress("nPU", &nPU);
  tree->SetBranchAddress("nPV", &nPV);
  tree->SetBranchAddress("weight", &weight);
  tree->SetBranchAddress("RF_weights", &RF_weights);
	
  tree->SetBranchAddress("PU_reweight", &PU_reweight);

}
