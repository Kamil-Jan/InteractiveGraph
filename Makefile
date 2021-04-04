SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj

LINUX_CC := g++
WINDOWS_CC := i686-w64-mingw32-g++
CC := $(LINUX_CC)

ifeq ($(CC), $(LINUX_CC))
	OBJ_EXT := o
	TARGET_EXT := out
	LIBS := -lSDL2 -lSDL2_ttf -lSDL2_image
	INCLUDES := -I$(INC_DIR)
else
	OBJ_EXT := out
	TARGET_EXT := exe
	LIBS := -Llib/SDL2-2.0.14/i686-w64-mingw32/lib/ -Llib/SDL2_ttf-2.0.15/i686-w64-mingw32/lib/ -Llib/SDL2_image-2.0.5/i686-w64-mingw32/lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -mwindows
	INCLUDES := -I$(INC_DIR) -Ilib/SDL2-2.0.14/i686-w64-mingw32/include/ -Ilib/SDL2_ttf-2.0.15/i686-w64-mingw32/include/ -Ilib/SDL2_image-2.0.5/i686-w64-mingw32/include/
endif

CFLAGS := `pkg-config --cflags sdl2 SDL2_ttf` -MMD -MP -std=c++17 -Wall

SRCS := $(shell find $(SRC_DIR) -name *.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.$(OBJ_EXT), $(SRCS))
DEPS := $(OBJS:.$(OBJ_EXT)=.d)

TARGET := graph.$(TARGET_EXT)

# recipe for building the final executable
$(TARGET): $(OBJS)
	@echo "--- Linking target $@"
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $@ $(LIBS)

# recipe for building header object files
$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "--- Building $@"
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@
	@echo ""

# recipe to build the project
install: $(TARGET)

# recipe to clean the workspace
clean:
	rm -r $(OBJ_DIR)

-include $(DEPS)

.PHONY: install clean

