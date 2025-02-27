# until there is a fujinet-lib release for pmd85
# manually copy and unzip the pmd85 library files into _libs/4.7.4-pmd85
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

