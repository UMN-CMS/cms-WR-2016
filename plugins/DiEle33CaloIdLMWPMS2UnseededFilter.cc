// -*- C++ -*-
//
// Package:    ExoAnalysis/DiEle33CaloIdLMWPMS2UnseededFilter
// Class:      DiEle33CaloIdLMWPMS2UnseededFilter
// 
/**\class DiEle33CaloIdLMWPMS2UnseededFilter DiEle33CaloIdLMWPMS2UnseededFilter.cc ExoAnalysis/DiEle33CaloIdLMWPMS2UnseededFilter/plugins/DiEle33CaloIdLMWPMS2UnseededFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andrew Christopher Evans
//         Created:  Fri, 10 Mar 2017 15:50:07 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

//
// class declaration
//

class DiEle33CaloIdLMWPMS2UnseededFilter : public edm::stream::EDFilter<> {
   public:
      explicit DiEle33CaloIdLMWPMS2UnseededFilter(const edm::ParameterSet&);
      ~DiEle33CaloIdLMWPMS2UnseededFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      // ----------member data ---------------------------
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
DiEle33CaloIdLMWPMS2UnseededFilter::DiEle33CaloIdLMWPMS2UnseededFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

}


DiEle33CaloIdLMWPMS2UnseededFilter::~DiEle33CaloIdLMWPMS2UnseededFilter()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
DiEle33CaloIdLMWPMS2UnseededFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   return true;
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
DiEle33CaloIdLMWPMS2UnseededFilter::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
DiEle33CaloIdLMWPMS2UnseededFilter::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
DiEle33CaloIdLMWPMS2UnseededFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
DiEle33CaloIdLMWPMS2UnseededFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
DiEle33CaloIdLMWPMS2UnseededFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
DiEle33CaloIdLMWPMS2UnseededFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DiEle33CaloIdLMWPMS2UnseededFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(DiEle33CaloIdLMWPMS2UnseededFilter);
