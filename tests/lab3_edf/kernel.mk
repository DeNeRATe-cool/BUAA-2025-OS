CFLAGS        += -DMOS_SCHED_END_PC=0x400180
CFLAGS        += -DMOS_SCHED_MAX_TICKS=500
CFLAGS        += -DMOS_SCHED_MIN_TICKS=30
init-override := $(test_dir)/init.c
pre-env-run   := $(test_dir)/pre_env_run.c
