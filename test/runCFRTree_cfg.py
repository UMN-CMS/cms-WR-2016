import FWCore.ParameterSet.Config as cms

process = cms.Process("CutResultTree")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(5000)

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing('standard') 
options.maxEvents = -1
options.parseArguments()


with open(options.files.pop(),'r') as filelist:
	for line in filelist:
		if line:
			options.files.append("root://xrootd.unl.edu/" + line)


#
# Define input data to read
#
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )

process.source = cms.Source ("PoolSource", fileNames = cms.untracked.vstring(options.files) )

#
# Set up electron ID (VID framework)
#

from ExoAnalysis.cmsWR.heepSelector_cfi import loadHEEPIDSelector
loadHEEPIDSelector(process)
process.load("ExoAnalysis.cmsWR.heepSelector_cfi")
process.load("ExoAnalysis.cmsWR.cutResultTree_cfi")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string( options.output )
                                   )

process.options = cms.untracked.PSet(
		wantSummary = cms.untracked.bool(True)
		)

# Make sure to add the ID sequence upstream from the user analysis module
process.p = cms.Path(process.egmGsfElectronIDSequence * process.cutflowresult )
