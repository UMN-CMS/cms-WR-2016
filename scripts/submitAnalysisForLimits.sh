arguments="--toys 500 --lumi 35867"
logdir=$PWD/log
mass_n=$(seq 1 27)

queue=1nd
tag=$1
dir=$2
finaldir=$dir/$tag/

mkdir -p $finaldir

for mass in $mass_n
do
	bsub -q $queue -eo $logdir/${mass}_signal_ee_${tag}.err -oo $logdir/${mass}_signal_ee_${tag}.out "$PWD/runJob.sh EE signal $finaldir $PWD \"$arguments --signalN $mass\""
	bsub -q $queue -eo $logdir/${mass}_signal_mumu_${tag}.err -oo $logdir/${mass}_signal_mumu_${tag}.out "$PWD/runJob.sh MuMu signal $finaldir $PWD \"$arguments --signalN $mass\""
done

#EMu data
bsub -q $queue -eo $logdir/data_emu_ee_${tag}.err   -oo $logdir/data_emu_ee_${tag}.out   "$PWD/runJob.sh EMu data $finaldir $PWD \"$arguments  --cut_channel EE\""
bsub -q $queue -eo $logdir/data_emu_mumu_${tag}.err -oo $logdir/data_emu_mumu_${tag}.out "$PWD/runJob.sh EMu data $finaldir $PWD \"$arguments  --cut_channel MuMu\""

#EE dy
bsub -q $queue -eo $logdir/dy_1_ee_${tag}.err -oo $logdir/dy_1_ee_${tag}.out "$PWD/runJob.sh EE DYAMCPT_1 $finaldir $PWD \"$arguments\""

#MUMU dy
bsub -q $queue -eo $logdir/dy_1_mumu_${tag}.err -oo $logdir/dy_1_mumu_${tag}.out "$PWD/runJob.sh MuMu DYAMCPT_1 $finaldir $PWD \"$arguments\""

##EE ttbar mc
#bsub -q $queue -eo $logdir/ttbar_ee_${tag}.err -oo $logdir/ttbar_ee_${tag}.out "$PWD/runJob.sh EE TT $finaldir $PWD \"$arguments\""

##MUMU ttbar mc
#bsub -q $queue -eo $logdir/ttbar_mumu_${tag}.err -oo $logdir/ttbar_mumu_${tag}.out "$PWD/runJob.sh MuMu TT $finaldir $PWD \"$arguments\""

#EE Other
bsub -q $queue -eo $logdir/other_ee_${tag}.err -oo $logdir/other_ee_${tag}.out "$PWD/runJob.sh EE Other $finaldir $PWD \"$arguments\""

#MUMU Other
bsub -q $queue -eo $logdir/other_mumu_${tag}.err -oo $logdir/dy_mumu_${tag}.out "$PWD/runJob.sh MuMu Other $finaldir $PWD \"$arguments\""
