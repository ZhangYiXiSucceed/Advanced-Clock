# ###########################################################
# File  : makfile for project compiler
# Author: guoqing.zhang
# date  : 2022/11/20
###########################################################

# 
TARGET = AdvancedClockBoot
DEBUG = -O2

# directoty for compiler object file
BUILD_OBJ_DIR  :=obj_boot
BUILD_DEP_DIR  :=dep_boot
OUT_DIR    :=out

# directoty for compiler source file
CORE       := ../Core
DRIVER     := ../Driver
SOURCECODE := ../SourceCode

# source file path
C_SOURCE = \
$(wildcard $(CORE)/*.c)  				\
$(wildcard $(DRIVER)/src/misc.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_adc.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_dac.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_dma.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_dma2d.c)  		\
$(wildcard $(DRIVER)/src/stm32f4xx_exti.c)  		\
$(wildcard $(DRIVER)/src/stm32f4xx_flash.c)  		\
$(wildcard $(DRIVER)/src/stm32f4xx_gpio.c)  		\
$(wildcard $(DRIVER)/src/stm32f4xx_iwdg.c)  		\
$(wildcard $(DRIVER)/src/stm32f4xx_pwr.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_rcc.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_rtc.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_spi.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_syscfg.c)  		\
$(wildcard $(DRIVER)/src/stm32f4xx_tim.c)  			\
$(wildcard $(DRIVER)/src/stm32f4xx_usart.c)  		\
$(wildcard $(DRIVER)/src/stm32f4xx_wwdg.c)  		\
$(wildcard $(SOURCECODE)/common/*.c)  	\
$(wildcard $(SOURCECODE)/msg_deal/*.c)  \
$(wildcard $(SOURCECODE)/queue/*.c)  	\
$(wildcard $(SOURCECODE)/timer/*.c)  	\
$(wildcard $(SOURCECODE)/uart/*.c)  	\
$(wildcard $(SOURCECODE)/wdg/*.c)  		\
$(wildcard $(SOURCECODE)/wifi/*.c)  	\
$(wildcard $(SOURCECODE)/flash/*.c)  	\
$(wildcard $(SOURCECODE)/bootloader/*.c)  	\
$(wildcard $(SOURCECODE)/region_header/*.c)  	\
$(wildcard $(SOURCECODE)/crc/*.c)  	\

# asm source file path
ASM_SOURCE = \
$(wildcard $(CORE)/startup_stm32f40_41xxx.s)  				\

# inlcude file path
C_INCLUDE  =							\
-I$(CORE)/     							\
-I$(DRIVER)/inc/     					\
-I$(SOURCECODE)/     					\
-I$(SOURCECODE)/common/    				\
-I$(SOURCECODE)/msg_deal/    			\
-I$(SOURCECODE)/queue/    				\
-I$(SOURCECODE)/timer/    				\
-I$(SOURCECODE)/uart/    				\
-I$(SOURCECODE)/wdg/    				\
-I$(SOURCECODE)/wifi/    				\
-I$(SOURCECODE)/flash/    				\
-I$(SOURCECODE)/bootloader/    			\
-I$(SOURCECODE)/bluetooth/    			\
-I$(SOURCECODE)/cJson/    				\
-I$(SOURCECODE)/common/    				\
-I$(SOURCECODE)/key/    				\
-I$(SOURCECODE)/ht_sensor/    			\
-I$(SOURCECODE)/led/    				\
-I$(SOURCECODE)/nrf24l01/    			\
-I$(SOURCECODE)/oled/    				\
-I$(SOURCECODE)/radio/    				\
-I$(SOURCECODE)/rtc/    				\
-I$(SOURCECODE)/show/    				\
-I$(SOURCECODE)/region_header/    		\
-I$(SOURCECODE)/crc/    				\

# compiler exe 
CC_EXEC := armcc.exe 
AR_EXEC := armar.exe
ASM_EXEC := armasm.exe
LINK_EXEC := armlink.exe
FROMELF_EXEC := fromelf.exe
DOWNLOAD_EXEC := ST-LINK_CLI.exe

RM   := rm -rf 
ECHO := echo


CMN_CFLAGS = --C99 -c --cpu Cortex-M4.fp.sp -g --apcs=interwork --split_sections \
$(DEBUG) $(C_INCLUDE)  \
--diag_suppress=1295,111,1293,167,513,177 \
-ID:/Software/Keil/pack/Keil/STM32F4xx_DFP/2.13.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
-D__UVISION_VERSION="537"    \
-DSTM32F405xx \
-DSTM32F40_41xxx \
-DUSE_STDPERIPH_DRIVER \
-DBOOT

CMN_AFLAGS = --cpu Cortex-M4.fp.sp -g --apcs=interwork \
-I D:\Software\Keil\pack\Keil\STM32F4xx_DFP\2.13.0\Drivers\CMSIS\Device\ST\STM32F4xx\Include \
--pd "__UVISION_VERSION SETA 537" --pd "STM32F405xx SETA 1" \

LINK_SCRIPT := AdvanceClockBoot.sct
CMN_LFLAGS = --cpu Cortex-M4.fp.sp \
--strict --scatter $(LINK_SCRIPT)


# compiler objects
OBJECTS  = $(addprefix $(BUILD_OBJ_DIR)/,$(notdir $(C_SOURCE:.c=.o)))
 vpath %.c $(sort $(dir $(C_SOURCE)))

OBJECTS  += $(addprefix $(BUILD_OBJ_DIR)/,$(notdir $(ASM_SOURCE:.s=.o)))
 vpath %.s $(sort $(dir $(ASM_SOURCE)))


.PHONY : build
build :  $(OUT_DIR)/$(TARGET).axf

$(OUT_DIR)/$(TARGET).axf: $(OBJECTS) 
	@$(ECHO) Link $@ ...
	@$(LINK_EXEC) $(OBJECTS) $(CMN_LFLAGS) -o $@ --map --info totals --list_mapping_symbols --list=$(OUT_DIR)/$(TARGET).map 
	@$(FROMELF_EXEC) --bin --output $(OUT_DIR)/$(TARGET).bin $@

$(BUILD_OBJ_DIR)/%.o : %.s Makefile | $(BUILD_OBJ_DIR)
	@$(ECHO) 'Compiling' $<
	@$(ASM_EXEC)  $(CMN_AFLAGS) $< -o $@ 

# compiler process
$(BUILD_OBJ_DIR)/%.o : %.d Makefile | $(BUILD_DEP_DIR)
$(BUILD_OBJ_DIR)/%.o : %.c Makefile | $(BUILD_OBJ_DIR)
	@$(ECHO)  'Compiling' $<
	@$(CC_EXEC) -c  $(CMN_CFLAGS) $< -o $@ 

$(BUILD_DEP_DIR)/%.d : OBJ_BNAME=$(basename $(notdir $@)) 
$(BUILD_DEP_DIR)/%.d : %.c Makefile | $(BUILD_DEP_DIR)
	$(CC_EXEC) -M   $(CMN_CFLAGS) $< -o $(BUILD_OBJ_DIR)/$(OBJ_BNAME).o > $@
	$(CC_EXEC) -c   $(CMN_CFLAGS) $< -o $(BUILD_OBJ_DIR)/$(OBJ_BNAME).o


$(OUT_DIR):
	mkdir $@

$(BUILD_OBJ_DIR):
	mkdir $@

$(BUILD_DEP_DIR):
	mkdir $@

# dwonload 
.PHONY : flash
flash:
	$(DOWNLOAD_EXEC) -c -SE 0 1  -V "while_programming" -P $(OUT_DIR)/$(TARGET).bin 0x08000000
	$(DOWNLOAD_EXEC) -Rst


# clean 
.PHONY : clean
clean:
	$(RM) obj_boot/*
	$(RM) out/*