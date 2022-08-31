#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <conio.h>

using namespace std;//====================================================================

enum Tcell : char {
    CROSS = 'X',
    ZERO = 'O',
    EMPTY = '_'
};

struct TCoord
{
    size_t y{ 0U };
    size_t x{ 0U };
};

enum TProgress
{
    IN_PROGREESS,
    WON_HUMAN,
    WON_AI,
    DRAW
};

#pragma pack (push, 1)

struct TGame
{
    Tcell** ppField{ nullptr };
    const size_t SIZE{ 3U };
    TProgress progreess{ IN_PROGREESS };
    Tcell human{}, ai{};
    size_t turn{ 0U };
    int difficulty{ 0 };
};

//===============================================================================================
#pragma pack (pop)
int32_t getRandomNum(const int32_t min, const int32_t max)
{
    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}

void iritgame(TGame& g) {
    g.ppField = new Tcell * [g.SIZE];
    for (size_t i = 0u; i < g.SIZE; i++)
    {
        g.ppField[i] = new Tcell[g.SIZE];
    }
    for (size_t y = 0u; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            g.ppField[y][x] = EMPTY;
        }
    }
    if (getRandomNum(0, 1000) > 500)
    {
        g.human = CROSS;
        g.ai = ZERO;
        g.turn = 0;

    }
    else
    {
        g.human = ZERO;
        g.ai = CROSS;
        g.turn = 1;
    }
}

void deinitGame(TGame& g)
{
    for (size_t i = 0u; i < g.SIZE; i++)
    {
        delete[] g.ppField[i];
    }
    delete[] g.ppField;
    g.ppField = nullptr;
}

void printGame(const TGame& g) {
    cout << "     ";
    for (size_t x = 0u; x < g.SIZE; x++) {
        cout << x + 1 << "   ";
    }
    cout << endl;
    for (size_t y = 0u; y < g.SIZE; y++)
    {
        cout << " " << y + 1 << " | ";

        for (size_t x = 0; x < g.SIZE; x++)
        {
            cout << g.ppField[y][x] << " | ";
        }
        cout << endl;
    }
    cout << endl << " Player: " << g.human << endl << " AI: " << g.ai << endl;
}

//==============congrats=====================================

void congrats(TGame& g) {
    if (g.progreess == WON_HUMAN)
        cout << " Human Won!!" << endl;
    else if (g.progreess == WON_AI)
        cout << " AI Won!! " << endl;
    else if (g.progreess == DRAW)
    {
        cout << " Draw !! " << endl;
    }
}

TProgress getWon(const TGame& g)
{
    // 1 в строках 
    for (size_t y = 0; y < g.SIZE; y++)
    {
        if (g.ppField[y][0] == g.ppField[y][1] && g.ppField[y][0] == g.ppField[y][2])
        {
            if (g.ppField[y][0] == g.human)
                return WON_HUMAN;
            if (g.ppField[y][0] == g.ai)
                return WON_AI;
        }
    }
    // 2 в столбцах
    for (size_t x = 0; x < g.SIZE; x++)
    {
        if (g.ppField[0][x] == g.ppField[1][x] && g.ppField[0][x] == g.ppField[2][x])
        {
            if (g.ppField[0][x] == g.human)
                return WON_HUMAN;
            if (g.ppField[0][x] == g.ai)
                return WON_AI;
        }
    }
    // 3 в диагональ 
    if (g.ppField[0][0] == g.ppField[1][1] && g.ppField[0][0] == g.ppField[2][2])
    {
        if (g.ppField[1][1] == g.human)
            return WON_HUMAN;
        if (g.ppField[1][1] == g.ai)
            return WON_AI;
    }
    if (g.ppField[0][2] == g.ppField[1][1] && g.ppField[2][0] == g.ppField[1][1])
    {
        if (g.ppField[1][1] == g.human)
            return WON_HUMAN;
        if (g.ppField[1][1] == g.ai)
            return WON_AI;
    }
    // 4 нечья
    bool draw{ true };
    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                draw = false;
                break;
            }
        }
        if (!draw)
            break;
    }
    if (draw)
        return DRAW;

    return IN_PROGREESS;
}
//====================HUMAM COORD=================================
TCoord getHunmanCoord(const TGame& g) {
    TCoord c;
    do
    {
        cout << "Enter X: ";
        cin >> c.x;
        cout << "Enter Y: ";
        cin >> c.y;
        c.x--;
        c.y--;
    } while (c.x > 2 || c.y > 2 || g.ppField[c.y][c.x] != EMPTY);
    return c;
}
//====================AI COORD=========================================
TCoord getAICoord1(TGame& g)//easy
{
    // пустой ли центр
    if (g.ppField[1][1] == EMPTY)
    {
        return { 1, 1 };
    }

    TCoord buf[4];
    size_t num{ 0u };
    //==== углы
    if (g.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0, 0 };
        num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
        buf[num] = { 2, 0 };
        num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
        buf[num] = { 0, 2 };
        num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
        buf[num] = { 2, 2 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }
    //====== не углы
    if (g.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1, 0 };
        num++;
    }
    if (g.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0, 1 };
        num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2, 1 };
        num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1, 2 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }
    return { 0, 0 };
}

