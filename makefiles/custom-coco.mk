# COMPILE FLAGS

################################################################
# DISK creation

SUFFIX = .bin
DISK_TASKS += .create-dsk

DISK_FILE = $(DIST_DIR)/$(PROGRAM).dsk

.create-dsk:
	@which decb > /dev/null 2>&1 ; \
	if [ $$? -eq 0 ] ; then \
		echo "creating $(DISK_FILE) from program $(PROGRAM_TGT)" ; \
		if [ -f "$(DISK_FILE)" ] ; then \
			rm "$(DISK_FILE)" ; \
		fi ; \
                decb dskini $(DISK_FILE) ; \
		writecocofile $(DISK_FILE) $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) ; \
	else \
		echo -e "\nERROR! You must install toolshed from https://github.com/nitros9project/toolshed to create coco disks\n" ; \
		exit 1 ; \
	fi
