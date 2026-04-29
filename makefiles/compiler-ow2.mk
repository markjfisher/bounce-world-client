CC := wcc
AS := wasm
LD := wlink
AR := wlib

CFLAGS += -0 -bt=dos -fr=$(basename $@).err

CFLAGS += -i=src/common -i=src/$(CURRENT_PLATFORM) -i=src/current-target/$(CURRENT_TARGET)
ASFLAGS += -i=src/common -i=src/$(CURRENT_PLATFORM) -i=src/current-target/$(CURRENT_TARGET)

CFLAGS += -i=$(SRCDIR)
ASFLAGS += -i=$(SRCDIR)

CFLAGS += -i=$(SRCDIR)/include
ASFLAGS += -i=$(SRCDIR)/include

CFLAGS += -D__MSDOS__

LDFLAGS += SYSTEM dos

ifneq ($(strip $(LIBS)),)
LINK_LIBRARY = library {$(LIBS)}
else
LINK_LIBRARY =
endif

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) $(CFLAGS) -ad=$(@:.o=.d) -fo=$@ $<

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.asm $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(AS) -c $(ASFLAGS) -fo=$@ $<

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.s $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(AS) -c $(ASFLAGS) -fo=$@ $<

$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) $(LIBS) | $(BUILD_DIR)
	$(LD) $(LDFLAGS) disable 1014 name $@ file {$(OBJECTS)} $(LINK_LIBRARY)
