logdir=$PWD/log
syst_n=$(seq 1 110)

queue=1nd
tag=$1
dir=$2
finaldir=$dir/$tag/

mkdir -p $finaldir
mkdir -p $logdir

for s in $syst_n
do
    #EE dy
    bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${toy}.err -oo $logdir/dy_1_ee_${tag}_${toy}.out "$PWD/runJob.sh EE DYAMCPT_1 $finaldir $PWD \"-f PDF_${s}\""
    #low dilepton sideband
    # bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${s}.err -oo $logdir/dy_1_ee_${tag}_${s}.out "$PWD/runJob.sh EE DYAMCPT_1 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${s}.err -oo $logdir/dy_1_ee_${tag}_${s}.out "$PWD/runJob.sh EE DYAMCPT_2 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${s}.err -oo $logdir/dy_1_ee_${tag}_${s}.out "$PWD/runJob.sh EE DYAMCPT_3 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${s}.err -oo $logdir/dy_1_ee_${tag}_${s}.out "$PWD/runJob.sh EE DYAMCPT_4 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${s}.err -oo $logdir/dy_1_ee_${tag}_${s}.out "$PWD/runJob.sh EE DYAMCPT_5 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${s}.err -oo $logdir/dy_1_ee_${tag}_${s}.out "$PWD/runJob.sh EE DYAMCPT_6 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${s}.err -oo $logdir/dy_1_ee_${tag}_${s}.out "$PWD/runJob.sh EE DYAMCPT_7 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""


    #MUMU dy
    bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${toy}.err -oo $logdir/dy_1_mumu_${tag}_${toy}.out "$PWD/runJob.sh MuMu DYAMCPT_1 $finaldir $PWD \"-f PDF_${s}\""
	#low dilepton sideband
    # bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${s}.err -oo $logdir/dy_1_mumu_${tag}_${s}.out "$PWD/runJob.sh MuMu DYAMCPT_1 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${s}.err -oo $logdir/dy_1_mumu_${tag}_${s}.out "$PWD/runJob.sh MuMu DYAMCPT_2 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${s}.err -oo $logdir/dy_1_mumu_${tag}_${s}.out "$PWD/runJob.sh MuMu DYAMCPT_3 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${s}.err -oo $logdir/dy_1_mumu_${tag}_${s}.out "$PWD/runJob.sh MuMu DYAMCPT_4 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${s}.err -oo $logdir/dy_1_mumu_${tag}_${s}.out "$PWD/runJob.sh MuMu DYAMCPT_5 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${s}.err -oo $logdir/dy_1_mumu_${tag}_${s}.out "$PWD/runJob.sh MuMu DYAMCPT_6 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""
    # bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${s}.err -oo $logdir/dy_1_mumu_${tag}_${s}.out "$PWD/runJob.sh MuMu DYAMCPT_7 $finaldir $PWD \"-f PDF_${s} --isLowDilepton\""

done

