
// -*- C++ -*-
//
// Package:    ExoAnalysis/cmsWR
// Class:      cutResultTree
//
/**\class cutResultTree cutResultTree.cc ExoAnalysis/cmsWR/plugins/cutResultTree.cc
 Description: Test analyzer to demonstrate use of heepID
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Peter Hansen
//         Created:  Mon Jul 20 16:03:38 2015
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/VIDCutFlowResult.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TString.h"
#include "Math/VectorUtil.h"

class cutResultTree : public edm::EDAnalyzer
{
public:
	explicit cutResultTree(const edm::ParameterSet&);
	~cutResultTree();

	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
	virtual void beginJob() override;
	virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
	virtual void endJob() override;

	//virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
	//virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
	//virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
	//virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

	// ----------member data ---------------------------

	// MiniAOD case data members
	edm::EDGetToken m_electronsMiniAODToken;

	// ID decisions objects
	edm::EDGetTokenT<edm::ValueMap<bool> > m_eleHEEPIdMapToken;
	edm::EDGetTokenT<edm::ValueMap<vid::CutFlowResult> > m_eleHEEPIdMapCFRToken;


	TTree * m_cutResultTree;
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
cutResultTree::cutResultTree(const edm::ParameterSet& iConfig):
	m_eleHEEPIdMapToken(consumes<edm::ValueMap<bool> >(iConfig.getParameter<edm::InputTag>("eleHEEPIdMap"))),
	m_eleHEEPIdMapCFRToken(consumes<edm::ValueMap<vid::CutFlowResult> >(iConfig.getParameter<edm::InputTag>("eleHEEPIdMap")))
{

	//
	// Prepare tokens for all input collections and objects
	//
	// MiniAOD tokens
	// For electrons, use the fact that pat::Electron can be cast into
	// GsfElectron
	m_electronsMiniAODToken    = mayConsume<edm::View<reco::GsfElectron> >
	                             (iConfig.getParameter<edm::InputTag>
	                              ("electrons"));

	edm::Service<TFileService> fs;
	m_cutResultTree = fs->make<TTree>("CutFlowResultTree", "Tree with decisions and values of the VIDCut");
}


cutResultTree::~cutResultTree()
{

	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
cutResultTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	// Retrieve the collection of electrons from the event.
	edm::Handle<edm::View<reco::GsfElectron> > electrons;
	iEvent.getByToken(m_electronsMiniAODToken, electrons);

	// Get the electron ID data from the event stream.
	// Note: this implies that the VID ID modules have been run upstream.
	// If you need more info, check with the EGM group.
	edm::Handle<edm::ValueMap<bool> > heep_id_decisions;
	iEvent.getByToken(m_eleHEEPIdMapToken , heep_id_decisions);

	edm::Handle<edm::ValueMap<vid::CutFlowResult> > heep_id_CFR;
	iEvent.getByToken(m_eleHEEPIdMapCFRToken , heep_id_CFR);

	// Loop over electrons

	Int_t run = iEvent.run();
	Int_t event = iEvent.id().event();

	Int_t nele = 0;
	Float_t eta = 0;

	if(m_cutResultTree->GetBranch("run") == NULL) m_cutResultTree->Branch("run", &run, "run/I");
	else m_cutResultTree->SetBranchAddress("run", &run);

	if(m_cutResultTree->GetBranch("event") == NULL) m_cutResultTree->Branch("event", &event, "event/I");
	else m_cutResultTree->SetBranchAddress("event", &event);

	if(m_cutResultTree->GetBranch("nele") == NULL) m_cutResultTree->Branch("nele", &nele, "nele/I");
	else m_cutResultTree->SetBranchAddress("nele", &nele);

	if(m_cutResultTree->GetBranch("eta") == NULL) m_cutResultTree->Branch("eta", &eta, "eta/F");
	else m_cutResultTree->SetBranchAddress("eta", &eta);


	for (size_t ele_i = 0; ele_i < electrons->size(); ++ele_i) {
		const auto el = electrons->ptrAt(ele_i);
                if (el->pt() < 35) continue;
		nele = ele_i;
		eta = el->eta();
		size_t n_cuts = (*heep_id_CFR)[el].cutFlowSize();
		std::vector<int> results(n_cuts, 0);
		std::vector<float> values(n_cuts, 0);

		for (size_t i = 0; i < n_cuts; i++) {
			TString name = (*heep_id_CFR)[el].getNameAtIndex(i);
			results[i]  = (*heep_id_CFR)[el].getCutResultByIndex(i);
			values[i]   = (*heep_id_CFR)[el].getValueCutUpon(i);

			TString name_res = name + "_result";
			TString name_val = name + "_value";
			
			if(m_cutResultTree->GetBranch(name_res) == NULL) m_cutResultTree->Branch(name_res, results.data() + i, name_res + "/I");
			else m_cutResultTree->SetBranchAddress(name_res, results.data() + i);

			if(m_cutResultTree->GetBranch(name_val) == NULL) m_cutResultTree->Branch(name_val, values.data() + i, name_val + "/F");
			else m_cutResultTree->SetBranchAddress(name_val, values.data() + i );
		}
		m_cutResultTree->Fill();
	}

}


// ------------ method called once each job just before starting event loop  ------------
void
cutResultTree::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
cutResultTree::endJob()
{
}

// ------------ method called when starting to processes a run  ------------
/*
void
cutResultTree::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
cutResultTree::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
cutResultTree::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
cutResultTree::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
cutResultTree::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(cutResultTree);
