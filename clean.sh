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
