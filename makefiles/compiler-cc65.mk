CC := cl65

ASFLAGS += --asm-include-dir src/common --asm-include-dir src/$(CURRENT_PLATFORM) --asm-include-dir src/current-target/$(CURRENT_TARGET)
CFLAGS += --include-dir src/common --include-dir src/$(CURRENT_PLATFORM) --include-dir src/current-target/$(CURRENT_TARGET)

ASFLAGS += --asm-include-dir $(SRCDIR)
CFLAGS += --include-dir $(SRCDIR)

ASFLAGS += --asm-include-dir $(SRCDIR)/include
CFLAGS += --include-dir $(SRCDIR)/include

define _listing_
  CFLAGS += --listing $$(@:.o=.lst)
  ASFLAGS += --listing $$(@:.o=.lst)
endef

define _mapfile_
  LDFLAGS += --mapfile $$@.map
endef

define _labelfile_
  LDFLAGS += -Ln $$@.lbl
endef

CFLAGS += -Osir

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) -o $@ $<

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.s $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(ASFLAGS) -o $@ $<


$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) $(LIBS) | $(BUILD_DIR)
	$(CC) -t $(CURRENT_TARGET) $(LDFLAGS) -o $@ $^
