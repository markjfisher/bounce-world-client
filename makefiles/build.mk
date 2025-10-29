# Ensure WSL2 Ubuntu and other linuxes use bash by default instead of /bin/sh, which does not always like the shell commands.
SHELL := /usr/bin/env bash
ALL_TASKS =
DISK_TASKS =

-include ./makefiles/os.mk

SRCDIR := src
BUILD_DIR := build
OBJDIR := obj
DIST_DIR := dist
CACHE_DIR := _cache

# This allows src to be nested withing sub-directories.
rwildcard=$(wildcard $(1)$(2))$(foreach d,$(wildcard $1*), $(call rwildcard,$d/,$2))

PROGRAM_TGT := $(PROGRAM).$(CURRENT_TARGET)

SOURCES := $(wildcard $(SRCDIR)/*.c)
SOURCES += $(wildcard $(SRCDIR)/*.s)
SOURCES += $(wildcard $(SRCDIR)/*.asm)

# allow for a src/common/ dir and recursive subdirs
ifeq ($(CURRENT_TARGET),pmd85)
SOURCES += $(call rwildcard,$(SRCDIR)/common/,*.asm)
else
SOURCES += $(call rwildcard,$(SRCDIR)/common/,*.s)
endif
SOURCES += $(call rwildcard,$(SRCDIR)/common/,*.c)

# allow src/<platform>/ and its recursive subdirs
ifeq ($(CURRENT_TARGET),pmd85)
SOURCES_PF := $(call rwildcard,$(SRCDIR)/$(CURRENT_PLATFORM)/,*.asm)
else
SOURCES_PF := $(call rwildcard,$(SRCDIR)/$(CURRENT_PLATFORM)/,*.s)
endif
SOURCES_PF += $(call rwildcard,$(SRCDIR)/$(CURRENT_PLATFORM)/,*.c)

# allow src/current-target/<target>/ and its recursive subdirs
ifeq ($(CURRENT_TARGET),pmd85)
SOURCES_TG := $(call rwildcard,$(SRCDIR)/current-target/$(CURRENT_TARGET)/,*.asm)
else
SOURCES_TG := $(call rwildcard,$(SRCDIR)/current-target/$(CURRENT_TARGET)/,*.s)
endif
SOURCES_TG += $(call rwildcard,$(SRCDIR)/current-target/$(CURRENT_TARGET)/,*.c)

# remove trailing and leading spaces.
SOURCES := $(strip $(SOURCES))
SOURCES_PF := $(strip $(SOURCES_PF))
SOURCES_TG := $(strip $(SOURCES_TG))

# convert from src/your/long/path/foo.[c|s] to obj/<target>/your/long/path/foo.o
# we need the target because compiling for previous target does not pick up potential macro changes
OBJ1 := $(SOURCES:.c=.o)
OBJECTS := $(OBJ1:.s=.o)
OBJECTS := $(OBJECTS:.asm=.o)
OBJECTS := $(OBJECTS:$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)

OBJ2 := $(SOURCES_PF:.c=.o)
OBJECTS_PF := $(OBJ2:.s=.o)
OBJECTS_PF := $(OBJECTS_PF:.asm=.o)
OBJECTS_PF := $(OBJECTS_PF:$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)

OBJ3 := $(SOURCES_TG:.c=.o)
OBJECTS_TG := $(OBJ3:.s=.o)
OBJECTS_TG := $(OBJECTS_TG:.asm=.o)
OBJECTS_TG := $(OBJECTS_TG:$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)

OBJECTS += $(OBJECTS_PF)
OBJECTS += $(OBJECTS_TG)

# Ensure make recompiles parts it needs to if src files change
DEPENDS := $(OBJECTS:.o=.d)

#
# load the sub-makefiles
#

-include ./makefiles/common.mk
-include ./makefiles/custom-$(CURRENT_PLATFORM).mk

# allow for application specific config
-include ./application.mk

# allow for local env specific deployment options
-include ./deployment.mk

ifeq ($(CURRENT_TARGET),pmd85)
-include ./makefiles/compiler-z88dk.mk
else ifeq ($(CURRENT_TARGET),coco)
-include ./makefiles/compiler-cmoc.mk
-include ./makefiles/hirestxt-mod-lib.mk
else
-include ./makefiles/compiler-cc65.mk
endif

.SUFFIXES:
.PHONY: all clean release $(DISK_TASKS) $(BUILD_TASKS) $(PROGRAM_TGT) $(ALL_TASKS)

all: $(ALL_TASKS) $(PROGRAM_TGT)

STATEFILE := Makefile.options
-include $(DEPENDS)
-include $(STATEFILE)

ifeq ($(origin _OPTIONS_),file)
OPTIONS = $(_OPTIONS_)
$(eval $(OBJECTS): $(STATEFILE))
endif

# Transform the abstract OPTIONS to the actual cc65 options.
$(foreach o,$(subst $(COMMA),$(SPACE),$(OPTIONS)),$(eval $(_$o_)))

ifeq ($(BUILD_DIR),)
BUILD_DIR := build
endif

ifeq ($(OBJDIR),)
OBJDIR := obj
endif

ifeq ($(DIST_DIR),)
DIST_DIR := dist
endif

$(OBJDIR):
	$(call MKDIR,$@)

$(BUILD_DIR):
	$(call MKDIR,$@)

$(DIST_DIR):
	$(call MKDIR,$@)

SRC_INC_DIRS := \
  $(sort $(dir $(wildcard $(SRCDIR)/$(CURRENT_TARGET)/*))) \
  $(sort $(dir $(wildcard $(SRCDIR)/common/*))) \
  $(SRCDIR)

vpath %.c $(SRC_INC_DIRS)
vpath %.s $(SRC_INC_DIRS)
vpath %.asm $(SRC_INC_DIRS)

$(PROGRAM_TGT): $(BUILD_DIR)/$(PROGRAM_TGT) | $(BUILD_DIR)

test: $(PROGRAM_TGT)
	$(PREEMUCMD)
	$(EMUCMD) $(BUILD_DIR)\\$<
	$(POSTEMUCMD)

# Use "./" in front of all dirs being removed as a simple safety guard to
# ensure deleting from current dir, and not something like root "/".
clean:
	@for d in $(BUILD_DIR) $(OBJDIR) $(DIST_DIR); do \
      if [ -d "./$$d" ]; then \
	    echo "Removing $$d"; \
        rm -rf ./$$d; \
      fi; \
    done

release: all | $(BUILD_DIR) $(DIST_DIR)
	cp $(BUILD_DIR)/$(PROGRAM_TGT) $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX)

disk: release $(DISK_TASKS)
