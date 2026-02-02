set FLAGS=-Wall -Werror -Wextra -std=c++17

:: Compile files separately

g++ -c src\position.cpp -o bin\position.o %FLAGS%
g++ -c src\piece\piece.cpp -o bin\piece.o %FLAGS%
g++ -c src\piece\king.cpp -o bin\king.o %FLAGS%
g++ -c src\piece\queen.cpp -o bin\queen.o %FLAGS%
g++ -c src\piece\rook.cpp -o bin\rook.o %FLAGS%
g++ -c src\piece\bishop.cpp -o bin\bishop.o %FLAGS%
g++ -c src\piece\knight.cpp -o bin\knight.o %FLAGS%
g++ -c src\piece\pawn.cpp -o bin\pawn.o %FLAGS%
g++ -c src\game.cpp -o bin\game.o %FLAGS%
g++ -c src\main.cpp -o bin\main.o %FLAGS%


:: Link object files

g++ -o chess.exe bin\position.o bin\piece.o bin\king.o bin\queen.o bin\rook.o bin\bishop.o bin\knight.o bin\pawn.o bin\game.o bin\main.o %FLAGS%

:: Correct codepage for box-drawing characters
chcp 65001
