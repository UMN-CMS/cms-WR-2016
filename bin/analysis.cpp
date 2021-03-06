/**
 * this is the main program for the analysis
 * please document it here
 */

#include "TH1F.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "TRandom3.h"
// #include <iostream>
// // #include "ExoAnalysis/cmsWR/interface/FitRooDataSet.h"

#include "ExoAnalysis/cmsWR/interface/miniTreeEvent.h"
#include "ExoAnalysis/cmsWR/interface/AnalysisResult.h"
#include "ExoAnalysis/cmsWR/interface/Selector.h"
#include "ExoAnalysis/cmsWR/interface/SelectorHist.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <utility>
#include <boost/program_options.hpp>
#include "FitRooDataSet.h"
#include "rooFitFxns.h"
#include "ToyThrower.h"
#include "JetResolution.h"
#include "analysisTools.h"
#include "configReader.h"

#include <unordered_set>
#include <string>
#include <TStopwatch.h>
#define _ENDSTRING std::string::npos
//#define DEBUG
//#define DEBUGG
//process only 50000 events when DEBUGG is defined

/**
	 TT
	 DY TANDP POWHEG AMC MAD POWINCL
	 W
	 WZ
	 ZZ
	 WW
	 SingleTop
	 data TANDP
*/

/** \class chainNames
		\brief this class helps in finding the right tree name based on the sample, sideband and channel one wants to analyze
*/

class chainNames
{

public:
	chainNames(): ///< default constructor
		all_modes(  // list of all possible modes
				{"TT", "TTAMC", "W", "WZ", "ZZ", "WW", "SingleTop", "QCD_1", "QCD_2", "QCD_3", "Other", "data", "DYPOWHEG", "DYMADHT", "DYAMC", "DYAMCPT_1", "DYAMCPT_2", "DYAMCPT_3", "DYAMCPT_4", "DYAMCPT_5", "DYAMCPT_6", "DYAMCPT_7", "DYMAD","DYCOMB", "DYPOWINCL", "signal"
			}
		)
	{
	};

	bool isData(std::string mode)
	{
		if(mode == "data") return true;
		return false;
	}

	std::vector<std::string> getChainNames(std::string mode, Selector::tag_t channel, bool isTagAndProbe)
	{

		std::vector<std::string> TTchainNames;
		if(checkValidMode(mode) == false) {
			std::cerr << "[ERROR]" << std::endl;
			return TTchainNames;
		}
		if(mode == "TT") {
			//TTchainNames.push_back("TTJets_DiLept_v1");
			//TTchainNames.push_back("TTJets_DiLept_v2");
			TTchainNames.push_back("TTJets");
		} else if(mode == "TTAMC") {
			TTchainNames.push_back("TTJets_DiLept_amcatnlo_v1");
			TTchainNames.push_back("TTJets_DiLept_amcatnlo_v2");
		} else if(mode.find("DY") != _ENDSTRING) {
			//if(mode.Contains("TANDP") ) tree_channel = "_dytagandprobe";
			std::string tagName = "";
			if(channel == Selector::EE) tagName = "EE";
			if(channel == Selector::MuMu) tagName = "MuMu";
			if(channel == Selector::EMu) { ///\todo to be fixes, it should be possible
	std::cout << "ERROR looking for DY in EMu channel" << std::endl;
	//return TTchainNames;
			}
			if(mode == "DYAMC") {
	//amc at nlo inclusive sample gen dilepton mass greater than 50 GeV
	TTchainNames.push_back("DYJets_amcatnlo");
			}
			if(mode.find("AMCPT_1") != _ENDSTRING) {
	//amc at nlo pT binned sample gen dilepton mass greater than 50 GeV
	TTchainNames.push_back("DYJets_amcatnlo_pt50_100_v1");
	} if(mode.find("AMCPT_2") != _ENDSTRING) {
	TTchainNames.push_back("DYJets_amcatnlo_pt50_100_v2");
	} if(mode.find("AMCPT_3") != _ENDSTRING) {
	TTchainNames.push_back("DYJets_amcatnlo_pt100_250_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt100_250_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt100_250_v3");
	} if(mode.find("AMCPT_4") != _ENDSTRING) {		
	TTchainNames.push_back("DYJets_amcatnlo_pt100_250_v4");
	} if(mode.find("AMCPT_5") != _ENDSTRING) {		
	TTchainNames.push_back("DYJets_amcatnlo_pt250_400_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt250_400_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt250_400_v3");
	TTchainNames.push_back("DYJets_amcatnlo_pt250_400_v4");
	} if(mode.find("AMCPT_6") != _ENDSTRING) {		
	TTchainNames.push_back("DYJets_amcatnlo_pt400_650_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt400_650_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt400_650_v3");
	} if(mode.find("AMCPT_7") != _ENDSTRING) {		
	TTchainNames.push_back("DYJets_amcatnlo_pt650_Inf_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt650_Inf_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt650_Inf_v3");			
			} else if(mode.find("POWINCL") != _ENDSTRING && channel == Selector::EE) {
	TTchainNames.push_back("DYToEE_powheg");
			} else if(mode.find("MADHT") != _ENDSTRING) {
	TTchainNames.push_back("DYJets_HT_100to200_v1");
	TTchainNames.push_back("DYJets_HT_200to400_v1");
	TTchainNames.push_back("DYJets_HT_400to600_v1");
	TTchainNames.push_back("DYJets_HT_600to800");
	TTchainNames.push_back("DYJets_HT_800to1200");
	TTchainNames.push_back("DYJets_HT_1200to2500");
	TTchainNames.push_back("DYJets_HT_2500toInf");
			}
			else if(mode == "DYCOMB"){
	TTchainNames.push_back("DYJets_amcatnlo_pt50_100_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt50_100_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt100_250_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt100_250_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt100_250_v3");
	TTchainNames.push_back("DYJets_amcatnlo_pt100_250_v4");
	TTchainNames.push_back("DYJets_amcatnlo_pt250_400_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt250_400_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt250_400_v3");
	TTchainNames.push_back("DYJets_amcatnlo_pt250_400_v4");
	TTchainNames.push_back("DYJets_amcatnlo_pt400_650_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt400_650_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt400_650_v3");
	TTchainNames.push_back("DYJets_amcatnlo_pt650_Inf_v1");
	TTchainNames.push_back("DYJets_amcatnlo_pt650_Inf_v2");
	TTchainNames.push_back("DYJets_amcatnlo_pt650_Inf_v3");			
	TTchainNames.push_back("DYJets_HT_100to200_v1");
	TTchainNames.push_back("DYJets_HT_200to400_v1");
	TTchainNames.push_back("DYJets_HT_400to600_v1");
	TTchainNames.push_back("DYJets_HT_600to800");
	TTchainNames.push_back("DYJets_HT_800to1200");
	TTchainNames.push_back("DYJets_HT_1200to2500");
	TTchainNames.push_back("DYJets_HT_2500toInf");
			}
		} else if(mode == "W") {
			TTchainNames.push_back("WJetsLNu");
		} else if(mode == "WZ") {
			TTchainNames.push_back("WZ");
		} else if(mode == "ZZ") {
			TTchainNames.push_back("ZZ");
		} else if(mode == "WW") {
			TTchainNames.push_back("WW");
		} else if(mode == "QCD_1") {
			TTchainNames.push_back("QCD_EMEnriched_20_30_v1");
			TTchainNames.push_back("QCD_EMEnriched_30_50_v1");
			TTchainNames.push_back("QCD_EMEnriched_30_50_v2");
			TTchainNames.push_back("QCD_EMEnriched_50_80_v1");
			TTchainNames.push_back("QCD_EMEnriched_50_80_v2");
			TTchainNames.push_back("QCD_EMEnriched_80_120_v1");
			TTchainNames.push_back("QCD_EMEnriched_80_120_v2");
			TTchainNames.push_back("QCD_EMEnriched_120_170_v1");
			TTchainNames.push_back("QCD_EMEnriched_120_170_v2");
			TTchainNames.push_back("QCD_EMEnriched_170_300_v1");
			TTchainNames.push_back("QCD_EMEnriched_300_Inf_v1");
			TTchainNames.push_back("QCD_bcToE_15_20");
			TTchainNames.push_back("QCD_bcToE_20_30");
			TTchainNames.push_back("QCD_bcToE_30_80");
			TTchainNames.push_back("QCD_bcToE_80_170");
			TTchainNames.push_back("QCD_bcToE_170_250");
			TTchainNames.push_back("QCD_bcToE_250_Inf");
		} else if(mode == "QCD_2") {
			TTchainNames.push_back("QCD_MuEnrichedPt15");
		} else if(mode == "QCD_3") {
			//TTchainNames.push_back("QCD_EMEnriched_30_Inf_v1");
			TTchainNames.push_back("QCD_EMEnriched_30_40_v1");
			TTchainNames.push_back("QCD_EMEnriched_40_Inf_v1");
		} else if(mode == "SingleTop") {
			TTchainNames.push_back("SingleTop_TTWLL");
			TTchainNames.push_back("SingleTop_TWNuNu");
			//TTchainNames.push_back("SingleTop_tbar");
			TTchainNames.push_back("SingleTop_tbarinc");
			//TTchainNames.push_back("SingleTop_t");
			TTchainNames.push_back("SingleTop_tinc");
		} else if(mode == "Other") {
			TTchainNames.push_back("SingleTop_TTWLL");
			TTchainNames.push_back("SingleTop_TWNuNu");
			TTchainNames.push_back("SingleTop_tbarinc");
			TTchainNames.push_back("SingleTop_tinc");
			TTchainNames.push_back("WW");
			TTchainNames.push_back("WZ");
			TTchainNames.push_back("ZZ");
			TTchainNames.push_back("WJetsLNu");						
		} else if(mode == "data") {
			std::string dataTag = "";
			//if(channel == Selector::EMu)  dataTag = "MuEG";
			if(channel == Selector::EMu)  dataTag = "SingleMu";
			if(channel == Selector::EE)   dataTag = "DoubleEG";
			if(channel == Selector::MuMu) dataTag = "SingleMu";
			TTchainNames.push_back(dataTag + "_RunB_v3");
			TTchainNames.push_back(dataTag + "_RunC");
			TTchainNames.push_back(dataTag + "_RunD");
			TTchainNames.push_back(dataTag + "_RunE");
			TTchainNames.push_back(dataTag + "_RunF");
			TTchainNames.push_back(dataTag + "_RunG");
			TTchainNames.push_back(dataTag + "_RunH_v2");
			TTchainNames.push_back(dataTag + "_RunH_v3");
		}
		if(mode.find("WRto") != _ENDSTRING) {
			TTchainNames.push_back(mode);
		}
		return TTchainNames;
	};


