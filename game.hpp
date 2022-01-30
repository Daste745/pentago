#include <string>

const unsigned int BOARD_SIZE = 6;
static_assert(BOARD_SIZE % 2 == 0);
const unsigned int MAX_PLAYER_NAME_LEN = 10;

enum GameState {
    Setup,
    Win,
    End,
    TicTacToe,
    Pentago,
};

enum EndState {
    Player1Win = 0,
    Player2Win = 1,
    Draw = 2,
};

enum Token {
    Player1 = 0,
    Player2 = 1,
    Empty = 2,
};

struct Player {
    std::string name;
    char symbol;
};

class Game {
   private:
    std::string title;
    GameState state = GameState::Setup;
    EndState end_state;
    Token board[BOARD_SIZE][BOARD_SIZE];
    Player players[2];
    Token current_player;
    void setCurrentPlayer(Token player) { this->current_player = player; }

   public:
    Game(const std::string title);
    void draw();
    void drawStats();
    static void drawHelp();
    void drawPause();
    void drawMenu();
    void drawEnd();
    void update();
    void handleInput();
    void checkWinCondition(unsigned int n_crossed);
    void rotateQuadRight(unsigned int y, unsigned int x);
    void rotateQuadLeft(unsigned int y, unsigned int x);
    bool active() { return this->state != GameState::End; }
    void setState(GameState state) { this->state = state; }
    void fillBoard(const int board[BOARD_SIZE][BOARD_SIZE]);
    int setPlayerName(Token player, const std::string name);
    int setPlayerSymbol(Token player, const char symbol);
    void loadExampleBoard();
    int placeToken(uint y, uint x, Token token);
};
