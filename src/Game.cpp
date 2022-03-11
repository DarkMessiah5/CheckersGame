#include "Game.h"

Game::Game()
{
    whiteCheckers = 12;
    blackCheckers = 12;

    player = 1;

    bool c = false;

    for (short i = 0; i < MAP_WIDTH; i++) {
        for (short j = 0; j < MAP_HEIGHT; j++) {
            board[i][j].colour = c; c = !c;
            board[i][j].isCheckerChosen = false;
            board[i][j].isFilled = false;
            board[i][j].isCheckerKing = false;
            board[i][j].isCheckerMovable = true;
        }
        c = !c;
    }

    for (short i = 0; i < MAP_WIDTH; i+=2) {
        board[i][0].isFilled = true;
        board[i][0].checkerColour = true;

        board[i+1][1].isFilled = true;
        board[i+1][1].checkerColour = true;

        board[i][2].isFilled = true;
        board[i][2].checkerColour = true;

        board[i+1][5].isFilled = true;
        board[i+1][5].checkerColour = false;

        board[i][6].isFilled = true;
        board[i][6].checkerColour = false;

        board[i+1][7].isFilled = true;
        board[i+1][7].checkerColour = false;
    }
}

Game::~Game()
{
}

bool Game::neighbourCheck(byte x, byte y, bool colour)
{
    short dy = 0;

    if (!colour) dy = 2;

    return ((board[x+1][y-1 + dy].isCheckerChosen && (board[x+1][y-1 + dy].checkerColour == colour) && isCellOnBoard(x+1, y-1 + dy) /// для белых на одну клетку
            || board[x-1][y-1 + dy].isCheckerChosen && (board[x-1][y-1 + dy].checkerColour == colour) && isCellOnBoard(x-1, y-1 + dy)) && !board[x][y].isFilled);
}

bool Game::throughOneCheck(byte x, byte y, bool colour, bool f)
{
    short dy1 = 0, dy2 = 0;

    if (f) {
        dy1 = 2;
        dy2 = 4;
    }
    return ((board[x+2][y-2 + dy2].isCheckerChosen && (board[x+2][y-2 + dy2].checkerColour == colour) && isCellOnBoard(x+2, y-2 + dy2)
                                        && (!board[x+1][y-1 + dy1].checkerColour == colour) && board[x+1][y-1 + dy1].isFilled)
          || (board[x-2][y-2 + dy2].isCheckerChosen && (board[x-2][y-2 + dy2].checkerColour == colour) && isCellOnBoard(x-2, y-2 + dy2)
                                        && (!board[x-1][y-1 + dy1].checkerColour == colour) && board[x-1][y-1 + dy1].isFilled));
}

bool Game::canEat(byte x, byte y, byte _x, byte _y, bool colour)
{
    short dx1 = 0, dx2 = 0, dy1 = 0, dy2 = 0;

    for (short i = -2; i <= 2; i+=4)
        for (short j = -2; j <= 2; j+=4) {
            dx1 = i / 2; dy1 = j / 2;
            dx2 = i;     dy2 = j;
        if (board[_x + dx1][_y + dy1].isFilled
            && (board[_x + dx1][_y + dy1].checkerColour != colour)
            && !board[_x + dx2][_y + dy2].isFilled && isCellOnBoard(_x + dx1, _y + dy1) && isCellOnBoard(_x + dx2, _y + dy2))
            return true;
        }
    return false;
}

void Game::setCheckersMovable(byte x, byte y, bool movable)
{
   board[x][y].isCheckerMovable = movable;
}

void Game::setAllCheckersMovable(bool movable)
{
    for (short i = 0; i < MAP_WIDTH; i++)
        for (short j = 0; j < MAP_HEIGHT; j++)
            board[i][j].isCheckerMovable = movable;
}

void Game::setAllCheckersUnchosen()
{
    for (short i = 0; i < MAP_WIDTH; i++)
        for (short j = 0; j < MAP_HEIGHT; j++)
            board[i][j].isCheckerChosen = false;
}

void Game::getCoords(HWND hwnd, int x, int y, float *ox, float *oy)
{
    RECT rct;
    GetClientRect(hwnd, &rct);
    *ox = x / (float)rct.right * MAP_WIDTH;
    *oy = MAP_HEIGHT - y / (float)rct.bottom * MAP_HEIGHT;
}

