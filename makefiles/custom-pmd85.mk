# COMPILE FLAGS

# before we will get real fujinet-lib ...
ASFLAGS += -Isrc/$(CURRENT_PLATFORM)/fujinet-lib-stub
CFLAGS += -Isrc/$(CURRENT_PLATFORM)/fujinet-lib-stub

################################################################
# ROM creation

DISK_TASKS += .create-rmm

SUFFIX = 
ROM_FILE = $(DIST_DIR)/$(PROGRAM).rmm

.create-rmm:
	echo "creating $(ROM_FILE) from program $(PROGRAM_TGT)" ; \
	if [ -f "$(ROM_FILE)" ] ; then \
		rm "$(ROM_FILE)" ; \
	fi ; \
	(/bin/printf '\xcd\x00\x8c\x00\x00\xff\x5f\x00\x00'; \
	dd if=$(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) count=4 bs=1 status=none; \
	dd if=$(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) skip=13 bs=1 status=none) > $(ROM_FILE)
