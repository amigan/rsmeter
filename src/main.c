/*
 * RSMeter - read from 22-168A series meters. See README for details.
 * By Dan Ponte
 */
/* $Amigan: rsmeter/src/main.c,v 1.5 2004/11/29 03:47:31 dcp1990 Exp $ */
#include "rsmeter.h"
short metertimeout = 12;
char cvsid[] = "$Amigan: rsmeter/src/main.c,v 1.5 2004/11/29 03:47:31 dcp1990 Exp $";
char* logfi;
time_t now, metertime = 0;
void usage(char* whatcalled)
{
	
	fprintf(stderr, "RSMeter v%s\n(C) Dan Ponte.\nUsage: %s [-c] [-l logfile] [-t timeout] <SerDevice>\n"
			"\t-c: Hide cursor.\n"
			"\t-l logfile: logs to file. Will APPEND, not write over.\n"
			"\t-t timeout: Quit after timeout seconds of no response.\n",
			VERSION, whatcalled);
	exit(-1);
}
int main(int argc, char* argv[])
{
	int ch, sset = 0, hidecurs;
	while ((ch = getopt(argc, argv, "hcl:t:")) != -1)
		switch (ch) {
/*			case 's':
				dev = strdup(optarg);
				sset = 1;
				break;
				*/
			case 'c':
				hidecurs = 1;
				break;
			case 'l':
				logfil = fopen(optarg, "a");
				logfi = strdup(optarg);
				if(logfil == NULL) { perror("rsmeter"); exit(1); }
				logging = 1;
				break;
			case 't':
				metertimeout = atoi(optarg);
				break;
			case '?':
			case 'h':
			default:
				usage(argv[0]);
		}
	if(argv[optind] != NULL) {
		dev = strdup(argv[optind]);
		sset = 1;
	} else {
		usage(argv[0]);
	}
	if(!sset) usage(argv[0]);
	setser(argv[0]);
	initscr();
	argc -= optind;
	argv += optind;
	cbreak();
	start_color();
	keypad(stdscr, TRUE);
	noecho();
	now = time(NULL);
	if(logging) {
		fprintf(logfil, "RSMeter Logfile started at %s\n", ctime(&now));
	}
	nums = newwin(LPN + 3, (COLPN * 6) + 6, (LINES - (LPN + 3))/ 2, (COLS - (COLPN * 6)) / 2);
	alph = newwin(1, 20, ((LINES - (LPN + 3)) / 2) -1, (COLS - (COLPN * 6)) / 2);
	refresh();
	printw("RSMeter v%s. (C)2004, Dan Ponte\n", VERSION);
	mvprintw(2, 1, "                   \n");
	refresh();
	mvprintw(LINES - 1, 1, "Keys: q - quit\t\t%s", logging ? "s - stop logging" : "" );
	mvprintw(LINES - 2, 1, "%s %s", logging ? "Logging to" : "Not logging", logging ? logfi : "");
	refresh();
	if(hidecurs) curs_set(0);
	mainloop();
	return 0;
}
void setser(char* ourname)
{
	struct termios tr;
	if(strcmp(dev, "/dev/cu") != 0 && strcmp(dev, "/dev/ttyS") != 0 && strcmp(dev, "/dev/ttyd")
			!= 0) {
		fprintf(stderr, "%s: %s not a serial device.\n", ourname, dev);
		exit(1);
	}
	if(access(dev, R_OK | W_OK) != 0) {
		perror(ourname);
		exit(-1);
	}
	fd = open(dev, O_RDWR);
	if(fd == -1) {
	perror(ourname);
	exit(-1);
	}
	tcgetattr(fd, &tr);
	cfsetispeed(&tr, 1200);
	cfsetospeed(&tr, 1200);
	tr.c_cflag &= ~PARENB;
	tr.c_cflag &= ~CSTOPB;
	tr.c_cflag |= CSTOPB;
	tr.c_cflag &= ~CSIZE;
	tr.c_cflag |= CS7;
	tcsetattr(fd, TCSANOW, &tr);
	ioctl(fd, TIOCMGET, &status);
	status &= ~TIOCM_RTS;
	ioctl(fd, TIOCMSET, &status);
	ioctl(fd, TIOCMGET, &status);
	if(status & TIOCM_RTS) {
		fprintf(stderr, "RTS Not asserted! Aborting.\n");
		close(fd);
		exit(1);
	}
}
int mainloop(void)
{
	char buffer[14];
	char cbuf[3];
	struct timeval tv;
	int cou = 0, rval;
	int didwr = 0;
	fd_set fds_read;
	memset(buffer, 0, sizeof(buffer));
	FD_ZERO(&fds_read);
	if(write(fd, "D", sizeof("D")) == -1) ;
	cou = 0;
	didwr = 1;
	metertime = time(NULL);
	while(1) {
		FD_ZERO(&fds_read);
		FD_SET(fd, &fds_read);
		FD_SET(fileno(stdin), &fds_read);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		switch((rval = select(fd + 1, &fds_read, NULL, NULL, &tv)))
		{
			case -1:
				exit(-1);
				break;
			case 0:
				now = time(NULL);
				move(3, 4);
				clrtoeol();
				refresh();
				if(now - metertime > 2 && metertime != 0) {
					mvprintw(3, 4, "ERROR: No response from meter in %d seconds!",
							now - metertime);
					if(now - metertime >= metertimeout) {
						closeprop();
						fprintf(stderr, "No response from meter in %d seconds,"
								" aborted.\n", metertimeout);
						exit(3);
					}
					refresh();
					break;
				}
				if(metertime != 0) {
					break;
				}
				if(write(fd, "D", sizeof("D")) == -1) ;
				memset(buffer, 0, sizeof(buffer));
				cou = 0;
				didwr = 1;
				metertime = time(NULL);
				break;
			default:
				{
					if(FD_ISSET(fd, &fds_read) != 0)
					{
						metertime = 0;
						siz = read(fd, cbuf, 1);
						if(cou == 13) {
						if(cbuf[0] >= 32 && cbuf[0] <= 126) buffer[cou] = cbuf[0];
							now = time(NULL);
							tim = *localtime(&now);
/* #ifdef USEOLDWAY */
							mvprintw(2, 1, "%d:%d.%d: %s\n", tim.tm_hour,
								tim.tm_min, tim.tm_sec, buffer);
							refresh();
/* #endif */
							prettyprint(buffer);
							if(logging) {
								fprintf(logfil, "%d:%d.%d: %s\n", 
										tim.tm_hour, tim.tm_min,
										tim.tm_sec, buffer);
							}
							cou = 0;
							memset(buffer, 0, sizeof(buffer));
							didwr = 0;
						} else if(cou < 13) {
						if(cbuf[0] >= 32 && cbuf[0] <= 126) buffer[cou] = cbuf[0];
							cou++;
						}
						cbuf[0] = cbuf[1] = '\0';
					}
					if(FD_ISSET(fileno(stdin), &fds_read) != 0)
					{
						unsigned char kbbuf[1];
						int count;
						if((count = read(fileno(stdin), kbbuf, sizeof(kbbuf))))
						{
							switch(kbbuf[0])
							{
								case 'q':
									closeprop();
									exit(0);
									break;
								case 's':
									now = time(NULL);
									if(logging) {
									       	fprintf(logfil,
									"RSMeter logfile closed at %s\n",
									ctime(&now));
										logging = 0;
										fclose(logfil);
									}
									move(LINES - 2, 1);
									clrtoeol();
									printw("Not logging");
									refresh();
									break;
							}
						}
					}
				}
		}
	}
	return 0;
}
void closeprop(void)
{

	now = time(NULL);
	close(fd);
	if(logging) {
	       	fprintf(logfil,
			"RSMeter logfile closed at %s\n",
	ctime(&now));
	fclose(logfil);
	}	
	endwin();
}
void prettyprint(char* str)
{
	int i, ndig = 0;
	wclear(nums);
	wclear(alph);
	for(i = 0; i < strlen(str); i++)
	{
		if(isdigit(str[i])) {
			drawnum(nums, str[i] - 48, 1, 1 + (ndig * (COLPN + 1)));
			wrefresh(nums);
			ndig++;
		}
		if(str[i] == '.') {
			drawnum(nums, 10, 1, 1 + (ndig * (COLPN + 1)));
			wrefresh(nums);
			ndig++;
		} else if(str[i] == '-') {
			drawnum(nums, 11, 1, 1 + (ndig * (COLPN + 1)));
			wrefresh(nums);
			ndig++;
		} else if(isalpha(str[i]) || str[i] == ' ') {
			waddch(alph, str[i]);
			wrefresh(alph);
		}
	}
	move(2, 0);
}
