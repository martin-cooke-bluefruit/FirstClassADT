PROJECT_NAME  ?= FirstClassADT
COMPILER_PATH ?= C:/MinGW/bin

export PATH := $(COMPILER_PATH):$(PATH)

SRCS := $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS := $(patsubst src/%,obj/%,$(SRCS:.c=.o))

all:
	$(MAKE) $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	gcc -o $(PROJECT_NAME) $(OBJS)

obj/%.o: src/%.c
	if not exist "$(@D)" mkdir $(subst /,\,$(@D))
	gcc -c $< -o $@

clean:
	if exist obj rmdir /Q /S obj
	if exist $(PROJECT_NAME).exe del $(PROJECT_NAME).exe
