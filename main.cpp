#include <iostream>
#include <string>

#include "game.hpp"
#include "util.hpp"

int chooseGameMode() {
    int input;

    std::cout << "Welcome, please choose the game:" << std::endl
              << "\t1 - Tic-Tac-Toe" << std::endl
              << "\t2 - Pentago" << std::endl;

    while (true) {
        std::cout << "> ";
        std::cin >> input;
        std::cin.clear();
        std::cin.ignore();

        if (input != 1 && input != 2) {
            std::cout << "This game doesn't exist. Please choose one from "
                         "the list above."
                      << std::endl;
        } else {
            break;
        }
    }

    return input;
}

int main() {
    srand(time(NULL));

    // Needed in order for clearScreen and box drawings to work
    system("chcp 65001 >nul");

    clearScreen();
    int mode = chooseGameMode();
    std::cout << std::endl;

    std::string title;
    if (mode == 1) {
        title = "Tic-Tac-Toe";
    } else if (mode == 2) {
        title = "Pentago";
    }

    Game game = Game(title);

    // Player name and symbol choices
    for (int i = Token::Player1; i <= Token::Player2; i++) {
        std::string name;
        char symbol;

        while (true) {
            std::cout << "Player " << i + 1 << " name: ";
            std::cin >> name;
            if (game.setPlayerName((Token)i, name) != 0) {
                std::cout << "Your name can't be longer than " << MAX_PLAYER_NAME_LEN
                          << " characters." << std::endl;
            } else {
                break;
            }
        }

        while (true) {
            std::cout << "Player " << i + 1 << " token: ";
            std::cin >> symbol;
            if (game.setPlayerSymbol((Token)i, symbol) != 0) {
                std::cout << "This token is already in use." << std::endl;
            } else {
                break;
            }
        }

        std::cout << std::endl;
    }

    clearScreen();

    game.setState((mode == 1) ? GameState::TicTacToe : GameState::Pentago);

    std::cout << std::endl << std::endl;

    // Game loop
    while (game.active()) {
        game.update();
    }

    return 0;
}
