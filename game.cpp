#include "game.hpp"

#include <iostream>

#include "util.hpp"

const std::string nlu = "\u250c";  // ┌
const std::string nlc = "\u251c";  // ├
const std::string nld = "\u2514";  // └
const std::string nru = "\u2510";  // ┐
const std::string nrc = "\u2524";  // ┤
const std::string nrd = "\u2518";  // ┘
const std::string nmu = "\u252c";  // ┬
const std::string nmc = "\u253c";  // ┼
const std::string nmd = "\u2534";  // ┴
const std::string nvv = "\u2502";  // │
const std::string nhh = "\u2500";  // ─
const std::string blu = "\u2554";  // ╔
const std::string blc = "\u2560";  // ╠
const std::string bld = "\u255a";  // ╚
const std::string bru = "\u2557";  // ╗
const std::string brc = "\u2563";  // ╣
const std::string brd = "\u255d";  // ╝
const std::string bmu = "\u2566";  // ╦
const std::string bmc = "\u256c";  // ╬
const std::string bmd = "\u2569";  // ╩
const std::string bvv = "\u2551";  // ║
const std::string bhh = "\u2550";  // ═

Game::Game(const std::string title) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            this->board[y][x] = Token::Empty;
        }
    }

    this->title = title;
    this->players[Token::Player1] = { "Player 1", ' ' };
    this->players[Token::Player2] = { "Player 2", ' ' };

    this->current_player = (Token)(rand() % 2);
}

// Constructs a string used for printing box-drawing borders
std::string border(int seg_width, int seg_count, const std::string lcor,
                   const std::string rcor, const std::string mid,
                   const std::string sep) {
    std::string out;

    out.append(lcor);

    for (int seg = 1; seg <= seg_count; seg++) {
        for (int i = 1; i <= seg_width; i++) {
            out.append(mid);
            if (seg != seg_count && i == seg_width) {
                out.append(sep);
            }
        }
    }

    out.append(rcor);

    return out;
}

void waitForUnpause() {
    char input;
    std::cout << "Type any key and press <Enter> to unpause the game. ";
    std::cin >> input;
    std::cin.clear();
}

void Game::draw() {
    // Amount of segments in each board region
    int seg_n = BOARD_SIZE / 2;
    // Width of one main border segment
    int border_width = 3 * seg_n + seg_n + 2;
    // Width of the entire game board
    int board_width = border_width * 2 + 3;

    // Game Title
    int pad_len = (board_width - this->title.length()) / 2;
    std::cout << std::string(pad_len, ' ') << this->title << std::endl;

    // Top border
    std::cout << border(border_width, 2, blu, bru, bhh, bhh) << std::endl;

    for (int y = 0; y < BOARD_SIZE; y++) {
        // Segment top border
        if (y % (BOARD_SIZE / 2) == 0) {
            std::cout << bvv << " " << border(3, seg_n, nlu, nru, nhh, nmu)
                      << " " << border(3, seg_n, nlu, nru, nhh, nmu) << " "
                      << bvv << std::endl;
        }

        // Row start
        std::cout << bvv << " " << nvv;

        for (int x = 0; x < BOARD_SIZE; x++) {
            if (this->board[y][x] == Token::Empty) {
                std::cout << "   ";
            } else {
                std::cout << ' ' << this->players[this->board[y][x]].symbol
                          << ' ';
            }

            if (x != BOARD_SIZE - 1) {
                std::cout << nvv;
            }

            if (x == BOARD_SIZE / 2 - 1) {
                std::cout << " " << nvv;
            }
        }

        // Row end
        std::cout << nvv << " " << bvv << std::endl;

        // Segment bottom border and segment separator
        if (y % (BOARD_SIZE / 2) == BOARD_SIZE / 2 - 1) {
            std::cout << bvv << " " << border(3, seg_n, nld, nrd, nhh, nmd)
                      << " " << border(3, seg_n, nld, nrd, nhh, nmd) << " "
                      << bvv << std::endl;
        } else {
            std::cout << bvv << " " << border(3, seg_n, nlc, nrc, nhh, nmc)
                      << " " << border(3, seg_n, nlc, nrc, nhh, nmc) << " "
                      << bvv << std::endl;
        }
    }

    // Bottom border
    std::cout << border(border_width, 2, bld, brd, bhh, bhh) << std::endl;
}

