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
        int whiteCheckers, blackCheckers;
        bool player; /// 1 - player one(white); false - player two(black)
        bool checkerIsChosen = false;
        bool neighbourCheck(byte x, byte y, bool colour);
        bool throughOneCheck(byte x, byte y, bool colour, bool f);
        bool canEat(byte x, byte y, byte _x, byte _y, bool colour);
        void setCheckersMovable(byte x, byte y, bool movable);
        void setAllCheckersMovable(bool movable);
        void setAllCheckersUnchosen();
        bool canSomeoneEat(byte& x, byte& y, bool player);
    public:
        Game();
        ~Game();
        void getCoords(HWND hwnd, int x, int y, float *ox, float *oy);
        void drawBoard();
        void drawCell(byte i, byte j);
        void findChosenChecker(int *x, int *y);
        void checkerManipulation(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool isCellOnBoard(byte x, byte y);
        bool isCellOnPathLine(byte x, byte y, byte _x, byte _y);
        void moveChecker(byte x, byte y, byte _x, byte _y);
        int getWhiteCheckers();
        int getBlackCheckers();
};

#endif // GAME_H
