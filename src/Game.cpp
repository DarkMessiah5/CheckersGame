#include "Game.h"

Game::Game()
{
    whiteCheckers = 12;
    blackCheckers = 12;

    player = 1;

    ate = false;

    bool c = false;

    for (short i = 0; i < MAP_WIDTH; i++) {
        for (short j = 0; j < MAP_HEIGHT; j++) {
            board[i][j].colour = c; c = !c;
            board[i][j].isCheckerChosen = false;
            board[i][j].isFilled = false;
            board[i][j].isCheckerKing = false;
            board[i][j].isCheckerMovable = true;
            moveBoard[i][j] = false;
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

bool Game::canEat(short x, short y, bool colour, short& _x, short& _y)
{
    short dx1 = 0, dx2 = 0, dy1 = 0, dy2 = 0;

    if (!board[x][y].isCheckerKing) {
        std::cout << "NOT CHECKER KING!\n\n";
        for (short i = -2; i <= 2; i+=4)
        for (short j = -2; j <= 2; j+=4) {
            dx1 = i / 2; dy1 = j / 2;
            dx2 = i;     dy2 = j;
        if (board[x + dx1][y + dy1].isFilled
            && (board[x + dx1][y + dy1].checkerColour != colour)
            && !board[x + dx2][y + dy2].isFilled && isCellOnBoard(x + dx1, y + dy1) && isCellOnBoard(x + dx2, y + dy2))
            return true;
        }
    return false;
    } else {
        std::cout << "CHECKER KING!\n\n";
        short kx = 1, ky = 1, s1, s2;

    for (short i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                kx = 1;
                ky = 1;
                break;
            case 1:
                kx = 1;
                ky = -1;
                break;
            case 2:
                kx = -1;
                ky = -1;
                break;
            case 3:
                kx = -1;
                ky = 1;
                break;
        }
     for (short j = 1; j < MAP_HEIGHT; j++) {
            if (isCellOnBoard(x + kx * j, y + ky * j))
                if (isCellOnBoard(x + kx * (j + 1), x + ky * (j + 1)) && !board[x + kx * (j + 1)][y + ky * (j + 1)].isFilled
                                                                     && board[x + kx * (j)][y + ky * (j)].isFilled
                                                                     && board[x + kx * (j)][y + ky * (j)].checkerColour != player)
                   return true;



        }
    }
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

void Game::drawCell(short i, short j)
{
    glBegin(GL_TRIANGLE_STRIP);
        if (board[i][j].colour)
            glColor3f(1, 1, 1);
        else
            glColor3f(0.235, 0.196, 0.196);
        if (moveBoard[i][j])
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

    glBegin(GL_TRIANGLE_FAN);
    if (board[i][j].isCheckerKing && board[i][j].isFilled) {
                glColor3f(1, 0.8, 0.00);
                for (int i = -1; i < points; i++) {
                    x = sin(alpha * i) * len / 5;
                    y = cos(alpha * i) * len / 5;
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

            short a, b;

            setAllCheckersMovable(false);
            bool f = false;
            for (short i = 0; i < MAP_WIDTH; i++) {
                for (short j = 0; j < MAP_HEIGHT; j++) {
                    if ((board[i][j].checkerColour == player) && board[i][j].isFilled && canEat(i, j, board[i][j].checkerColour,  a, b)) {
                        f = true;
                        setCheckersMovable(i,j, true);
                    }
                }
            }

            if (!f) setAllCheckersMovable(true);

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
               ate = false;
               if (!board[x][y].isCheckerKing)
                   moveBacklight(x, y);
               else
                   kingMoveBacklight(x, y);
            }
            else
                if (checkerIsChosen){
                    bool wasMovement = false;
                    int _x, _y;
                    findChosenChecker(&_x, &_y);
                    bool _canEat;

    if (board[_x][_y].isCheckerMovable) {
            moveChecker(_x, _y, x, y, wasMovement);
            _canEat = canEat(x, y, board[x][y].checkerColour, a, b);
            std::cout << "WAS MOVEMENT: " << wasMovement << "\n\n";
            std::cout << "ATE: " << ate << "\n\n";
            std::cout << "CAN EAT: " << _canEat << "\n\n";

            if (wasMovement) {
                if (ate) {
                    if (board[x][y].checkerColour == player)
                    if (_canEat) {
                            checkerIsChosen = false;
                            setAllCheckersUnchosen();
                            setAllCheckersMovable(false);
                            setCheckersMovable(x, y, true);
                    } else {
                        setAllCheckersMovable(true);
                        player = !player;
                    setAllCheckersUnchosen();
                    checkerIsChosen = false;
                    }
                } else {
                    player = !player;
                    setAllCheckersUnchosen();
                    checkerIsChosen = false;
                }
            } else {
                setAllCheckersUnchosen();
                checkerIsChosen = false;
                clearBaclight();
            }


    }
    }
}

bool Game::isCellOnBoard(byte x, byte y)
{
    return ((x < MAP_WIDTH) && (y < MAP_HEIGHT));
}

void Game::moveChecker(short x, short y, short _x, short _y, bool& M)
{
        /// Если двигаемся через одну клетку, то "съедаем" шашку
    if (x == _x && y == _y || !moveBoard[_x][_y]) {
        M = false;
        //player = !player;
        return;
    }

    ate = false;

    short kx, ky;

    if (!board[x][y].isCheckerKing) {
        if (x + 2 == _x) {
            if (board[x][y].checkerColour)
                blackCheckers--;
            else
                whiteCheckers--;
            ate = true;
        if (_y == y + 2)
            board[x + 1][y + 1].isFilled = false;
        else
            board[x + 1][y - 1].isFilled = false;
    } else {
        if (x - 2 == _x) {
            if (board[x][y].checkerColour)
                blackCheckers--;
            else
                whiteCheckers--;
            ate = true;
            if (_y == y + 2)
                board[x - 1][y + 1].isFilled = false;
            else
                board[x - 1][y - 1].isFilled = false;
        }

    }
    } else {
        if (_x - x > 0)
            kx = 1;
        else
            kx = -1;
        if (_y - y > 0)
            ky = 1;
        else ky = -1;

        for (short i = x + kx; i != _x; i += kx) {
            for (short j = y + ky; j != _y; j += ky) {
                std::cout << i << ", " << j << "\n";
                if (board[i][j].isFilled) {
                    if (board[x][y].checkerColour)
                        blackCheckers--;
                    else
                        whiteCheckers--;
                    board[i][j].isFilled = false;
                    ate = true;
                }
            }
        }
    }



    M = true;


    board[x][y].isFilled = false;
    board[_x][_y].isFilled = true;
    board[_x][_y].checkerColour = board[x][y].checkerColour;

    board[x][y].isCheckerChosen = false;
    board[_x][_y].isCheckerChosen = true;

    board[_x][_y].isCheckerKing = board[x][y].isCheckerKing;
    board[x][y].isCheckerKing = false;

   // if (_y == 7 && board[_x][_y].checkerColour)
   //     board[_x][_y].isCheckerKing = true;

   // if (_y == 0 && !board[_x][_y].checkerColour)
   //     board[_x][_y].isCheckerKing = true;
    //player = !player;
    clearBaclight();
}

void Game::_kingMoveBacklight(short x, short y, short kx, short ky, bool f)
{
    for (short j = 1; j < MAP_HEIGHT; j++) {
            if (isCellOnBoard(x + kx * j, y + ky * j)) {
                if (!board[x + kx * j][y + ky * j].isFilled && !f) {
                        moveBoard[x + kx * j][y + ky * j] = true;
                } else if (f && isCellOnBoard(x + kx * (j + 1), y + ky * (j + 1)) && !board[x + kx * (j + 1)][y + ky * (j + 1)].isFilled
                           && board[x + kx * (j)][y + ky * (j)].isFilled
                           && (board[x + kx * (j)][y + ky * (j)].checkerColour == player)) {
                               std::cout << "ENTERED THE CONDITION!!!\n\n";
                               moveBoard[x + kx * (j + 1)][y + ky * (j + 1)] = true;
                }
                if ((board[x + kx * j][y + ky * j].checkerColour == board[x][y].checkerColour) && board[x + kx * j][y + ky * j].isFilled) {
                    j = 7;
                    continue;
                }
                if (isCellOnBoard(x + kx * (j + 1), y + ky * (j + 1)) && board[x + kx * (j + 1)][y + ky * (j + 1)].isFilled
                                                                     && board[x + kx * (j)][y + ky * (j)].isFilled) {
                   j = 7;
                   continue;
                }

            }
        }
}

void Game::kingMoveBacklight(short x, short y)
{
    if (player != board[x][y].checkerColour) {
        clearBaclight();
        return;
    }

    short kx, ky;
    short s1, s2;
    bool f = canEat(x, y, board[x][y].checkerColour, kx, ky);

    for (short i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                kx = 1;
                ky = 1;
                break;
            case 1:
                kx = 1;
                ky = -1;
                break;
            case 2:
                kx = -1;
                ky = -1;
                break;
            case 3:
                kx = -1;
                ky = 1;
                break;
        }
        _kingMoveBacklight(x, y, kx, ky, f);
    }
}

int Game::getWhiteCheckers()
{
    return whiteCheckers;
}

int Game::getBlackCheckers()
{
    return blackCheckers;
}

void Game::moveBacklight(short x, short y)
{
    if (player != board[x][y].checkerColour) {
        clearBaclight();
        return;
    }
    int dy = 0, dy2 = 0;
    bool f = false;
    if (!board[x][y].checkerColour) {
        dy = -2;
        dy2 = -4;
    }

    moveBoard[x][y] = true;

    if (isCellOnBoard(x + 2, y + 2) &&
        !board[x + 2][y + 2].isFilled &&
        board[x + 1][y + 1].isFilled &&
        board[x + 1][y + 1].checkerColour != board[x][y].checkerColour) {
            moveBoard[x + 2][y + 2] = true;
            f = true;
    }


    if (isCellOnBoard(x - 2, y + 2) &&
        !board[x - 2][y + 2].isFilled &&
        board[x - 1][y + 1].isFilled &&
        board[x - 1][y + 1].checkerColour != board[x][y].checkerColour) {
            moveBoard[x - 2][y + 2] = true;
            f = true;
    }

    if (isCellOnBoard(x + 2, y - 2) &&
        !board[x + 2][y - 2].isFilled &&
        board[x + 1][y - 1].isFilled &&
        board[x + 1][y - 1].checkerColour != board[x][y].checkerColour) {
            moveBoard[x + 2][y - 2] = true;
            f = true;
    }


    if (isCellOnBoard(x - 2, y - 2) &&
        !board[x - 2][y - 2].isFilled &&
        board[x - 1][y - 1].isFilled &&
        board[x - 1][y - 1].checkerColour != board[x][y].checkerColour) {
            moveBoard[x - 2][y - 2] = true;
            f = true;
    }

    if (f) {
        return;
    }

    if (isCellOnBoard(x + 1, y + 1 + dy) && !board[x + 1][y + 1 + dy].isFilled)
        moveBoard[x + 1][y + 1 + dy] = true;


    if (isCellOnBoard(x - 1, y + 1 + dy)&& !board[x - 1][y + 1 + dy].isFilled)
        moveBoard[x - 1][y + 1 + dy] = true;
}

void Game::clearBaclight()
{
    for (short i = 0; i < MAP_WIDTH; i++)
        for (short j = 0; j < MAP_HEIGHT; j++)
            moveBoard[i][j] = false;
}
