#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

clock_t t = 0;
clock_t last = 0;
int p = 49;
int flag = 0;


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
        gotoxy(3, 7);
        printf("%c", ' ');
        gotoxy(3, 6);
        printf("%c", '*');
        t = clock();
        flag = 1;
        jump = 0;
    }
    if (flag == 1 && difftime(clock(), t) > 60 * 5) {
        gotoxy(3, 7);
        printf("%c", '*');
        gotoxy(3, 6);
        printf("%c", ' ');
        flag = 0;
    }
    if (difftime(clock(), last) > 60) {
        last = clock();
        gotoxy(p, 8);
        if (p == 0) {
            p = 49;
        }
        printf("%c", '|');
        printf("%c", '=');
        p--;
    }
}


int main() {
    int i;
    char ch;
    gotoxy(0, 8);
    for (i = 0; i < 50; i++) {
        printf("%c", '=');
    }

    hidecursor();


    while (1) {
        while (!kbhit()) {
            rail(0);
        }

        ch = getch();
        if (ch == ' ') {
            rail(1);
        } else if (ch == 'x'){
            return (0);
        }
    }
    return 0;
}
