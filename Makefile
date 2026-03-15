CC=g++
RM=rm
RMDIR=rmdir

CFLAGS=-Wall -Werror -Wextra -std=c++17 -O2

OBJ_DIR=bin
SRC_DIR=src
OBJ_LIST=piece/piece.o piece/king.o piece/queen.o piece/rook.o piece/bishop.o piece/knight.o piece/pawn.o position.o random.o game.o main.o
OBJS=$(addprefix $(OBJ_DIR)/,$(OBJ_LIST))


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | bin bin/piece
	$(CC) $(CFLAGS) -c $< -o $@

main: $(OBJS)
	$(CC) -o chess $^

bin bin/piece:
	mkdir $@

clean:
	$(RM) $(OBJS) main
	$(RMDIR) bin/piece bin
