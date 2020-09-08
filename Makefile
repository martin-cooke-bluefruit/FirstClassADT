# Name of executable to create
PROJECT_NAME ?= FirstClassADT

# Build mode for project: DEBUG or RELEASE
BUILD_MODE ?= RELEASE

# Compiler path used by Windows only
COMPILER_PATH ?= C:/MinGW/bin

ifeq ($(OS),Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname)
endif

ifeq ($(detected_OS),Windows)
	export PATH := $(COMPILER_PATH):$(PATH)
endif

SRCS := $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS := $(patsubst src/%,obj/%,$(SRCS:.c=.o))


CFLAGS += -Wall -std=c99 -Wno-missing-braces
ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -O0
else
    CFLAGS += -s -O1
endif

all:
	@$(MAKE) -s $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	gcc -o $(PROJECT_NAME) $(OBJS) $(CFLAGS)

obj/%.o: src/%.c
ifeq ($(detected_OS),Windows)
	if not exist "$(@D)" mkdir $(subst /,\,$(@D))
else
	mkdir -p $(@D)
endif
	gcc -c $< -o $@ $(CFLAGS)

clean:
ifeq ($(detected_OS),Windows)
	@if exist obj rmdir /Q /S obj
	@if exist $(PROJECT_NAME).exe del $(PROJECT_NAME).exe
else
	@rm -rf obj || true
	@rm -f $(PROJECT_NAME) || true
endif
