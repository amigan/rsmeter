#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/select.h>
#include <sys/ioctl.h>
int fd, siz, stat, logging;
FILE* logfil;
struct tm tim;
sig_t* than(int s);
WINDOW* alph;
char* dev;
void prettyprint(char* str);
void setser(char* ourname);
void closeprop(void);
int mainloop(void);
WINDOW* nums;
int drawnum(WINDOW* twin, short tdig, int yos, int xos);
#define LPN 5
#define COLPN 6
