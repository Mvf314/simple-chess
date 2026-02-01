set FLAGS=-Wall -Werror -Wextra -std=c++17

:: Compile files separately

g++ -c src\position.cpp -o bin\position.o %FLAGS%
g++ -c src\piece.cpp -o bin\piece.o %FLAGS%
g++ -c src\game.cpp -o bin\game.o %FLAGS%
g++ -c src\main.cpp -o bin\main.o %FLAGS%


:: Link object files

g++ -o chess.exe bin\position.o bin\piece.o bin\game.o bin\main.o %FLAGS%

:: Correct codepage for box-drawing characters
chcp 65001
