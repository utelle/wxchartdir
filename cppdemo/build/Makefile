# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug_win32
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug_win32)
  cppdemoall_config = debug_win32

else ifeq ($(config),debug_win64)
  cppdemoall_config = debug_win64

else ifeq ($(config),release_win32)
  cppdemoall_config = release_win32

else ifeq ($(config),release_win64)
  cppdemoall_config = release_win64

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := cppdemoall

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

cppdemoall:
ifneq (,$(cppdemoall_config))
	@echo "==== Building cppdemoall ($(cppdemoall_config)) ===="
	@${MAKE} --no-print-directory -C . -f cppdemoall.make config=$(cppdemoall_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f cppdemoall.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug_win32"
	@echo "  debug_win64"
	@echo "  release_win32"
	@echo "  release_win64"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   cppdemoall"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"