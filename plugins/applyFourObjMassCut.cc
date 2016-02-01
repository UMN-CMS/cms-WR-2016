// -*- C++ -*-
//
// 

/**\class applyFourObjMassCut applyFourObjMassCut.cc doubleElectronTracklessTrigger/applyFourObjMassCut/plugins/applyFourObjMassCut.cc
 Description: [one line class summary]


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
#include "DataFormats/Common/interface/OwnVector.h"



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
#include "Math/GenVector/LorentzVector.h"

//#define NOBJ 500
//#define DEBUG

//
// class declaration
//

class applyFourObjMassCut : public edm::EDProducer {
   public:
      explicit applyFourObjMassCut(const edm::ParameterSet&);
      ~applyFourObjMassCut();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	  ///calculate the dilepton mass using two const_iterators to reco::Candidate objects
	  double getDileptonMass(edm::OwnVector<reco::Candidate>::const_iterator& one, edm::OwnVector<reco::Candidate>::const_iterator& two){
		  double mass = TMath::Sqrt( 2*(one->pt())*(two->pt())*( TMath::CosH( (one->eta())-(two->eta()) ) - TMath::Cos( (one->phi())-(two->phi()) ) ) );
		  return mass;
	  }///end getDileptonMass()

	  void findLeadingAndSubleading(edm::OwnVector<reco::Candidate>::const_iterator& first, edm::OwnVector<reco::Candidate>::const_iterator& second, edm::Handle<edm::OwnVector<reco::Candidate> > collection,bool doDileptonMassCut){

#ifdef DEBUG
		  std::cout<<"checking pt of particles in findLeadingAndSubleading fxn"<<std::endl;
#endif

		  if(!doDileptonMassCut){

			  for(edm::OwnVector<reco::Candidate>::const_iterator genIt = collection->begin(); genIt != collection->end(); genIt++){
#ifdef DEBUG
				  std::cout<<"pT = \t"<< genIt->pt() << std::endl;
#endif
				  if(first==collection->end()) first=genIt;
				  else{
					  if(genIt->pt() > first->pt() && deltaR(genIt->eta(), genIt->phi(), first->eta(), first->phi() ) > 0.4 ){
						  second = first;
						  first = genIt;
					  }
					  else if( ( second==collection->end() || genIt->pt() > second->pt() ) && deltaR(genIt->eta(), genIt->phi(), first->eta(), first->phi() ) > 0.4 ) second = genIt;
				  }
			  }//end loop over reco::Candidate collection

		  }///end if(!doDileptonMassCut)

		  if(doDileptonMassCut){

			  for(edm::OwnVector<reco::Candidate>::const_iterator genIt = collection->begin(); genIt != collection->end(); genIt++){
#ifdef DEBUG
				  std::cout<<"pT = \t"<< genIt->pt() << std::endl;
#endif
				  if(first==collection->end()) first=genIt;
				  else{
					  if(genIt->pt() > first->pt() && getDileptonMass(first,genIt) > minDileptonMass_ && deltaR(genIt->eta(), genIt->phi(), first->eta(), first->phi() ) > 0.4 ){
						  second = first;
						  first = genIt;
					  }
					  else if( (second==collection->end() || genIt->pt() > second->pt()) && getDileptonMass(first,genIt) > minDileptonMass_ && deltaR(genIt->eta(), genIt->phi(), first->eta(), first->phi() ) > 0.4 ) second = genIt;
				  }
			  }//end loop over reco::Candidate collection

		  }///end if(doDileptonMassCut)

#ifdef DEBUG
		  std::cout<<"leaving findLeadingAndSubleading fxn"<<std::endl;
#endif

	  }///end findLeadingAndSubleading()


	  /**this fxn checks that the object pointed to by objIt does not already exist
	   * in the collection pointed at by ptrToObjColl
	   * returns false if objIt already exists in the collection pointed to by ptrToObjColl
	   */
	  bool isNotDuplicate(edm::OwnVector<reco::Candidate>::const_iterator & objIt,
			  std::auto_ptr<edm::OwnVector<reco::Candidate> >& ptrToObjColl){
		  if(ptrToObjColl->size()==0) return true;
		  for(unsigned int i=0; i<ptrToObjColl->size(); i++){
#ifdef DEBUG
			  std::cout<<"about to check if reco::Candidate object has already been added to another collection"<<std::endl;
			  std::cout<<"size of other collection = \t"<< ptrToObjColl->size() <<std::endl;
#endif
			  if(objIt->pt()==(*ptrToObjColl)[i].pt() && objIt->eta()==(*ptrToObjColl)[i].eta() 
					  && objIt->phi()==(*ptrToObjColl)[i].phi()) return false;

		  }///end loop over objects in collection pointed to by ptrToObjColl
		  return true;
	  }///end isNotDuplicate()


   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
 
      // ----------member data ---------------------------
	  edm::EDGetTokenT<edm::OwnVector<reco::Candidate> > inputJetsToken_;
	  edm::EDGetTokenT<edm::OwnVector<reco::Candidate> > inputLeptonsToken_;	///< leptons which have passed dilepton mass and earlier cuts

	  std::string outputJetsCollName_;
	  std::string outputLeptonsCollName_;
	  double minFourObjMass_;	///< minimum four obj mass
	  double minDileptonMass_;	///< min dilepton mass

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
applyFourObjMassCut::applyFourObjMassCut(const edm::ParameterSet& iConfig):
	outputJetsCollName_(iConfig.getParameter<std::string>("outputJetsCollectionName")),
	outputLeptonsCollName_(iConfig.getParameter<std::string>("outputLeptonsCollectionName")),
	minFourObjMass_(iConfig.getParameter<double>("minFourObjMassCut")),
	minDileptonMass_(iConfig.getParameter<double>("minDileptonMassCut"))

