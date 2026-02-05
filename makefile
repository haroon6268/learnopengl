CC = gcc

SRC = main.c glad.c

INCLUDE = -Iinclude

LIBDIR = -L.

LIBS = -lglfw3 -lopengl32 -lgdi32

OUT = app.exe

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(INCLUDE) $(LIBDIR) $(LIBS) -o $(OUT)


