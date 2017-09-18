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
config.Data.unitsPerJob = 5
#config.Data.totalUnits = 1
config.Data.outLFNDirBase = ''
config.Data.lumiMask = ''
# config.Site.storageSite = 'T3_US_FNALLPC'
config.Site.storageSite = 'T2_FR_GRIF_IRFU'

#%(user)s/runAnalysis_80X_%(name)s

datasets = []
datasetTags = []

# f = open('configs/datasets_80X_moriond.dat')
f = open('configs/datasets_80X_newXs.dat')
for line in f:
    if '#' not in line:
        datasetTags.append(line.split('\t')[0])
        datasets.append(line.split('\t')[1])

json = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/Final/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON.txt'

for d,dt in zip(datasets,datasetTags):
    if 'DoubleEG' in dt:
        continue
    print dt
    if os.path.isdir("crab/crab_runAnalysis_80X_WRv07_"+dt):
        print "Directory already exists.\nContinue."
        continue
    config_tmp = config
    config_tmp.General.requestName = 'runAnalysis_80X_WRv07_' + dt
    config_tmp.Data.inputDataset = d
    # config_tmp.Data.outLFNDirBase = '/store/user/jchavesb/runAnalysis_80X_WRv07_'+dt
    config_tmp.Data.outLFNDirBase = '/store/user/gnegro/cmsWR16/runAnalysis_80X_WRv07_'+dt
    if 'Run2016' in d:
        if 'RunH' in dt:
            config_tmp.JobType.pyCfgParams = ['isMC=0','datasetTag='+dt, 'RunH=1']
        else:
            config_tmp.JobType.pyCfgParams = ['isMC=0','datasetTag='+dt]
        config_tmp.Data.lumiMask = json
    elif 'WRto' in dt or dt == 'WW' or dt == 'WZ' or dt == 'ZZ' or 'QCD' in dt:
        config_tmp.JobType.pyCfgParams = ['isMC=1','datasetTag='+dt,'lheAvailable=False']
        config_tmp.Data.lumiMask = ''
    #else:
    #    config_tmp.JobType.pyCfgParams = ['isMC=1','datasetTag='+dt,'runHLT=0']
    #    config_tmp.Data.lumiMask = ''
    else:
        config_tmp.JobType.pyCfgParams = ['isMC=1','datasetTag='+dt]
        config_tmp.Data.lumiMask = ''
    if '/USER' in d:
        config_tmp.Data.inputDBS = 'phys03'
    crabCommand('submit', config=config_tmp)
    config_tmp = 0
for d,dt in zip(datasets,datasetTags):
    if 'DoubleEG' not in dt:
        continue
    for j in ['data/ElectronTriggerGsfTrkIdVL.json','data/ElectronTriggerMW.json']:
        jname = dt+'_'+j.strip("data/").strip(".json")
        print jname
        if os.path.isdir("crab/crab_runAnalysis_80X_WRv07_"+jname):
            print "Directory already exists.\nContinue."
            continue
        config_tmp = config
        config_tmp.General.requestName = 'runAnalysis_80X_WRv07_' + jname
        config_tmp.Data.inputDataset = d
        # config_tmp.Data.outLFNDirBase = '/store/user/jchavesb/runAnalysis_80X_WRv07_'+jname
        config_tmp.Data.outLFNDirBase = '/store/user/gnegro/cmsWR16/runAnalysis_80X_WRv07_'+jname
        if 'GsfTrkIdVL' in j:
            if 'RunH' in dt or 'RunB' in dt or 'RunC' in dt:
                continue
            else:
                config_tmp.JobType.pyCfgParams = ['isMC=0','datasetTag='+dt,'EleMW=0']
                config_tmp.Data.lumiMask = j
        else:
            if 'RunE' in dt or 'RunF' in dt:
                continue
            if 'RunH' in dt:
                config_tmp.JobType.pyCfgParams = ['isMC=0','datasetTag='+dt,'EleMW=1','RunH=1']
                config_tmp.Data.lumiMask = json
            else:
                config_tmp.JobType.pyCfgParams = ['isMC=0','datasetTag='+dt,'EleMW=1']
                config_tmp.Data.lumiMask = j
            
        crabCommand('submit', config=config_tmp)
        config_tmp = 0
