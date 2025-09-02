CC := cmoc

ASFLAGS += -Isrc/common -Isrc/$(CURRENT_PLATFORM) -Isrc/current-target/$(CURRENT_TARGET)
CFLAGS += -Isrc/common -Isrc/$(CURRENT_PLATFORM) -Isrc/current-target/$(CURRENT_TARGET)

ASFLAGS += -I$(SRCDIR)
CFLAGS += -I$(SRCDIR)

ASFLAGS += -I$(SRCDIR)/include
CFLAGS += -I$(SRCDIR)/include

LIBFLAGS := $(foreach lib,$(LIBS),-l$(lib))

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.asm $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -c -o $@ $<

$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) $(LIBS) | $(BUILD_DIR)
	$(CC) -o $@ $(OBJECTS) $(LIBFLAGS)
