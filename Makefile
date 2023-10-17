
OBJS = src/game/*.c

CC = gcc

INCLUDE_PATHS = -Isrc/Include

LIBRARY_PATHS = -Lsrc/lib

# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -Wextra -g -O0

LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

OBJ_NAME = build/Game

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
