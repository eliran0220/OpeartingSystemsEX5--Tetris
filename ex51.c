//
// Created by eliran on 6/7/19.
//
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

#define DRAW "./draw.out"
#define EXIT_FAIL -1
#define FAILURE -1
#define ERROR_SIZE 21
#define ERROR_MESSAGE "Error in system call\n"
#define STDERR 2
#define SIZE 2
#define QUIT 'q'
#define W 'w'
#define D 'd'
#define S 's'
#define A 'a'

char getch();

void displayError();

int main() {
    int fd[SIZE];
    int status;
    pid_t pid;
    if (pipe(fd) < 0) {
        displayError();
    }
    pid = fork();
    if (pid < 0) {
        displayError();
    }
    if (pid > 0) {
        char c;
        do {
            c = getch();
            if (write(fd[1], &c, 1) == FAILURE) {
                displayError();
            }
            if (kill(pid, SIGUSR2) == FAILURE) {
                displayError();
            }
        } while (c != QUIT);
    } else {
        dup2(fd[0], 0);
        char *args[SIZE] = {DRAW, NULL};
        if (execvp(args[0], args) < 0) {
            displayError();
        }
    }
    return 0;
}

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

void displayError() {
    write(STDERR, ERROR_MESSAGE, ERROR_SIZE);
    exit(EXIT_FAIL);
}









