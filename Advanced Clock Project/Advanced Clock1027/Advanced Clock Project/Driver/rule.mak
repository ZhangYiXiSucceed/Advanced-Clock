
PRO_DIRS += \
			$(FIRMWARE_DIR)/Driver/src

SRC := $(wildcard $(FIRMWARE_DIR)/Driver/src/*.c)


$(FIRMWARE_DIR)/Driver/libDriver.a:$(SRC:%.c=%.o)
								