	std::string getTreeName(Selector::tag_t channel, bool isTagAndProbe, bool isLowDiLepton, bool isQCD)
	{
		std::string tree_channel = "";

		// Select the channel to be studied //
		if(isQCD)
			tree_channel = "_qcd";
		else if(isLowDiLepton && channel != Selector::EMu)
			tree_channel = "_lowdileptonsideband";
		else if(isTagAndProbe)
			tree_channel = "_dytagandprobe";
		else if(channel == Selector::EE)
			tree_channel = "_signal_ee";
		else if(channel == Selector::MuMu)
			tree_channel = "_signal_mumu";
		else if(channel == Selector::EMu)
			tree_channel = "_flavoursideband";
		else {
			tree_channel = "";
			std::cerr << "[ERROR] No channel defined" << std::endl;
		}

		return  tree_channel;
	};

	bool checkValidMode(std::string mode)
	{
		if(mode.find("WRto") != _ENDSTRING) {
			return true;
		}

		if(all_modes.count(mode) == 0) {
			std::cerr << "[ERROR] Mode " << mode << " not part of the standard modes:" << std::endl;
			for(auto allowed_mode : all_modes) std::cerr << "        " << allowed_mode << std::endl;
			return false;
		}
		return true;
	};

private:
	std::unordered_set<std::string> all_modes;

};




