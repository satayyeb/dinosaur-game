//include necessary libraries
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>

//configurations
#define duration  40        //The length of time that cactus moves
#define DINOSAUR_X_POS 7    //the X-position of dinosaur
#define EARTH 24            //the Y-position of the earth
#define LENGTH 90           //the length of the earth

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
long long score = 0;
int speed = 1;
int last_cactus_pos = LENGTH;
int game_over_flag = 0;
int which_foot = 0;
clock_t t = 0;
clock_t t2 = 0;
clock_t last = 0;
int status = 0; // 0= on earth / 1= jumping / 2= on air / 3= landing 
                //it is better to have a enum for this variable :(

enum COLOR {
    Black, Blue, Green, Aqua, Red, Purple, Yellow, White,
    Gray, LightBlue, LightGreen, LightAqua, LightRed, LightPurple, LightYellow, BrightWhite
};

char cloud[3][17] = {
    {' ',220,223,223,223,223,223,223,223,223,223,223,223,223,223,220,' '},
    {219,176,' ',176,' ',176,' ',176,' ',176,' ',176,' ',176,' ',176,219},
    {' ',223,220,220,220,220,220,220,220,220,220,220,220,220,220,223,' '},
};

char moon[4][7] = {
    {220,223 ,223,219,219,220,' '},
    {' ',' ' , ' ',' ',219,219,219},
    {223,220,220,219,219,223,' '},
};

char sun[6][7] = { {' ','\\',' '  ,'|',' '  ,'/',' '},
    {196,' ',220,220,220,' ',196},
    {196,' ',219,219,219,' ',196},
    {196,' ',223,223,223,' ',196},
    {' ','/',' ','|',' ' , '\\',' '}
};

char cactus[5][5] = {
    {' ', ' ', DOWN, ' ', ' '},
    {FULL, ' ', FULL, ' ', FULL},
    {FULL, ' ', FULL, ' ', FULL},
    {UP, UP, FULL, UP, UP},
    {' ', ' ', FULL, ' ', ' '} };

