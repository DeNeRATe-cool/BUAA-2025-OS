init-envs := test_adel_ades/1
CFLAGS    += -DMOS_SCHED_END_PC=0x400180
pre-env-run := $(test_dir)/pre_env_run.c
