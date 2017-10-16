mass_n=$(seq 1 27)

mkdir -p selectedTrees

for mass in $mass_n
do
	let m=200*$mass+600
	echo $mass $m 

	./bin/analysis -c EE -m signal --signalN $mass -d selectedTrees/ 
	# ./bin/analysis -c MuMu -m signal --signalN $mass -d selectedTrees/ 

done