int dinosaur_higth = 7;
int dinosaur_width = 16;
char dinosaur[7][16] = {
    {' ' , ' ', ' ', ' ', ' ', ' ', ' ', ' ',DOWN,FULL,UP  ,FULL,FULL,FULL,FULL,DOWN},
    {' ' , ' ', ' ', ' ', ' ', ' ', ' ', ' ',FULL,FULL,FULL,FULL,DOWN,DOWN,DOWN, ' '},
    {FULL, ' ', ' ', ' ', ' ', ' ',DOWN,FULL,FULL,FULL,FULL, ' ', ' ', ' ', ' ', ' '},
    {FULL,FULL,DOWN, ' ',DOWN,FULL,FULL,FULL,FULL,FULL,FULL, UP , UP ,FULL, ' ', ' '},
    {UP  ,FULL,FULL,FULL,FULL,FULL,FULL,FULL,FULL,FULL,UP  , ' ', ' ', ' ', ' ', ' '},
    {' ' , ' ', UP ,FULL,FULL,FULL,FULL,FULL,UP  , ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ' , ' ', ' ',FULL,DOWN, ' ',FULL,DOWN,' ' , ' ', ' ', ' ', ' ', ' ', ' ', ' '},
};

//functions prototype:
void SetColor(int ForgC);
void gotoxy(int x, int y);
void hidecursor();
void print_dinosaur();
void pa_zadan();
void check_is_X_pressed();
void last_music();
void faster_the_speed();
void cactus_rail();
void clear_cactus();
void print_cactus();


//the main function
int main() {

    hidecursor();

    //first screen
    status = 2;
    print_dinosaur();
    SetColor(Purple);
    gotoxy(8, 3);
    printf("Dinosaur Game");
    gotoxy(1, 4);
    printf("It worked! I have no idea why...");
    gotoxy(5, 5);
    printf("Press space to play");
    gotoxy(6, 6);
    printf("    x to exit");
    SetColor(BrightWhite);

    do {
        ch = getch();
    } while (ch != ' ' && ch != 'x' && ch != 'X');

    if (ch == 'x' || ch == 'X') {
        return 0;
    }
    system("cls");

    //init the game
    status = 0;
    print_dinosaur();
    SetColor(BrightWhite);
    gotoxy(0, EARTH);
    SetColor(6);
    for (int i = 0; i < LENGTH; i++) {
        printf("%c", EARTH_MATERILAL);
    }
    SetColor(BrightWhite);
    gotoxy(3, 1);
    printf("score: ");


    //printing cloud 1&2&3
    gotoxy(30, 5);
    for (int i = 0; i < 3; i++)
    {
        // printf("%*s",35," ");
        SetColor(LightAqua);
        for (int j = 0; j < 17; j++)
        {
            printf("%c", cloud[i][j]);
        }
        printf("\n");
        printf("%*.S", 30, " ");
    }

    gotoxy(60, 1);
    for (int i = 0; i < 3; i++)
    {
        SetColor(LightAqua);
        for (int j = 0; j < 17; j++)
        {
            printf("%c", cloud[i][j]);
        }
        printf("\n");
        printf("%*.S", 60, " ");
    }
    SetColor(BrightWhite);


    gotoxy(50, 7);
    for (int i = 0; i < 3; i++)
    {
        // printf("%*s",35," ");
        SetColor(LightAqua);
        for (int j = 0; j < 17; j++)
        {
            printf("%c", cloud[i][j]);
        }
        printf("\n");
        printf("%*.S", 50, " ");
    }



    while (1) {

        if (last_cactus_pos == LENGTH) {
            faster_the_speed();
        }

        cactus_rail();


        if (game_over_flag) {
            system("cls");
            gotoxy(5, 5);
            SetColor(Blue);
            printf("U lost :(");
            gotoxy(5, 6);
            printf("Your Score: %lld", score);
            gotoxy(5, 7);
            printf("Press x to exit...");
            SetColor(BrightWhite);

            // last_music();

            while (1) {
                ch = getch();
                if (ch == 'x' || ch == 'X')
                    return 0;
            }
        }



        if ((score / 500) % 2 == 0)
        {
            //printing sun
            for (int i = 0; i < 6; i++)
            {
                // printf("%*s",35," ");
                SetColor(LightYellow);
                for (int j = 0; j < 7; j++)
                {
                    gotoxy(48 + j, 1 + i);
                    printf("%c", sun[i][j]);
                }
                printf("\n");
                //printf("%*.S",20," ");
            }
            SetColor(BrightWhite);
            //printing cloud 1&2&3
        } else if ((score / 500) % 2 == 1) {
            SetColor(BrightWhite);
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 7; ++j) {
                    gotoxy(48 + j, 1 + i);
                    printf("%c", moon[i][j]);
                }
                printf("\n");
            }
            SetColor(BrightWhite);
            gotoxy(48, 4);
            printf("                       \n            ");
            gotoxy(48, 5);
            printf("                   \n             ");
        }
        SetColor(BrightWhite);





        if (status == 1 && difftime(clock(), t) > duration * 2) {
            t = clock();
            status = 2;
            print_dinosaur();
            SetColor(BrightWhite);
        }
        if (status == 2 && difftime(clock(), t) > (duration * 35 / speed) + 12) {
            t = clock();
            status = 3;
            print_dinosaur();
            SetColor(BrightWhite);
        }
        if (status == 3 && difftime(clock(), t) > duration * 2) {
            t = clock();
            status = 0;
            print_dinosaur();
            SetColor(BrightWhite);
        }




        if (status != 0 && kbhit()) {
            ch = getch();
            if (ch == 'G') {
                score -= 100;
                if (score < 0)
                    score = 0;
            }
            if (ch == 'H')
                score += 100;
        }

        if (status != 0 || !kbhit()) {
            continue;
        }

        ch = getch();
        if (ch == ' ') {
            t = clock();
            status = 1;
            print_dinosaur();
            SetColor(BrightWhite);
        }

        if (ch == 'G') {
            score -= 100;
            if (score < 0)
                score = 0;
        }

        if (ch == 'H')
            score += 100;
        else if (ch == 'x' || ch == 'X') {
            SetColor(LightYellow);
            system("cls");
            printf("\n\n        It worked! I have no idea why...  :P    \n\n\n\n");
            SetColor(BrightWhite);
            return (0);
        }
    }
    return 0;
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

