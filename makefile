SRC    := main.cpp planets.cpp
HDR    := planets.h
STD    := c++17
TARGET := orrery

# Release build: optimise, disable asserts
CXXFLAGS := -std=$(STD) -O2 -DNDEBUG

ifeq ($(OS),Windows_NT)
    TARGET := main.exe
    # static runtime so no extra DLLs are needed; -s strips symbols
    NATIVE_FLAGS := -s -static -static-libgcc -static-libstdc++
    PLATFORM := -lraylib -lopengl32 -lgdi32 -lwinmm
else ifeq ($(shell uname -s),Darwin)
    BREW := $(shell brew --prefix)
    PLATFORM := -I$(BREW)/include -L$(BREW)/lib \
        -lraylib -framework IOKit -framework Cocoa -framework OpenGL
    # Apple's linker ignores -s, so strip after linking
    POST_BUILD := strip $(TARGET)
else
    NATIVE_FLAGS := -s
    PLATFORM := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

$(TARGET): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(NATIVE_FLAGS) $(SRC) -o $(TARGET) $(PLATFORM)
	$(POST_BUILD)

# Web build: needs Emscripten (brew install emscripten) and raylib built with
#   cd ~/raylib/src && make PLATFORM=PLATFORM_WEB -B
RAYLIB_WEB := $(HOME)/raylib/src

# raylib's web build produces libraylib.web.a in newer versions, libraylib.a in older ones
ifeq ($(wildcard $(RAYLIB_WEB)/libraylib.web.a),)
    RAYLIB_WEB_LIB := $(RAYLIB_WEB)/libraylib.a
else
    RAYLIB_WEB_LIB := $(RAYLIB_WEB)/libraylib.web.a
endif

# raylib ships a minimal page template (shell.html in newer versions, minshell.html in older ones)
ifeq ($(wildcard $(RAYLIB_WEB)/shell.html),)
    WEB_SHELL := $(RAYLIB_WEB)/minshell.html
else
    WEB_SHELL := $(RAYLIB_WEB)/shell.html
endif

web: $(SRC) $(HDR)
	mkdir -p web
	em++ $(CXXFLAGS) -DPLATFORM_WEB $(SRC) -o web/orrery-jenns-26.html \
		-I$(RAYLIB_WEB) $(RAYLIB_WEB_LIB) \
		-s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s SINGLE_FILE=1 \
		--shell-file $(WEB_SHELL)

.PHONY: web