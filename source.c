#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>




#define DURATION 90
#define DINOSAUR_X_POS 3
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
int status = 0; // 0= on earth / 1= jumping / 2= on air / 3= landing / it is better to have a struct for this :/

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
void print_dinosaur() {

    //go to the last position
    switch (status) {
    case 0:
        gotoxy(0, EARTH - 10);
        break;
    case 1:
        gotoxy(0, EARTH - 7);
        break;
    case 2:
        gotoxy(0, EARTH - 10);
        break;
    case 3:
        gotoxy(0, EARTH - 13);
        break;
    default:
        break;
    }

    //erase the previous dinosaur
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 16 + DINOSAUR_X_POS; j++) {
            printf("%c", ' ');
        }
        printf("\n");
    }

    //go to the next position
    switch (status) {
    case 0:
        gotoxy(0, EARTH - 7);
        break;
    case 1:
        gotoxy(0, EARTH - 10);
        break;
    case 2:
        gotoxy(0, EARTH - 13);
        break;
    case 3:
        gotoxy(0, EARTH - 10);
        break;
    default:
        break;
    }

    //print the dinosaur
    for (int i = 0; i < 7; i++) {
        for (int i = 0; i < DINOSAUR_X_POS; i++) {
            printf(" ");
        }
        for (int j = 0; j < 16; j++) {
            printf("%c", dinosaur[i][j]);
        }
        printf("\n");
    }
}

void jump() {
    print_dinosaur();
}
void jump_more() {
    print_dinosaur();
}
void land() {
    print_dinosaur();
}
void full_land() {
    print_dinosaur();
}

int main() {
    char ch;
    print_dinosaur();
    gotoxy(0, EARTH);
    for (int i = 0; i < LENGTH; i++) {
        printf("%c", '=');
    }

    hidecursor();


    while (1) {
        if (status == 1 && difftime(clock(), t) > 50) {
            t = clock();
            status = 2;
            jump_more();
        }
        if (status == 2 && difftime(clock(), t) > 600) {
            t = clock();
            status = 3;
            land();
        }
        if (status == 3 && difftime(clock(), t) > 50) {
            t = clock();
            status = 0;
            full_land();
        }

        if (status != 0 || !kbhit()) {
            continue;
        }

        ch = getch();
        if (ch == ' ') {
            t = clock();
            status = 1;
            jump();
        }

        else if (ch == 'x') {
            system("cls");
            printf("\n\n        It worked! I have no idea why...  :P    \n\n\n\n");
            return (0);
        }
    }
    return 0;
}