void Game::drawBoard()
{
    for (short i = 0; i < MAP_WIDTH; i++)
        for (short j = 0; j < MAP_HEIGHT; j++)
        {
            glLoadIdentity();
            glScalef(2.0/MAP_WIDTH, 2.0/MAP_HEIGHT, 1);
            glTranslatef(-MAP_WIDTH * 0.5, -MAP_HEIGHT * 0.5, 0);
            glPushMatrix();
            glTranslatef(i, j, 0);
            drawCell(i, j);
            glPopMatrix();
        }
}

void Game::drawCell(byte i, byte j)
{
    glBegin(GL_TRIANGLE_STRIP);
        if (board[i][j].colour)
            glColor3f(1, 1, 1);
        else
            glColor3f(0.235, 0.196, 0.196);
        if (board[i][j].isCheckerChosen || neighbourCheck(i, j, true) /// для белых на одну клетку
                                    /// если через одну клетку стоит выделенная, а на соседней - противоположного цвета;
                                    || throughOneCheck(i, j, true, false) && !board[i][j].isFilled /// отображение спереди
                                    || throughOneCheck(i, j, true, true) && !board[i][j].isFilled /// отображение сзади

                                     || neighbourCheck(i, j, false) /// для черных на одну клетку
                                     /// если через одну клетку стоит выделенная, а на соседней - противоположного цвета;
                                    || throughOneCheck(i, j, false, false) && !board[i][j].isFilled /// отображение спереди

                                    || throughOneCheck(i, j, false, true) && !board[i][j].isFilled) /// отображение сзади
            glColor3f(0.2, 1, 0.2);
        glVertex2f(0, 1);
        glVertex2f(1, 1);
        glVertex2f(0, 0);
        glVertex2f(1, 0);
    glEnd();

    float x, y;
    float points = 40;
    float len = 0.5;
    float alpha = M_PI * 2 / points;

    glBegin(GL_TRIANGLE_FAN);
        if (board[i][j].isFilled) {
            if (board[i][j].checkerColour)
                glColor3f(1, 0.8, 1);
            else
                glColor3f(0.05, 0.05, 0.05);
            for (int i = -1; i < points; i++) {
                x = sin(alpha * i) * len;
                y = cos(alpha * i) * len;
                glVertex2f(x + 0.5, y + 0.5);
            }
        }
    glEnd();
}

void Game::findChosenChecker(int *x, int *y)
{
    for (int i = 0; i < MAP_WIDTH; i++)
        for (int j = 0; j < MAP_HEIGHT; j++)
            if (board[i][j].isCheckerChosen) {
                *x = i; *y = j;
                return;
            }
    *x = -1; *y = -1;
}

