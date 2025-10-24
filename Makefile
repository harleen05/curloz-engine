TOOLCHAIN ?= gcc

ifeq ($(TOOLCHAIN), clang)
		CC := clang
		CXX := clang++
		LIBS_DIR := lib/linux
		LIBS := -lGL -ldl -L./lib/linux -lglfw3 -lassimp -lz

else ifeq ($(TOOLCHAIN), gcc)
		CC = gcc
		CXX = g++
		LIBS := -L./lib/linux -lglfw3 -lassimp -lz  -lGL -ldl

else ifeq ($(TOOLCHAIN), mingw)
		CC = x86_64-w64-mingw32-gcc
		CXX = x86_64-w64-mingw32-g++
		CXXFLAGS += -static 
		LDFLAGS := -static 
		LIBS := -L./lib/windows -lglfw3 -lassimp -lz -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm

endif

INC_DIRS = include
SRC_DIRS := src
BUILD_DIR := build

CFLAGS += -Wall
CXXFLAGS += $(CFLAGS)

TARGET_EXEC := prog

SRCS := $(shell find $(SRC_DIRS)/* -name '*.cpp' -or -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)


USE_MOLD ?= 0
ifeq ($(USE_MOLD), 1)
		LDFLAGS += -fuse-ld=mold
endif


DEBUG ?= 1
RELEASE ?= 0

ifeq ($(RELEASE),1)
		DEBUG = 0
endif


ifeq ($(DEBUG), 1)
		CFLAGS += -DDEBUG -g -O0 
		ifeq ($(TOOLCHAIN), clang)
				CXXFLAGS := $(CFLAGS) -stdlib=libc++ 
				LDFLAGS += -stdlib=libc++ -lc++ 
				ifeq ($(USE_MOLD), 0)
						LDFLAGS += -fuse-ld=lld
				endif
		endif

else ifeq ($(RELEASE), 1)
		CFLAGS += -DNDEBUG -O3
		CXXFLAGS := $(CFLAGS)

		ifeq ($(TOOLCHAIN), clang)
				CFLAGS += -flto=thin
				CXXFLAGS := $(CFLAGS) -stdlib=libc++
				ifeq ($(USE_MOLD), 0)
						LDFLAGS := -fuse-ld=lld
				endif
				LDFLAGS += -flto=thin -lc++

		else ifeq ($(TOOLCHAIN), gnu)
				CFLAGS += -flto
				CXXFLAGS := $(CFLAGS)
				ifeq ($(USE_MOLD), 0)
						LDFLAGS := -fuse-ld=ld
				endif
				LDFLAGS += -flto

		endif
endif


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
		$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

$(BUILD_DIR)/%.c.o: %.c
		@mkdir -p $(dir $@)
		$(CC) -I./$(INC_DIRS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
		@mkdir -p $(dir $@)
		$(CXX) -I./$(INC_DIRS) $(CXXFLAGS) -c $< -o $@



.PHONY: debug clean run

debug:
		@lldb $(BUILD_DIR)/$(TARGET_EXEC)
clean:
		@rm -rf $(BUILD_DIR)/*
run:
		@./$(BUILD_DIR)/$(TARGET_EXEC)

