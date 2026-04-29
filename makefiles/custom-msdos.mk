################################################################
# DISK creation

SUFFIX = .exe
DISK_TASKS += .img

DISK_FILE = $(DIST_DIR)/$(PROGRAM).img
DISK_SIZE_KB := 360

MSDOS_CACHE_DIR := $(CACHE_DIR)/msdos

.img:
	@$(call require,mformat,https://www.gnu.org/software/mtools/)
	@$(call require,mcopy,https://www.gnu.org/software/mtools/)
	$(call MKDIR,$(DIST_DIR))
	$(call MKDIR,$(MSDOS_CACHE_DIR))
	@if [ -f $(DISK_FILE) ] ; then rm $(DISK_FILE) ; fi
	dd if=/dev/zero of=$(DISK_FILE) bs=1024 count=$(DISK_SIZE_KB)
	mformat -i $(DISK_FILE) -f $(DISK_SIZE_KB)
	mcopy -i $(DISK_FILE) $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX) "::/$(PROGRAM)$(SUFFIX)"

################################################################
# require helper (used above)

define require
	@command -v $(1) >/dev/null 2>&1 || { \
		echo >&2 ""; \
		echo >&2 "ERROR - Missing required tool: $(1)"; \
		echo >&2 "Get it here: $(2)"; \
		echo >&2 ""; \
		exit 1; \
	}
endef
