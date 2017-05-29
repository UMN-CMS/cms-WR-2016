arguments="--toys 25 --lumi 35867"
logdir=$PWD/log
mass_n=$(seq 1 27)
toys_n=$(seq 1 20)

queue=1nd
tag=$1
dir=$2
finaldir=$dir/$tag/

mkdir -p $finaldir

for toy in $toys_n
do
    let t=100*${toy}+1234

    for mass in $mass_n
    #for mass in {8,19,23}
    do
	let m=200*$mass+600
	#echo $mass $m $t
	bsub -q $queue -eo $logdir/${mass}_signal_ee_${tag}_${toy}.err -oo $logdir/${mass}_signal_ee_${tag}_${toy}.out "$PWD/runJob.sh EE signal $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy} --signalN $mass\""
	bsub -q $queue -eo $logdir/${mass}_signal_mumu_${tag}_${toy}.err -oo $logdir/${mass}_signal_mumu_${tag}_${toy}.out "$PWD/runJob.sh MuMu signal $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy} --signalN $mass\""
    done

    #EMu data
    bsub -q $queue -eo $logdir/data_emu_ee_${tag}_${toy}.err   -oo $logdir/data_emu_ee_${tag}_${toy}.out   "$PWD/runJob.sh EMu data $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy} --cut_channel EE\""
    bsub -q $queue -eo $logdir/data_emu_mumu_${tag}_${toy}.err -oo $logdir/data_emu_mumu_${tag}_${toy}.out "$PWD/runJob.sh EMu data $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy} --cut_channel MuMu\""

    #EE dy
    #bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${toy}.err -oo $logdir/dy_1_ee_${tag}_${toy}.out "$PWD/runJob.sh EE DYAMCPT_1 $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy}\""
    bsub -q $queue -eo $logdir/dy_ee_${tag}_${toy}.err -oo $logdir/dy_ee_${tag}_${toy}.out "$PWD/runJob.sh EE DYCOMB $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy}\""
    #MUMU dy
    #bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${toy}.err -oo $logdir/dy_1_mumu_${tag}_${toy}.out "$PWD/runJob.sh MuMu DYAMCPT_1 $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy}\""
    bsub -q $queue -eo $logdir/dy_mumu_${tag}_${toy}.err -oo $logdir/dy_mumu_${tag}_${toy}.out "$PWD/runJob.sh MuMu DYCOMB $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy}\""

    #EE Other
    bsub -q $queue -eo $logdir/other_ee_${tag}_${toy}.err -oo $logdir/other_ee_${tag}_${toy}.out "$PWD/runJob.sh EE Other $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy}\""
    #MUMU Other
    bsub -q $queue -eo $logdir/other_mumu_${tag}_${toy}.err -oo $logdir/dy_mumu_${tag}_${toy}.out "$PWD/runJob.sh MuMu Other $finaldir $PWD \"$arguments --seed ${t} -f _Toy${toy}\""
done


##EE ttbar mc
#bsub -q $queue -eo $logdir/ttbar_ee_${tag}.err -oo $logdir/ttbar_ee_${tag}.out "$PWD/runJob.sh EE TT $finaldir $PWD \"$arguments\""

##MUMU ttbar mc
#bsub -q $queue -eo $logdir/ttbar_mumu_${tag}.err -oo $logdir/ttbar_mumu_${tag}.out "$PWD/runJob.sh MuMu TT $finaldir $PWD \"$arguments\""

