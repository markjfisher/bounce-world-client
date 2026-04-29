# msdos has no stable fujinet-lib release; resolve via fnlib.py from a git URL.
# Override on the command line to use a fork, local path, or branch (URL#branch).
FUJINET_LIB ?= https://github.com/FozzTexx/fujinet-lib-experimental.git

FNLIB_CACHE := $(CACHE_DIR)/fnlib-$(CURRENT_TARGET).mk

$(info CACHE_DIR = $(CACHE_DIR))
$(info FUJINET_LIB = $(FUJINET_LIB))

ifeq (,$(wildcard $(FNLIB_CACHE)))
$(shell mkdir -p $(CACHE_DIR))
$(info Resolving fujinet-lib for $(CURRENT_TARGET) from $(FUJINET_LIB))
FNLIB_RC := $(shell PLATFORM=$(CURRENT_TARGET) CACHE_DIR=$(CACHE_DIR) python3 ./makefiles/fnlib.py "$(FUJINET_LIB)" > $(FNLIB_CACHE).tmp && mv $(FNLIB_CACHE).tmp $(FNLIB_CACHE) && echo ok)
ifneq ($(FNLIB_RC),ok)
$(error fnlib.py failed to resolve FUJINET_LIB=$(FUJINET_LIB); see output above. Remove $(FNLIB_CACHE).tmp before retrying)
endif
endif

-include $(FNLIB_CACHE)

ifeq (,$(FUJINET_LIB_INCLUDE))
$(error FUJINET_LIB_INCLUDE empty after fnlib.py; remove $(FNLIB_CACHE) and retry)
endif

CFLAGS  += -i=$(FUJINET_LIB_INCLUDE)
ASFLAGS += -i=$(FUJINET_LIB_INCLUDE)
LIBS    += $(FUJINET_LIB_DIR)/$(FUJINET_LIB_FILE)
