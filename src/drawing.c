#include "rsmeter.h"
char numbs[14][6][7] = {
	{
		"","","","",""
	},
	{
		"******",
		"*    *",
		"*    *",
		"*    *",
		"******"
	},
	{
		"  **  ",
		" ***  ",
		"  **  ",
		"  **  ",
		" **** "
	},
	{
		"******",
		"     *",
		"******",
		"*     ",
		"******"
	},
	{
		"******",
		"     *",
		" *****",
		"     *",
		"******"
	},
	{
		"*    *",
		"*    *",
		"******",
		"     *",
		"     *"
	},
	{
		"******",
		"*     ",
		"******",
		"     *",
		"******"
	},
	{
		"******",
		"*     ",
		"******",
		"*    *",
		"******"
	},
	{
		"******",
		"    * ",
		"   *  ",
		"  *   ",
		" *    "
	},
	{
		"******",
		"*    *",
		"******",
		"*    *",
		"******"
	},
	{
		"******",
		"*    *",
		"******",
		"     *",
		"******"
	},
	{
		"      ",
		"      ",
		"      ",
		"  *** ",
		"  *** "
	},
	{
		"      ",
		"      ",
		" **** ",
		"      ",
		"      "
	},
	{
		"","","","",""
	}
};
int drawnum(WINDOW* twin, short tdig, int yos, int xos)
{
	short i, j;
#ifdef SLOWWAY
	init_pair(5, COLOR_RED, COLOR_RED);
#endif
	for(i = 0; i <= LPN - 1; i++) {
		wmove(twin, yos + i, xos);
	       	for(j = 0; j <= COLPN - 1; j++) {
#ifdef SLOWWAY
			if(numbs[tdig+1][i][j] == '*') {
				wattron(twin, COLOR_PAIR(5));
				waddch(twin, ' ');
				wattroff(twin, COLOR_PAIR(5));
			} else waddch(twin, ' ');
#else
			if(numbs[tdig+1][i][j] == '*') waddch(twin, ACS_BLOCK); else waddch(twin, ' ');
#endif
		}
	}
	return 0;
}
int oldmain(void)
{
	initscr();
	noecho();
	start_color();
	attron(A_ALTCHARSET);
	refresh();
	drawnum(stdscr, 2, 4, 4);
	refresh();
	getch();
	endwin();
	return 0;
}
