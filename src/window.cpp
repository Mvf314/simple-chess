#include <iostream>

#include <ftxui/component/app.hpp>
#include <ftxui/component/loop.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>

#include "window.h"

Window::Window(Game* g) {
	game = g;
	auto app = ftxui::App::Fullscreen();

	screenWidth = ftxui::Terminal::Size().dimx;
	screenHeight = ftxui::Terminal::Size().dimy;

	// TODO maybe pass as arguments? const somewhere.
	boardWidth = 64;
	boardHeight = 36;

	pieceSelected = 0;
	moveSelected = std::vector<int>(16);
}

void Window::updateComponents() {
	const std::string board_row =   "+---+---+---+---+---+---+---+---+";
	const std::string board_other = "|   |   |   |   |   |   |   |   |";

	boardComponent = ftxui::Renderer([board_row, board_other]{
		//std::stringstream ss;
		//ss << "| " << piece_selected << " | " << move_selected[piece_selected] << " |   |   |   |   |   |   |";
		return ftxui::vbox({
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(board_other) 	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(board_other) 	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(board_other) 	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(board_other) 	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(board_other)	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(board_other) 	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(board_other) 	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
			ftxui::text(board_other) 	| ftxui::center,
			ftxui::text(board_row) 		| ftxui::center,
		}) | ftxui::center;
	});

	titleComponent = ftxui::Renderer([this] {
		return ftxui::vbox({
			ftxui::text("hello there i am text") 	| ftxui::borderRounded | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, screenHeight - boardHeight),
			boardComponent->Render()			| ftxui::border | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, boardHeight),
		}) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, boardWidth);
	});

	std::vector<std::string> pieces = {
		"Piece 0",
		"Piece 1",
		"Piece 2",
	};
	piecesComponent = ftxui::Menu(pieces, &pieceSelected) | ftxui::borderLight | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 16);

	std::vector<std::string> moves0 = {
		"Move 0-0",
	};
	std::vector<std::string> moves1 = {
		"Move 1-0",
		"Move 1-1",
		"Move 1-2",
		"Move 1-3",
		"Move 1-5",
	};
	std::vector<std::string> moves2 = {
		"Move 2-0",
		"Move 2-1",
		"Move 2-2",
	};

	movesComponent = ftxui::Container::Tab({
		ftxui::Menu(moves0, &moveSelected[0]),
		ftxui::Menu(moves1, &moveSelected[1]),
		ftxui::Menu(moves2, &moveSelected[2]),
	}, &pieceSelected) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 16);

	std::vector<std::vector<std::string>> moves = {
		moves0,
		moves1,
		moves2,
	};

	std::vector<std::string> history_strs = {
		"Moved p to d4",
		"Moved p to e5",
		"p captured p at e5",
	};
	std::vector<std::string> history_alg = {
		"1.\td4",
		"  \te5",
		"2.\txe5",
	};

	// We capture the vectors by value:
	// The vectors go out of scope at the end of this function,
	// at this moments the references point to unallocated memory.
	infoComponent = ftxui::Renderer([history_strs, history_alg, this] {
		ftxui::Elements history_strs_els = ftxui::Elements();
		ftxui::Elements history_alg_not_els = ftxui::Elements();
		for (const std::string& str : history_strs) {
			history_strs_els.push_back(ftxui::text(str));
		}
		for (const std::string& alg_not : history_alg) {
			history_alg_not_els.push_back(ftxui::text(alg_not));
		}
		return ftxui::hbox({
			ftxui::vbox(history_strs_els)		| ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 16),
			ftxui::separator(),
			ftxui::vbox(history_alg_not_els) 	| ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 8),
			ftxui::separator()
		}) | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, screenWidth - boardWidth - 32);
	});

	layout = ftxui::Container::Horizontal({
		titleComponent,
		piecesComponent,
		movesComponent,
		infoComponent,
	});
	
	content = ftxui::Renderer(layout, [&]{
		return ftxui::hbox({
			titleComponent->Render(),
			piecesComponent->Render(),
			movesComponent->Render(),
			infoComponent->Render(),
		});
	});
}

void Window::startApp(ftxui::App app) {
	app.Loop(content);
}
