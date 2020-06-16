PROJECT_NAME     := standalone-error-program
TARGETS          := linker_nrf52833_xxaa
OUTPUT_DIRECTORY := _build

PROJ_DIR := ../..
NRFX_ROOT := ../../nrfx
NRFXSDK_ROOT := ../../nrfx-support
TOOLCHAIN_ROOT := ../../toolchain

$(OUTPUT_DIRECTORY)/linker_nrf52833_xxaa.out: \
  LINKER_SCRIPT  := gcc_nrf52.ld

# Source files common to all targets
SRC_FILES += \
  $(NRFX_ROOT)/mdk/gcc_startup_nrf52833.S \
  $(NRFX_ROOT)/mdk/system_nrf52833.c \
  $(PROJ_DIR)/MicroBitSAPanicFont.c \
  $(PROJ_DIR)/MicroBitSAPanic.c \
  $(PROJ_DIR)/main_linker.c \

# Include folders common to all targets
INC_FOLDERS += \
  $(NRFX_ROOT)/mdk \
  $(NRFX_ROOT) \
  ../config \
  $(NRFX_ROOT)/hal \
  $(NRFXSDK_ROOT) \
  $(NRFXSDK_ROOT)/cmsis \
  $(PROJ_DIR) \

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -Os -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DCONFIG_NFCT_PINS_AS_GPIOS
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DNRF52833_XXAA
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT)
# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DCONFIG_NFCT_PINS_AS_GPIOS
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DNRF52833_XXAA

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(NRFX_ROOT)/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

linker_nrf52833_xxaa: CFLAGS += -D__HEAP_SIZE=256
linker_nrf52833_xxaa: CFLAGS += -D__STACK_SIZE=256
linker_nrf52833_xxaa: ASMFLAGS += -D__HEAP_SIZE=256
linker_nrf52833_xxaa: ASMFLAGS += -D__STACK_SIZE=256

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: linker_nrf52833_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		linker_nrf52833_xxaa
	@echo		flash_softdevice
	@echo		sdk_config - starting external tool for editing sdk_config.h
	@echo		flash      - flashing binary

TEMPLATE_PATH := $(TOOLCHAIN_ROOT)/gcc
include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash flash_softdevice erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/linker_nrf52833_xxaa.hex
	nrfjprog -f nrf52 --program $(OUTPUT_DIRECTORY)/linker_nrf52833_xxaa.hex --sectorerase
	nrfjprog -f nrf52 --reset

# Flash softdevice
flash_softdevice:
	@echo Flashing: s140_nrf52_7.0.1_softdevice.hex
	nrfjprog -f nrf52 --program $(SDK_ROOT)/components/softdevice/s140/hex/s140_nrf52_7.0.1_softdevice.hex --sectorerase
	nrfjprog -f nrf52 --reset

erase:
	nrfjprog -f nrf52 --eraseall

SDK_CONFIG_FILE := ../config/sdk_config.h
CMSIS_CONFIG_TOOL := $(SDK_ROOT)/external_tools/cmsisconfig/CMSIS_Configuration_Wizard.jar
sdk_config:
	java -jar $(CMSIS_CONFIG_TOOL) $(SDK_CONFIG_FILE)