void Game::drawStats() {
    std::cout << "Current player: " << this->players[this->current_player].name
              << " (" << this->players[this->current_player].symbol << ")"
              << std::endl;
}

void Game::drawHelp() {
    std::cout << "\t" << border(6, 1, blu, bru, bhh, bhh) << std::endl
              << "\t" << bvv << " HELP " << bvv << std::endl
              << "\t" << border(6, 1, bld, brd, bhh, bhh) << std::endl;

    std::cout << std::endl
              << "Placing tokens:" << std::endl
              << "\tq/w/a/s - board part as seen on the keyboard" << std::endl
              << "\t\t(upper left, upper right, lower left, lower right)"
              << std::endl
              << "\t1..9 - field as seen on the numeric keyboard" << std::endl
              << "\t\t(7 - upper left, 5 - middle, 2 - lower middle etc.)"
              << std::endl
              << "\tz/x - rotation: z - clockwise, x - anti-clockwise"
              << std::endl
              << "\t\t(used only when playing Pentago)" << std::endl
              << std::endl
              << "Other commands:" << std::endl
              << "\tp - pause the game, stopping the timer" << std::endl
              << "\th - show help (this screen)" << std::endl
              << "\to - load an example predefined board" << std::endl
              << "\tm - menu, where you can change your name and token symbol"
              << std::endl
              << "\tz - exit the game" << std::endl;

    std::cout << std::endl << std::endl;
}

void Game::drawPause() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << std::endl;
    }

    std::cout << "\t" << border(13, 1, blu, bru, bhh, bhh) << std::endl
              << "\t" << bvv << " GAME PAUSED " << bvv << std::endl
              << "\t" << border(13, 1, bld, brd, bhh, bhh) << std::endl;

    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << std::endl;
    }
}

void Game::drawMenu() {
    std::cout << "\t" << border(6, 1, blu, bru, bhh, bhh) << std::endl
              << "\t" << bvv << " MENU " << bvv << std::endl
              << "\t" << border(6, 1, bld, brd, bhh, bhh) << std::endl;

    std::cout << std::endl
              << std::endl
              << "1 - change name" << std::endl
              << "2 - change token" << std::endl
              << "z - exit this menu" << std::endl
              << std::endl;

    bool exit = false;
    while (!exit) {
        std::string input;
        std::cout << "(choose option) > ";
        std::cin >> input;
        std::cin.clear();
        std::cin.ignore();

        Player player = this->players[this->current_player];

        switch (input[0]) {
            case '1':
                std::cout << "Enter new name for player " << player.name << " ("
                          << player.symbol << "): ";
                std::cin >> input;
                if (input == this->players[Token::Player1].name ||
                    input == this->players[Token::Player2].name) {
                    std::cout << "This name is taken." << std::endl;
                } else {
                    this->players[this->current_player].name = input;
                }
                break;
            case '2':
                std::cout << "Enter new token for player " << player.name
                          << " (" << player.symbol << "): ";
                std::cin >> input;
                if (input[0] == this->players[Token::Player1].symbol ||
                    input[0] == this->players[Token::Player2].symbol) {
                    std::cout << "This symbol is already used." << std::endl;
                } else {
                    this->players[this->current_player].symbol = input[0];
                }
                break;
            case 'z':
                exit = true;
                break;
            default:
                break;
        }
    }
}

void Game::drawEnd() {
    std::cout << std::endl << "Game ended. ";

    Player player;
    switch (this->end_state) {
        case EndState::Draw:
            std::cout << "It's a draw!";
            break;
        case EndState::Player1Win:
        case EndState::Player2Win:
            player = this->players[(Token)this->end_state];
            std::cout << player.name << " (" << player.symbol << ") won!";
            break;
        default:
            break;
    }

    std::cout << std::endl;
}

