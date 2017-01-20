#!/bin/bash

WrMasses=(800 1600 2400 3200 4000 6000) 
NuMasses=(400 800 1200 1600 2000 3000)  


inputDatasetsEE=('/WRToEEJJ_MW-800_MNu-400_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-d6c7bcd1995440aefecbee0f7f0a6b40/USER'
	'/WRToEEJJ_MW-1600_MNu-800_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-f5476f387b0cafd2f4a821c8b076898d/USER'
	'/WRToEEJJ_MW-2400_MNu-1200_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-40d4e4e5b0d661ea35b11420377a8bd2/USER'
	'/WRToEEJJ_MW-3200_MNu-1600_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-94244898ed59f0d5e3e237b4d6593922/USER'
	'/WRToEEJJ_MW-4000_MNu-2000_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-d15ddc4d92033ae8750f8824e5dace36/USER'
	'/WRToEEJJ_MW-6000_MNu-3000_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-9dabd287c5cf8e309fa15727a9a29451/USER')

inputDatasetsMuMu=('/WRToMuMuJJ_MW-800_MNu-400_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-46898c885fe714e54c27c3481d73c162/USER' 
	'/WRToMuMuJJ_MW-1600_MNu-800_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-0d24d8f03d0b9fe625c518516116a2ac/USER'
	'/WRToMuMuJJ_MW-2400_MNu-1200_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-9c7ba5ab24f52c28c90683a52b1b7801/USER'
	'/WRToMuMuJJ_MW-3200_MNu-1600_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-a9de18ef67980001c0fc15bd61b8c5c1/USER'
	'/WRToMuMuJJ_MW-4000_MNu-2000_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-7f38ac051f73ee60414fce7ab6da59f8/USER'
	'/WRToMuMuJJ_MW-6000_MNu-3000_TuneCUETP8M1_13TeV-pythia8/gnegro-RunIIWinter16_80X_GEN-SIM-ffb059ac05623d978c901d787a56f11b/USER')


for h in ${!WrMasses[*]}
do

	## EEJJ
	#make python config file for HLT step 
	eval "sed 's/MASSWR/${WrMasses[$h]}/g' WR_M-UNDEF_ToLNu_M-UNDEF_HLT.py > WR_temp1.py"                        
	eval "sed 's/MASSNU/${NuMasses[$h]}/g'  WR_temp1.py > WR_M-${WrMasses[$h]}_ToLNu_M-${NuMasses[$h]}_HLT_EEJJ.py"
	rm WR_temp1.py 

	#make crab config file
	eval "sed 's/WR_M-UNDEF/WR_M-${WrMasses[$h]}/g' crabConfig_WR_M-UNDEF_ToLNu_M-UNDEF_HLT_EEJJ.py > crabConfig_WR_temp1.py"
	eval "sed 's/ToLNu_M-UNDEF/ToLNu_M-${NuMasses[$h]}/g' crabConfig_WR_temp1.py > crabConfig_WR_temp2.py"
	eval "sed 's@datasetFromDBS@${inputDatasetsEE[$h]}@g' crabConfig_WR_temp2.py > crabConfig_WR_M-${WrMasses[$h]}_ToLNu_M-${NuMasses[$h]}_HLT_EEJJ.py"
	rm crabConfig_WR_temp1.py crabConfig_WR_temp2.py 


	## MuMuJJ
	#make python config file for HLT step 
	eval "sed 's/MASSWR/${WrMasses[$h]}/g' WR_M-UNDEF_ToLNu_M-UNDEF_HLT.py > WR_temp1.py"                        
	eval "sed 's/MASSNU/${NuMasses[$h]}/g'  WR_temp1.py > WR_M-${WrMasses[$h]}_ToLNu_M-${NuMasses[$h]}_HLT_MuMuJJ.py"
	rm WR_temp1.py 

	#make crab config file
	eval "sed 's/WR_M-UNDEF/WR_M-${WrMasses[$h]}/g' crabConfig_WR_M-UNDEF_ToLNu_M-UNDEF_HLT_MuMuJJ.py > crabConfig_WR_temp1.py"
	eval "sed 's/ToLNu_M-UNDEF/ToLNu_M-${NuMasses[$h]}/g' crabConfig_WR_temp1.py > crabConfig_WR_temp2.py"
 	eval "sed 's@datasetFromDBS@${inputDatasetsMuMu[$h]}@g' crabConfig_WR_temp2.py > crabConfig_WR_M-${WrMasses[$h]}_ToLNu_M-${NuMasses[$h]}_HLT_MuMuJJ.py"
	rm crabConfig_WR_temp1.py crabConfig_WR_temp2.py 

done