TOOLCHAIN ?= gnu

ifeq ($(TOOLCHAIN), clang)
		CC := clang
		CXX := clang++
else
		CC ?= gcc
		CXX ?= g++
endif

INC_DIRS = include
SRC_DIRS := src
BUILD_DIR := build
LIB_DIR := lib

CFLAGS := -Wall
CXXFLAGS := $(CFLAGS)

TARGET_EXEC := prog


SRCS := $(shell find $(SRC_DIRS)/* -name '*.cpp' -or -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

ifeq ($(OS), Window_NT)
		LIBS := -lopengl32
else 
		LIBS := -lGL -ldl
endif
LIBS += -lglfw3 -lassimp -lz


USE_MOLD ?= 1
ifeq ($(USE_MOLD), 1)
		LDFLAGS := -fuse-ld=mold
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
				CXXFLAGS := $(CFLAGS)
				CXXFLAGS += -stdlib=libc++
				CXXFLAGS += -fcolor-diagnostics
				CXXFLAGS += -fdiagnostics-show-template-tree
				ifeq ($(USE_MOLD), 0)
						LDFLAGS := -stdlib=libc++ -lc++ -fuse-ld=lld
				endif
				LDFLAGS += -flto=thin

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
		$(CXX) $(OBJS) -o $@ $(LDFLAGS) -L./$(LIB_DIR) $(LIBS)

$(BUILD_DIR)/%.c.o: %.c
		@mkdir -p $(dir $@)
		$(CC) -I./$(INC_DIRS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
		@mkdir -p $(dir $@)
		$(CXX) -I./$(INC_DIRS) $(CXXFLAGS) -c $< -o $@

.PHONY: debug clean

debug:
		@lldb $(BUILD_DIR)/$(TARGET_EXEC)
clean:
		@rm -rf $(BUILD_DIR)/*