void Game::update() {
    switch (this->state) {
        case TicTacToe:
        case Pentago:
            this->draw();
            this->drawStats();
            this->handleInput();
            this->checkWinCondition(5);
            break;
        case Win:
            this->draw();
            this->drawEnd();
            this->state = GameState::End;
            break;
        default:
            break;
    }
}

// Calculates the relative offset used for rotating and placing tokens
void quadOffset(const char q, int *y, int *x) {
    switch (q) {
        case 'w':
            *x = BOARD_SIZE / 2;
            break;
        case 'a':
            *y = BOARD_SIZE / 2;
            break;
        case 's':
            *x = BOARD_SIZE / 2;
            *y = BOARD_SIZE / 2;
            break;
        default:
            break;
    }
}

// Parses user input and decides what to do with it
void Game::handleInput() {
    // Get user input
    std::string input;
    std::cout << std::endl << "(h for help) > ";
    std::cin >> input;
    std::cin.clear();

    clearScreen();

    // Parse input
    int input_len = input.length();
    int input_num;
    char input_rot, input_rot_dir;
    int y = 0, x = 0, rot_y = 0, rot_x = 0;
    bool rotate = false;

    switch (input[0]) {
        case 'q':
        case 'w':
        case 'a':
        case 's':
            if (input_len < 2) {
                if (this->state == GameState::TicTacToe) {
                    std::cout << "Input a move.";

                } else if (this->state == GameState::Pentago) {
                    std::cout << "Input a move and (optionally) a rotation.";
                }

                break;
            }

            if (input_len >= 2) {
                input_num = input[1] - '0';

                if (input_num < 1 || input_num > 9) {
                    std::cout << "Enter a number between 1 and 9.";
                    break;
                }
            }

            // Rotation input is checked before making any changes to the board
            // in order to prevent accidental user input erorrs
            if (this->state == GameState::Pentago) {
                if (input_len >= 3) {
                    input_rot = input[2];

                    if (input_rot != 'q' && input_rot != 'w' &&
                        input_rot != 'a' && input_rot != 's') {
                        std::cout << "Choose a correct quad (q/w/a/s).";
                        break;
                    }

                    if (input_len < 4) {
                        std::cout << "Input a rotation direction (z or x).";
                        break;
                    }
                }

                if (input_len >= 4) {
                    input_rot_dir = input[3];

                    if (input_rot_dir != 'z' && input_rot_dir != 'x') {
                        std::cout << "Choose a correct rotation (z or x).";
                        break;
                    }

                    rotate = true;
                }
            }

            quadOffset(input[0], &y, &x);

            y += (BOARD_SIZE / 2 - 1) - (input_num - 1) / (BOARD_SIZE / 2);
            x += (input_num - 1) % (BOARD_SIZE / 2);

            if (this->placeToken(y, x, this->current_player) != 0) {
                std::cout << "This spot is taken.";
                break;
            }

            if (this->state == GameState::Pentago && rotate) {
                quadOffset(input_rot, &rot_y, &rot_x);
                if (input_rot_dir == 'z') {
                    this->rotateQuadRight(rot_y, rot_x);
                } else if (input_rot_dir == 'x') {
                    this->rotateQuadLeft(rot_y, rot_x);
                }
            }

            this->setCurrentPlayer(this->current_player == Token::Player1
                                       ? Token::Player2
                                       : Token::Player1);

            break;

        case 'p':
            this->drawPause();
            waitForUnpause();
            clearScreen();
            break;

        case 'o':
            this->loadExampleBoard();
            std::cout << "Loaded example board";
            break;

        case 'm':
            this->drawMenu();
            clearScreen();
            break;

        case 'h':
            Game::drawHelp();
            waitForUnpause();
            clearScreen();
            break;

        case 'z':
            std::cout << "Thanks for playing " << this->title << "!";
            this->setState(GameState::End);
            break;

        default:
            std::cout << "This command doesn't exist. Check the 'h' "
                         "command for help.";
            break;
    }

    std::cout << std::endl << std::endl;
}

