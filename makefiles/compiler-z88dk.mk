CC := zcc

ifneq ($(CURRENT_PLATFORM), adam)
TARGET_TYPE := +$(CURRENT_TARGET)
CFLAGS += -create-app
else
TARGET_TYPE := +coleco -subtype=adam
endif

ASFLAGS += -Isrc/common -Isrc/$(CURRENT_PLATFORM) -Isrc/current-target/$(CURRENT_TARGET)
CFLAGS += -Isrc/common -I/usr/local/share/z88dk/include/ -Isrc/$(CURRENT_PLATFORM) -Isrc/current-target/$(CURRENT_TARGET)

ASFLAGS += -I$(SRCDIR)
CFLAGS += -I$(SRCDIR)

ASFLAGS += -I$(SRCDIR)/include
CFLAGS += -I$(SRCDIR)/include

LDFLAGS += -create-app

LIBFLAGS := $(foreach lib,$(LIBS),-l$(lib))

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) $(TARGET_TYPE) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.asm $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) $(TARGET_TYPE) -c $(ASFLAGS) -o $@ $<


$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(TARGET_TYPE) $(SUBTYPE) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBFLAGS)
