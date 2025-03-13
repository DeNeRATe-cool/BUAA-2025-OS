if [[ $# -eq 0 ]]
then
	cat stderr.txt
elif [[ $# -eq 1 ]]
then
	sed "$1,$ p" stderr.txt
else
	sed "$1,$2 p" stderr.txt
fi
