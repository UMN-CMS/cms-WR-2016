// -*- C++ -*-
//
// 

/**\class FindHigherLevelMatchedObject FindHigherLevelMatchedObject.cc doubleElectronTracklessTrigger/FindHigherLevelMatchedObject/plugins/FindHigherLevelMatchedObject.cc
 Description: [one line class summary]

 this module takes a collection of reco::CompositeCandidate objects and parses each object into its
 two daughters.  Two vectors of edm::Ref objects pointing to reco::RecoEcalCandidate objects are produced
 and saved to the input .root file.  These two collections can then be used to access kinematic and isolation
 information from the daughters.

 Implementation:
     [Notes on implementation]
*/
//
//
//

// system include files
#include <memory>
#include <map>
#include <utility>
#include <cstring>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <array>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/getRef.h"


#include "DataFormats/EgammaReco/interface/SuperCluster.h"
//#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaReco/interface/ClusterShapeFwd.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
//#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"


#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TAttFill.h"
#include "TAttMarker.h"
#include <TString.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TROOT.h>
#include "TTree.h"


//
// class declaration
//

class FindHigherLevelMatchedObject : public edm::EDProducer {
   public:
      explicit FindHigherLevelMatchedObject(const edm::ParameterSet&);
      ~FindHigherLevelMatchedObject();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
 
      // ----------member data ---------------------------
	  /*
	  edm::EDGetTokenT<std::vector<reco::CompositeCandidate>> momToken;
	  edm::EDGetTokenT<std::vector<reco::RecoEcalCandidate>> momParentOneToken;
      edm::EDGetTokenT<std::vector<reco::RecoEcalCandidate>> momParentTwoToken;
	
   	  std::string daughterOneCollection;
	  std::string daughterTwoCollection;
	  */
 	  
	  ///the point of this producer is to match an object from the lowLevel collection with
	  ///an object in the higherLevel collection.  An example of this could be gen quarks
	  ///(from "genParticles" with |pdgId| < 7), as lowLevel objects, and genJets as higherLevel
	  ///objects.  The matching is done purely by deltaR.  The closest dR match wins!
	  ///
	  ///This producer adds a collection of reco::Candidate object pointers to each event.
	  edm::EDGetTokenT<edm::OwnVector<reco::Candidate> > lowLevelToken;
	  edm::EDGetTokenT<edm::OwnVector<reco::Candidate> > higherLevelToken;

	  std::string matchedHigherLevelCollection;
	
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
FindHigherLevelMatchedObject::FindHigherLevelMatchedObject(const edm::ParameterSet& iConfig):
	matchedHigherLevelCollection(iConfig.getParameter<std::string>("matchedHigherLevelCollectionName"))
{
   //register your products
 
   lowLevelToken = consumes<edm::OwnVector<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("lowLevelCollName"));
   higherLevelToken = consumes<edm::OwnVector<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("higherLevelCollName"));

   /*	
   momToken = consumes<std::vector<reco::CompositeCandidate>>(iConfig.getParameter<edm::InputTag>("zedLabel"));
   momParentOneToken = consumes<std::vector<reco::RecoEcalCandidate>>(iConfig.getParameter<edm::InputTag>("tracklessHltEle"));
   momParentTwoToken = consumes<std::vector<reco::RecoEcalCandidate>>(iConfig.getParameter<edm::InputTag>("trackedHltEle"));
   //daughterOneCollection = iConfig.getParameter<std::string>("tracklessEleCollectionName");
   //daughterTwoCollection = iConfig.getParameter<std::string>("trackedEleCollectionName");
   
   //register the two collections of products - std::vector<edm::Refs to RecoEcalCandidate objects> (daughters of Z)
   produces<reco::RecoEcalCandidateRefVector>(daughterOneCollection);
   produces<reco::RecoEcalCandidateRefVector>(daughterTwoCollection);
   */

}


