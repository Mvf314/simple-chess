set FLAGS=-Wall -Wextra -std=c++17

:: Compile files separately

g++ -c src\position.cpp -o bin\position.o %FLAGS%
g++ -c src\piece.cpp -o bin\piece.o %FLAGS%
g++ -c src\king.cpp -o bin\king.o %FLAGS%
g++ -c src\rook.cpp -o bin\rook.o %FLAGS%
g++ -c src\bishop.cpp -o bin\bishop.o %FLAGS%
g++ -c src\game.cpp -o bin\game.o %FLAGS%
g++ -c src\main.cpp -o bin\main.o %FLAGS%


:: Link object files

g++ -o chess.exe bin\position.o bin\piece.o bin\king.o bin\rook.o bin\bishop.o bin\game.o bin\main.o %FLAGS%

:: Correct codepage for box-drawing characters
chcp 65001
