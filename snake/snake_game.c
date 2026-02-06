#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define WIDTH 20
#define HEIGHT 10
typedef struct SnakeSegment {
    int x, y;
    struct SnakeSegment* next;
} Segment;
Segment* head = NULL;
Segment* tail = NULL;
int foodX, foodY, gameOver = 0;
char dir = 'd'; // initial direction
void gotoxy(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void delay(int ms) {
    Sleep(ms);
}
void drawBoard() {
    system("cls");
    for (int i = 0; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            if (i == 0 || i == HEIGHT || j == 0 || j == WIDTH)
                printf("#");
            else if (i == foodY && j == foodX)
                printf("*");
            else {
                int printed = 0;
                Segment* temp = head;
                while (temp != NULL) {
                    if (temp->x == j && temp->y == i) {
                        printf("O");
                        printed = 1;
                        break;
                    }
                    temp = temp->next;
                }
                if (!printed) printf(" ");
            }
        }
        printf("\n");
    }
}
void placeFood() {
    int valid = 0;
    while (!valid) {
        valid = 1;
        foodX = rand() % (WIDTH - 2) + 1;
        foodY = rand() % (HEIGHT - 2) + 1;
        Segment* temp = head;
        while (temp != NULL) {
            if (temp->x == foodX && temp->y == foodY) {
                valid = 0;
                break;
            }
            temp = temp->next;
        }
    }
}
void addSegment(int x, int y) {
    Segment* newSegment = (Segment*)malloc(sizeof(Segment));
    newSegment->x = x;
    newSegment->y = y;
    newSegment->next = NULL;
    if (head == NULL) {
        head = tail = newSegment;
    } else {
        tail->next = newSegment;
        tail = newSegment;
    }
}
void moveSnake(int grow) {
    int newX = tail->x;
    int newY = tail->y;
    if (dir == 'w') newY--;
    else if (dir == 's') newY++;
    else if (dir == 'a') newX--;
    else if (dir == 'd') newX++;
    // check for collision
    if (newX == 0 || newY == 0 || newX == WIDTH || newY == HEIGHT) {
        gameOver = 1;
        return;
    }
    Segment* temp = head;
    while (temp != NULL) {
        if (temp->x == newX && temp->y == newY) {
            gameOver = 1;
            return;
        }
        temp = temp->next;
    }
    addSegment(newX, newY);
    if (!grow) {
        Segment* prev = head;
        head = head->next;
        free(prev);
    }
}
void input() {
    if (_kbhit()) {
        char ch = _getch();
        if ((ch == 'w' && dir != 's') ||
            (ch == 's' && dir != 'w') ||
            (ch == 'a' && dir != 'd') ||
            (ch == 'd' && dir != 'a')) {
            dir = ch;
        }
    }
}
int main() {
    srand(time(NULL));
    addSegment(5, 5); // start position
    placeFood();
    while (!gameOver) {
        drawBoard();
        input();
        int grow = 0;
        if (tail->x + (dir == 'd') - (dir == 'a') == foodX &&
            tail->y + (dir == 's') - (dir == 'w') == foodY) {
            grow = 1;
            placeFood();
        }
        moveSnake(grow);
        delay(150);
    }
    printf("Game Over!\n");
    return 0;
}