FindHigherLevelMatchedObject::~FindHigherLevelMatchedObject()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
FindHigherLevelMatchedObject::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<edm::OwnVector<reco::Candidate> > lowLevelObjectColl;
   iEvent.getByToken(lowLevelToken, lowLevelObjectColl);

   /*
   //std::cout<<"entered daughter producer code"<<std::endl;

   //read collection of reco::CompositeCandidate objects from iEvent
   Handle<std::vector<reco::CompositeCandidate> > momIn;
   iEvent.getByToken(momToken, momIn);

   Handle<std::vector<reco::RecoEcalCandidate> > momParentOneIn;	//for trackless leg RECs
   iEvent.getByToken(momParentOneToken, momParentOneIn);

   Handle<std::vector<reco::RecoEcalCandidate> > momParentTwoIn;	//for tracked leg RECs
   iEvent.getByToken(momParentTwoToken, momParentTwoIn);

   //std::cout<<"made handles to input collections"<<std::endl;

   //create empty output collections, one for each daughter, and pointers to each collection
   std::auto_ptr<reco::RecoEcalCandidateRefVector> daughterOneRefColl(new reco::RecoEcalCandidateRefVector );	//trackless collection
   std::auto_ptr<reco::RecoEcalCandidateRefVector> daughterTwoRefColl(new reco::RecoEcalCandidateRefVector );	//tracked collection


   
   for(std::vector<reco::CompositeCandidate>::const_iterator momIt = momIn->begin(); momIt != momIn->end(); momIt++){
	   //get a Ref to a daughter via momIt->daughter()->masterClone()
	   //then find the matching (pt, eta, phi) object in momParent(One or Two)In handles, and
	   //save a reference to the object into the appropriate output collection via
	   //getRef(momParent(One or Two)In, index number) 
	   //std::cout<<"looping over CompositeCandidate objects"<<std::endl;
	   if((momIt->daughter("tracklessRecoEle"))->hasMasterClone() ){
		   //std::cout<<"found tracklessRecoEle daughter with a master clone"<<std::endl;
		   reco::CandidateBaseRef dauOneRef = (momIt->daughter("tracklessRecoEle"))->masterClone();
		   //std::cout<<"made a reference obj to a trackless daughter"<<std::endl;
		   for(unsigned int h=0; h<momParentOneIn->size(); h++){
			   if(dauOneRef->pt() == (getRef(momParentOneIn, h))->pt() ){
				   if(dauOneRef->eta() == (getRef(momParentOneIn, h))->eta() ){
					   if(dauOneRef->phi() == (getRef(momParentOneIn, h))->phi() ){
						   daughterOneRefColl->push_back( getRef(momParentOneIn, h) );
					   }//end filter on phi

				   }//end filter on eta

			   }//end filter on pt

		   }//end loop over objects in momParentOneIn

	   }//end requirement that a master clone exists

	   if((momIt->daughter("trackedRecoEle"))->hasMasterClone() ){
		   //std::cout<<"found trackedRecoEle daughter with a master clone"<<std::endl;
		   reco::CandidateBaseRef dauTwoRef = (momIt->daughter("trackedRecoEle"))->masterClone();
		   for(unsigned int m=0; m<momParentTwoIn->size(); m++){
			   if(dauTwoRef->pt() == (getRef(momParentTwoIn, m))->pt() ){
				   if(dauTwoRef->eta() == (getRef(momParentTwoIn, m))->eta() ){
					   if(dauTwoRef->phi() == (getRef(momParentTwoIn, m))->phi() ){
						   daughterTwoRefColl->push_back( getRef(momParentTwoIn, m) );
					   }//end filter on phi

				   }//end filter on eta

			   }//end filter on pt

		   }//end loop over objects in momParentTwoIn

	   }//end requirement that a master clone exists
	
   }//end loop over all CompositeCandidate objects in the event


   //std::cout<<"about to put daughter collections into root file"<<std::endl;
   //now put the two collections of Refs to daughter particles into the event
   iEvent.put(daughterOneRefColl, daughterOneCollection);
   iEvent.put(daughterTwoRefColl, daughterTwoCollection);

   */


}

// ------------ method called once each job just before starting event loop  ------------
void 
FindHigherLevelMatchedObject::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
FindHigherLevelMatchedObject::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
FindHigherLevelMatchedObject::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
FindHigherLevelMatchedObject::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
FindHigherLevelMatchedObject::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
FindHigherLevelMatchedObject::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FindHigherLevelMatchedObject::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(FindHigherLevelMatchedObject);
