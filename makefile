JO_COMPILE_WITH_VIDEO_MODULE = 0
JO_COMPILE_WITH_BACKUP_MODULE = 0
JO_COMPILE_WITH_TGA_MODULE = 1
JO_COMPILE_WITH_AUDIO_MODULE = 0
JO_COMPILE_WITH_3D_MODULE = 1
JO_COMPILE_WITH_PSEUDO_MODE7_MODULE = 0
JO_COMPILE_WITH_EFFECTS_MODULE = 0
JO_COMPILE_WITH_RAM_CARD_MODULE = 0
JO_GLOBAL_MEMORY_SIZE_FOR_MALLOC = 262144
JO_DEBUG = 1
SRCS = main.c ZT/ZT_LOAD_MODEL.c ZT/ZT_TOOLS.c animate.c input.c game_object.c timer.c collision.c separate.c test2D.c separate_3D.c
JO_ENGINE_SRC_DIR=../../jo_engine
COMPILER_DIR=../../Compiler
include $(COMPILER_DIR)/COMMON/jo_engine_makefile