int main(int ac, char* av[])
{
	namespace po = boost::program_options;

	std::vector<std::string> modes;
	chainNames chainNames_;

	std::string channel_str, outDir, outFileTag;
	std::string channel_cut_str;
	float integratedLumi;
	Int_t nToys;
	bool debug;
	bool isTagAndProbe, isLowDiLepton, saveToys, ignoreDyScaleFactors, isLowFOM, isQCD;
	int nStatToys;
	int signalN;
	int seed;
	bool makeSelectorPlots;
	// Declare the supported options.
	po::options_description required("Mandatory command line options");
	required.add_options()
		("mode,m", po::value<std::vector<std::string> >(&modes)->required(), "Set mode to use:\n")
		("channel,c", po::value<std::string>(&channel_str)->required(), "Set Channel (EE, MuMu, EMu)")
		;

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("lumi,l", po::value<float>(&integratedLumi)->default_value(35867), "Integrated luminosity")
		("toys,t", po::value<int>(&nToys)->default_value(0), "Number of Toys")
		("seed,s", po::value<int>(&seed)->default_value(123456), "Starting seed")
		("saveToys", po::bool_switch(&saveToys)->default_value(false), "Save t1 tree vector for every toy iteration")
		("outputDir,d", po::value<std::string>(&outDir)->default_value(""), "output dir for file with plotting trees")
		("outputFileTag,f", po::value<std::string>(&outFileTag)->default_value(""), "tag name added to output file with plotting trees")
		("ignoreDYSF", po::bool_switch(&ignoreDyScaleFactors)->default_value(false), "Ignore DyScaleFactors defined in configs directory")
		("verbose,v", po::bool_switch(&debug)->default_value(false), "Turn on debug statements")
		("isTagAndProbe", po::bool_switch(&isTagAndProbe)->default_value(false), "use the tag&probe tree variants")
		("isLowDilepton", po::bool_switch(&isLowDiLepton)->default_value(false), "low di-lepton sideband")
		("isLowFOM", po::bool_switch(&isLowFOM)->default_value(false), "low four-object sideband")
		("isQCD", po::bool_switch(&isQCD)->default_value(false), "QCD tree")
		("nStatToys", po::value<int>(&nStatToys)->default_value(0), "throw N toys for stat uncertainty.")
		("signalN", po::value<int>(&signalN)->default_value(0), "pick one signal mass to process")
		("makeSelectorPlots", po::bool_switch(&makeSelectorPlots)->default_value(false), "Turn on plot making in Selector")
		("cut_channel", po::value<std::string>(&channel_cut_str)->default_value(""), "if channel is EMu choose which Mass cut to apply")
		;

	po::variables_map vm;
	po::options_description all("all");
	all.add(desc).add(required);
	po::store(po::parse_command_line(ac, av, all), vm);


	if (vm.count("help")) {
		std::cout << required << "\n";
		std::cout << desc << "\n";
		return 1;
	}

	try {
		po::notify(vm);
	} catch(const po::required_option & e) {
		std::cerr << "[ERROR] "  << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return 1;
	}


	//EnergyScaleCorrection_class eSmearer("ExoAnalysis/Calibration/ZFitter/data/scales_smearings/74X_Prompt_2015");

	//------------------------------ check if modes given in the command line are allowed
	for(auto s : modes ) {
		if(chainNames_.checkValidMode(s) == false) return 1;
	}


	//------------------------------ translate the channel option into the selector type
	std::cout<<"CHANNEL="<<channel_str<<std::endl;
	Selector::tag_t channel = Selector::getTag(channel_str);

	Selector::tag_t cut_channel;
	if(channel == Selector::EMu) {
		//cut_channel = Selector::getTag(channel_cut_str);
		outFileTag += channel_cut_str;
	} else
		cut_channel = channel;

	configReader myReader("configs/2016-v2.conf");
	if(debug) std::cout << myReader << std::endl;
		
	std::cout << "[INFO] Selected modes: \n";
	unsigned int msize = modes.size();
	modes.erase( std::remove( modes.begin(), modes.end(), "signal" ), modes.end() );
	if(modes.size() != msize) {
		int di = 0;
		for(std::string datasetName : myReader.getDatasetNames()) {
			if(datasetName.find("WRto" + channel_str + "JJ_") != _ENDSTRING) {
	di++;
	if(!signalN || signalN == di)
		modes.push_back(datasetName);
			}
		}
	}
	for(auto s : modes) {
		std::cout << "       - " << s << "\n";
	}
	std::cout << std::endl;

	char name[100];// to name the tree per iteration of the Toy
	using namespace RooFit;
	std::cout << "******************************* Analysis ******************************" << std::endl;
	std::cout << "[WARNING] no weights associated to jets yet" << std::endl;

	//myReader.setupDyMllScaleFactor("configs/dyScaleFactors.txt");

	//std::cout << "DY SCALE = "<<myReader.DYScale(channel)<<std::endl;

	
	std::map< std::pair<Selector::tag_t,  int>, std::pair<int, int> > mass_cut = getMassCutMap();
	std::vector<int> mass_vec = getMassVec();
	TString dataPUfn = "data/MyDataPileupHistogram.root";
	std::map<float, double> pu_weights = PUreweight(dataPUfn);

	std::string treeName = "miniTree" + chainNames_.getTreeName(channel, isTagAndProbe, isLowDiLepton, isQCD);
	unsigned long long zMass60to120EvtCount = 0;	///<count the number of evts from each dataset with 60 < dilepton_mass < 120 which pass loose selector cuts
	unsigned long long zMass65to115EvtCount = 0;
	unsigned long long zMass70to110EvtCount = 0;
	unsigned long long zMass75to105EvtCount = 0;
	unsigned long long zMass80to100EvtCount = 0;
	unsigned long long zMass85to95EvtCount = 0;

	for(auto mode : modes) {
		bool isData = chainNames_.isData(mode);
		// TChain *c = new TChain("miniTree_lowdileptonsideband/t");
		// c->Add("myfile.root");
		// c->Add("/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/ntuples/DYJets_amcatnlo_pt100_250_v4_WRv07/myfile_1.root");
		//c->Add("~/eos/ntuples/DYJets_amcatnlo_pt100_250_v4_WRv07/myfile_1.root");
		//c->Add("~/eos/ntuples/DYJets_amcatnlo_pt100_250_v4_WRv07/myfile_2.root");
		// c->Add("~/eos/ntuples/DYJets_amcatnlo_pt100_250_v4_WRv07/myfile_3.root");
		// c->Add("~/eos/ntuples/DYJets_amcatnlo_pt100_250_v4_WRv07/myfile_4.root");
		// c->Add("~/eos/ntuples/DYJets_amcatnlo_pt100_250_v4_WRv07/myfile_5.root");
		// c->Add("~/eos/ntuples/DYJets_amcatnlo_pt100_250_v4_WRv07/myfile_6.root");
				
		TChain * c = myReader.getMiniTreeChain(chainNames_.getChainNames(mode, channel, isTagAndProbe), treeName);
			
#ifdef DEBUG
		c->Print();
#endif
		std::cout << "[INFO] Entries: " <<  c->GetEntries() << std::endl;
		if(c->GetEntries() == 0) {
			std::cerr << "[ERROR] No entries in chain... something went wrong" << std::endl;
			return 1;
		}

		// if you want to check if the config file is read correctly:
		if(debug) std::cout << myReader.getNorm1fb("TTJets_DiLept_v1") << std::endl;

		// Plotting trees
		std::string chnlName = channel_str, usingWeights = "";
		if(ignoreDyScaleFactors && mode.find("DY") != _ENDSTRING) usingWeights = "_withoutMllWeight";
		if(isLowFOM) outFileTag += "_lowFourObjectMass";
		if(isQCD) outFileTag += "_QCD";
		TFile f((outDir + "selected_tree_" + mode + chainNames_.getTreeName(channel, isTagAndProbe, isLowDiLepton, isQCD) + chnlName + usingWeights + outFileTag + ".root").c_str(), "recreate");
		f.WriteObject(&mass_vec, "signal_mass");
		// store the fitted results for every toy in a tree
		AnalysisResult result;
		result.nmasses = mass_vec.size();

		TTree * syst_tree = new TTree("syst_tree", "");
		TTree * central_value_tree = new TTree("central_value_tree", "");

		//First loop will always be central value
		result.SetBranches(syst_tree);
		result.SetBranches(central_value_tree);

		miniTreeEvent myEvent;
		
		myEvent.SetBranchAddresses(c);
		Selector selEvent;

		std::vector<TTree *> t1(nToys + 1, NULL);
		TTree * tDyCheck = new TTree("treeDyCheck", "");
		ULong64_t nEntries = c->GetEntries();

		TRandom3 Rand;
		const int Total_Number_of_Systematics_Smear = 1;
		const int Total_Number_of_Systematics_Up_Down = 8;
		float Random_Numbers_for_Systematics_Smear[Total_Number_of_Systematics_Smear] = {0.};
		float Random_Numbers_for_Systematics_Up_Down[Total_Number_of_Systematics_Up_Down] = {0.};


		std::vector<std::string> List_Systematics;
		//List_Systematics.push_back("smear");
		std::string word;
		std::ifstream Syst_File;

		std::cout << "[INFO] Reading systematics to be evaluated" << std::endl;
		std::string systFileName = "Systematics_To_Be_Evaluated.txt";
		Syst_File.open(systFileName);
		if(Syst_File.is_open() == false) {
			std::cerr << "[ERROR] File " << systFileName << " not opened. Check if the file exists" << std::endl;
			return 1;
		}

		while(Syst_File.peek() != EOF && Syst_File.good()) {
			if(Syst_File.peek() == 10) { // 10 = \n
	Syst_File.get();
	continue;
			}

			if(Syst_File.peek() == 35) { // 35 = #
	Syst_File.ignore(1000, 10); // ignore the rest of the line until \n
	continue;
			}

			Syst_File >> word;
			std::cout << word << std::endl;
			List_Systematics.push_back(word);
		}
		std::cout << "[INFO] nSyst = " << List_Systematics.size() << std::endl;
		if(List_Systematics.size() == 0) {
			std::cerr << "[ERROR] No systematics defined!" << std::endl;
			return 1;

		}

		TStopwatch ts;

		TFile RecoSF("data/egammaEffi.txt_EGM2D.root");
		TH2F *Recoh = (TH2F*)RecoSF.Get("EGamma_SF2D");

		
		TFile IDSF1("data/EfficienciesAndSF_IdBF.root");
		TFile IDSF2("data/EfficienciesAndSF_IdGH.root");
		TH2F *IDh1 = (TH2F*)IDSF1.Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/abseta_pair_ne_ratio");
		TH2F *IDh2 = (TH2F*)IDSF2.Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/abseta_pair_ne_ratio");
	
		TFile ISOSF1("data/EfficienciesAndSF_IsoBF.root");
		TFile ISOSF2("data/EfficienciesAndSF_IsoGH.root");
		TH2F *ISOh1 = (TH2F*)ISOSF1.Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");
		TH2F *ISOh2 = (TH2F*)ISOSF2.Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");

		TFile EWKcorr("data/kfactor_NLOEWK_DY.root");
		TH1F *histoEWKcorr = (TH1F*)EWKcorr.Get("kFactor_NLOEWL_DY");

		/*
			std::vector< miniTreeEvent> myEventVector;
			ts.Start();
			std::cout << "Loading events (nEvents = " << nEntries << "): [ 0%]" << std::flush;
			unsigned long long int nEntries_100 = nEntries / 100;
			for(unsigned long long int ev = 0; ev < nEntries; ++ev) {
			if(nEntries > 100 && ev % nEntries_100 == 1) {
			std::cout << "\b\b\b\b\b[" << std::setw (2) <<  (int)(ev / nEntries_100) << "%]" << std::flush;
			}
			c->GetEntry(ev);									    
			unsigned int nEle = myEvent.electrons_p4->size();
			unsigned int nMu = myEvent.muons_p4->size();

			// Apply JER
			Rand.SetSeed(seed + 1);
			JetResolution( &myEvent, Rand, isData);		

			for(unsigned int mu = 0; mu < nMu; ++mu) {
			if(isData){
			(*myEvent.muon_IDSF_central2).push_back(1.0);
			(*myEvent.muon_IDSF_error2).push_back(0.0);
			(*myEvent.muon_IsoSF_central2).push_back(1.0);
			(*myEvent.muon_IsoSF_error2).push_back(0.0);

			}
			else{
			float mupt = (myEvent.muons_p4)->at(mu).Pt();
			float mueta = (myEvent.muons_p4)->at(mu).Eta();
			(*myEvent.muon_IDSF_central2).push_back(IdSF(fabs(mueta), mupt, IDh1, IDh2).first);
			(*myEvent.muon_IDSF_error2).push_back(IdSF(fabs(mueta), mupt, IDh1, IDh2).second);
			(*myEvent.muon_IsoSF_central2).push_back(IsoSF(fabs(mueta), mupt, ISOh1, ISOh2).first);
			(*myEvent.muon_IsoSF_error2).push_back(IsoSF(fabs(mueta), mupt, ISOh1, ISOh2).second);
			}
			}
			
			if(nEle > 0) {
			///if there are electrons in the event, then write the electron SF and SF errors into the miniTreeEvent object named myEvent
			///before calling the Selector constructor
			for(unsigned int ele = 0; ele < nEle; ++ele) {
			if(isData) {
			(*myEvent.electron_IDSF_central).push_back(1.0);
			(*myEvent.electron_IDSF_error).push_back(0.);
			(*myEvent.electron_RecoSF_central).push_back(1.0);
			(*myEvent.electron_RecoSF_error).push_back(0.);
			(*myEvent.electron_HltSF_central).push_back(1.0);
			(*myEvent.electron_HltSF_error).push_back(0.);

			}//end if(isData)
			else {						
			float elept = (myEvent.electrons_p4)->at(ele).Pt();
			float sceta = (myEvent.electron_SC_eta)->at(ele);
			float recoSFerror = ElectronRecoSF(sceta, elept, Recoh).second;
			(*myEvent.electron_RecoSF_central).push_back(ElectronRecoSF(sceta, elept, Recoh).first);
			if (elept > 80)
			recoSFerror = TMath::Sqrt(recoSFerror*recoSFerror + 0.01*0.01);
			(*myEvent.electron_RecoSF_error).push_back(recoSFerror);

			if(fabs(sceta) < 1.4222){
			(*myEvent.electron_IDSF_central).push_back(0.968816);
			(*myEvent.electron_IDSF_error).push_back(0.);
			}
			else if(fabs(sceta) < 2.4 && fabs(sceta) > 1.566){
			(*myEvent.electron_IDSF_central).push_back(0.980451);
			(*myEvent.electron_IDSF_error).push_back(0.);
			}
			else {
			(*myEvent.electron_IDSF_central).push_back(1.0);
			(*myEvent.electron_IDSF_error).push_back(0.);
			}
						
			if(isTagAndProbe == true && channel_str == "EE") {
			///only apply non unity HltSF to DY MC used for ee tagandprobe
			(*myEvent.electron_HltSF_central).push_back(0.960473);
			(*myEvent.electron_HltSF_error).push_back(0.002551);
			} else { ///not EE tagandprobe
			(*myEvent.electron_HltSF_central).push_back(1.0);
			(*myEvent.electron_HltSF_error).push_back(0.);
			//(*myEvent.electron_RecoSF_central).push_back(1.0);
			//(*myEvent.electron_RecoSF_error).push_back(0.0);
			}
			}//end if(!isData)

			}//end loop over reco electrons in the event

			}//end if there are reco electrons in the event, and the channel is EE or EMu
			Selector sel(myEvent);
			if((isTagAndProbe == true && ( (channel_str == "EE" && myEvent.electrons_p4->size() > 1) || (channel_str == "MuMu" && myEvent.muons_p4->size() > 1) ) ) || sel.isPassingPreselect(makeSelectorPlots)) {
			myEventVector.push_back(myEvent);
			}
			myEvent.clear();
			}
			nEntries = myEventVector.size();
			nEntries_100 = nEntries / 100;
			ts.Stop();
			ts.Print();
		*/
		
		std::cout << "[INFO] Running nToys = " << nToys << std::endl;
		bool loop_one = true;
		int seed_i = seed + 1;
		RoccoR rc("data/rcdata.2016.v3");

		for(int i = 0; i < nToys + 1; ++i, ++seed_i) {

			Rand.SetSeed(seed_i+seed);			

			//for central values, we take the central value of Mu ID/ISO efficiencies and dont smear for JES systematics
			// Roch and Electron scales are smeared with a pre-defined seed(1), to give consistent results.
			if(loop_one) {
	Random_Numbers_for_Systematics_Up_Down[0] = 0.;//Mu Scale
	Random_Numbers_for_Systematics_Up_Down[1] = 0.;//Mu Eff Id Iso Trigger
	Random_Numbers_for_Systematics_Up_Down[2] = 0.;//Mu Res
	Random_Numbers_for_Systematics_Up_Down[3] = 0.;//Electron Scale
	Random_Numbers_for_Systematics_Up_Down[4] = 0.;//Electron Smear
	Random_Numbers_for_Systematics_Up_Down[5] = 0.;//Electron Reco Id Trigger	
	Random_Numbers_for_Systematics_Up_Down[6] = 0.;//JES
	Random_Numbers_for_Systematics_Up_Down[7] = 0.;//JER

			} else {
	for(int Rand_Up_Down_Iter = 0; Rand_Up_Down_Iter < Total_Number_of_Systematics_Up_Down; Rand_Up_Down_Iter++)
		Random_Numbers_for_Systematics_Up_Down[Rand_Up_Down_Iter] = Rand.Gaus(0.0, 1.);
			}
			//RooDataSet * tempDataSet = new RooDataSet("temp", "temp", Fits::vars);
			sprintf(name, "Tree_Iter%i", i);
			t1[i] = new TTree(name, "");
			t1[i]->SetDirectory(0);
			selEvent.SetBranches(t1[i]);
			if(loop_one) selEvent.SetBranches(tDyCheck);

			ts.Stop();
			ts.Print();
			ts.Start();

			//std::cout << "Processing events (nEvents = " << nEntries << "): [ 0%]" << std::flush;

			//unsigned long long int ev = 0;

			//------------------------------ scale random numbers: one set of numbers per toy, common to all events and electrons
			// it's the Random_Numbers_for_Systematics_Up_Down[2]

			//for(auto myEventIt : myEventVector) {

			std::cout << "Loading events (nEvents = " << nEntries << "): [ 0%]" << std::flush;
			unsigned long long int nEntries_100 = nEntries / 100;
			for(unsigned long long int ev = 0; ev < nEntries; ++ev) {
	if(nEntries > 100 && ev % nEntries_100 == 1) {
		std::cout << "\b\b\b\b\b[" << std::setw (2) <<  (int)(ev / nEntries_100) << "%]" << std::flush;
	}
	c->GetEntry(ev);									    
	unsigned int nEle = myEvent.electrons_p4->size();
	unsigned int nMu = myEvent.muons_p4->size();

	// Apply JER
	JetResolution( &myEvent, Rand, isData);		

	for(unsigned int mu = 0; mu < nMu; ++mu) {
		if(isData){
			(*myEvent.muon_IDSF_central2).push_back(1.0);
			(*myEvent.muon_IDSF_error2).push_back(0.0);
			(*myEvent.muon_IsoSF_central2).push_back(1.0);
			(*myEvent.muon_IsoSF_error2).push_back(0.0);	    
		}
		else{
			float mupt = (myEvent.muons_p4)->at(mu).Pt();
			float mueta = (myEvent.muons_p4)->at(mu).Eta();
			(*myEvent.muon_IDSF_central2).push_back(IdSF(fabs(mueta), mupt, IDh1, IDh2).first);
			(*myEvent.muon_IDSF_error2).push_back(IdSF(fabs(mueta), mupt, IDh1, IDh2).second);
			(*myEvent.muon_IsoSF_central2).push_back(IsoSF(fabs(mueta), mupt, ISOh1, ISOh2).first);
			(*myEvent.muon_IsoSF_error2).push_back(IsoSF(fabs(mueta), mupt, ISOh1, ISOh2).second);
		}
	}
			
	if(nEle > 0) {
		///if there are electrons in the event, then write the electron SF and SF errors into the miniTreeEvent object named myEvent
		///before calling the Selector constructor
		for(unsigned int ele = 0; ele < nEle; ++ele) {
			if(isData) {
				(*myEvent.electron_IDSF_central).push_back(1.0);
				(*myEvent.electron_IDSF_error).push_back(0.);
				(*myEvent.electron_RecoSF_central).push_back(1.0);
				(*myEvent.electron_RecoSF_error).push_back(0.);
				(*myEvent.electron_HltSF_central).push_back(1.0);
				(*myEvent.electron_HltSF_error).push_back(0.);

			}//end if(isData)
			else {						
				float elept = (myEvent.electrons_p4)->at(ele).Pt();
				float sceta = (myEvent.electron_SC_eta)->at(ele);
				float recoSFerror = ElectronRecoSF(sceta, elept, Recoh).second;
				(*myEvent.electron_RecoSF_central).push_back(ElectronRecoSF(sceta, elept, Recoh).first);
				if (elept > 80)
		recoSFerror = TMath::Sqrt(recoSFerror*recoSFerror + 0.01*0.01);
				(*myEvent.electron_RecoSF_error).push_back(recoSFerror);

				if(fabs(sceta) < 1.4442){
		(*myEvent.electron_IDSF_central).push_back(0.968816);
		(*myEvent.electron_IDSF_error).push_back(0.033);
				}
				else if(fabs(sceta) < 2.4 && fabs(sceta) > 1.566){
		(*myEvent.electron_IDSF_central).push_back(0.980451);
		(*myEvent.electron_IDSF_error).push_back(0.08);
				}
				else {
		(*myEvent.electron_IDSF_central).push_back(1.0);
		(*myEvent.electron_IDSF_error).push_back(0.);
				}

				if(isTagAndProbe == true && channel_str == "EE") {
		///only apply non unity HltSF to DY MC used for ee tagandprobe
		(*myEvent.electron_HltSF_central).push_back(0.960473);
		(*myEvent.electron_HltSF_error).push_back(0.002551);
				} else { ///not EE tagandprobe
		(*myEvent.electron_HltSF_central).push_back(1.0);
		(*myEvent.electron_HltSF_error).push_back(0.);
		//(*myEvent.electron_RecoSF_central).push_back(1.0);
		//(*myEvent.electron_RecoSF_error).push_back(0.0);
				}
			}//end if(!isData)

		}//end loop over reco electrons in the event

	}//end if there are reco electrons in the event, and the channel is EE or EMu

	if(nEntries > 100 && ev % nEntries_100 == 1) {
		std::cout << "\b\b\b\b\b[" << std::setw (2) <<  (int)(ev / nEntries_100) << "%]" << std::flush;
	}

	for(int Rand_Smear_Iter = 0; Rand_Smear_Iter < Total_Number_of_Systematics_Smear; Rand_Smear_Iter++)
		Random_Numbers_for_Systematics_Smear[Rand_Smear_Iter] = Rand.Gaus(0.0, 1.);
	ToyThrower( &myEvent, Random_Numbers_for_Systematics_Smear, Random_Numbers_for_Systematics_Up_Down, seed_i, List_Systematics, isData, rc);

	Selector sel(myEvent);
	if(!sel.isPassingPreselect(makeSelectorPlots)) continue;
		
	//Selector tmp_selEvent(myEventIt);
	selEvent = sel;
	// Select events with one good WR candidate
	// Tags:
	// 0 -- EEJJ Channel
	// 1 -- MuMuJJ Channel
	// 2 -- EMuJJ Channel
	/*
		if(loop_one && selEvent.isPassingLooseCuts(channel)) {
		if(isData == false) {
		selEvent.weight *= myReader.getNorm1fb(selEvent.datasetName) * myReader.getExtraWeight(selEvent.datasetName) * integratedLumi * pu_weights[int(selEvent.nPU)]; // the weight is the event weight * single object weights
						
		#ifdef DEBUGG
		std::cout << "PU weight=\t" << selEvent.pu_weight << std::endl;
		std::cout << "num vertices=\t" << selEvent.nPV << std::endl;
		std::cout << "num PV from miniTreeEvent=\t" << myEventIt.nPV << std::endl;
		std::cout << "num PU from miniTreeEvent=\t" << myEventIt.nPU << std::endl;
		#endif

		//Multiply by Renormalization and Factorization weights
		if(mode.find("DY") != _ENDSTRING && outFileTag != ""){
		selEvent.weight *= myEventIt.RF_weights->at(std::stoi(outFileTag));
		}
		//multiply by an additional weight when processing DY samples
		if(mode.find("DY") != _ENDSTRING && !ignoreDyScaleFactors) {
		selEvent.weight *= myReader.DYScale(channel);
		}
		} else {
		selEvent.weight = 1;
		assert(selEvent.weight == 1);
		}
		#ifdef DEBUGG
		std::cout << "selEvent.weight=\t" << selEvent.weight << std::endl;
		std::cout << "integratedLumi=\t" << integratedLumi << std::endl;
		std::cout << "myReader.getNorm1fb(selEvent.datasetName)=\t" << myReader.getNorm1fb(selEvent.datasetName) << std::endl;
		std::cout << "myEventIt.weight=\t" << myEventIt.weight << std::endl;
		#endif

		if(selEvent.dilepton_mass > 61.2 && selEvent.dilepton_mass < 121.2 && loop_one) ++zMass60to120EvtCount;
		if(selEvent.dilepton_mass > 66.2 && selEvent.dilepton_mass < 116.2 && loop_one) ++zMass65to115EvtCount;
		if(selEvent.dilepton_mass > 71.2 && selEvent.dilepton_mass < 111.2 && loop_one) ++zMass70to110EvtCount;
		if(selEvent.dilepton_mass > 76.2 && selEvent.dilepton_mass < 106.2 && loop_one) ++zMass75to105EvtCount;
		if(selEvent.dilepton_mass > 81.2 && selEvent.dilepton_mass < 101.2 && loop_one) ++zMass80to100EvtCount;
		if(selEvent.dilepton_mass > 86.2 && selEvent.dilepton_mass < 96.2 && loop_one)  ++zMass85to95EvtCount ;
		if(loop_one) tDyCheck->Fill();
		}
	*/
	if(selEvent.isPassing(channel, makeSelectorPlots && loop_one) && !isQCD) {

		if (channel == Selector::EMu && selEvent.dilepton_mass < 200) continue;
		if (isLowDiLepton && selEvent.dilepton_mass > 200) continue;
		if (!isLowDiLepton && selEvent.dilepton_mass < 200) continue;
		if (isLowFOM && selEvent.WR_mass > 600) continue;

		if(isData == false) {
			selEvent.weight *= myReader.getNorm1fb(selEvent.datasetName) * myReader.getExtraWeight(selEvent.datasetName) * integratedLumi * pu_weights[int(selEvent.nPU)]; // the weight is the event weight * single object weights

			// Multiply by Renormalization and Factorization weights
			if(mode.find("DY") != _ENDSTRING && outFileTag.find("PDF_") != _ENDSTRING){
				selEvent.weight *= myEvent.RF_weights->at(std::stoi(outFileTag.substr(4)));
			}
			if(mode.find("Other") != _ENDSTRING && outFileTag.find("PDF_") != _ENDSTRING){
				selEvent.weight *= myEvent.RF_weights->at(std::stoi(outFileTag.substr(4)));
			}


			// Multiply by EWK NLO corrections weight when processing DY samples
			if(mode.find("DY") != _ENDSTRING && channel != Selector::EMu) {
				TLorentzVector gen_leadlepton;
				TLorentzVector gen_subleadlepton;
				float genZ_pt = 0.0;

				if (channel == Selector::EE) {
					for(unsigned int i=0; i<(myEvent.genps_p4)->size(); i++) {
						if( abs((myEvent.genps_pdgId)->at(i)) == 11 ) {
							if ( deltaR(((myEvent.genps_p4)->at(i)).Eta(), ((myEvent.genps_p4)->at(i)).Phi(), selEvent.lead_lepton_eta, selEvent.lead_lepton_phi)<0.2 ) {
								// std::cout << "status leading gen = " << (myEvent.genps_status)->at(i) << std::endl;
								gen_leadlepton = ((myEvent.genps_p4)->at(i));
							}
							if ( deltaR(((myEvent.genps_p4)->at(i)).Eta(), ((myEvent.genps_p4)->at(i)).Phi(), selEvent.sublead_lepton_eta, selEvent.sublead_lepton_phi)<0.2 ) {
								// std::cout << "status subleading gen = " << (myEvent.genps_status)->at(i) << std::endl;
								gen_subleadlepton = ((myEvent.genps_p4)->at(i));
							}
						}
					}
				}

				if (channel == Selector::MuMu) {
					for(unsigned int i=0; i<(myEvent.genps_p4)->size(); i++) {
						if( abs((myEvent.genps_pdgId)->at(i)) == 13 ) {
							if ( deltaR(((myEvent.genps_p4)->at(i)).Eta(), ((myEvent.genps_p4)->at(i)).Phi(), selEvent.lead_lepton_eta, selEvent.lead_lepton_phi)<0.2 ) {
								// std::cout << "status leading gen = " << (myEvent.genps_status)->at(i) << std::endl;
								gen_leadlepton = ((myEvent.genps_p4)->at(i));
							}
							if ( deltaR(((myEvent.genps_p4)->at(i)).Eta(), ((myEvent.genps_p4)->at(i)).Phi(), selEvent.sublead_lepton_eta, selEvent.sublead_lepton_phi)<0.2 ) {
								// std::cout << "status subleading gen = " << (myEvent.genps_status)->at(i) << std::endl;
								gen_subleadlepton = ((myEvent.genps_p4)->at(i));
							}
						}
					}
				}

				genZ_pt = (gen_leadlepton+gen_subleadlepton).Pt();
				// std::cout << "genZ_pt = " << genZ_pt << std::endl;
				float EWKcorr_weight = (1+histoEWKcorr->GetBinContent(histoEWKcorr->FindBin(genZ_pt)));
				// std::cout << "EWKcorr_weight = " << EWKcorr_weight << std::endl;
				selEvent.weight *= EWKcorr_weight;
			}


			// Multiply by an additional weight when processing DY samples
			if(mode.find("DY") != _ENDSTRING && !ignoreDyScaleFactors) {
				if(mode.find("DYMADHT") != _ENDSTRING)
		selEvent.weight *= myReader.DYScale(channel,true);
				else
		selEvent.weight *= myReader.DYScale(channel,false);
			}
		} else {
			selEvent.weight = 1;
			assert(selEvent.weight == 1);
		}
		
		//Fits::massWR.setVal(selEvent.WR_mass);
		//Fits::evtWeight.setVal(selEvent.weight);
		//tempDataSet->add(Fits::vars);

		t1[i]->Fill();

	}
	else if(selEvent.isPassingQCD(channel, makeSelectorPlots && loop_one) && isQCD) {

		//if (channel == Selector::EMu && selEvent.dilepton_mass < 200) continue;
		if (isLowDiLepton && selEvent.dilepton_mass > 200) continue;
		if (!isLowDiLepton && selEvent.dilepton_mass < 200) continue;
		if (isLowFOM && selEvent.WR_mass > 600) continue;

		if(isData == false) {
			selEvent.weight *= myReader.getNorm1fb(selEvent.datasetName) * myReader.getExtraWeight(selEvent.datasetName) * integratedLumi * pu_weights[int(selEvent.nPU)]; // the weight is the event weight * single object weights

			// Multiply by Renormalization and Factorization weights
			if(mode.find("DY") != _ENDSTRING && outFileTag.find("PDF_") != _ENDSTRING){
				selEvent.weight *= myEvent.RF_weights->at(std::stoi(outFileTag.substr(4)));
			}
			
			// Multiply by an additional weight when processing DY samples
			if(mode.find("DY") != _ENDSTRING && !ignoreDyScaleFactors) {
				if(mode.find("DYMADHT") != _ENDSTRING)
		selEvent.weight *= myReader.DYScale(channel,true);
				else
		selEvent.weight *= myReader.DYScale(channel,false);
			}
		} else {
			selEvent.weight = 1;
			assert(selEvent.weight == 1);
		}
		
		//Fits::massWR.setVal(selEvent.WR_mass);
		//Fits::evtWeight.setVal(selEvent.weight);
		//tempDataSet->add(Fits::vars);

		t1[i]->Fill();

	}
	myEvent.clear();

			}//end loop over all input evts, and adding events to the RooDataSet pointer named tempDataSet

			ts.Stop();
			ts.Print();
			ts.Start();

			if(loop_one) std::cout << zMass60to120EvtCount << "\tevents from the dataset named\t" << selEvent.datasetName << "\tpass isPassingLooseCuts and have 61.2 < dilepton_mass < 121.2" << std::endl;
			if(loop_one) std::cout << zMass65to115EvtCount << "\tevents from the dataset named\t" << selEvent.datasetName << "\tpass isPassingLooseCuts and have 66.2 < dilepton_mass < 116.2" << std::endl;
			if(loop_one) std::cout << zMass70to110EvtCount << "\tevents from the dataset named\t" << selEvent.datasetName << "\tpass isPassingLooseCuts and have 71.2 < dilepton_mass < 111.2" << std::endl;
			if(loop_one) std::cout << zMass75to105EvtCount << "\tevents from the dataset named\t" << selEvent.datasetName << "\tpass isPassingLooseCuts and have 76.2 < dilepton_mass < 106.2" << std::endl;
			if(loop_one) std::cout << zMass80to100EvtCount << "\tevents from the dataset named\t" << selEvent.datasetName << "\tpass isPassingLooseCuts and have 81.2 < dilepton_mass < 101.2" << std::endl;
			if(loop_one) std::cout << zMass85to95EvtCount << "\tevents from the dataset named\t" << selEvent.datasetName << "\tpass isPassingLooseCuts and have 86.2 < dilepton_mass < 96.2" << std::endl;

			///make a permanent RooDataSet which has the same information as tempDataSet, but with events which are weighted according to the var named evtWeight
			//RooDataSet * permanentWeightedDataSet = new RooDataSet("permanentWeightedDataSet", "permanentWeightedDataSet", tempDataSet, Fits::vars, "", Fits::evtWeight.GetName());
			// Count number of events in each mass range to store in tree.
			TH1F * hWR_mass = new TH1F("hWR_mass", "hWR_mass", 140, 0, 7000);
			hWR_mass->Sumw2();
			t1[i]->Draw("WR_mass>>hWR_mass", "weight", "goff");
			double error = 0;
			for(size_t mass_i = 0; mass_i < mass_vec.size(); mass_i++) {
	auto range = mass_cut[std::make_pair(cut_channel, mass_vec.at(mass_i))];
	result.events_in_range[mass_i] = hWR_mass->IntegralAndError(hWR_mass->FindBin(range.first), hWR_mass->FindBin(range.second), error);
	result.error_in_range[mass_i] = float(error);
			}
			//TString hname = "hWR_mass_"+std::to_string(i);
			//hWR_mass->Write(hname);
			delete hWR_mass;

			if(loop_one) {
	TH1F * hWR_mass2 = new TH1F("hWR_mass2", "hWR_mass2", 140, 0, 7000);
	TString hist_name(mode + "_unweighted");
	hWR_mass2 = new TH1F(hist_name, hist_name, 140, 0, 7000);

	//Draw unweighted histogram
	t1[i]->Draw("WR_mass>>" + hist_name, "", "goff");
	for(size_t massi = 0; massi < mass_vec.size(); ++massi) {
		auto mass = mass_vec[massi];
		auto range = mass_cut[std::make_pair(cut_channel, mass)];
		float nEvents = hWR_mass2->Integral(hWR_mass2->FindBin(range.first), hWR_mass2->FindBin(range.second));
		result.unweighted_events_in_range[massi] = (UInt_t) nEvents;
		std::cout << "[DEBUG]\t" << mass << '\t' << nEvents << std::endl;
	}
	delete hWR_mass2;
			}

			f.cd();
			if(saveToys) t1[i]->Write();
			if(loop_one) {
	if(!saveToys) t1[i]->Write();
	//permanentWeightedDataSet->Write();
	tDyCheck->Write();
	delete tDyCheck;
			}
			delete t1[i];

			//permanentWeightedDataSet->Print();

			//if((isTagAndProbe == false) && (mode == "TT" || mode.find("DY") != _ENDSTRING || (mode == "data" && channel == Selector::EMu) ) ) {
			//assert(permanentWeightedDataSet->sumEntries() > 0);
			//Fits::expPower.setVal(-0.004);
			//RooFitResult * tempFitRslt = NULL;
			// fit dataset to given PDF
			//fitRooDataSet(tempFitRslt, permanentWeightedDataSet, Fits::expPdfRooAbsPdf);	///< expPdfRooAbsPdf defined over massWR 600 to 6500

			// dataset normalization is the number of entries in the dataset with fourObjectMass (name of Fits::massWR) above 600
			//result.normalization = permanentWeightedDataSet->sumEntries("fourObjectMass > 600");
			// set of variables in the PDF
			//RooArgSet *vset = Fits::expPdfRooAbsPdf->getVariables();
			// loop over RooRealVars in the set
			//TIterator * iter = vset->createIterator();
			//TObject * var = iter->Next();
			//RooRealVar *var_pdf;
			//result.nparam = 0;
			// while (var) {
			//   // ignore the M_WR variable
			//   if(strcmp(var->GetName(), "fourObjectMass") != 0) {
			//     var_pdf = (RooRealVar*)vset->find(var->GetName());
			//     // store the value of the fitted parameters and the corresponding errors
			//     result.fit_parameters[result.nparam] = var_pdf->getVal();
			//     result.fit_parameter_errors[result.nparam++] = var_pdf->getError();
			//   }
			//   var = iter->Next();
			// }

			// for(size_t mass_i = 0; mass_i < mass_vec.size(); mass_i++) {
			// 	  auto range = mass_cut[std::make_pair(cut_channel, mass_vec.at(mass_i))];
			// 	  double integral =  NormalizedIntegral(Fits::expPdfRooAbsPdf, Fits::massWR, range.first, range.second);
			// 	  result.fit_integral_in_range[mass_i] = integral;
			// 	}

			// 	if(nStatToys && loop_one) {
			// 	  std::cout << "Doing " << nStatToys << " stat Toys" << std::endl;
			// 	  AnalysisResult stat_result;
			// 	  stat_result.normalization = result.normalization;
			// 	  stat_result.nmasses = mass_vec.size();
			// 	  TTree * stat_tree = new TTree("stat_tree", "");
			// 	  stat_result.SetBranches(stat_tree);

			// 	  for(int stat_i = 0; stat_i < nStatToys; stat_i++) {
			// 	    // loop over RooRealVars in the set, vary them and store
			// 	    TIterator * iter = vset->createIterator();
			// 	    TObject * var = iter->Next();
			// 	    RooRealVar *var_pdf;
			// 	    stat_result.nparam = 0;
			// 	    while (var) {
			// 	      // ignore the M_WR variable
			// 	      if(strcmp(var->GetName(), "fourObjectMass") != 0) {
			// 		var_pdf = (RooRealVar*)vset->find(var->GetName());
			// 		var_pdf->setVal(Rand.Gaus(var_pdf->getVal(), var_pdf->getError()));
			// 		stat_result.fit_parameters[stat_result.nparam++] = var_pdf->getVal();
			// 	      }
			// 	      var = iter->Next();
			// 	    }
			// 	    //Calculate integrals for each fit
			// 	    for(size_t mass_i = 0; mass_i < mass_vec.size(); mass_i++) {
			// 	      auto range = mass_cut[std::make_pair(cut_channel, mass_vec.at(mass_i))];
			// 	      double integral =  NormalizedIntegral(Fits::expPdfRooAbsPdf, Fits::massWR, range.first, range.second);
			// 	      stat_result.fit_integral_in_range[mass_i] = integral;
			// 	    }

			// 	    stat_tree->Fill();

			// 	  }
			// 	  stat_tree->Write();
			// 	  delete stat_tree;

			// 	}
	
			// }

			// fill the tree with the normalization, parameters, and errors
			if(loop_one) {
	central_value_tree->Fill();
	sel::hists.PrintEntries("plots/", mode);
	sel::hists.Draw("plots/", mode);
	sel::hists.Clear();
			} else syst_tree->Fill();

			loop_one = false;
		}

		syst_tree->SetDirectory(&f);
		syst_tree->Write();
		central_value_tree->Write();
		delete c;
	}
	return 0;

}
