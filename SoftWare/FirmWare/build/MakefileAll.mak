
# directoty for compiler source file
CORE       := ../Core
DRIVER     := ../Driver
SOURCECODE := ../SourceCode

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

CMN_CFLAGS = --C99 -c --cpu Cortex-M4.fp.sp -g --apcs=interwork --split_sections \
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

# compiler exe 
CC_EXEC := armcc.exe 
AR_EXEC := armar.exe
ASM_EXEC := armasm.exe
LINK_EXEC := armlink.exe
FROMELF_EXEC := fromelf.exe
DOWNLOAD_EXEC := ST-LINK_CLI.exe

FIRMWARE_DIR := ..
PRO_DIRS = 

LIBS  := ../Core/libCore.a \
		 ../Driver/libDriver.a \
		 ../SourceCode/libSource.a

.PHONY : build
build : Ad.axf

-include    ../SourceCode/rule.mak ../Driver/rule.mak ../Core/rule.mak

$(info SRC=$(SRC))
Ad.axf : $(LIBS)

VPATH = $(PRO_DIRS)
$(info VPATH=$(VPATH))

%.o:%.s
	@echo "building $<"
	@$(ASM_EXEC)  $(CMN_AFLAGS) $< -o $@

%.o:%.c
	@echo "building $<"
	@$(CC_EXEC) -c $(CMN_CFLAGS) $< -o $@

%.a:
	@echo "building $@"
	@$(AR_EXEC) $@ $^ --create

%.axf:
	@echo "Link $@"
	@$(LINK_EXEC) $(CMN_LFLAGS) $^ -o $@


.PHONY : clean
clean:
	-rm -rf ../Core/*.o  ../Core/*.a
	-rm -rf ../Driver/src/*.o  ../Driver/*.a
	-rm -rf ../SourceCode/*.o  ../SourceCode/*.a ../SourceCode/*/*.o