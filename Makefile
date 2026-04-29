# Generic CC65 TARGETS makefile++
#
# Set the TARGETS and PROGRAM values as required.
# See makefiles/build.mk for details on directory structure for src files and how to add custom extensions to the build.

TARGETS = apple2enh atari c64 coco adam msdos
PROGRAM := bwc

SUB_TASKS := clean disk test release
.PHONY: all help $(SUB_TASKS) $(TARGETS)

# Parse the command-line goals into a target subset and a sub-task subset, so
# `make msdos disk` builds disk only for msdos, not all platforms.
TARGETS_IN_GOALS  := $(filter $(TARGETS),$(MAKECMDGOALS))
SUBTASKS_IN_GOALS := $(filter $(SUB_TASKS),$(MAKECMDGOALS))
SELECTED_TARGETS  := $(if $(TARGETS_IN_GOALS),$(TARGETS_IN_GOALS),$(TARGETS))

all:
	@for target in $(SELECTED_TARGETS); do \
		echo "-------------------------------------"; \
		echo "Building $$target"; \
		echo "-------------------------------------"; \
		if [ "$$target" != "msdos" ] && [ "$$target" != "pmd85" ]; then \
			$(MAKE) --no-print-directory -f ./makefiles/build.mk CURRENT_TARGET=$$target PROGRAM=$(PROGRAM) .get_fujinet_lib || exit 1; \
		fi; \
		if [ "$$target" = "coco" ]; then \
			$(MAKE) --no-print-directory -f ./makefiles/build.mk CURRENT_TARGET=$$target PROGRAM=$(PROGRAM) .get_hirestxt_lib || exit 1; \
		fi; \
		$(MAKE) --no-print-directory -f ./makefiles/build.mk CURRENT_TARGET=$$target PROGRAM=$(PROGRAM) $(SUBTASKS_IN_GOALS); \
	done

# Per-target build entry points, e.g. `make msdos` or `make atari`.
# When a sub-task is also requested (e.g. `make msdos disk`), the sub-task path
# (all:) handles the build, so this rule is a no-op to avoid duplicate work.
$(TARGETS):
ifeq ($(SUBTASKS_IN_GOALS),)
	@echo "-------------------------------------"
	@echo "Building $@"
	@echo "-------------------------------------"
	@if [ "$@" != "msdos" ] && [ "$@" != "pmd85" ]; then \
		$(MAKE) --no-print-directory -f ./makefiles/build.mk CURRENT_TARGET=$@ PROGRAM=$(PROGRAM) .get_fujinet_lib || exit 1; \
	fi
	@if [ "$@" = "coco" ]; then \
		$(MAKE) --no-print-directory -f ./makefiles/build.mk CURRENT_TARGET=$@ PROGRAM=$(PROGRAM) .get_hirestxt_lib || exit 1; \
	fi
	$(MAKE) --no-print-directory -f ./makefiles/build.mk CURRENT_TARGET=$@ PROGRAM=$(PROGRAM)
else
	@:
endif

$(SUB_TASKS): _do_all
$(SUB_TASKS):
	@:

_do_all: all

help:
	@echo "Makefile for $(PROGRAM)"
	@echo ""
	@echo "Available tasks:"
	@echo "all       - do all compilation tasks, create app in build directory"
	@echo "clean     - remove all build artifacts"
	@echo "disk      - generate platform specific disk versions of application (PO/ATR etc)"
	@echo "test      - run application in emulator for given platform."
	@echo "            specific platforms may expose additional variables to run with"
	@echo "            different emulators, see makefiles/custom-<platform>.mk"
	@echo "release   - create a release of the executable in the dist/ dir"
	@echo ""
	@echo "Per-target build: make <platform>  (e.g. make msdos)"
	@echo "Supported per-target platforms: $(TARGETS)"
