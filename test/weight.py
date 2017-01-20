import FWCore.ParameterSet.Config as cms
import os,glob,sys

process = cms.Process('ANA')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            #fileNames=cms.untracked.vstring('/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/00000/001AFDCE-C33B-E611-B032-0025905D1C54.root',
                            fileNames=cms.untracked.vstring('/store/mc/RunIISpring16MiniAODv2/DYJetsToLL_Pt-400To650_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/50000/20BE2D8A-E758-E611-99DE-002590DC03AC.root',
                            ))
'''                            
os.system('das_client.py --query="file dataset=/DYJetsToLL_Pt-400To650_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/MINIAODSIM instance=prod/global" --limit=0 > pappy.txt')

sec_file = open('pappy.txt', 'r')
mysecfilelist = []
for line in sec_file:
    # add as many files as you wish this way
    mysecfilelist.append(line.strip())
process.source.fileNames = mysecfilelist
'''

outfile = 'weight.root'

process.TFileService = cms.Service('TFileService', fileName = cms.string(outfile))

process.weight = cms.EDAnalyzer('Weight')


process.p = cms.Path(process.weight)
