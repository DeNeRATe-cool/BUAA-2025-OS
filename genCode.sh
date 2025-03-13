mkdir -p codeSet
for file in $(find ./code -name "*.sy")
do
	echo $(basename $file)
	name=$(echo $(basename $file) | cut -d '.' -f 1)
	echo $name
	cat $file | sed "1i\#include\"include/libsy.h\"" | sed 's/getInt/getint/g' > codeSet/$name.c
done
