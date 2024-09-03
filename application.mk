ASFLAGS += --asm-include-dir $(SRCDIR)/include
CFLAGS += --include-dir $(SRCDIR)/include

ifneq ($(ENDPOINT_URL),)
$(info ENDPOINT_URL: $(ENDPOINT_URL))
CFLAGS += -DENDPOINT_URL=\"$(ENDPOINT_URL)\"
ASFLAGS += -DENDPOINT_URL=\"$(ENDPOINT_URL)\"
endif
