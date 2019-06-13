#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define BOARD_ROW 20
#define BOARD_HEIGHT 20
#define BLOCK '*'
#define BLANK ' '
#define NO_STOP 1
#define ERROR_SIZE 21
#define ERROR_MESSAGE "Error in system call\n"
#define STDERR 2
#define RESET_PLAYER 0
#define CAN_MOVE 1
#define CANT_MOVE_DIRECTION 2
#define MOVE_DOWN 3
#define MOVE_LEFT 4
#define MOVE_RIGHT 5
#define CHANGE_POSITION 6

typedef enum State {
    STANDING, LAYING
} State;

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Shape {
    State state;
    Position position;
} Shape;

char board[BOARD_ROW][BOARD_HEIGHT];
Shape player;

void savePositionStanding();

void savePositionLaying();

void displayError();

void changeState();

void moveDown();

void moveLeft();

void moveRight();

void initializeBoard();

void displayBoard();

void signal_hand(int sig);

void alarm_hand(int sig);

void resetBoard();

void initializePlayer();

int main() {
    initializeBoard();
    displayBoard();
    signal(SIGUSR2, signal_hand);
    signal(SIGALRM, alarm_hand);
    alarm(1);
    while (NO_STOP) {
        pause();
    }
    return 0;
}

void resetBoard() {
    int i, j;
    for (i = 0; i < BOARD_ROW; i++) {
        for (j = 0; j < BOARD_HEIGHT; j++) {
            if (j == 0 || j == BOARD_HEIGHT - 1 || i == BOARD_ROW - 1) {
                board[i][j] = BLOCK;
            } else {
                board[i][j] = BLANK;
            }

        }
    }
}

void initializeBoard() {
    int i, j;
    for (i = 0; i < BOARD_ROW; i++) {
        for (j = 0; j < BOARD_HEIGHT; j++) {
            if (j == 0 || j == BOARD_HEIGHT - 1 || i == BOARD_ROW - 1) {
                board[i][j] = BLOCK;
            } else {
                board[i][j] = BLANK;
            }

        }
    }
    initializePlayer();

}

void initializePlayer() {
    player.position.x = 0;
    player.position.y = BOARD_ROW / 2 - 1;
    player.state = STANDING;
    board[player.position.x][player.position.y] = '-';
    board[player.position.x + 1][player.position.y] = '-';
    board[player.position.x + 2][player.position.y] = '-';
}

