logdir=$PWD/log
syst_n=$(seq 1 110)

queue=1nd
tag=$1
dir=$2
finaldir=$dir/$tag/

mkdir -p $finaldir

for s in $syst_n
do
    #EE dy
    bsub -q $queue -eo $logdir/dy_1_ee_${tag}_${toy}.err -oo $logdir/dy_1_ee_${tag}_${toy}.out "$PWD/runJob.sh EE DYAMCPT_1 $finaldir $PWD \"-f PDF_${s}\""
    #MUMU dy
    bsub -q $queue -eo $logdir/dy_1_mumu_${tag}_${toy}.err -oo $logdir/dy_1_mumu_${tag}_${toy}.out "$PWD/runJob.sh MuMu DYAMCPT_1 $finaldir $PWD \"-f PDF_${s}\""

done
