#!/bin/bash

find . -mindepth 1 \
! -name 'init.sh' \
! -name 'clean.sh' \
! -name 'exam_[1-9].sh' \
! -name 'run_exam.sh' \
! -name 'Makefile' \
! -path './.git*' \
! -path './.gitignore' \
-exec rm -rf {} +

# Make Quiz
touch Makefile

cat >check.c <<'EOF'
#include <stdio.h>
int main() {
	  printf(">>>>>QUIZ CHECK.C<<<<<\n");
	  return 0;
}
EOF

mkdir src
mkdir src/include
mkdir out

cat >src/include/inter.h <<'EOF'
#include <stdio.h>

#define INTER_NUM 998244353

void inter_output(int num);
EOF

cat >src/main.c <<'EOF'
#include "inter.h"

int main() {
	  inter_output(INTER_NUM);
	  return 0;
}
EOF

cat >src/output.c <<'EOF'
#include "inter.h"

void inter_output(int num) {
    printf(">>>>>QUIZ OUTPUT.C CAUGHT %d<<<<<\n", num);
}
EOF

# Bash Quiz

mkdir origin
cat >origin/basic.c <<'EOF'
#include <stdio.h>

int main() {
    printf("RIGHT! hello world\n");
    printf("WRONG! Hello world\n");
    return 0;
}
EOF

mkdir origin/code

echo "#include <stdio.h>" >> origin/code/0.c

for ((i=1; i<=20; i=i+1))
do
  echo "void output$i();" >> origin/code/0.c
done

echo "int main() {" >> origin/code/0.c
for ((i=1; i<=20; i=i+1))
do
  echo "    output$i();" >> origin/code/0.c
done
echo "    return 0;" >> origin/code/0.c
echo "}" >> origin/code/0.c

for ((i=1; i<=20; i=i+1))
do
  cat >origin/code/$i.c <<EOF
#include <stdio.h>

void output$i() {
    fprintf(stderr, ">>>>QUIZ %d.C\\n", REPLACE);
}
EOF
done

echo "RIGHT! APPEND" >> stderr.txt
