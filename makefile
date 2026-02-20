# Compiler
CC = gcc

# Source files
SRC = main.c glad.c shader.c stb_image.c
# Include directories
INCLUDE = -Iinclude

# Library directory
LIBDIR = -L./lib



# Default output
OUT = app

# Platform-specific settings
ifeq ($(OS),Windows_NT)
OUT := $(OUT).exe
LIBS = -lglfw3 -lopengl32 -lgdi32
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
INCLUDE += -I/opt/homebrew/include
LIBDIR += -L/opt/homebrew/lib
LIBS = -lglfw -framework OpenGL
else
LIBS = -lglfw -lGL -lm -ldl -lpthread
endif
endif

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(INCLUDE) $(LIBDIR) $(LIBS) -o $(OUT)

# Clean
clean:
	rm -f $(OUT

