CC = gcc
CFLAGS = -Wall -std=c99
INCLUDE_PATH = -I/opt/homebrew/include
LIB_PATH = -L/opt/homebrew/Cellar/raylib/5.0/lib
LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit
TARGET = game
SRC = main.c # add here more source files

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -o $(TARGET) $(SRC) $(LIB_PATH) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: clean $(TARGET)
	./$(TARGET)