TCoord getAICoord2(TGame& g)//normal 
{
    for (size_t y = 0; y < g.SIZE; y++)//помешать человеку
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                g.ppField[y][x] = g.human;
                if (getWon(g) == WON_HUMAN)
                {
                    g.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                g.ppField[y][x] = EMPTY;
            }
        }
    }
    // пустой ли центр
    if (g.ppField[1][1] == EMPTY)
    {
        return { 1, 1 };
    }

    TCoord buf[4];
    size_t num{ 0u };
    //==== углы
    if (g.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0, 0 };
        num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
        buf[num] = { 2, 0 };
        num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
        buf[num] = { 0, 2 };
        num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
        buf[num] = { 2, 2 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }
    //====== не углы
    if (g.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1, 0 };
        num++;
    }
    if (g.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0, 1 };
        num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2, 1 };
        num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1, 2 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }
    return { 0, 0 };
}

TCoord getAICoord3(TGame& g)//hard
{

    for (size_t y = 0; y < g.SIZE; y++)
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                g.ppField[y][x] = g.ai;
                if (getWon(g) == WON_AI)
                {
                    g.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                g.ppField[y][x] = EMPTY;
            }
        }
    }

    for (size_t y = 0; y < g.SIZE; y++)//помешать человеку
    {
        for (size_t x = 0; x < g.SIZE; x++)
        {
            if (g.ppField[y][x] == EMPTY)
            {
                g.ppField[y][x] = g.human;
                if (getWon(g) == WON_HUMAN)
                {
                    g.ppField[y][x] = EMPTY;
                    return { y, x };
                }
                g.ppField[y][x] = EMPTY;
            }
        }
    }
    // пустой ли центр
    if (g.ppField[1][1] == EMPTY)
    {
        return { 1, 1 };
    }

    TCoord buf[4];
    size_t num{ 0u };
    //==== углы
    if (g.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0, 0 };
        num++;
    }
    if (g.ppField[2][0] == EMPTY)
    {
        buf[num] = { 2, 0 };
        num++;
    }
    if (g.ppField[0][2] == EMPTY)
    {
        buf[num] = { 0, 2 };
        num++;
    }
    if (g.ppField[2][2] == EMPTY)
    {
        buf[num] = { 2, 2 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }
    //====== не углы
    if (g.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1, 0 };
        num++;
    }
    if (g.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0, 1 };
        num++;
    }
    if (g.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2, 1 };
        num++;
    }
    if (g.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1, 2 };
        num++;
    }
    if (num > 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }
    return { 0, 0 };
}
//=======================difficulty====================================================================
TCoord choice(TGame& g)
{
    TCoord aiMove;
    switch (g.difficulty) {
    case('1'):
        aiMove = getAICoord1(g);// easy 
        break;

    case('3'):
        aiMove = getAICoord3(g);// hard
        break;
    case('2'):
    default:
        aiMove = getAICoord2(g);// normal
        break;
    }
    return aiMove;
}

void call_difficulty(TGame* g)
{
    
    cout << "Выберите сложность AI (1-3): " << endl << "Выбор через клавиши 1,2,3 =)"<< endl << "1. Легкий AI." << endl << "2. Средний AI." << endl << "3. Сложный AI." << endl;
    int a;
    do 
    { 
        a = _getch(); 
    } while (a != 49 && a != 50 && a != 51);
    g->difficulty = a;
}
//=====================================Logo=========================================================
void Logo()
{
    cout << "WELCOME " << endl;
    cout << "press button)";
    _getch();
    system("cls");
}


//==============================================================================================
int main()
{
    setlocale(LC_ALL, "rus");
    Logo();
    TGame g;
    iritgame(g);
    call_difficulty(&g);
    system("cls");
    printGame(g);
    do
    {
        if (g.turn % 2 == 0)
        {
            //ход человека
            const TCoord c = getHunmanCoord(g);
            g.ppField[c.y][c.x] = g.human;
        }
        else
        {
            //ход PC
            const TCoord c = choice(g);
            g.ppField[c.y][c.x] = g.ai;
        }
        system("cls");
        printGame(g);
        g.turn++;
        g.progreess = getWon(g);
    } while (g.progreess == IN_PROGREESS);

    congrats(g);
    deinitGame(g);
    system("pause");
}

