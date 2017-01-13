import time,os
from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import config as Config

config = Config()

config.General.requestName = ''
config.General.transferLogs = True
config.General.workArea = 'crab'
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'test/runAnalysis_cfg.py'
config.JobType.pyCfgParams = []
config.Data.inputDataset = ''
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
#config.Data.totalUnits = 1
config.Data.outLFNDirBase = ''
config.Data.lumiMask = ''
config.Site.storageSite = 'T3_US_FNALLPC'

#%(user)s/runAnalysis_80X_%(name)s

datasets = []
datasetTags = []

f = open('configs/datasets_80X.dat')
for line in f:
    if '#' not in line:
        datasetTags.append(line.split('\t')[0])
        datasets.append(line.split('\t')[1])

json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
jsonPrompt = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Final/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt'

for d,dt in zip(datasets,datasetTags):
    if 'DoubleEG' in dt:
        continue
    print dt
    if os.path.isdir("crab/crab_runAnalysis_80X_WRv04_"+dt):
        print "Directory already exists.\nContinue."
        continue
    config_tmp = config
    config_tmp.General.requestName = 'runAnalysis_80X_WRv04_' + dt
    config_tmp.Data.inputDataset = d
    config_tmp.Data.outLFNDirBase = '/store/user/jchavesb/runAnalysis_80X_WRv04_'+dt
    if 'Run2016' in d:
        config_tmp.JobType.pyCfgParams = ['isMC=0','datasetTag='+dt]
        if 'RunH' in dt:
            config_tmp.Data.lumiMask = jsonPrompt
        else:
            config_tmp.Data.lumiMask = json
    elif 'HLT' in d:
        config_tmp.JobType.pyCfgParams = ['isMC=1','datasetTag='+dt]
        config_tmp.Data.lumiMask = ''
    else:
        config_tmp.JobType.pyCfgParams = ['isMC=1','datasetTag='+dt,'runHLT=0']
        config_tmp.Data.lumiMask = ''
    if '/USER' in d:
        config.Data.inputDBS = 'phys03'
    crabCommand('submit', config=config_tmp)
    config_tmp = 0

for d,dt in zip(datasets,datasetTags):
    if 'DoubleEG' not in dt:
        continue
    for j in ['data/ElectronTriggerGsfTrkIdVL.json','data/ElectronTriggerMW.json']:
        jname = dt+'_'+j.strip("data/").strip(".json")
        print jname
        if os.path.isdir("crab/crab_runAnalysis_80X_WRv04_"+jname):
            print "Directory already exists.\nContinue."
            continue
        config_tmp = config
        config_tmp.General.requestName = 'runAnalysis_80X_WRv04_' + jname
        config_tmp.Data.inputDataset = d
        config_tmp.Data.outLFNDirBase = '/store/user/jchavesb/runAnalysis_80X_WRv04_'+jname
        if 'GsfTrkIdVL' in j:
            config_tmp.JobType.pyCfgParams = ['isMC=0','datasetTag='+dt,'EleMW=0']
        else:
            config_tmp.JobType.pyCfgParams = ['isMC=0','datasetTag='+dt]
        if 'RunH' in dt:
            config_tmp.Data.lumiMask = jsonPrompt
        else:
            config_tmp.Data.lumiMask = j
        crabCommand('submit', config=config_tmp)
        config_tmp = 0
