PRO_DIRS += \
			$(FIRMWARE_DIR)/SourceCode \
			$(FIRMWARE_DIR)/SourceCode/bluetooth  \
			$(FIRMWARE_DIR)/SourceCode/cJson   \
			$(FIRMWARE_DIR)/SourceCode/cmd \
			$(FIRMWARE_DIR)/SourceCode/common \
			$(FIRMWARE_DIR)/SourceCode/crc \
			$(FIRMWARE_DIR)/SourceCode/flash \
			$(FIRMWARE_DIR)/SourceCode/ht_sensor \
			$(FIRMWARE_DIR)/SourceCode/key \
			$(FIRMWARE_DIR)/SourceCode/led \
			$(FIRMWARE_DIR)/SourceCode/msg_deal \
			$(FIRMWARE_DIR)/SourceCode/nrf24l01 \
			$(FIRMWARE_DIR)/SourceCode/oled \
			$(FIRMWARE_DIR)/SourceCode/queue \
			$(FIRMWARE_DIR)/SourceCode/radio \
			$(FIRMWARE_DIR)/SourceCode/region_header \
			$(FIRMWARE_DIR)/SourceCode/rtc \
			$(FIRMWARE_DIR)/SourceCode/show \
			$(FIRMWARE_DIR)/SourceCode/timer \
			$(FIRMWARE_DIR)/SourceCode/uart \
			$(FIRMWARE_DIR)/SourceCode/wdg \
			$(FIRMWARE_DIR)/SourceCode/wifi 

SRC :=   $(wildcard $(FIRMWARE_DIR)/SourceCode/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/bluetooth/*.c)  \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/cJson/*.c)   \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/cmd/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/common/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/crc/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/flash/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/ht_sensor/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/key/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/led/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/msg_deal/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/nrf24l01/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/oled/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/queue/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/radio/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/region_header/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/rtc/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/show/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/timer/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/uart/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/wdg/*.c) \
		 $(wildcard	$(FIRMWARE_DIR)/SourceCode/wifi/*.c)  

$(FIRMWARE_DIR)/SourceCode/libSource.a : $(SRC:%.c=%.o)
$(FIRMWARE_DIR)/build/Ad.axf : $(LIBS)
								