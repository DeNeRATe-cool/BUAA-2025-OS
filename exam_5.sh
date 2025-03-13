for ((i=0;i<=20;i++))
do
	sed "s/REPLACE/$i/g" origin/code/$i.c > result/code/$i.c
done