void Game::checkerManipulation(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINTFLOAT pf;
    getCoords(hwnd, LOWORD(lParam), HIWORD(lParam), &pf.x, &pf.y);
            int x = (int)pf.x;
            int y = (int)pf.y;
            int __x = -1, __y = -1;
            std::cout << "WHITE CHECKERS: " << whiteCheckers;
            std::cout << "\n\nBLACK CHECKERS: " << blackCheckers;
            std::cout << "\n\nCELL: " << x << ", " << y << "\n";

            std::cout << "PLAYER: " << player << "\n";




            setAllCheckersMovable(false);
            std::cout << "ALL CHECKERS ARE DISABLED!!!!!!!\n";

            for (short i = 0; i < MAP_WIDTH; i++)
                for (short j = 0; j < MAP_HEIGHT; j++) {
                    if (board[i][j].checkerColour == player && board[i][j].isFilled) {
                        setCheckersMovable(i, j, false);
                        if (canEat(i, j, i, j, board[i][j].checkerColour)) {
                            std::cout << "WE ARE HERE!!!!!!!\n";
                           // board[i][j].checkerColour = !board[i][j].checkerColour;
                            setCheckersMovable(i, j, true);
                            __x = (int)i; __y = (int)j;
                            std::cout << "CELL: " << __x << ", " << __y << " <--CHECKER ENABLED\n";
                            //break;
                        }
                    }
                }
            if (__x == -1) {
                std::cout << "ALL CHECKERS ARE ENABLED!!!!!!!\n";
                setAllCheckersMovable(true);
            }



            std::cout << "|||MAP CELL: " << x << ", " << y
                              << "\nCELL COLOUR: " << board[x][y].colour
                              << "\nIS FILLED: " << board[x][y].isFilled
                              << "\n COLOUR: " << board[x][y].checkerColour
                              << "\nIS CHECKER KING: " << board[x][y].isCheckerKing
                             << "\nIS CHECKER CHOSEN: " << board[x][y].isCheckerChosen
                              << "\n IS CHECKER MOVABLE" << board[x][y].isCheckerMovable
                              << "|||\n\n";



            if (!checkerIsChosen && board[x][y].isFilled && board[x][y].isCheckerMovable) {
               if (player == true && board[x][y].checkerColour) {
                   checkerIsChosen = true;
                   board[x][y].isCheckerChosen = !board[x][y].isCheckerChosen;
                   //player = false;
               }

               if (player == false && !board[x][y].checkerColour) {
                   checkerIsChosen = true;
                   board[x][y].isCheckerChosen = !board[x][y].isCheckerChosen;
                   //player = true;
               }

            }
            else
                if (checkerIsChosen){
                    bool wasMovement = false;
                    int _x, _y;
                    findChosenChecker(&_x, &_y);
                    bool _canEat;
                /**
                    Если выбрана белая шашка - то походить можно вверх, если черная - то вниз.
                    Если перед шашкой стоит того же цвета, то ходить сюда нельзя
                    Если другого цвета то можно через нее, при условии, что после нее шашки нет
                    Если шашка дошла до конца доски, то она становится дамкой.
                **/ if (board[_x][_y].isCheckerMovable)
                    if (board[_x][_y].checkerColour) {  /// Если шашка белая
                        if (!board[_x][_y].isCheckerKing) { /// Если не дамка
                            _canEat = canEat(x, y, _x, _y, true);
                            std::cout << "_canEat: " << _canEat << "\n";
                            if  (!_canEat && (x == _x + 1 || x == _x - 1) && (y == _y + 1)) { /// походить влево/вправо на одну клетку
                                if (isCellOnBoard(x, y)) {
                                  if (!board[x][y].isFilled) {
                                    moveChecker(_x, _y, x, y);
                                    wasMovement = true;
                                    board[_x][_y].isCheckerChosen = false;
                                    std::cout << "ЗАШЛИ В 1е УСЛОВИЕ\n";
                                  }

                                }
                            } else if (_canEat && ((x == _x + 2 || x == _x - 2) && (y == _y + 2 || y == _y - 2))) { /// походить влево/вправо через клетку назад
                                if (isCellOnBoard(x, y)) {
                                    if (!board[x][y].isFilled) {
                                        moveChecker(_x, _y, x, y);
                                        std::cout << "MOVED WHITE CHECKER THROUNGH ONE\n\n";
                                        //_x += x; _y += y;
                                        whiteCheckers--;
                                        board[_x][_y].isCheckerChosen = false;
                                        if (canEat(x, y, x, y, true)) {
                                            //setCheckersMovable(x, y, false);
                                            board[x][y].isCheckerChosen = true;
                                            wasMovement = false;
                                        } else {
                                            //setCheckersMovable(x, y, true);
                                            board[x][y].isCheckerChosen = false;
                                            wasMovement = true;
                                        }

                                    } else {
                                       // board[_x][_y].isCheckerChosen = false;
                                      //  checkerIsChosen = false;
                                      std::cout << "ENTERED 'ELSE' OF CONDITION 'IS FILLED' FOR WHITE \n";
                                    }

                                }
                            } else {
                                std::cout << "ENTERED 'ELSE' OF CONDITION 'CAN EAT AND MOVE THROUGH' FOR WHITE \n";
                               // board[_x][_y].isCheckerChosen = false;
                              //  checkerIsChosen = false;

                            }
                        } else { /// Если дамка
                        }
                    } else { /// Если шашка черная
                        std::cout << "ENTERED BLACK_CHECKER CONDITION\n";
                        if (!board[_x][_y].isCheckerKing) { /// Если не дамка
                            _canEat = canEat(x, y, _x, _y, false);
                            if (!_canEat && (x == _x + 1 || x == _x - 1) && y == _y - 1) { /// походить влево/вправо на одну клетку
                                if (isCellOnBoard(x, y)) {
                                  if (!board[x][y].isFilled) {
                                    moveChecker(_x, _y, x, y);
                                    wasMovement = true;
                                    board[_x][_y].isCheckerChosen = false;

                                  } else {
                                      board[_x][_y].isCheckerChosen = false;
                                     checkerIsChosen = false;

                                  }
                                }
                            } else if (_canEat && ((x == _x + 2 || x == _x - 2) && (y == _y + 2 || y == _y - 2))) { /// походить влево/вправо через клетку назад
                                if (isCellOnBoard(x, y)) {
                                    if (!board[x][y].isFilled) {
                                        moveChecker(_x, _y, x, y);
                                        //_x += x; _y += y;
                                        blackCheckers--;
                                        board[_x][_y].isCheckerChosen = false;
                                        if (canEat(x, y, x, y, false)) {
                                            //setCheckersMovable(x, y, false);
                                            board[x][y].isCheckerChosen = true;
                                            //board[_x][_y].isCheckerChosen = false;
                                            wasMovement = false;
                                        } else {
                                           // setCheckersMovable(x, y, true);
                                            board[x][y].isCheckerChosen = false;
                                            wasMovement = true;
                                        }
                                    }
                                }
                            } else {
                                board[_x][_y].isCheckerChosen = false;
                                board[x][y].isCheckerChosen = false;
                                checkerIsChosen = false;
                            }
                        } else { /// Если дамка
                        }
                    }
                    setAllCheckersUnchosen();
                    if (!wasMovement) {
                        //player = !player;
                        if (_canEat) {
                            //wasMovement = !wasMovement;
                            checkerIsChosen = true;
                            board[x][y].isCheckerChosen = true;
                            setCheckersMovable(x, y, true);
                        } else {
                            board[x][y].isCheckerChosen = false;
                            checkerIsChosen = false;
                            setAllCheckersMovable(true);
                        }

                    } else {
                        player = !player;
                        checkerIsChosen = false;
                        board[x][y].isCheckerChosen = false;
                       //if ()
                    }

            }
            //for (byte i = 0; i < MAP_WIDTH; i++)
            //    for (byte j = 0; j < MAP_HEIGHT; j++) {
            //        std::cout << "MAP CELL: " << (int)i << ", " << (int)j
            //                  << "\nCELL COLOUR: " << board[i][j].colour
            //                  << "\nIS FILLED: " << board[i][j].isFilled
            //                  << "\n COLOUR: " << board[i][j].checkerColour
            //                  << "\nIS CHECKER KING: " << board[i][j].isCheckerKing
            //                 << "\nIS CHECKER CHOSEN: " << board[i][j].isCheckerChosen
             //                 << "\n IS CHECKER MOVABLE" << board[i][j].isCheckerMovable
               //               << "\n\n";
              //  }
}