{
   
   //edm::Service<TFileService> fs;
   
   ///register the input collections	
   inputJetsToken_ = consumes<edm::OwnVector<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("inputJetsCollTag"));
   inputLeptonsToken_ = consumes<edm::OwnVector<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("inputLeptonsCollTag"));

   ///register the collections which are added to the event
   produces<edm::OwnVector<reco::Candidate> >(outputJetsCollName_);
   produces<edm::OwnVector<reco::Candidate> >(outputLeptonsCollName_);

}


applyFourObjMassCut::~applyFourObjMassCut()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
applyFourObjMassCut::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

#ifdef DEBUG
   std::cout<<"entered applyFourObjMassCut produce method"<<std::endl;
#endif

   Handle<edm::OwnVector<reco::Candidate> > inputJetsColl;
   iEvent.getByToken(inputJetsToken_, inputJetsColl);

   Handle<edm::OwnVector<reco::Candidate> > inputLeptonsColl;
   iEvent.getByToken(inputLeptonsToken_, inputLeptonsColl);


#ifdef DEBUG
   std::cout<<"made handles to input collections"<<std::endl;
#endif

   ///make two empty output collection to eventually hold the two hardest jets and leptons, and a pointer to both collections
   std::auto_ptr<edm::OwnVector<reco::Candidate> > outputLeptonObjsColl(new edm::OwnVector<reco::Candidate>());
   std::auto_ptr<edm::OwnVector<reco::Candidate> > outputJetObjsColl(new edm::OwnVector<reco::Candidate>());


   ///find the two highest pT leptons whose dilepton mass is > 200 GeV, and assign const_iterators to these two leptons
   edm::OwnVector<reco::Candidate>::const_iterator leadLepton=inputLeptonsColl->end(), subleadLepton=inputLeptonsColl->end();
   findLeadingAndSubleading(leadLepton, subleadLepton, inputLeptonsColl, true);

   ///find the two highest pT jets in inputJetsColl
   ///as long as the dR producer/filter is run before this producer, all jets in the inputJetsColl handle 
   ///are separated from the two leading leptons
   edm::OwnVector<reco::Candidate>::const_iterator leadJet=inputJetsColl->end(), subleadJet=inputJetsColl->end();
   findLeadingAndSubleading(leadJet, subleadJet, inputJetsColl, false);

   if(leadJet==inputJetsColl->end() || subleadJet==inputJetsColl->end() || leadLepton==inputLeptonsColl->end() || subleadLepton==inputLeptonsColl->end() ){
	   iEvent.put(outputJetObjsColl, outputJetsCollName_);
	   iEvent.put(outputLeptonObjsColl, outputLeptonsCollName_);
	   return;
   }

   ///check if the four object mass (using the selected leptons and jets) is greater than the threshold value
   ///if the four obj mass exceeds the threshold, add the two highest pT leptons and jets to the output collections
   double fourObjMass = (leadJet->p4() + subleadJet->p4() + leadLepton->p4() + subleadLepton->p4()).M();
   if(fourObjMass > minFourObjMass_){
	   outputLeptonObjsColl->push_back(*leadLepton);
	   outputLeptonObjsColl->push_back(*subleadLepton);
	   outputJetObjsColl->push_back(*leadJet);
	   outputJetObjsColl->push_back(*subleadJet);
   }///end filter to check that four obj mass is high enough

#ifdef DEBUG
   std::cout<<"about to put collection of matched reco::Candidate objects into root file"<<std::endl;
#endif
  
   ///now put the collection of matched higher level reco::Candidate objects into the event
   iEvent.put(outputJetObjsColl, outputJetsCollName_);
   iEvent.put(outputLeptonObjsColl, outputLeptonsCollName_);
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
applyFourObjMassCut::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
applyFourObjMassCut::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
applyFourObjMassCut::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
applyFourObjMassCut::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
applyFourObjMassCut::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
applyFourObjMassCut::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
applyFourObjMassCut::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(applyFourObjMassCut);
