#include <iostream>
#include <conio.h>
#include <stdlib.h>

static const int BOARD_WIDTH = 8;
static const int BOARD_HEIGHT = 8;

enum
{
    COLOR_NONE = -1,
    COLOR_BLACK = 0,
    COLOR_WHITE = 1,
    COLOR_MAX
};

enum
{
    DIRECTION_UP,
    DIRECTION_UP_LEFT,
    DIRECTION_LEFT,
    DIRECTION_DOWN_LEFT,
    DIRECTION_DOWN,
    DIRECTION_DOWN_RIGHT,
    DIRECTION_RIGHT,
    DIRECTION_UP_RIGHT,
    DIRECTION_MAX
};

char colorNames[][5 + 1] =
{
    "Black",
    "White"
};

int directions[][2] =
{
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
};

int cells[BOARD_HEIGHT][BOARD_WIDTH];

int cursorX;
int cursorY;

int turn;

void display();
bool checkCanPut(int argColor, int argCursorX, int argCursorY, bool turnOver);
bool checkCanPutAll(int argColor);

int main()
{
    // initialize
    for (int y = 0; y < BOARD_HEIGHT; ++y)
    {
        for (int x = 0; x < BOARD_WIDTH; ++x)
        {
            cells[y][x] = COLOR_NONE;
        }
    }

    cells[3][3] = COLOR_WHITE;
    cells[4][4] = COLOR_WHITE;
    cells[3][4] = COLOR_BLACK;
    cells[4][3] = COLOR_BLACK;

    bool canPut = true;

    while (1)
    {
        display();

        if (!canPut)
        {
            std::cout << "Can't put!!" << std::endl;
        }

        std::cout << "turn : " << colorNames[turn] << std::endl;

        canPut = true;

        switch (_getch())
        {
        case 'w':
            --cursorY;
            break;
        case 's':
            ++cursorY;
            break;
        case 'a':
            --cursorX;
            break;
        case 'd':
            ++cursorX;
            break;
        default:
            if (checkCanPut(turn, cursorX, cursorY, false))
            {
                // turn over stone before do selected point
                checkCanPut(turn, cursorX, cursorY, true);
                cells[cursorY][cursorX] = turn;
                turn = !turn;

                if (!checkCanPutAll(turn))
                {
                    std::cout << "Pass!" << std::endl;
                    turn = !turn;
                }
            }
            else
            {
                canPut = false;
            }
            break;
        }

        if ((!checkCanPutAll(COLOR_BLACK)) && (!checkCanPutAll(COLOR_WHITE)))
        {
            int count[COLOR_MAX] = {};

            for (int y = 0; y < BOARD_HEIGHT; ++y)
            {
                for (int x = 0; x < BOARD_WIDTH; ++x)
                {
                    if (COLOR_NONE != cells[y][x])
                    {
                        ++count[cells[y][x]];
                    }

                }
            }

            display();

            for (int i = 0; i < COLOR_MAX; ++ i)
            {
                std::cout << colorNames[i] << ":" << count[i] << std::endl;
            }

            if (count[COLOR_BLACK] == count[COLOR_WHITE])
            {
                std::cout << "Draw!" << std::endl;
            }
            else if (count[COLOR_BLACK] > count[COLOR_WHITE])
            {
                std::cout << "Winner:" << colorNames[COLOR_BLACK]<< std::endl;
            }
            else
            {
                std::cout << "Winner:" << colorNames[COLOR_WHITE] << std::endl;
            }

            break;
        }
    }
    return 0;
}

void display()
{
    system("cls");
    for (int y = 0; y < BOARD_HEIGHT; ++y)
    {
        for (int x = 0; x < BOARD_WIDTH; ++x)
        {
            if (cursorX == x && cursorY == y)
            {
                std::cout << "";
            }
            else
            {
                switch (cells[y][x])
                {
                case COLOR_NONE:
                    std::cout << "E";
                    break;
                case COLOR_BLACK:
                    std::cout << "›";
                    break;
                case COLOR_WHITE:
                    std::cout << "œ";
                    break;
                default:
                    break;
                }
            }
        }
        std::cout << std::endl;
    }
}

bool checkCanPut(int argColor, int argCursorX, int argCursorY, bool argTurnOver)
{
    if (COLOR_NONE != cells[argCursorY][argCursorX])
    {
        return false;
    }

    for (int i = 0; i < DIRECTION_MAX; ++i)
    {
        int x = argCursorX;
        int y = argCursorY;

        x += directions[i][0];
        y += directions[i][1];

        if((!argColor) != cells[y][x])
        {
            continue;
        }

        while (1)
        {
            x += directions[i][0];
            y += directions[i][1];

            if (x < 0 || x >= BOARD_WIDTH || y < 0 || y > BOARD_HEIGHT)
            {
                break;
            }
            if (COLOR_NONE == cells[y][x])
            {
                break;
            }
            if (argColor == cells[y][x])
            {
                if (!argTurnOver)
                {
                    return true;
                }

                int turnOverX = argCursorX;
                int turnOverY = argCursorY;

                while (1)
                {
                    cells[turnOverY][turnOverX] = argColor;

                    turnOverX += directions[i][0];
                    turnOverY += directions[i][1];

                    if (x == turnOverX && y == turnOverY)
                    {
                        break;
                    }
                }
            }
        }
    }

    return false;
}

bool checkCanPutAll(int argColor)
{
    for (int y = 0; y < BOARD_HEIGHT; ++y)
    {
        for (int x = 0; x < BOARD_WIDTH; ++x)
        {
            if (checkCanPut(argColor, x, y, false))
            {
                return true;
            }
        }
    }

    return false;
}
