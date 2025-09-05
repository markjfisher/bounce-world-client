CC := cmoc

ASFLAGS += -I src/common -I src/$(CURRENT_PLATFORM) -I src/current-target/$(CURRENT_TARGET)
CFLAGS += -I src/common -I src/$(CURRENT_PLATFORM) -I src/current-target/$(CURRENT_TARGET)

ASFLAGS += -I $(SRCDIR)
CFLAGS += -I $(SRCDIR)

ASFLAGS += -I $(SRCDIR)/include
CFLAGS += -I $(SRCDIR)/include

CFLAGS += -Wno-const


LDFLAGS += $(foreach lib, $(LIBS), -L $(dir $(lib)))

LIBFLAGS := $(foreach lib,$(LIBS),-l$(notdir $(lib)))

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/$(CURRENT_TARGET)/%.o: %.asm $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
	$(CC) -c -o $@ $<

$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) $(LIBS) | $(BUILD_DIR)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS) $(LIBFLAGS)
