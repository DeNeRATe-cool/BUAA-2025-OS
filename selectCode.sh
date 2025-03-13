rm -f testfile.c
# touch testfile.c
# ln -s testfile.c "codeSet/$1.c"
ln -s "codeSet/$1.c" testfile.c
gcc -Iinclude testfile.c -o test.out
