ifneq ($(CURRENT_TARGET),pmd85)
-include ./makefiles/fujinet-lib.mk
endif

VERSION_FILE := src/version.txt
ifeq (,$(wildcard $(VERSION_FILE)))
VERSION_FILE =
VERSION_STRING =
else
VERSION_STRING := $(file < $(VERSION_FILE))
endif

