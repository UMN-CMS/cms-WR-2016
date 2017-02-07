import FWCore.ParameterSet.Config as cms
import glob

process = cms.Process("ANA")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring('/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/80000/BC54430B-E0CD-E611-8BB1-0CC47A78A4B0.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/80000/BCB07281-E3CD-E611-9728-003048FFD72C.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/80000/ACF2D598-E6CD-E611-9D85-0CC47A7C3424.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/80000/A455FD74-ECCD-E611-8A04-0025905A609A.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/80000/C692F933-A9CE-E611-93E9-0CC47A4D76C0.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/80000/C8371044-A9CE-E611-AB11-0025905B8582.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/8A7F09CF-EECD-E611-80A3-549F3525B154.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/126F7F80-3BCE-E611-9576-842B2B42C164.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/CA6DF761-49CE-E611-9C48-141877411C7F.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/1CC79D5E-46CE-E611-864E-B083FED046D9.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/F035FF9E-48CE-E611-BCDE-1866DAEECFDC.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/A044208D-48CE-E611-953E-842B2B180882.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/6204C475-49CE-E611-A5CE-549F3525C380.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/101BD416-4CCE-E611-96E2-141877411C7F.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/DC64296A-4CCE-E611-9D20-1866DAEA8190.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/7A80548A-4CCE-E611-A5AE-B083FED3EE25.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/56955ABA-4DCE-E611-9A03-1866DAEEB358.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/4C9D6682-49CE-E611-9219-782BCB537DBB.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/9871909B-4FCE-E611-83F4-141877411C7F.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/E0BEB0A2-4CCE-E611-A9F8-90B11C0BCBC2.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/6AA264CC-4CCE-E611-A368-782BCB536C90.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/26BFA21E-52CE-E611-9043-141877411C7F.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/E295CDB4-53CE-E611-9523-1866DAEEB0A8.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/5C9D9F2B-57CE-E611-B57F-1866DAEB296C.root',
'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/120000/7217D1CC-60D0-E611-BE40-90B11C0BB9FC.root'),
                            )
outfile = 'puAna.root'

process.TFileService = cms.Service('TFileService', fileName = cms.string(outfile))


process.ana = cms.EDAnalyzer('PUAnalyzer',                             
                             )
                             

process.p = cms.Path(process.ana)