void displayBoard() {
    int i, j;
    if (system("clear") == -1) {
        displayError();
    }
    for (i = 0; i < BOARD_ROW; i++) {
        for (j = 0; j < BOARD_HEIGHT; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

void signal_hand(int sig) {
    char c;
    scanf("%c", &c);
    switch (c) {
        case 'a':
            moveLeft();
            break;
        case 'd':
            moveRight();
            break;
        case 'w':
            changeState();
            break;
        case 'q':
            exit(0);
        case 's':
            moveDown();
            break;
        default:
            break;
    }
    displayBoard();
    signal(SIGUSR2, signal_hand);
}

void alarm_hand(int sig) {
    signal(SIGALRM, alarm_hand);
    moveDown();
    displayBoard();
    alarm(1);
}

int checkBoundries(int moveType) {
    // if move type == 0 than we check for down boundries
    if (moveType == MOVE_DOWN) {
        if (player.state == LAYING) {
            if ((player.position.x + 1 == BOARD_ROW - 1)) {
                return RESET_PLAYER;
            }
        } else if (player.state == STANDING) {
            if (player.position.x + 3 == BOARD_ROW - 1) {
                return RESET_PLAYER;
            }
        }

    } else if (moveType == MOVE_LEFT) {
        // check for right and left boundries
        if (player.state == LAYING) {
            if (player.position.y - 1 == 1) {
                return CANT_MOVE_DIRECTION;
            }
        } else if (player.state == STANDING) {
            if (player.position.y == 1) {
                return CANT_MOVE_DIRECTION;
            }
        }
    } else if (moveType == MOVE_RIGHT) {
        if (player.state == LAYING) {
            if (player.position.y + 1 == BOARD_HEIGHT - 2) {
                return CANT_MOVE_DIRECTION;
            }
        } else if (player.state == STANDING) {
            if (player.position.y + 1 == BOARD_HEIGHT - 1) {
                return CANT_MOVE_DIRECTION;
            }
        }
    } else if (moveType == CHANGE_POSITION) {
        if (player.state == LAYING) {
            if (player.position.x == BOARD_ROW - 2) {
                return CANT_MOVE_DIRECTION;
            }
        } else if (player.state == STANDING) {
            if (player.position.y >= BOARD_ROW - 2 || player.position.y == 1) {
                return CANT_MOVE_DIRECTION;
            }
        }
    }
    return CAN_MOVE;

}


void moveDown() {
    resetBoard();
    if (player.state == LAYING) {
        int check = checkBoundries(MOVE_DOWN);
        switch (check) {
            case RESET_PLAYER:
                initializePlayer();
                break;
            case CAN_MOVE:
                player.position.x += 1;
                board[player.position.x][player.position.y] = '-';
                board[player.position.x][player.position.y - 1] = '-';
                board[player.position.x][player.position.y + 1] = '-';
                break;
            case CANT_MOVE_DIRECTION:
                break;
            default:
                break;

        }
    } else if (player.state == STANDING) {
        int check = checkBoundries(MOVE_DOWN);
        switch (check) {
            case RESET_PLAYER:
                initializePlayer();
                break;
            case CAN_MOVE:
                player.position.x += 1;
                board[player.position.x][player.position.y] = '-';
                board[player.position.x + 1][player.position.y] = '-';
                board[player.position.x + 2][player.position.y] = '-';
                break;
            default:
                break;
        }
    }
}

void moveLeft() {
    resetBoard();
    if (player.state == LAYING) {
        int check = checkBoundries(MOVE_LEFT);
        switch (check) {
            case CAN_MOVE:
                player.position.y -= 1;
                board[player.position.x][player.position.y - 1] = '-';
                board[player.position.x][player.position.y] = '-';
                board[player.position.x][player.position.y + 1] = '-';
                break;
            case CANT_MOVE_DIRECTION:
                savePositionLaying();
                break;
            default:
                break;
        }
    } else if (player.state == STANDING) {
        int check = checkBoundries(MOVE_LEFT);
        switch (check) {
            case RESET_PLAYER:
                initializePlayer();
                break;
            case CAN_MOVE:
                player.position.y -= 1;
                board[player.position.x][player.position.y] = '-';
                board[player.position.x + 1][player.position.y] = '-';
                board[player.position.x + 2][player.position.y] = '-';
                break;
            case CANT_MOVE_DIRECTION:
                savePositionStanding();
                break;
            default:
                break;
        }
    }
}

void moveRight() {
    resetBoard();
    if (player.state == LAYING) {
        int check = checkBoundries(MOVE_RIGHT);
        switch (check) {
            case CAN_MOVE:
                player.position.y += 1;
                board[player.position.x][player.position.y] = '-';
                board[player.position.x][player.position.y - 1] = '-';
                board[player.position.x][player.position.y + 1] = '-';
                break;
            case CANT_MOVE_DIRECTION:
                savePositionLaying();
                break;
            default:
                break;
        }
    } else if (player.state == STANDING) {
        int check = checkBoundries(MOVE_RIGHT);
        switch (check) {
            case RESET_PLAYER:
                initializePlayer();
                break;
            case CAN_MOVE:
                player.position.y += 1;
                board[player.position.x][player.position.y] = '-';
                board[player.position.x + 1][player.position.y] = '-';
                board[player.position.x + 2][player.position.y] = '-';
                break;
            case CANT_MOVE_DIRECTION:
                savePositionStanding();
                break;
            default:
                break;
        }
    }
}

void changeState() {
    if (player.state == STANDING) {
        int check = checkBoundries(CHANGE_POSITION);
        switch (check) {
            case CAN_MOVE:
                resetBoard();
                player.position.x += 1;
                board[player.position.x][player.position.y - 1] = '-';
                board[player.position.x][player.position.y] = '-';
                board[player.position.x][player.position.y + 1] = '-';
                player.state = LAYING;
                break;
            case CANT_MOVE_DIRECTION:
                savePositionStanding();
                break;
            default:
                break;
        }

    } else if (player.state == LAYING) {
        int check = checkBoundries(CHANGE_POSITION);
        switch (check) {
            case CAN_MOVE:
                resetBoard();
                player.position.x-=1;
                board[player.position.x][player.position.y] = '-';
                board[player.position.x+1][player.position.y] = '-';
                board[player.position.x+2][player.position.y] = '-';
                player.state = STANDING;
                break;
            case CANT_MOVE_DIRECTION:
                savePositionLaying();
            default:
                break;
        }

    }

}


void savePositionLaying() {
    board[player.position.x][player.position.y - 1] = '-';
    board[player.position.x][player.position.y] = '-';
    board[player.position.x][player.position.y + 1] = '-';
}


void savePositionStanding() {
    board[player.position.x][player.position.y] = '-';
    board[player.position.x + 1][player.position.y] = '-';
    board[player.position.x + 2][player.position.y] = '-';
}


void displayError() {
    write(STDERR, ERROR_MESSAGE, ERROR_SIZE);
    exit(EXIT_FAILURE);
}
