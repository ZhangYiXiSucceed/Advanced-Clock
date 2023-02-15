
PRO_DIRS += \
			$(FIRMWARE_DIR)/Core

ASM := $(wildcard $(FIRMWARE_DIR)/Core/startup_stm32f40_41xxx.s)
SRC := $(wildcard $(FIRMWARE_DIR)/Core/*.c)


$(FIRMWARE_DIR)/Core/libCore.a: $(ASM:%.s=%.o)\
								$(SRC:%.c=%.o)