// Checks the win condition with at least `n_crossed` tokens in a row
void Game::checkWinCondition(unsigned int n_crossed) {
    // Vertical and horizontal win conditions
    for (int y = 0; y < BOARD_SIZE; y++) {
        int streak_h = 0, streak_v = 0;
        Token streak_h_player = Token::Empty, streak_v_player = Token::Empty;

        for (int x = 0; x < BOARD_SIZE; x++) {
            // Horizontal
            if (streak_h_player != this->board[y][x]) {
                streak_h_player = this->board[y][x];
                streak_h = 1;
            } else {
                streak_h++;
            }

            // Vertical
            if (streak_v_player != this->board[x][y]) {
                streak_v_player = this->board[x][y];
                streak_v = 1;
            } else {
                streak_v++;
            }

            // Draw
            if (streak_h >= n_crossed && streak_v >= n_crossed &&
                streak_h_player != Token::Empty &&
                streak_v_player != Token::Empty) {
                this->state = GameState::Win;
                this->end_state = EndState::Draw;
                return;
            }

            // Horizontal win
            if (streak_h >= n_crossed && streak_h_player != Token::Empty) {
                this->state = GameState::Win;
                this->end_state = (EndState)streak_h_player;
                return;
            }

            // Vertical win
            if (streak_v >= n_crossed && streak_v_player != Token::Empty) {
                this->state = GameState::Win;
                this->end_state = (EndState)streak_v_player;
                return;
            }
        }
    }

    // Diagonal win conditions
    int streak_dr = 0, streak_dl = 0;
    Token streak_dr_player = Token::Empty, streak_dl_player = Token::Empty;

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Diagonal going down
        if (streak_dr_player != this->board[i][i]) {
            streak_dr_player = this->board[i][i];
            streak_dr = 1;
        } else {
            streak_dr++;
        }

        // Diagonal going up
        if (streak_dl_player != this->board[i][i]) {
            streak_dl_player = this->board[i][i];
            streak_dl = 1;
        } else {
            streak_dl++;
        }

        // Diagonal going down win
        if (streak_dr >= n_crossed && streak_dr_player != Token::Empty) {
            this->state = GameState::Win;
            this->end_state = (EndState)streak_dr_player;
            return;
        }

        // Diagonal going up win
        if (streak_dl >= n_crossed && streak_dl_player != Token::Empty) {
            this->state = GameState::Win;
            this->end_state = (EndState)streak_dl_player;
            return;
        }
    }

    // Offset diagonal win conditions
    int streak_dra = 0, streak_dru = 0, streak_dla = 0, streak_dlu = 0;
    Token streak_dra_player = Token::Empty, streak_dru_player = Token::Empty,
          streak_dla_player = Token::Empty, streak_dlu_player = Token::Empty;

    for (int i = 0, j = 1; j < BOARD_SIZE; i++, j++) {
        // Going down above middle
        if (streak_dra_player != this->board[i][j]) {
            streak_dra_player = this->board[i][j];
            streak_dra = 1;
        } else {
            streak_dra++;
        }

        // Going down under middle
        if (streak_dru_player != this->board[j][i]) {
            streak_dru_player = this->board[j][i];
            streak_dru = 1;
        } else {
            streak_dru++;
        }

        // Going up above middle
        if (streak_dla_player != this->board[i][BOARD_SIZE - j]) {
            streak_dla_player = this->board[i][BOARD_SIZE - j];
            streak_dla = 1;
        } else {
            streak_dla++;
        }

        // Going up under middle
        if (streak_dlu_player != this->board[BOARD_SIZE - j][j]) {
            streak_dlu_player = this->board[BOARD_SIZE - j][j];
            streak_dlu = 1;
        } else {
            streak_dlu++;
        }

        // Draw going down
        if (streak_dra >= n_crossed && streak_dru >= n_crossed &&
            streak_dra_player != Token::Empty &&
            streak_dru_player != Token::Empty) {
            this->state = GameState::Win;
            this->end_state = EndState::Draw;
            return;
        }

        // Draw going up
        if (streak_dla >= n_crossed && streak_dlu >= n_crossed &&
            streak_dla_player != Token::Empty &&
            streak_dlu_player != Token::Empty) {
            this->state = GameState::Win;
            this->end_state = EndState::Draw;
            return;
        }

        // Going down under win
        if (streak_dru >= n_crossed && streak_dru_player != Token::Empty) {
            this->state = GameState::Win;
            this->end_state = (EndState)streak_dru_player;
            return;
        }

        // Going down above win
        if (streak_dra >= n_crossed && streak_dra_player != Token::Empty) {
            this->state = GameState::Win;
            this->end_state = (EndState)streak_dra_player;
            return;
        }

        // Going up under win
        if (streak_dlu >= n_crossed && streak_dlu_player != Token::Empty) {
            this->state = GameState::Win;
            this->end_state = (EndState)streak_dlu_player;
            return;
        }

        // Going up above win
        if (streak_dla >= n_crossed && streak_dla_player != Token::Empty) {
            this->state = GameState::Win;
            this->end_state = (EndState)streak_dla_player;
            return;
        }
    }
}

