#include "Game.h"

Game::Game()
{
    whiteCheckers = 12;
    blackCheckers = 12;

    player = 1;

    bool c = false;

    for (byte i = 0; i < MAP_WIDTH; i++) {
        for (byte j = 0; j < MAP_HEIGHT; j++) {
            board[i][j].colour = c; c = !c;
            board[i][j].isChosen = false;
            board[i][j].isFilled = false;
            board[i][j].isKing = false;
        }
        c = !c;
    }

    for (byte i = 0; i < MAP_WIDTH; i+=2) {
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
    //dtor
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
    for (byte i = 0; i < MAP_WIDTH; i++)
        for (byte j = 0; j < MAP_HEIGHT; j++)
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
            glColor3f(1, 0.8, 1);
        else
            glColor3f(0.05, 0.05, 0.05);
        if (board[i][j].isChosen || ((board[i+1][j-1].isChosen && board[i+1][j-1].checkerColour && isCellOnBoard(i+1, j-1) /// для белых на одну клетку
                                   || board[i-1][j-1].isChosen && board[i-1][j-1].checkerColour && isCellOnBoard(i-1, j-1)) && !board[i][j].isFilled)
                                    /// если через одну клетку стоит выделенная, а на соседней - противоположного цвета;
                                    || ((board[i+2][j-2].isChosen && board[i+2][j-2].checkerColour && isCellOnBoard(i+2, j-2)
                                        && !board[i+1][j-1].checkerColour && board[i+1][j-1].isFilled)
                                    || (board[i-2][j-2].isChosen && board[i-2][j-2].checkerColour && isCellOnBoard(i-2, j-2)
                                        && !board[i-1][j-1].checkerColour && board[i-1][j-1].isFilled)) && !board[i][j].isFilled /// отображение спереди

                                    || ((board[i+2][j+2].isChosen && board[i+2][j+2].checkerColour && isCellOnBoard(i+2, j+2)
                                        && !board[i+1][j+1].checkerColour && board[i+1][j+1].isFilled)
                                    || (board[i-2][j+2].isChosen && board[i-2][j+2].checkerColour && isCellOnBoard(i-2, j+2)
                                        && !board[i-1][j+1].checkerColour && board[i-1][j+1].isFilled)) && !board[i][j].isFilled /// отображение сзади


                                     || ((board[i+1][j+1].isChosen && !board[i+1][j+1].checkerColour && isCellOnBoard(i+1, j+1) /// для черных на одну клетку
                                     || board[i-1][j+1].isChosen && !board[i-1][j+1].checkerColour && isCellOnBoard(i-1, j+1)) && !board[i][j].isFilled)
                                     /// если через одну клетку стоит выделенная, а на соседней - противоположного цвета;
                                    || ((board[i+2][j-2].isChosen && !board[i+2][j-2].checkerColour && isCellOnBoard(i+2, j-2)
                                        && board[i+1][j-1].checkerColour && board[i+1][j-1].isFilled)
                                    || (board[i-2][j-2].isChosen && !board[i-2][j-2].checkerColour && isCellOnBoard(i-2, j-2)
                                        && board[i-1][j-1].checkerColour && board[i-1][j-1].isFilled)) && !board[i][j].isFilled /// отображение спереди

                                    || ((board[i+2][j+2].isChosen && !board[i+2][j+2].checkerColour && isCellOnBoard(i+2, j+2)
                                        && board[i+1][j+1].checkerColour && board[i+1][j+1].isFilled)
                                    || (board[i-2][j+2].isChosen && !board[i-2][j+2].checkerColour && isCellOnBoard(i-2, j+2)
                                        && board[i-1][j+1].checkerColour && board[i-1][j+1].isFilled)) && !board[i][j].isFilled) /// отображение сзади
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
                glColor3f(1, 1, 1);
            else
                glColor3f(0.235, 0.196, 0.196);
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
            if (board[i][j].isChosen) {
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
            if (!checkerIsChosen && board[x][y].isFilled) {
               if (player == true && board[x][y].checkerColour) {
                   checkerIsChosen = true;
                   board[x][y].isChosen = !board[x][y].isChosen;
                   player = false;
               }

               if (player == false && !board[x][y].checkerColour) {
                   checkerIsChosen = true;
                   board[x][y].isChosen = !board[x][y].isChosen;
                   player = true;
               }

            }
            else
                if (checkerIsChosen){
                    bool wasMovement = false;
                    int _x, _y;
                    findChosenChecker(&_x, &_y);
                /**
                    Если выбрана белая шашка - то походить можно вверх, если черная - то вниз.
                    Если перед шашкой стоит того же цвета, то ходить сюда нельзя
                    Если другого цвета то можно через нее, при условии, что после нее шашки нет
                    Если шашка дошла до конца доски, то она становится дамкой.
                **/
                    if (board[_x][_y].checkerColour) {  /// Если шашка белая
                        if (!board[_x][_y].isKing) { /// Если не дамка
                            if ((x == _x + 1 || x == _x - 1) && y == _y + 1) { /// походить влево/вправо на одну клетку
                                if (isCellOnBoard(x, y)) {
                                  if (!board[x][y].isFilled) {
                                    moveChecker(_x, _y, x, y);
                                    wasMovement = true;
                                  }

                                }
                            } else if (((((x == _x + 2) && board[_x + 1][_y + 1].isFilled && !board[_x + 1][_y + 1].checkerColour) ||
                                       ((x == _x - 2) && board[_x - 1][_y + 1].isFilled && !board[_x - 1][_y + 1].checkerColour))
                                      && y == _y + 2) /// походить влево/вправо через клетку вперед
                                        || (((x == _x + 2) && board[_x + 1][_y - 1].isFilled && !board[_x + 1][_y - 1].checkerColour) ||
                                           ((x == _x - 2) && board[_x - 1][_y - 1].isFilled && !board[_x - 1][_y - 1].checkerColour)
                                          && y == _y - 2)) { /// походить влево/вправо через клетку назад
                                if (isCellOnBoard(x, y)) {
                                    if (!board[x][y].isFilled) {
                                        moveChecker(_x, _y, x, y);
                                        blackCheckers--;
                                        wasMovement = true;
                                    }

                                }
                            }
                        } else { /// Если дамка
                        }
                    } else { /// Если шашка черная
                        if (!board[_x][_y].isKing) { /// Если не дамка
                            if ((x == _x + 1 || x == _x - 1) && y == _y - 1) { /// походить влево/вправо на одну клетку
                                if (isCellOnBoard(x, y)) {
                                  if (!board[x][y].isFilled) {
                                    moveChecker(_x, _y, x, y);
                                    wasMovement = true;
                                  }
                                }
                            } else if (((((x == _x + 2) && board[_x + 1][_y - 1].isFilled && board[_x + 1][_y - 1].checkerColour) ||
                                       ((x == _x - 2) && board[_x - 1][_y - 1].isFilled && board[_x - 1][_y - 1].checkerColour))
                                         && y == _y - 2) /// походить влево/вправо через клетку
                                         || (((x == _x + 2) && board[_x + 1][_y + 1].isFilled && board[_x + 1][_y + 1].checkerColour) ||
                                           ((x == _x - 2) && board[_x - 1][_y + 1].isFilled && board[_x - 1][_y + 1].checkerColour))
                                          && y == _y + 2) { /// походить влево/вправо через клетку назад
                                if (isCellOnBoard(x, y)) {
                                    if (!board[x][y].isFilled) {
                                        moveChecker(_x, _y, x, y);
                                        whiteCheckers--;
                                        wasMovement = true;
                                    }
                                }
                            }
                        } else { /// Если дамка
                        }
                    }
                    board[x][y].isChosen = false;
                    if (_x > -1 && _y > -1)
                        board[_x][_y].isChosen = false;
                    checkerIsChosen = false;
                    if (!wasMovement) {
                        player = !player;
                        wasMovement = !wasMovement;
                    }
            }
}

bool Game::isCellOnBoard(byte x, byte y)
{
    return ((x < MAP_WIDTH) && (y < MAP_HEIGHT));
}

bool Game:: isCellOnPathLine(byte x, byte y, byte _x, byte _y)
{
    for (byte i = 0; i < MAP_WIDTH; i++)
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
}

int Game::getWhiteCheckers()
{
    return whiteCheckers;
}

int Game::getBlackCheckers()
{
    return blackCheckers;
}
