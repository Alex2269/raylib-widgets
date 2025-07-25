### run make SILENT=0 , with  all print.
### SILENT=1 , default silent mode

SILENT ?= 1
ifeq (1,$(SILENT))
.SILENT:
endif

TARGET = application

# debug build?
DEBUG = 0
# optimization
OPT = -Og
# debug output periph regs
OPT += -g3

# Build paths
BUILD_DIR = build
BUILD_ASM_DIR = $(BUILD_DIR)/asm
BUILD_APP_DIR = $(BUILD_DIR)/app
BUILD_CC_DIR  = $(BUILD_DIR)/ccc
BUILD_CPP_DIR = $(BUILD_DIR)/cpp

# recursive find sources files
# sources
ROOT_DIR = .
C_SOURCES   += $(shell find ${ROOT_DIR} -name '*.c')
CPP_SOURCES += $(shell find ${ROOT_DIR} -name '*.cpp')
ASM_SOURCES += $(shell find ${ROOT_DIR} -name '*.s')

# binaries
PREFIX =
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
CPU = -m64
MCU = $(CPU)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS +=
# C_DEFS += '-D GLEW_STATIC'
# C_DEFS += '-D __align(value)=__attribute__((aligned(value)))'
# C_DEFS += '-D __forceinline=__attribute__((always_inline))'
# C_DEFS += '-D __irq=__attribute__((interrupt("IRQ")))'
# C_DEFS += '-D __packed=__attribute__((__packed__))'
# C_DEFS += '-D __weak=__attribute__((weak))'

C_DEFS += '-D RAYGUI_IMPLEMENTATION'

# AS includes
AS_INCLUDES = 

# C includes
# Find header files: h, hh, hpp.
INCDIR = .
# C_INC       =$(shell find -L app/Inc   -name '*.h*' -exec dirname {} \; | uniq)
# C_INC      +=$(shell find -L FontsUG   -name '*.h*' -exec dirname {} \; | uniq)
# C_INC      +=$(shell find -L graphics  -name '*.h*' -exec dirname {} \; | uniq)
# C_INC      +=$(shell find -L libs      -name '*.h*' -exec dirname {} \; | uniq)
# C_INC      +=$(shell find -L Src*      -name '*.h*' -exec dirname {} \; | uniq)
C_INC      +=$(shell find -L $(INCDIR)/  -name '*.h*' -exec dirname {} \; | uniq)

C_INCLUDES  =$(C_INC:%=-I %)

# compile gcc flags
# ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

WARNINGS := -Wall
# WARNINGS += -Wextra
# WARNINGS += -Wshadow
# WARNINGS += -Wundef
# WARNINGS += -Wmaybe-uninitialized
# # WARNINGS += -Wmissing-prototypes
# # WARNINGS += -Wno-discarded-qualifiers
# WARNINGS += -Wno-unused-function
# WARNINGS += -Wno-error=strict-prototypes
# WARNINGS += -Wno-error=cpp
# WARNINGS += -Wno-unused-parameter
# WARNINGS += -Wno-missing-field-initializers
# WARNINGS += -Wno-format-nonliteral
# WARNINGS += -Wno-cast-qual
# WARNINGS += -Wno-switch-default
# WARNINGS += -Wno-ignored-qualifiers
# WARNINGS += -Wno-error=pedantic
# WARNINGS += -Wno-sign-compare
# WARNINGS += -Wno-error=missing-prototypes
# WARNINGS += -Wpointer-arith -fno-strict-aliasing
# WARNINGS += -Wuninitialized
# WARNINGS += -Wunreachable-code
# WARNINGS += -Wreturn-type
# WARNINGS += -Wmultichar
# WARNINGS += -Wformat-security
# WARNINGS += -Wdouble-promotion
# WARNINGS += -Wclobbered
# WARNINGS += -Wdeprecated
# WARNINGS += -Wempty-body
# WARNINGS += -Wshift-negative-value
# WARNINGS += -Wtype-limits
# WARNINGS += -Wsizeof-pointer-memaccess
# WARNINGS += -Wpointer-arith

GCCFLAGS += -O0 -g $(WARNINGS)

CFLAGS_STD = -c -Os -w -std=gnu17 $(GCCFLAGS)
CXXFLAGS_STD = -c -Os -w -std=gnu++17 $(GCCFLAGS)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CFLAGS_STD)
CFLAGS += -Wno-implicit-function-declaration
CPPFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CXXFLAGS_STD)

# # Generate dependency information
# CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# libraries
LIBDIR =
LIBS =
LIBS += -lraylib -lGL -lm -lpthread -ldl -lrt
LIBS += -lX11
LIBS += -lwayland-client -lwayland-cursor -lwayland-egl -lxkbcommon
LIBS += -lc

# LIBS  += -lSDL2
# LIBS  += -lglut /usr/lib/x86_64-linux-gnu/libGL.so
# LIBS  += -lCORE -lGLEW
# LIBS  += -lglfw -lglut
# LIBS  += -lGLU
# LIBS += -lGL -lXi -lXext -lX11

LDFLAGS +=  $(LIBDIR) $(LIBS)
LDFLAGS += -Wl,--start-group
LDFLAGS += -lgcc
LDFLAGS += -lstdc++
LDFLAGS += -Wl,--end-group

# default action: build all
all: $(BUILD_APP_DIR)/$(TARGET).elf $(BUILD_APP_DIR)/$(TARGET).hex $(BUILD_APP_DIR)/$(TARGET).bin

## shell color ##
green=\033[0;32m
YELLOW=\033[1;33m
NC=\033[0m
##-------------##

###
# build the application
###
# list of objects
OBJECTS = $(addprefix $(BUILD_CC_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_CPP_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_ASM_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_CC_DIR)/%.o: %.c Makefile | $(BUILD_CC_DIR)
	@echo " ${green} [compile:] ${YELLOW} $< ${NC}"
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CC_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_CPP_DIR)/%.o: %.cpp Makefile | $(BUILD_CPP_DIR)
	@echo " ${green} [compile:] ${YELLOW} $< ${NC}"
	$(CXX) -c $(CPPFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CPP_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_ASM_DIR)/%.o: %.s Makefile | $(BUILD_ASM_DIR)
	@echo " ${green} [compile:] ${YELLOW} $< ${NC}"
	$(AS) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_CC_DIR)/$(notdir $(<:.s=.lst)) $< -o $@

$(BUILD_APP_DIR)/$(TARGET).elf: $(OBJECTS) Makefile | $(BUILD_APP_DIR)
	@echo " ${green} [linking:] ${YELLOW} $@ ${NC} \n"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@ --format=Berkeley
	cp $(BUILD_APP_DIR)/$(TARGET).elf ./
	# $(BUILD_APP_DIR)/$(TARGET).elf
	# $(SZ) $@ --format=SysV --radix=16

$(BUILD_APP_DIR)/%.hex: $(BUILD_APP_DIR)/%.elf | $(BUILD_APP_DIR)
	$(HEX) $< $@
	
$(BUILD_APP_DIR)/%.bin: $(BUILD_APP_DIR)/%.elf | $(BUILD_APP_DIR)
	$(BIN) $< $@	
	
$(BUILD_CC_DIR):
	mkdir -p $@
$(BUILD_CPP_DIR):
	mkdir -p $@
$(BUILD_APP_DIR):
	mkdir -p $@
$(BUILD_ASM_DIR):
	mkdir -p $@

###
# clean up
###
clean:
	-rm -fR $(BUILD_DIR)
	-rm -f  $(TARGET).elf

-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