// Rotates a board subsection
void Game::rotateQuadRight(unsigned int y, unsigned int x) {
    // Transpose the quad
    for (int row = 0; row < BOARD_SIZE / 2; row++) {
        for (int col = row; col < BOARD_SIZE / 2; col++) {
            std::swap(this->board[y + row][x + col],
                      this->board[y + col][x + row]);
        }
    }

    // Mirror the board horizontally
    for (int row = 0; row < BOARD_SIZE / 2; row++) {
        for (int col = 0; col < BOARD_SIZE / 2 / 2; col++) {
            std::swap(this->board[y + row][x + col],
                      this->board[y + row][x + BOARD_SIZE / 2 - 1 - col]);
        }
    }
}

// Shortcut for using `rotateQuadRight` 3 times
void Game::rotateQuadLeft(unsigned int y, unsigned int x) {
    for (int i = 0; i < 3; i++) {
        this->rotateQuadRight(y, x);
    }
}

// Fills the game board by parsing a simplified board
//
// 0 - Empty field
// 1 - Player 1
// 2 - Player 2
void Game::fillBoard(const int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            switch (board[y][x]) {
                case 0:
                    this->board[y][x] = Token::Empty;
                    break;
                case 1:
                    this->board[y][x] = Token::Player1;
                    break;
                case 2:
                    this->board[y][x] = Token::Player2;
                    break;
            }
        }
    }
}

int Game::setPlayerName(Token player, const std::string name) {
    if (name.length() > MAX_PLAYER_NAME_LEN) {
        return -1;
    }

    this->players[player].name = name;

    return 0;
}

int Game::setPlayerSymbol(Token player, const char symbol) {
    if (this->players[Token::Player1].symbol == symbol ||
        this->players[Token::Player2].symbol == symbol) {
        return -1;
    }

    this->players[player].symbol = symbol;

    return 0;
}

// Example board used in the 'o' command
void Game::loadExampleBoard() {
    const int example_board[BOARD_SIZE][BOARD_SIZE] = {
        { 0, 1, 0, 0, 2, 0 }, { 2, 2, 1, 2, 1, 0 }, { 0, 2, 0, 0, 0, 0 },
        { 2, 2, 0, 0, 0, 0 }, { 1, 0, 2, 0, 1, 0 }, { 0, 1, 0, 0, 1, 1 },
    };
    this->fillBoard(example_board);
    this->setPlayerName(Token::Player1, "Blue");
    this->setPlayerSymbol(Token::Player1, 'x');
    this->setPlayerName(Token::Player2, "Red");
    this->setPlayerSymbol(Token::Player2, 'o');
    this->setCurrentPlayer(Token::Player2);
}

// Place a token with all the approperiate checks
int Game::placeToken(unsigned int y, unsigned int x, Token token) {
    if (y < 0 || y > BOARD_SIZE || x < 0 || x > BOARD_SIZE) {
        return -1;
    }

    if (this->board[y][x] != Token::Empty) {
        return -2;
    }

    this->board[y][x] = token;

    return 0;
}
