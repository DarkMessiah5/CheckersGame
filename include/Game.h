#ifndef GAME_H
#define GAME_H
#include <gl/gl.h>
#include <math.h>
#include <iostream>

#define MAP_WIDTH 8
#define MAP_HEIGHT 8

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

typedef struct {
    bool isFilled;
    bool colour; /// true - white, false - black
    bool checkerColour; /// true - white, false - black
    bool isCheckerChosen;
    bool isCheckerKing;
    bool isCheckerMovable;
} cell;

class Game
{
    private:
        cell board[MAP_WIDTH][MAP_HEIGHT];
        bool moveBoard[MAP_WIDTH][MAP_HEIGHT];
        int whiteCheckers, blackCheckers;
        bool player; /// 1 - player one(white); false - player two(black)
        bool checkerIsChosen = false;

        bool canEat(short x, short y, bool colour, short& _x, short& _y);
        void setCheckersMovable(byte x, byte y, bool movable);
        void setAllCheckersMovable(bool movable);
        void setAllCheckersUnchosen();
        //bool canSomeoneEat(byte& x, byte& y, bool player);
        void moveBacklight(short x, short y);
        void _kingMoveBacklight(short x, short y, short kx, short ky, bool f);
        void kingMoveBacklight(short x, short y);
        void clearBaclight();
        bool ate = false;
    public:
        Game();
        ~Game();
        void getCoords(HWND hwnd, int x, int y, float *ox, float *oy);
        void drawBoard();
        void drawCell(short i, short j);
        void findChosenChecker(int *x, int *y);
        void checkerManipulation(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool isCellOnBoard(byte x, byte y);
        bool isCellOnPathLine(short x, short y);
        void moveChecker(short x, short y, short _x, short _y, bool& M);
        int getWhiteCheckers();
        int getBlackCheckers();
};

#endif // GAME_H
