PROJECT_NAME  ?= FirstClassADT
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

all:
	@$(MAKE) -s $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	gcc -o $(PROJECT_NAME) $(OBJS)

obj/%.o: src/%.c
ifeq ($(detected_OS),Windows)
	if not exist "$(@D)" mkdir $(subst /,\,$(@D))
else
	mkdir -p $(@D)
endif
	gcc -c $< -o $@

clean:
ifeq ($(detected_OS),Windows)
	@if exist obj rmdir /Q /S obj
	@if exist $(PROJECT_NAME).exe del $(PROJECT_NAME).exe
else
	@rm -rf obj || true
	@rm -f $(PROJECT_NAME) || true
endif
