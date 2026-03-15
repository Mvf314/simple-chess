CC=g++
RM=rm -f

# Compilation flags
CFLAGS=-Wall -Werror -Wextra -std=c++17 -O2 -fdiagnostics-color=always

OBJ_DIR=bin
SRC_DIR=src
OBJ_LIST=piece/piece.o piece/king.o piece/queen.o piece/rook.o piece/bishop.o piece/knight.o piece/pawn.o window.o position.o random.o game.o main.o
OBJS=$(addprefix $(OBJ_DIR)/,$(OBJ_LIST))

LIB_PATH=/usr/local/lib
INCLUDE_PATH=/usr/local/include

# Linker flags
LFLAGS=-L$(LIB_PATH) -lftxui-component -lftxui-dom -lftxui-screen


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | bin bin/piece
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_PATH)

main: $(OBJS)
	$(CC) -o chess $^ $(LFLAGS)

bin bin/piece:
	mkdir $@

clean:
	$(RM) $(OBJS) chess
	$(RM) -r bin