void cactus_rail() {
    pa_zadan();

    if (difftime(clock(), last) > duration) {
        last = clock();

        //printing the score
        score++;
        gotoxy(10, 1);
        printf("%lld", score);
        for (int i = 0; i < 10; i++)
            printf(" ");


        gotoxy(last_cactus_pos, EARTH - 5);
        clear_cactus();

        last_cactus_pos -= speed;
        if (last_cactus_pos <= 0) {
            last_cactus_pos = LENGTH;
            return;
        }

        print_cactus();
    }
}

void check_is_X_pressed() {
    if (!kbhit())
        return;
    ch = getch();
    if (ch == 'x' || ch == 'X')
        exit(0);
    else
        return;
}

void pa_zadan() {
    SetColor(Red);
    if (difftime(clock(), t2) > duration * 2) {
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
    SetColor(BrightWhite);
}

void print_cactus() {
    SetColor(Green);
    for (int i = 0; i < 5; i++) {
        gotoxy(last_cactus_pos, EARTH - 5 + i);
        for (int j = 0; j < 5; j++) {
            printf("%c", cactus[i][j]);
        }
    }
    SetColor(BrightWhite);
    if ((status == 0) && (last_cactus_pos + 7) > DINOSAUR_X_POS && last_cactus_pos < DINOSAUR_X_POS + 15) {
        game_over_flag = 1;
    } else if ((status != 2 && status != 0) && (last_cactus_pos + 10) > DINOSAUR_X_POS && last_cactus_pos < DINOSAUR_X_POS + 14) {
        game_over_flag = 1;
    }
}

void clear_cactus() {
    for (int i = 0; i < 5; i++) {
        gotoxy(last_cactus_pos, EARTH - 5 + i);
        for (int j = 0; j < 5; j++) {
            printf("%c", ' ');
        }
    }
}

void print_dinosaur() {

    SetColor(Red);

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
    for (int i = 0; i < dinosaur_higth; i++) {
        for (int j = 0; j < dinosaur_width + DINOSAUR_X_POS; j++) {
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

    for (int i = 0; i < dinosaur_higth; i++) {
        for (int i = 0; i < DINOSAUR_X_POS; i++) {
            printf(" ");
        }
        for (int j = 0; j < dinosaur_width; j++) {
            printf("%c", dinosaur[i][j]);
        }
        printf("\n");
    }
}

void hidecursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    //We use csbi for the wAttributes word.
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //Mask out all but the background attribute, and add in the forgournd     color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}


void last_music() {
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Sleep(half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Sleep(half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(G5, half);
    check_is_X_pressed();
    Beep(C5, half);
    check_is_X_pressed();
    Beep(D5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(C4, half);
    check_is_X_pressed();
    Beep(D4, half);
    check_is_X_pressed();
    Beep(E4, half);
    check_is_X_pressed();
    Beep(F5, half);
    check_is_X_pressed();
    Beep(F5, half);
    check_is_X_pressed();
    Beep(F5, half);
    check_is_X_pressed();
    Sleep(half);
    check_is_X_pressed();
    Beep(F5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Sleep(half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(D5, half);
    check_is_X_pressed();
    Beep(D5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(D5, half);
    check_is_X_pressed();
    Sleep(half);
    check_is_X_pressed();
    Beep(G5, half);
    check_is_X_pressed();
    Beep(E5, half);
    check_is_X_pressed();
    Beep(D5, half);
    check_is_X_pressed();
    Beep(G5, half * 1.1);
    check_is_X_pressed();
    Beep(E5, half * 1.2);
    check_is_X_pressed();
}