bool Game::isCellOnBoard(byte x, byte y)
{
    return ((x < MAP_WIDTH) && (y < MAP_HEIGHT));
}

bool Game:: isCellOnPathLine(byte x, byte y, byte _x, byte _y)
{
    for (short i = 0; i < MAP_WIDTH; i++)
        if (x == _x + i && y == _y + i)
            return true;

    return false;
}

void Game::moveChecker(byte x, byte y, byte _x, byte _y)
{
        /// Если двигаемся через одну клетку, то "съедаем" шашку
        if (x + 2 == _x) {
           if (_y == y + 2)
                board[x + 1][y + 1].isFilled = false;
            else
                board[x + 1][y - 1].isFilled = false;
        } else {
            if (x - 2 == _x)
                if (_y == y + 2)
                    board[x - 1][y + 1].isFilled = false;
                else
                    board[x - 1][y - 1].isFilled = false;
        }



    board[x][y].isFilled = false;
    board[_x][_y].isFilled = true;
    board[_x][_y].checkerColour = board[x][y].checkerColour;

    board[_x][_y].isCheckerKing = board[x][y].isCheckerKing;
    board[x][y].isCheckerKing = false;

    if (_y == 7 && board[_x][_y].checkerColour) board[_x][_y].isCheckerKing = true;
    if (_y == 0 && !board[_x][_y].checkerColour) board[_x][_y].isCheckerKing = true;
}

int Game::getWhiteCheckers()
{
    return whiteCheckers;
}

int Game::getBlackCheckers()
{
    return blackCheckers;
}
