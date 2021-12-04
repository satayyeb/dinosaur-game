#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>



#define EARTH_MATERILAL '\xB2'
#define DURATION 45
#define DINOSAUR_X_POS 7
#define FULL    '\xDB'
#define UP  '\xDF'
#define DOWN    '\xDC'
#define EARTH 24
#define LENGTH 80

int last_cactus_pos = LENGTH;
long long score = 0;



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

int game_over_flag = 0;
int which_foot = 0;
clock_t t = 0;
clock_t t2 = 0;
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

int clear_cactus() {
    for (int i = 0; i < 5; i++) {
        gotoxy(last_cactus_pos, EARTH - 5 + i);
        for (int j = 0; j < 5; j++) {
            printf("%c", ' ');
        }
    }
}

int print_cactus() {
    for (int i = 0; i < 5; i++) {
        gotoxy(last_cactus_pos, EARTH - 5 + i);
        for (int j = 0; j < 5; j++) {
            printf("%c", cactus[i][j]);
        }
    }
    if ((status != 2) && (last_cactus_pos + 5) > DINOSAUR_X_POS && last_cactus_pos < DINOSAUR_X_POS + 15) {
        game_over_flag = 1;
    }
}

void pa_zadan() {
    if (difftime(clock(), t2) > DURATION * 2) {
        t2 = clock();
        if (status == 0) {
            gotoxy(10 + (which_foot * 3), EARTH - 1);
            printf("%c", UP);
            printf("%c", UP);
            gotoxy(10 + ((!which_foot) * 3), EARTH - 1);
            printf("%c", FULL);
            printf("%c", DOWN);
            which_foot ^= 1;
        }
    }
}

int cactus_rail() {

    pa_zadan();

    if (difftime(clock(), last) > DURATION) {
        last = clock();

        //printing the score
        score++;
        gotoxy(10, 1);
        printf("%lld", score);
        for (int i = 0; i < 10; i++)
            printf(" ");


        gotoxy(last_cactus_pos, EARTH - 5);
        clear_cactus();

        last_cactus_pos--;
        if (last_cactus_pos == 0) {
            last_cactus_pos = LENGTH;
        }

        print_cactus();
    }
}



int main() {
    char ch;
    hidecursor();

    //first screen
    status = 2;
    print_dinosaur();
    gotoxy(1, 4);
    printf("It worked! I have no idea why...");
    gotoxy(5, 5);
    printf("Press space to play");
    gotoxy(6, 6);
    printf("    x to exit");
    ch = getch();
    while (ch != ' ' && ch != 'x' && ch != 'X') {
        ch = getch();
    }
    if (ch == 'x' || ch == 'X') {
        return 0;
    }
    system("cls");

    //init the game
    status = 0;
    print_dinosaur();
    gotoxy(0, EARTH);
    for (int i = 0; i < LENGTH; i++) {
        printf("%c", EARTH_MATERILAL);
    }
    gotoxy(3, 1);
    printf("score: ");


    while (1) {
        cactus_rail();
        if (game_over_flag) {
            system("cls");
            gotoxy(5, 5);
            printf("U lost :(");
            gotoxy(5, 6);
            printf("Your Score: %lld", score);
            gotoxy(5, 7);
            printf("Press x to exit...");

            while (1) {
                ch = getch();
                if (ch == 'x' || ch == 'X')
                    return 0;
            }
        }

        if (status == 1 && difftime(clock(), t) > 100) {
            t = clock();
            status = 2;
            print_dinosaur();
        }
        if (status == 2 && difftime(clock(), t) > 1100) {
            t = clock();
            status = 3;
            print_dinosaur();
        }
        if (status == 3 && difftime(clock(), t) > 100) {
            t = clock();
            status = 0;
            print_dinosaur();
        }

        if (status != 0 || !kbhit()) {
            continue;
        }

        ch = getch();
        if (ch == ' ') {
            t = clock();
            status = 1;
            print_dinosaur();
        }

        else if (ch == 'x' || ch == 'X') {
            system("cls");
            printf("\n\n        It worked! I have no idea why...  :P    \n\n\n\n");
            return (0);
        }
    }
    return 0;
}
