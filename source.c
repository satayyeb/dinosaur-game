//include necessary libraries
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

//configurations
#define duration  30           //The length of time that cactus moves
#define DINOSAUR_X 7           //the X-position of dinosaur
#define EARTH_Y 30             //the Y-position of the earth
#define JUMP_HEIGHT 7
#define JUMP_STEP 3
#define SKY_X (earth_len / 2)
#define CACTUS_RESERVED 5
#define CACTUS_MIN_DIST 70
#define CACTUS_MAX_DIST 140
// #define EARTH_LEN 160           //the length of the earth

//specific characters:
#define EARTH_MATERILAL '\xB2'
#define FULL    '\xDB'
#define UP  '\xDF'
#define DOWN    '\xDC'

//music notes:
#define C4  261
#define D4  293
#define E4  329
#define F4  349
#define G4  392
#define A4  440
#define B4  493
#define C5  523
#define D5  587
#define E5  659
#define F5  698
#define G5  783
#define A5  880
#define B5  987
#define E5  659
#define full 450
#define half full

//global varialbes:
char ch;
int earth_len; // will be assigned at start;
long long score;
int speed = 1;
int last_cactus_pos; // set to window width;
int game_over_flag = 0;
int which_foot = 0;
clock_t t = 0;
clock_t t2 = 0;
clock_t last = 0;
clock_t cactus_timer = 0;

int cactus_arr[CACTUS_RESERVED], cactus_cnt = 0;
int dino_y = 0;


#define CLOUD_HEIGHT 3
#define CLOUD_WIDTH 17
#define SUN_HEIGHT 6
#define SUN_WIDTH 7
#define MOON_HEIGHT 4
#define MOON_WIDTH 7
#define CACTUS_HEIGHT 5
#define CACTUS_WIDTH 5
#define DINOSAUR_HEIGHT 7
#define DINOSAUR_WIDTH 16

enum COLOR {
    Black, Blue, Green, Aqua, Red, Purple, Yellow, White,
    Gray, LightBlue, LightGreen, LightAqua, LightRed, LightPurple, LightYellow, BrightWhite
};

enum DINOSAUR_STATUS {
    RUNNING, JUMPING, IN_AIR, LANDING
} dino_status = RUNNING;

enum GAME_STAT {
    WELCOME_STAT, RUNNING_STAT, LOSE_STAT, BYE_STAT, EXIT_PROGRAM_STAT
} game_status = WELCOME_STAT;

char cloud[CLOUD_HEIGHT][CLOUD_WIDTH + 1] = {
    {' ',220,223,223,223,223,223,223,223,223,223,223,223,223,223,220,' ', '\0'},
    {219,176,' ',176,' ',176,' ',176,' ',176,' ',176,' ',176,' ',176,219, '\0'},
    {' ',223,220,220,220,220,220,220,220,220,220,220,220,220,220,223,' ', '\0'},
};

char moon[MOON_HEIGHT][MOON_WIDTH + 1] = {
    {220,223 ,223,219,219,220,' ', '\0'},
    {' ',' ' , ' ',' ',219,219,219, '\0'},
    {223,220,220,219,219,223,' ', '\0'},
};

char sun[SUN_HEIGHT][SUN_WIDTH + 1] = { 
    {' ','\\',' '  ,'|',' '  ,'/',' ', '\0'},
    {196,' ',220,220,220,' ',196, '\0'},
    {196,' ',219,219,219,' ',196, '\0'},
    {196,' ',223,223,223,' ',196, '\0'},
    {' ','/',' ','|',' ' , '\\',' ', '\0'}
};

char cactus[CACTUS_HEIGHT][CACTUS_WIDTH + 1] = {
    {' ', ' ', DOWN, ' ', ' ', '\0'},
    {FULL, ' ', FULL, ' ', FULL, '\0'},
    {FULL, ' ', FULL, ' ', FULL, '\0'},
    {UP, UP, FULL, UP, UP, '\0'},
    {' ', ' ', FULL, ' ', ' ', '\0'}
};

