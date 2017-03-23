#include<fstream>
#include "TH1F.h"
#include "TH2F.h"
#include "TH3D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TRandom.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "ExoAnalysis/cmsWR/interface/ToyThrower.h"
//#include "ExoAnalysis/cmsWR/interface/RoccoR.h"
//#include "ExoAnalysis/cmsWR/src/Rochcor.cc"
#include <iostream>

float dR_TLv(TLorentzVector t1, TLorentzVector t2)
{
  return deltaR(t1.Eta(), t1.Phi(), t2.Eta(), t2.Phi());
}

void ToyThrower(miniTreeEvent *myEvent,  float rand_smear[], float rand_up_down[], int random_seed, std::vector<std::string> list, bool isData, RoccoR rc)
{

  int Iterator = 0;
  int Iterator_Up_Down = 0;
  int Flag_Smear_Muon_Scale = 0, Flag_Smear_Muon_ID_Iso_Trig = 0, Flag_Smear_Muon_Resolution = 0, Flag_Smear_Jet_Scale = 0, Flag_Smear_Jet_Resolution = 0, Flag_Smear_Elec_Scale = 0;
  double Smear_ID = rand_up_down[Iterator_Up_Down++];
  double Smear_ISO = rand_up_down[Iterator_Up_Down++];
  double Smear_TRIG = rand_up_down[Iterator_Up_Down++];
  double Smear_Res = rand_up_down[Iterator_Up_Down++];
  double Smear_Jet_Scale = rand_up_down[Iterator_Up_Down++];
  double Smear_Elec_Scale = rand_up_down[Iterator_Up_Down++];

  for(unsigned int iii = 0; iii < list.size(); iii++) {
    if(list[iii] == "Smear_Muon_Scale")     Flag_Smear_Muon_Scale = 1;
    else if(list[iii] == "Smear_Muon_ID_Iso_Trig")    Flag_Smear_Muon_ID_Iso_Trig = 1;
    else if(list[iii] == "Smear_Jet_Scale")      Flag_Smear_Jet_Scale = 1;
    else if(list[iii] == "Smear_Muon_Resolution")      Flag_Smear_Muon_Resolution = 1;
    else if(list[iii] == "Smear_Elec_Scale")      Flag_Smear_Elec_Scale = 1;
  }

  //rochcor2016 *rmcor = new rochcor2016(random_seed);
  //RoccoR rc("data/rcdata.2016.v3");
 	
  for(auto muons : * (myEvent->muons_p4)) {
#ifdef DEBUG
    std::cout << std::endl << " Muon number= " << Iterator << " Muon Pt Before = " << (*(myEvent->muons_p4))[Iterator].Pt() << " Muon Eta Before = " << (*(myEvent->muons_p4))[Iterator].Eta() << std::endl;
#endif

    if(Flag_Smear_Muon_ID_Iso_Trig && !isData ) {
      if(Smear_ID >= 0.) (*(myEvent->muon_IDSF_central))[Iterator] += Smear_ID * (*(myEvent->muon_IDSF_error))[Iterator];
      else            (*(myEvent->muon_IDSF_central))[Iterator] -= Smear_ID * (*(myEvent->muon_IDSF_error))[Iterator];

      if(Smear_ISO >= 0.) (*(myEvent->muon_IsoSF_central))[Iterator] += Smear_ISO * (*(myEvent->muon_IsoSF_error))[Iterator];
      else            (*(myEvent->muon_IsoSF_central))[Iterator] -= Smear_ISO * (*(myEvent->muon_IsoSF_error))[Iterator];

      if(Smear_TRIG >= 0.) (*(myEvent->muon_TrigSF_central))[Iterator] += Smear_TRIG * (*(myEvent->muon_TrigSF_error))[Iterator];
      else            (*(myEvent->muon_TrigSF_central))[Iterator] -= Smear_TRIG * (*(myEvent->muon_TrigSF_error))[Iterator];
    }

    float gen_mu_pt = 0.0;
    
    for(unsigned int i =0; i< (myEvent->genps_p4)->size(); i++) {
      if(abs((myEvent->genps_pdgId)->at(i)) == 13 && dR_TLv(((myEvent->genps_p4)->at(i)),(*(myEvent->muons_p4))[Iterator]) < 0.2 )
	gen_mu_pt = ((myEvent->genps_p4)->at(i)).Pt();
    }

    TRandom *tr = new TRandom();
    tr->SetSeed(random_seed);
    
    float u1 = tr->Rndm();
    float u2 = tr->Rndm();
    int nl = (*(myEvent->muon_trackerLayersWithMeasurement))[Iterator];
            
    if(Flag_Smear_Muon_Scale) {

      if(isData) {
	double dataSF = rc.kScaleDT((*(myEvent->muon_charge))[Iterator], (*(myEvent->muons_p4))[Iterator].Pt(), (*(myEvent->muons_p4))[Iterator].Eta(), (*(myEvent->muons_p4))[Iterator].Phi(), 0, 0);
	(*(myEvent->muons_p4))[Iterator].SetPtEtaPhiM((*(myEvent->muons_p4))[Iterator].Pt()*dataSF, (*(myEvent->muons_p4))[Iterator].Eta(), (*(myEvent->muons_p4))[Iterator].Phi(), (*(myEvent->muons_p4))[Iterator].M());
      }      	
      else {
	double mcSF = 1.0;
	if (gen_mu_pt != 0)
	  mcSF = rc.kScaleFromGenMC((*(myEvent->muon_charge))[Iterator], (*(myEvent->muons_p4))[Iterator].Pt(), (*(myEvent->muons_p4))[Iterator].Eta(), (*(myEvent->muons_p4))[Iterator].Phi(), nl, gen_mu_pt, u1, 0, 0);
	else
	  mcSF = rc.kScaleAndSmearMC((*(myEvent->muon_charge))[Iterator], (*(myEvent->muons_p4))[Iterator].Pt(), (*(myEvent->muons_p4))[Iterator].Eta(), (*(myEvent->muons_p4))[Iterator].Phi(), nl, u1, u2, 0, 0);
	
	(*(myEvent->muons_p4))[Iterator].SetPtEtaPhiM((*(myEvent->muons_p4))[Iterator].Pt()*mcSF, (*(myEvent->muons_p4))[Iterator].Eta(), (*(myEvent->muons_p4))[Iterator].Phi(), (*(myEvent->muons_p4))[Iterator].M());
      }
    }

    if(Flag_Smear_Muon_Resolution) {
      float smeared_pt = (*(myEvent->muons_p4))[Iterator].Pt();
      if(Smear_Res >= 0.)
	smeared_pt += Smear_Res * 0.04 * smeared_pt;
      else smeared_pt -= Smear_Res * 0.04 * smeared_pt;

      (*(myEvent->muons_p4))[Iterator].SetPtEtaPhiM(smeared_pt, (*(myEvent->muons_p4))[Iterator].Eta(), (*(myEvent->muons_p4))[Iterator].Phi(), (*(myEvent->muons_p4))[Iterator].M());

    }

#ifdef DEBUG
    std::cout << std::endl << " Muon number= " << Iterator << " Muon Pt After = " << (*(myEvent->muons_p4))[Iterator].Pt() << " Muon Eta After = " << (*(myEvent->muons_p4))[Iterator].Eta() << std::endl;
#endif
    Iterator++;
  }

  Iterator = 0;
  for(auto jets : * (myEvent->jets_p4)) {
#ifdef DEBUG
    std::cout << std::endl << " Jet number= " << Iterator << " " << (*(myEvent->jec_uncertainty))[Iterator] << " " << Smear << " Jet Pt Before = " << (*(myEvent->jets_p4))[Iterator].Pt() << " Jet Eta Before = " << (*(myEvent->jets_p4))[Iterator].Eta() << std::endl;
#endif

    if(Flag_Smear_Jet_Scale)
      (*(myEvent->jets_p4))[Iterator] = (1 + (Smear_Jet_Scale) * (*(myEvent->jec_uncertainty))[Iterator]) * (*(myEvent->jets_p4))[Iterator];
#ifdef DEBUG
    std::cout << std::endl << " Jet number= " << Iterator << " Jet Pt Before = " << (*(myEvent->jets_p4))[Iterator].Pt() << " Jet Eta After = " << (*(myEvent->jets_p4))[Iterator].Eta() << std::endl;
#endif


    if(Flag_Smear_Jet_Resolution) {
      float smeared_jet_pt = (*(myEvent->jets_p4))[Iterator].Pt();
      if(Smear_Res >= 0.)
	smeared_jet_pt += Smear_Res * 0.01 * smeared_jet_pt;
      else smeared_jet_pt -= Smear_Res * 0.01 * smeared_jet_pt;
      
      (*(myEvent->jets_p4))[Iterator].SetPtEtaPhiM(smeared_jet_pt, (*(myEvent->jets_p4))[Iterator].Eta(), (*(myEvent->jets_p4))[Iterator].Phi(), (*(myEvent->jets_p4))[Iterator].M());

    }
    
    Iterator++;
  }
  Iterator = 0;
  for(auto electrons : * (myEvent->electrons_p4)) {
#ifdef DEBUG
    std::cout << std::endl << " Electron number= " << Iterator << " " << (*(myEvent->electron_scale_error))[Iterator] << " Electron Pt Before = " << (*(myEvent->electrons_p4))[Iterator].Pt() << " Electron Eta Before = " << (*(myEvent->electronss_p4))[Iterator].Eta() << std::endl;
#endif
    if(Flag_Smear_Elec_Scale)
      (*(myEvent->electrons_p4))[Iterator] = (1 + (Smear_Elec_Scale) * (*(myEvent->electron_scale_error))[Iterator]) * (*(myEvent->electrons_p4))[Iterator];
#ifdef DEBUG
    std::cout << std::endl << " Electron number= " << Iterator << " " << (*(myEvent->electron_scale_error))[Iterator] << " Electron Pt After = " << (*(myEvent->electrons_p4))[Iterator].Pt() << " Electron Eta After = " << (*(myEvent->electronss_p4))[Iterator].Eta() << std::endl;
#endif
  }
  
}

