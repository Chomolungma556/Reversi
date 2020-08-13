#include <iostream>
#include <conio.h>

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
    {1, -1}
};

int cells[BOARD_WIDTH][BOARD_HEIGHT];

int cursorX;
int cursorY;

int turn;

bool checkCanPut(int argColor, int argX, int argY, bool turnOver);
bool checkCanPutAll(int argColor);
void display();

int main()
{
    for (int y = 0; y < BOARD_HEIGHT; ++y)
    {
        for (int x = 0; x < BOARD_WIDTH; ++x)
        {
            cells[x][y] = COLOR_NONE;
        }
    }

    cells[3][3] = COLOR_WHITE;
    cells[4][4] = COLOR_WHITE;
    cells[3][4] = COLOR_BLACK;
    cells[4][3] = COLOR_BLACK;

    bool canPut = true;

    while (true)
    {
        display();

        std::cout << "turn:" <<colorNames[turn] << std::endl;
        if (!canPut)
        {
            std::cout << "Can't put!" << std::endl;
        }

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
                checkCanPut(turn, cursorX, cursorY, true);
                cells[cursorX][cursorY] = turn;
                turn ^= 1;

                if (!checkCanPutAll(turn))
                {
                    std::cout << "Pass:" << colorNames[turn] << std::endl;
                    turn ^= 1;
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
                    if (COLOR_NONE != cells[x][y])
                    {
                        ++count[cells[x][y]];
                    }
                }
            }

            display();
            std::cout << "Game Set!" << std::endl;
            for (int i = 0; i < COLOR_MAX; ++i)
            {
                std::cout << colorNames[i] << ":" << count[i] << std::endl;
            }

            if (count[COLOR_BLACK] > count[COLOR_WHITE])
            {
                std::cout << colorNames[COLOR_BLACK] << ":Won!" << std::endl;
            }
            else if (count[COLOR_BLACK] < count[COLOR_WHITE])
            {
                std::cout << colorNames[COLOR_WHITE] << ":Won!" << std::endl;
            }
            else
            {
                std::cout << "Draw!" << std::endl;
            }
            _getch();
            break;
        }
    }
    return 0;
}

bool checkCanPut(int argColor, int argX, int argY, bool turnOver)
{
    if (COLOR_NONE != cells[argX][argY])
    {
        return false;
    }

    for (int i = 0; i < DIRECTION_MAX; ++i)
    {
        int x = argX;
        int y = argY;
        x += directions[i][0];
        y += directions[i][1];
        if ((argColor ^ 1) != cells[x][y])
        {
            continue;
        }

        while (true)
        {
            x += directions[i][0];
            y += directions[i][1];
            if ((x < 0) || (x >= BOARD_WIDTH) || (y < 0) || (y >= BOARD_HEIGHT))
            {
                break;
            }
            if (COLOR_NONE == cells[x][y])
            {
                break;
            }
            if (argColor == cells[x][y])
            {
                if (false == turnOver)
                {
                    return true;
                }

                int turnOverX = argX;
                int turnOverY = argY;
                while (true)
                {
                    cells[turnOverX][turnOverY] = argColor;
                    turnOverX += directions[i][0];
                    turnOverY += directions[i][1];

                    if ((x == turnOverX) && (y == turnOverY))
                    {
                        break;
                    }
                }
                break;
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

void display()
{
    system("cls");
    for (int y = 0; y < BOARD_HEIGHT; ++y)
    {
        for (int x = 0; x < BOARD_WIDTH; ++x)
        {
            if ((cursorX == x) && (cursorY == y))
            {
                std::cout << "";
            }
            else
            {
                switch (cells[x][y])
                {
                case COLOR_BLACK:
                    std::cout << "Z";
                    break;
                case COLOR_WHITE:
                    std::cout << "œ";
                    break;
                case COLOR_NONE:
                    std::cout << "E";
                    break;
                }
            }
        }
        std::cout << std::endl;
    }
}
