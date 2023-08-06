shopt -s nullglob
for file in "$@"/*;do
	if [[ "$file" == *".sat" ]];then
		start=`date +%s.%N`
		timeout 600s ./SATsolver "$file"
		end=`date +%s.%N`
		runtime=$( echo "$end - $start" | bc -l )
		echo "Runtime: $runtime (s)"
		echo " "
	fi
done
shopt -u nullglob
