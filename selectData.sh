mkdir -p dataSet
for file in $(find ./data -name "*")
do
	if [[ "$1" == "all" ]]
	then
		cp $file ./dataSet
	elif [[ $(echo $(basename $file) | cut -d '_' -f 1) == "$1" ]]
	then
		cp $file ./dataSet
	fi
done