char dinosaur[DINOSAUR_HEIGHT][DINOSAUR_WIDTH + 1] = {
    {' ' , ' ', ' ', ' ', ' ', ' ', ' ', ' ',DOWN,FULL,UP  ,FULL,FULL,FULL,FULL,DOWN, '\0'},
    {' ' , ' ', ' ', ' ', ' ', ' ', ' ', ' ',FULL,FULL,FULL,FULL,DOWN,DOWN,DOWN, ' ', '\0'},
    {FULL, ' ', ' ', ' ', ' ', ' ',DOWN,FULL,FULL,FULL,FULL, ' ', ' ', ' ', ' ', ' ', '\0'},
    {FULL,FULL,DOWN, ' ',DOWN,FULL,FULL,FULL,FULL,FULL,FULL, UP , UP ,FULL, ' ', ' ', '\0'},
    {UP  ,FULL,FULL,FULL,FULL,FULL,FULL,FULL,FULL,FULL,UP  , ' ', ' ', ' ', ' ', ' ', '\0'},
    {' ' , ' ', UP ,FULL,FULL,FULL,FULL,FULL,UP  , ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
    {' ' , ' ', ' ',FULL,DOWN, ' ',FULL,DOWN,' ' , ' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'},
};

HWND hwnd;
CONSOLE_SCREEN_BUFFER_INFO csbi;
HANDLE consoleHandle;

//functions prototype:
void SetColor(int ForgC);
void gotoxy(int x, int y);
void hidecursor();
void print_dinosaur(int x, int y);
void erase_dinosaur(int x, int y);
void pa_zadan();
void exit_if_X_pressed();
void last_music();
void faster_the_speed();
void cactus_rail();
void clear_cactus();
void print_cactus();
void print_cloud(int x, int y);
void print_sun(int x, int y);
void print_moon(int x, int y);
void initialize_console();
void push_cactus();
void pop_cactus();

enum GAME_STAT run_welcome_state();
enum GAME_STAT run_running_state();
enum GAME_STAT run_lose_state();
enum GAME_STAT run_bye_state();

//the main function
int main() {

    initialize_console();
    srand(time(0));
    earth_len = csbi.dwSize.X - 5;

    while (1) {
        switch (game_status) {
            case WELCOME_STAT:
            game_status = run_welcome_state();
            break;
            case RUNNING_STAT:
            game_status = run_running_state();
            break;
            case LOSE_STAT:
            game_status = run_lose_state();
            break;
            case BYE_STAT:
            game_status = run_bye_state();
            break;
            case EXIT_PROGRAM_STAT:
            return 0;
            break;
        }
    }
}

// ---------------------- function definitions --------------------------------


enum GAME_STAT run_welcome_state() {
    system("cls");
    print_dinosaur(DINOSAUR_X, EARTH_Y - 7);

    SetColor(Purple);
    gotoxy(8, 3);
    printf("Dinosaur Game");
    gotoxy(1, 4);
    printf("It worked! I have no idea why...");
    gotoxy(5, 5);
    printf("Press space to play");
    gotoxy(6, 6);
    printf("    x to exit");

    do {
        ch = getch();
    } while (ch != ' ' && ch != 'x' && ch != 'X');

    if (ch == ' ') {
        return RUNNING_STAT;
    } else {
        return BYE_STAT;
    }
}

enum GAME_STAT run_running_state() {
    score = 0;
    system("cls");

    //init the game
    print_dinosaur(DINOSAUR_X, EARTH_Y - 7);

    // print earth
    gotoxy(0, EARTH_Y);
    SetColor(Yellow);
    for (int i = 0; i < earth_len; i++) {
        putchar(EARTH_MATERILAL);
    }

    gotoxy(3, 1);
    SetColor(BrightWhite);
    printf("Score: ");

    //printing cloud 1&2&3
    print_cloud(SKY_X, 5);
    print_cloud(SKY_X + 30, 1);
    print_cloud(SKY_X + 20, 7);

    cactus_cnt = 0;
    for (int i = 0; i < CACTUS_RESERVED; ++i) push_cactus();

    while (1) {

        faster_the_speed();
        pa_zadan();

        if (difftime(clock(), last) > duration) {
            last = clock();

            score++;
            gotoxy(10, 1);
            SetColor(BrightWhite);
            printf("%lld", score);
            for (int i = 0; i < 10; i++) {
                putchar(' ');
            }

            cactus_rail();
        }

        int collision = FALSE;
        for (int ci = 0; ci < cactus_cnt; ++ci) {
            int cpos = cactus_arr[ci];
            if (((dino_status == RUNNING) && (cpos + 7) > DINOSAUR_X && cpos < DINOSAUR_X + 15) || 
            ((dino_status == JUMPING) && (cpos + 10) > DINOSAUR_X && cpos < DINOSAUR_X + 10)) {
                collision = TRUE;
            }
        }

        if (collision) {
            return LOSE_STAT;
        }

        if ((score / 500) % 2 == 0) {
            print_sun(SKY_X + 18, 1);
        } else if ((score / 500) % 2 == 1) {
            print_moon(SKY_X + 18, 1);
        }

        if (dino_status == JUMPING && difftime(clock(), t) > duration * 2/3) {
            t = clock();
            erase_dinosaur(DINOSAUR_X, EARTH_Y - DINOSAUR_HEIGHT - dino_y);
            dino_y = min(dino_y + JUMP_STEP, JUMP_HEIGHT);
            print_dinosaur(DINOSAUR_X, EARTH_Y - DINOSAUR_HEIGHT - dino_y);
            if (dino_y == JUMP_HEIGHT) {
                dino_status = IN_AIR;
            }
        }
        if (dino_status == IN_AIR && difftime(clock(), t) > duration * 20) {
            t = clock();
            dino_status = LANDING;
        }
        if (dino_status == LANDING && difftime(clock(), t) > duration * 2/3) {
            t = clock();
            erase_dinosaur(DINOSAUR_X, EARTH_Y - DINOSAUR_HEIGHT - dino_y);
            dino_y = max(dino_y - JUMP_STEP, 0);
            print_dinosaur(DINOSAUR_X, EARTH_Y - DINOSAUR_HEIGHT - dino_y);
            if (dino_y == 0) {
                dino_status = RUNNING;
            }
        }

        if (kbhit()) {
            ch = getch();
            if (dino_status == RUNNING && ch == ' ') {
                t = clock();
                dino_status = JUMPING;
            } else if (GetKeyState(VK_SHIFT) & GetKeyState(0x47) & 0x8000) { //if shift+G pressed
                score -= 100;
                if (score < 0) score = 0;
            } else if (GetKeyState(VK_SHIFT) & GetKeyState(0x48) & 0x8000) { //if shift+H pressed
                score += 100;
            } else if (GetKeyState(0x58) & 0x8000) {   //if x or X pressed
                return BYE_STAT;
            }
        }
    }
}

enum GAME_STAT run_lose_state() {
    system("cls");
    gotoxy(5, 5);
    SetColor(Blue);
    printf("U lost :(");
    gotoxy(5, 6);
    printf("Your Score: %lld", score);
    gotoxy(5, 7);
    printf("Press x to end the game...");
    gotoxy(5, 8);
    printf("Press space to play again...");

    //last_music();

    while (1) {
        ch = getch();
        if (ch == 'x' || ch == 'X') {
            return BYE_STAT;
        } else if (ch == ' ') {
            return RUNNING_STAT;
        }
    }
}

enum GAME_STAT run_bye_state() {
    system("cls");
    SetColor(LightYellow);
    printf("\n\n        It worked! I have no idea why...  :P    \n\n\n\n");
    return EXIT_PROGRAM_STAT;
}

void faster_the_speed() {
    if (1 <= score && score < 150) {
        speed = 1;
    }
    if (150 <= score && score < 500) {
        speed = 2;
    }
    if (500 <= score && score < 1100) {
        speed = 3;
    }
    if (1100 <= score && score < 2300) {
        speed = 4;
    }
    if (2300 <= score) {
        speed = 5;
    }
}

void push_cactus() {
    int gap = CACTUS_MIN_DIST + rand() % (CACTUS_MAX_DIST - CACTUS_MIN_DIST);
    cactus_arr[cactus_cnt++] = cactus_cnt == 0 ? earth_len : cactus_arr[cactus_cnt - 1] + gap;
}

void pop_cactus() {
    for (int ci = 0; ci < cactus_cnt - 1; ++ci) {
        cactus_arr[ci] = cactus_arr[ci + 1];
    }
    cactus_cnt--;
}

void cactus_rail() {

    for (int ci = 0; ci < cactus_cnt; ++ci) {
        if (cactus_arr[ci] < earth_len) clear_cactus(cactus_arr[ci], EARTH_Y - 5);
        cactus_arr[ci] -= speed;
        if (cactus_arr[ci] <= 0) {
            pop_cactus();
            push_cactus();
        } else if (cactus_arr[ci] < earth_len) print_cactus(cactus_arr[ci], EARTH_Y - 5);
    }
}

void exit_if_X_pressed() {
    if (kbhit()) {
        ch = getch();
        if (ch == 'x' || ch == 'X') exit(0);
    }
}

void pa_zadan() {
    SetColor(Red);
    if (difftime(clock(), t2) > duration * 2) {
        t2 = clock();
        if (dino_status == RUNNING) {
            gotoxy(10 + (which_foot * 3), EARTH_Y - 1);
            putchar(UP);
            putchar(UP);
            gotoxy(10 + ((!which_foot) * 3), EARTH_Y - 1);
            putchar(FULL);
            putchar(DOWN);
            which_foot ^= 1;
        }
    }
}

void print_cactus(int x, int y) {
    SetColor(Green);
    for (int i = 0; i < CACTUS_HEIGHT; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < CACTUS_WIDTH && x + j < earth_len; j++) {
            putchar(cactus[i][j]);
        }
    }
}

