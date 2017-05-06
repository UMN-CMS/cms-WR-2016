#ifndef miniTreeEvent_h
#define miniTreeEvent_h
#include <TLorentzVector.h>
#include <TTree.h>
#include <TChain.h>

/** \class miniTreeEvent miniTreeEvent.h miniTreeEvent.cc
 * This class defines the content of one event in the miniTree produced from microAOD
 * There are also methods to read and to write from a TTree the event
 */
class miniTreeEvent
{
 public:

  miniTreeEvent(); ///< default contructor (empty)
  ~miniTreeEvent(); ///< default contructor (empty)
  miniTreeEvent(const miniTreeEvent& otherEvent);

  // public members to be filled by your program
  Int_t run;
  Int_t lumi;
  Long64_t event;
  char datasetName[40]; ///< the main dataset name

  // RunTime
  // UInt_t run_time;

  std::vector<TLorentzVector> * electrons_p4;
  std::vector<Float_t> * electron_scale_error;
  std::vector<Float_t> * electron_smearing_sigma;
  std::vector<Float_t> * electron_smearing_sigma_phi_up;
  std::vector<Float_t> * electron_smearing_sigma_phi_down;
  std::vector<Float_t> * electron_smearing_sigma_rho_up;
  std::vector<Float_t> * electron_smearing_sigma_rho_down;
  std::vector<Float_t> * electron_r9;
  std::vector<Float_t> * electron_SC_eta;	
  std::vector<Int_t> * electron_charge;	
  ///none of these electron central values or errors are stored in the minitrees, so dont try to read them from minitrees
  std::vector<Float_t> * electron_IDSF_central;
  std::vector<Float_t> * electron_IDSF_error;
  std::vector<Float_t> * electron_RecoSF_central;
  std::vector<Float_t> * electron_RecoSF_error;
  std::vector<Float_t> * electron_HltSF_central;
  std::vector<Float_t> * electron_HltSF_error;
  ///end electron central values and errors
  std::vector<bool>    * electron_HltDiEle33CaloIdLMWPMS2UnseededFilter;
  std::vector<bool>    * electron_heepId;
  std::vector<Float_t>    * electron_HoverEM;
  std::vector<Float_t>    * electron_dxy;
  std::vector<Float_t>    * electron_dB;
  std::vector<Float_t>    * electron_dxyError;
  std::vector<Float_t>    * electron_pfIso;
  std::vector<Float_t>    * electron_GsfEleEmHadD1IsoRhoCut;
  std::vector<Float_t>    * electron_GsftrkIso;
  std::vector<Float_t>    * electron_trkIso;	
  std::vector<Int_t>    * electron_satCrystal;	
  std::vector<Int_t>    * electron_heepBitmap;

  std::vector<TLorentzVector> * loose_electrons_p4;
  std::vector<Float_t> * loose_electron_scale_error;
  std::vector<Float_t> * loose_electron_smearing_sigma;
  std::vector<Float_t> * loose_electron_smearing_sigma_phi_up;
  std::vector<Float_t> * loose_electron_smearing_sigma_phi_down;
  std::vector<Float_t> * loose_electron_smearing_sigma_rho_up;
  std::vector<Float_t> * loose_electron_smearing_sigma_rho_down;
  std::vector<Float_t> * loose_electron_r9;
  std::vector<Float_t> * loose_electron_SC_eta;	
  std::vector<Int_t> * loose_electron_charge;	
  ///none of these loose_electron central values or errors are stored in the minitrees, so dont try to read them from minitrees
  std::vector<Float_t> * loose_electron_IDSF_central;
  std::vector<Float_t> * loose_electron_IDSF_error;
  std::vector<Float_t> * loose_electron_RecoSF_central;
  std::vector<Float_t> * loose_electron_RecoSF_error;
  std::vector<Float_t> * loose_electron_HltSF_central;
  std::vector<Float_t> * loose_electron_HltSF_error;
  ///end loose_electron central values and errors
  std::vector<bool>    * loose_electron_HltDiEle33CaloIdLMWPMS2UnseededFilter;
  std::vector<bool>    * loose_electron_heepId;
  std::vector<Float_t>    * loose_electron_HoverEM;
  std::vector<Float_t>    * loose_electron_dxy;
  std::vector<Float_t>    * loose_electron_dB;
  std::vector<Float_t>    * loose_electron_dxyError;
  std::vector<Float_t>    * loose_electron_pfIso;
  std::vector<Float_t>    * loose_electron_GsfEleEmHadD1IsoRhoCut;
  std::vector<Float_t>    * loose_electron_GsftrkIso;
  std::vector<Float_t>    * loose_electron_trkIso;	
  std::vector<Int_t>    * loose_electron_satCrystal;	
  std::vector<Int_t>    * loose_electron_heepBitmap;

  std::vector<TLorentzVector> * muons_p4;
  std::vector<Int_t> * muon_charge;
  std::vector<Float_t> * muon_IDSF_central;
  std::vector<Float_t> * muon_IsoSF_central;
  std::vector<Float_t> * muon_TrigSF_central;
  std::vector<Float_t> * muon_IDSF_error;
  std::vector<Float_t> * muon_IsoSF_error;
  std::vector<Float_t> * muon_TrigSF_error;
  //////////
  std::vector<Float_t> * muon_IDSF_central2;
  std::vector<Float_t> * muon_IsoSF_central2;
  std::vector<Float_t> * muon_TrigSF_central2;
  std::vector<Float_t> * muon_IDSF_error2;
  std::vector<Float_t> * muon_IsoSF_error2;
  std::vector<Float_t> * muon_TrigSF_error2;
  //////////
  std::vector<Float_t> * muon_trackerLayersWithMeasurement;
  std::vector<Float_t> * muon_trkIso;
  std::vector<Float_t> * muon_ptRel;
  std::vector<Float_t> * muon_muonChambers;
  std::vector<Float_t> * muon_matchedStations;
  std::vector<Float_t> * muon_dxy;
  std::vector<Float_t> * muon_dxyError;
  std::vector<Float_t> * muon_pixelHits;
  std::vector<Float_t> * muon_stripHits;

  std::vector<TLorentzVector> * jets_p4;
  std::vector<Float_t> * jec_uncertainty;
  std::vector<Float_t> * jetResolution;
  std::vector<Float_t> * JER_sf;
  std::vector<Float_t> * JER_sf_up;
  std::vector<Float_t> * JER_sf_down;
  std::vector<Float_t> * genJetPt;
  std::vector<bool> * genJetMatch;

  std::vector<TLorentzVector> * genps_p4;
  std::vector<Int_t> * genps_pdgId;
  std::vector<Int_t> * genps_status;
  std::vector<Int_t> * genps_motherpdgId;
	
  Float_t nPU;
  Int_t nPV;
  Float_t weight;
  std::vector<Float_t> * RF_weights;
  Float_t PU_reweight;

  void clear();
  void SetBranches(TTree* tree);
  void SetBranchAddresses(TChain* tree);

 private:
  bool _owningMembers;
};


#endif
