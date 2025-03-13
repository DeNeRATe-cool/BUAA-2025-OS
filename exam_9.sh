if [[ $# -eq 0 ]]
then
	cat stderr.txt
elif [[ $# -eq 1 ]]
then
	sed "$2,$ p"
else
	sed "$2,$3 p"	
fi