void clear_cactus(int x, int y) {
    for (int i = 0; i < CACTUS_HEIGHT; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < CACTUS_WIDTH; j++) {
            putchar(' ');
        }
    }
}

void print_cloud(int x, int y) {
    SetColor(LightAqua);
    for (int i = 0; i < CLOUD_HEIGHT; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < CLOUD_WIDTH; j++) {
            putchar(cloud[i][j]);
        }
    }
}

void print_sun(int x, int y) {
    SetColor(LightYellow);
    for (int i = 0; i < SUN_HEIGHT; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < SUN_WIDTH; j++) {
            putchar(sun[i][j]);
        }
    }
}

void print_moon(int x, int y) {
    SetColor(BrightWhite);
    for (int i = 0; i < MOON_HEIGHT; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < MOON_WIDTH; j++) {
            putchar(moon[i][j]);
        }
    }
    // erase sun under moon
    gotoxy(x, y + 3);
    printf("                       \n            ");
    gotoxy(x, y + 4);
    printf("                   \n             ");
}

void erase_dinosaur(int x, int y) {
    for (int i = 0; i < DINOSAUR_HEIGHT; i++) {
        gotoxy(x, y + i);
        for (int j = 0; j < DINOSAUR_WIDTH; j++) {
            putchar(' ');
        }
    }
}

