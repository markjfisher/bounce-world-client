# COMPILE FLAGS

# until there is a fujinet-lib release for pmd85
# manually copy and unzip the pmd85 library files into _cache/fujinet-lib/4.7.4-pmd85
CFLAGS += -I_cache/fujinet-lib/4.7.4-pmd85
FUJINET_LIB_PATH = _cache/fujinet-lib/4.7.4-pmd85/fujinet-pmd85-4.7.4.lib
LIBS += $(FUJINET_LIB_PATH)

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
