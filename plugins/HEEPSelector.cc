
// -*- C++ -*-
//
// Package:    ExoAnalysis/cmsWR
// Class:      HEEPIDSelector
//
/**\class HEEPIDSelector HEEPIDSelector.cc ExoAnalysis/cmsWR/plugins/HEEPIDSelector.cc
   Description: Producer that creats a collection that passes the HEEPID
   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Peter Hansen
//
//         Created:  Thu Aug  6 14:17:04 CDT 2015
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "Math/VectorUtil.h"

class HEEPIDSelector : public edm::EDProducer
{
public:
  explicit HEEPIDSelector(const edm::ParameterSet&);
  ~HEEPIDSelector();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void produce(edm::Event&, const edm::EventSetup&) override;

  // ----------member data ---------------------------

  bool skipHEEP;

  edm::EDGetToken electronsToken_;
  edm::EDGetTokenT<edm::ValueMap<bool> > eleHEEPIdMapToken_;
  edm::EDGetTokenT<edm::ValueMap<float> > eleTrkPtIsoToken_;
  edm::EDGetTokenT<edm::ValueMap<int> > eleNrSaturateIn5x5Token_;
  edm::EDGetTokenT<edm::ValueMap<unsigned int> > vidBitmapToken_;


};

//
// constructors and destructor
//
HEEPIDSelector::HEEPIDSelector(const edm::ParameterSet& iConfig):
  skipHEEP ( iConfig.getParameter<bool>("skipHEEP")),
  //electronsToken_(consumes<reco::GsfElectron > (iConfig.getParameter<edm::InputTag> ("electrons"))),
  eleHEEPIdMapToken_(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleHEEPIdMap")))
  //eleTrkPtIsoToken_(consumes<edm::ValueMap<float> > (iConfig.getParameter<edm::InputTag> ("eleHEEPIdMap2"))),
  //eleNrSaturateIn5x5Token_(consumes<edm::ValueMap<int> > (iConfig.getParameter<edm::InputTag> ("eleHEEPIdMap2")))
  {  
    vidBitmapToken_=consumes<edm::ValueMap<unsigned int> >(iConfig.getParameter<edm::InputTag>("HEEPBitMap"));
    electronsToken_    = mayConsume<edm::View<pat::Electron> >(iConfig.getParameter<edm::InputTag>("electrons"));
    eleTrkPtIsoToken_ = mayConsume<edm::ValueMap<float> >(iConfig.getParameter<edm::InputTag>("eleHEEPIdMap2"));
    eleNrSaturateIn5x5Token_ = mayConsume<edm::ValueMap<int> >(iConfig.getParameter<edm::InputTag>("eleHEEPIdMap3"));
    produces<pat::ElectronCollection>();
  }


HEEPIDSelector::~HEEPIDSelector()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
HEEPIDSelector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // Retrieve the collection of electrons from the event.
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByToken(electronsToken_, electrons);

  // Get the electron ID data from the event stream.
  edm::Handle<edm::ValueMap<bool> > heep_id_decisions;
  iEvent.getByToken(eleHEEPIdMapToken_ , heep_id_decisions);

  edm::Handle<edm::ValueMap<float> > iso_vmap;
  iEvent.getByToken(eleTrkPtIsoToken_ , iso_vmap);

  edm::Handle<edm::ValueMap<int> > sat_vmap;
  iEvent.getByToken(eleNrSaturateIn5x5Token_ , sat_vmap);

  edm::Handle<edm::ValueMap<unsigned int> > vidBitmap;
  iEvent.getByToken(vidBitmapToken_,vidBitmap);

  std::auto_ptr<pat::ElectronCollection> elec_out(new pat::ElectronCollection());

  // Loop over electrons
  
   for (size_t i = 0; i < electrons->size(); ++i) {
     auto el = electrons->ptrAt(i);
     pat::Electron ele = *el;
     ele.addUserInt("heepID",(*heep_id_decisions)[el]);
     ele.addUserInt("heepBitmap",(*vidBitmap)[el]);
     ele.addUserFloat("tkrIso",(*iso_vmap)[el]);
     ele.addUserInt("satCrystals",(*sat_vmap)[el]);
     if ((*heep_id_decisions)[el] || skipHEEP)
       elec_out->push_back(ele);
   }
  iEvent.put(std::move(elec_out));

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HEEPIDSelector::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;
  desc.add<bool>("skipHEEP", bool());
  desc.add<edm::InputTag>("electrons", edm::InputTag("slimmedElectrons"));
  desc.add<edm::InputTag>("eleHEEPIdMap", edm::InputTag("egmGsfElectronIDs:heepElectronID-HEEPV70"));
  desc.add<edm::InputTag>("HEEPBitMap", edm::InputTag(""));
  desc.add<edm::InputTag>("eleHEEPIdMap2", edm::InputTag(""));
  desc.add<edm::InputTag>("eleHEEPIdMap3", edm::InputTag(""));
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HEEPIDSelector);
