#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>




#define DURATION 90
#define DINOSAUR_X_POS 7
#define FULL    '\xDB'
#define UP  '\xDF'
#define DOWN    '\xDC'
#define EARTH 24
#define LENGTH 80




char mini_cactus[3][3] = {
    {FULL, ' ', DOWN},
    {FULL, ' ', FULL},
    {FULL, UP, UP} };

char cactus[5][5] = {
    {' ', ' ', DOWN, ' ', ' '},
    {FULL, ' ', FULL, ' ', FULL},
    {FULL, ' ', FULL, ' ', FULL},
    {UP, UP, FULL, UP, UP},
    {' ', ' ', FULL, ' ', ' '} };

char dinosaur[7][16] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', DOWN, FULL, UP, FULL, FULL, FULL, FULL, DOWN},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', FULL, FULL, FULL, FULL, DOWN, DOWN, DOWN, ' '},
    {FULL, ' ', ' ', ' ', ' ', ' ', DOWN, FULL, FULL, FULL, FULL, ' ', ' ', ' ', ' ', ' '},
    {FULL, FULL, DOWN, ' ', DOWN, FULL, FULL, FULL, FULL, FULL, FULL, UP, UP, FULL, ' ', ' '},
    {UP, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, FULL, UP, ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', UP, FULL, FULL, FULL, FULL, FULL, UP, ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', FULL, DOWN, ' ', FULL, DOWN, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
};


clock_t t = 0;
clock_t last = 0;
int p = 49;
int flag = 0;
char c;
int on_air = 0;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidecursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void rail(int jump) {
    if (jump == 1) {
        gotoxy(DINOSAUR_X_POS, 7);
        printf("%c", ' ');
        gotoxy(DINOSAUR_X_POS, 6);
        printf("%c", '*');
        t = clock();
        flag = 1;
        jump = 0;
        on_air = 1;
    }
    if (flag == 1 && difftime(clock(), t) > DURATION * DINOSAUR_X_POS) {
        gotoxy(DINOSAUR_X_POS, 7);
        printf("%c", '*');
        gotoxy(DINOSAUR_X_POS, 6);
        printf("%c", ' ');
        on_air = 0;
        flag = 0;
    }
    if (difftime(clock(), last) > DURATION) {
        last = clock();
        gotoxy(p, 8);
        if (p == 0) {
            p = 49;
        }
        printf("%c", '|');
        printf("%c", '=');
        gotoxy(p, 7);
        printf("%c", '|');
        printf("%c", ' ');
        p--;
    }
    if ((p == DINOSAUR_X_POS || p == DINOSAUR_X_POS - 1) && !on_air) {
        printf("You lost :(");
        while (1);
    }

}


int main() {
    int i;
    char ch;
    gotoxy(DINOSAUR_X_POS, EARTH - 1);
    printf("%c", '*');
    gotoxy(0, EARTH);
    for (i = 0; i < LENGTH; i++) {
        printf("%c", '=');
    }

    hidecursor();


    while (1) {
        rail(0);

        if (!kbhit()) {
            continue;
        }

        ch = getch();
        if (ch == ' ') {
            //  jump();
            rail(1);
        } else if (ch == 'x') {
            system("cls");
            printf("\n\n        It worked! I have no idea why...  :P    \n\n\n\n");
            return (0);
        }
    }
    return 0;
}
