# ###########################################################
# File  : makfile for project compiler
# Author: guoqing.zhang
# date  : 2022/11/20
###########################################################

# 
TARGET = AdvancedClock
DEBUG = -O2

# directoty for compiler object file
BUILD_OBJ_DIR  :=obj
BUILD_DEP_DIR  :=dep
OUT_DIR    :=out

# directoty for compiler source file
CORE       := ../Core
DRIVER     := ../Driver
SOURCECODE := ../SourceCode

# source file path
C_SOURCE = \
$(wildcard $(CORE)/*.c)  				

C_SOURCE += \
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
$(wildcard $(DRIVER)/src/stm32f4xx_wwdg.c)  		

C_SOURCE += \
$(wildcard $(SOURCECODE)/*.c)  			\
$(wildcard $(SOURCECODE)/bluetooth/*.c) \
$(wildcard $(SOURCECODE)/cJson/*.c)  	\
$(wildcard $(SOURCECODE)/common/*.c)  	\
$(wildcard $(SOURCECODE)/ht_sensor/*.c)  \
$(wildcard $(SOURCECODE)/key/*.c)  		\
$(wildcard $(SOURCECODE)/led/*.c)  		\
$(wildcard $(SOURCECODE)/msg_deal/*.c)  \
$(wildcard $(SOURCECODE)/flash/*.c)  \
$(wildcard $(SOURCECODE)/nrf24l01/*.c)  \
$(wildcard $(SOURCECODE)/oled/*.c)  	\
$(wildcard $(SOURCECODE)/queue/*.c)  	\
$(wildcard $(SOURCECODE)/rtc/*.c)  		\
$(wildcard $(SOURCECODE)/timer/*.c)  	\
$(wildcard $(SOURCECODE)/uart/*.c)  	\
$(wildcard $(SOURCECODE)/wdg/*.c)  		\
$(wildcard $(SOURCECODE)/wifi/*.c)  	\
$(wildcard $(SOURCECODE)/show/*.c)  	\
$(wildcard $(SOURCECODE)/region_header/*.c)  	\
$(wildcard $(SOURCECODE)/cmd/*.c)  	\
$(wildcard $(SOURCECODE)/bank/*.c)  	\

C_SOURCE += \

# asm source file path
ASM_SOURCE = \
$(wildcard $(CORE)/startup_stm32f40_41xxx.s)  				\

# inlcude file path
C_INCLUDE  =							\
-I$(CORE)/     							\
-I$(DRIVER)/inc/     					\
-I$(SOURCECODE)/     					\
-I$(SOURCECODE)/bluetooth/    			\
-I$(SOURCECODE)/cJson/    				\
-I$(SOURCECODE)/common/    				\
-I$(SOURCECODE)/key/    				\
-I$(SOURCECODE)/ht_sensor/    			\
-I$(SOURCECODE)/led/    				\
-I$(SOURCECODE)/msg_deal/    			\
-I$(SOURCECODE)/nrf24l01/    			\
-I$(SOURCECODE)/oled/    				\
-I$(SOURCECODE)/queue/    				\
-I$(SOURCECODE)/radio/    				\
-I$(SOURCECODE)/rtc/    				\
-I$(SOURCECODE)/timer/    				\
-I$(SOURCECODE)/uart/    				\
-I$(SOURCECODE)/wdg/    				\
-I$(SOURCECODE)/wifi/    				\
-I$(SOURCECODE)/show/    				\
-I$(SOURCECODE)/region_header/    		\
-I$(SOURCECODE)/cmd/    				\
-I$(SOURCECODE)/flash/    				\
-I$(SOURCECODE)/bank/    				\

# compiler exe 
CC_EXEC := armcc.exe 
AR_EXEC := armar.exe
ASM_EXEC := armasm.exe
LINK_EXEC := armlink.exe
FROMELF_EXEC := fromelf.exe
DOWNLOAD_EXEC := ST-LINK_CLI.exe

RM   := rm -rf 
ECHO := echo
MKDIR := mkdir
PYTHON := python 


CMN_CFLAGS = -c --C99  --cpu Cortex-M4.fp.sp -g --apcs=interwork --split_sections \
--use_frame_pointer \
$(DEBUG) $(C_INCLUDE)  \
--diag_suppress=1295,111,1293,167,513,177 \
-ID:/Software/Keil/pack/Keil/STM32F4xx_DFP/2.13.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
-D__UVISION_VERSION="537"    \
-DSTM32F405xx \
-DSTM32F40_41xxx \
-DUSE_STDPERIPH_DRIVER 

CMN_AFLAGS = --cpu Cortex-M4.fp.sp -g --apcs=interwork \
-I D:\Software\Keil\pack\Keil\STM32F4xx_DFP\2.13.0\Drivers\CMSIS\Device\ST\STM32F4xx\Include \
--pd "__UVISION_VERSION SETA 537" --pd "STM32F405xx SETA 1" \

LINK_SCRIPT := AdvanceClock.sct
CMN_LFLAGS = --cpu Cortex-M4.fp.sp \
--strict --scatter $(LINK_SCRIPT) \
--remove


# compiler objects
OBJECTS  = $(addprefix $(BUILD_OBJ_DIR)/,$(notdir $(C_SOURCE:.c=.o)))
 vpath %.c $(sort $(dir $(C_SOURCE)))

OBJECTS  += $(addprefix $(BUILD_OBJ_DIR)/,$(notdir $(ASM_SOURCE:.s=.o)))
 vpath %.s $(sort $(dir $(ASM_SOURCE)))

DEPS = $(addprefix $(BUILD_DEP_DIR)/,$(notdir $(C_SOURCE:.c=.d)))
 vpath %.c $(sort $(dir $(C_SOURCE)))

$(shell mkdir -p $(OUT_DIR))
$(shell mkdir -p $(BUILD_OBJ_DIR))
$(shell mkdir -p $(BUILD_DEP_DIR))

ifeq ($(silent), )
MAKEFLAGS += --silent
endif

.PHONY : precompile
precompile : $(BUILD_OBJ_DIR)/$(f).i

.PHONY : build
build :  $(OUT_DIR)/$(TARGET).axf


$(OUT_DIR)/$(TARGET).axf: $(OBJECTS)
	$(LINK_EXEC) $(OBJECTS) $(CMN_LFLAGS) -o $@ --map --info totals --list_mapping_symbols --list=$(OUT_DIR)/$(TARGET).map 
	$(ECHO) image $(TARGET).bin ...
	$(FROMELF_EXEC) --bin --output $(OUT_DIR)/$(TARGET) $@

-include $(DEPS)

$(BUILD_OBJ_DIR)/%.o : %.s Makefile 
	$(ECHO) 'Compiling' $<
	$(ASM_EXEC)  $(CMN_AFLAGS) $< -o $@ 

# compiler process
$(BUILD_OBJ_DIR)/%.o : %.d  
$(BUILD_OBJ_DIR)/%.o : %.c  Makefile 
	$(ECHO)  'Compiling' $<
	$(CC_EXEC) -c  $(CMN_CFLAGS) $< -o $@ 

$(BUILD_DEP_DIR)/%.d : OBJ_BNAME=$(basename $(notdir $@))
$(BUILD_DEP_DIR)/%.d : %.c  Makefile 
	$(ECHO) 'Update dependency Compiling' $<
	$(CC_EXEC) -M   $(CMN_CFLAGS) $< -o "$(BUILD_OBJ_DIR)/$(OBJ_BNAME).o $(BUILD_DEP_DIR)/$(OBJ_BNAME).d"  >  $@
	$(CC_EXEC) -c   $(CMN_CFLAGS) $< -o $(BUILD_OBJ_DIR)/$(OBJ_BNAME).o

$(BUILD_OBJ_DIR)/$(f).i : ../SourceCode/$(f).c
	$(ECHO)  'Precompiling' $<
	$(CC_EXEC) -c  -E $(CMN_CFLAGS) $< -o $@ 

#package
.PHONY : package
package:
	$(PYTHON) package.py ./out/AdvancedClock

# dwonload 
.PHONY : flash
flash:
	$(DOWNLOAD_EXEC) -c -SE 4 7  -V "while_programming" -P $(OUT_DIR)/$(TARGET).bin 0x08010000
	$(DOWNLOAD_EXEC) -Rst


# clean 
.PHONY : clean
clean:
	$(ECHO) 'clean obj and dep ...' 
	$(RM) *.o
	$(RM) obj/*
	$(RM) obj_boot/*
	$(RM) dep/*
	$(RM) dep_boot/*
	$(RM) out/*