if [[ $# -eq 1 ]]
then
	cat stderr.txt
elif [[ $# -eq 2 ]]
then
	sed "$2,$ p"
else
	sed "$2,$3 p"	
fi
