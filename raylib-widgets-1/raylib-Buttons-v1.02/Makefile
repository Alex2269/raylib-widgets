### run make SILENT=0 , with all print.
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
# Check if we are on Windows
ifeq ($(OS),Windows_NT)
  # Windows specific settings
  ifdef GCC_PATH
    CC = $(GCC_PATH)/$(PREFIX)gcc.exe
    CXX = $(GCC_PATH)/$(PREFIX)g++.exe
    AS = $(GCC_PATH)/$(PREFIX)gcc.exe -x assembler-with-cpp
    CP = $(GCC_PATH)/$(PREFIX)objcopy.exe
    SZ = $(GCC_PATH)/$(PREFIX)size.exe
  else
    CC = $(PREFIX)gcc.exe
    CXX = $(PREFIX)g++.exe
    AS = $(PREFIX)gcc.exe -x assembler-with-cpp
    CP = $(PREFIX)objcopy.exe
    SZ = $(PREFIX)size.exe
  endif
  HEX = $(CP) -O ihex
  BIN = $(CP) -O binary -S
else
  # Linux/Unix specific settings
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
endif

CPU = -m64
MCU = $(CPU)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS +=

# AS includes
AS_INCLUDES =

# C includes
INCDIR = .
C_INC      +=$(shell find -L $(INCDIR)/  -name '*.h*' -exec dirname {} \; | uniq)
C_INCLUDES  =$(C_INC:%=-I %)

# compile gcc flags
WARNINGS := -Wall
GCCFLAGS += -O0 -g $(WARNINGS)

CFLAGS_STD = -c -Os -w -std=gnu17 $(GCCFLAGS)
CXXFLAGS_STD = -c -Os -w -std=gnu++17 $(GCCFLAGS)

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CFLAGS_STD)
CFLAGS += -Wno-implicit-function-declaration
CPPFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) $(CXXFLAGS_STD)

# libraries
LIBDIR =
LIBS =
LIBS += -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

LDFLAGS +=  $(LIBDIR) $(LIBS)
LDFLAGS += -Wl,--start-group
LDFLAGS += -lgcc
LDFLAGS += -lstdc++
LDFLAGS += -Wl,--end-group

# default action: build all
all: $(BUILD_APP_DIR)/$(TARGET).elf $(BUILD_APP_DIR)/$(TARGET).hex $(BUILD_APP_DIR)/$(TARGET).bin

# build the application
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

# clean up
clean:
	-rm -fR $(BUILD_DIR)
	-rm -f  $(TARGET).elf

-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
