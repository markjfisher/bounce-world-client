# msdos has no stable fujinet-lib release; clone and build from a git URL.
# Override on the command line to use a fork, local path, or branch (URL#branch).
FUJINET_LIB ?= https://github.com/FozzTexx/fujinet-lib-experimental.git

FUJINET_LIB_CACHE := $(CACHE_DIR)/fujinet-lib

# Literal '#' in a make value (so $(subst) can split on it).
HASH := \#

FUJINET_LIB_URL    := $(firstword $(subst $(HASH), ,$(FUJINET_LIB)))
FUJINET_LIB_BRANCH := $(word 2,$(subst $(HASH), ,$(FUJINET_LIB)))

# If FUJINET_LIB points at an existing directory, use it in place. Otherwise
# treat it as a git URL and clone into _cache/fujinet-lib/<basename>.
ifneq (,$(wildcard $(FUJINET_LIB_URL)/.))
FUJINET_LIB_REPO := $(FUJINET_LIB_URL)
else
FUJINET_LIB_REPO := $(FUJINET_LIB_CACHE)/$(basename $(notdir $(patsubst %/,%,$(FUJINET_LIB_URL))))
endif

FUJINET_LIB_INCLUDE := $(FUJINET_LIB_REPO)/include
FUJINET_LIB_DIR     := $(FUJINET_LIB_REPO)/r2r/$(CURRENT_TARGET)
FUJINET_LIB_FILE    := libfujinet.$(CURRENT_TARGET).a

CFLAGS  += -i=$(FUJINET_LIB_INCLUDE)
ASFLAGS += -i=$(FUJINET_LIB_INCLUDE)
LIBS    += $(FUJINET_LIB_DIR)/$(FUJINET_LIB_FILE)

.get_fujinet_lib:
	@set -e; \
	mkdir -p "$(FUJINET_LIB_CACHE)"; \
	if [ ! -d "$(FUJINET_LIB_REPO)" ]; then \
	    echo "Cloning $(FUJINET_LIB_URL) into $(FUJINET_LIB_REPO)"; \
	    git clone $(if $(FUJINET_LIB_BRANCH),-b $(FUJINET_LIB_BRANCH),) "$(FUJINET_LIB_URL)" "$(FUJINET_LIB_REPO)"; \
	fi; \
	if [ ! -f "$(FUJINET_LIB_DIR)/$(FUJINET_LIB_FILE)" ]; then \
	    echo "Building fujinet-lib for $(CURRENT_TARGET) in $(FUJINET_LIB_REPO)"; \
	    unset FUJINET_LIB; \
	    MAKEFLAGS="$$(echo " $$MAKEFLAGS " | sed -E 's/ FUJINET_LIB=[^ ]*//g; s/^ //; s/ $$//')" \
	        $(MAKE) -C "$(FUJINET_LIB_REPO)" $(CURRENT_TARGET)/r2r; \
	fi