void print_dinosaur(int x, int y) {
    SetColor(Red);
    for (int i = 0; i < DINOSAUR_HEIGHT; i++) {
        gotoxy(x, y + i);
        puts(dinosaur[i]);
    }
}

void initialize_console() {
    hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(consoleHandle, &csbi);

    hidecursor();
}

void hidecursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(consoleHandle, coord);
}

void SetColor(int ForgC) {
    WORD wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
    SetConsoleTextAttribute(consoleHandle, wColor);
}

void last_music() {
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Sleep(half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Sleep(half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(G5, half);
    exit_if_X_pressed();
    Beep(C5, half);
    exit_if_X_pressed();
    Beep(D5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(C4, half);
    exit_if_X_pressed();
    Beep(D4, half);
    exit_if_X_pressed();
    Beep(E4, half);
    exit_if_X_pressed();
    Beep(F5, half);
    exit_if_X_pressed();
    Beep(F5, half);
    exit_if_X_pressed();
    Beep(F5, half);
    exit_if_X_pressed();
    Sleep(half);
    exit_if_X_pressed();
    Beep(F5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Sleep(half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(D5, half);
    exit_if_X_pressed();
    Beep(D5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(D5, half);
    exit_if_X_pressed();
    Sleep(half);
    exit_if_X_pressed();
    Beep(G5, half);
    exit_if_X_pressed();
    Beep(E5, half);
    exit_if_X_pressed();
    Beep(D5, half);
    exit_if_X_pressed();
    Beep(G5, half * 1.1);
    exit_if_X_pressed();
    Beep(E5, half * 1.2);
    exit_if_X_pressed();
}
