/*
 * Test program to read from meter.
 * By Dan Ponte
 */
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
int fd, siz, stat;
struct tm tim;
time_t now;
sig_t* than(int s);
int main(int argc, char* argv[])
{
	char buffer[16];
	struct termios tr;
	if(argc < 2) exit(-1);
	fd = open(argv[1], O_RDWR);
	if(fd == -1) {
		perror(argv[0]);
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
	ioctl(fd, TIOCMGET, &stat);
	stat &= ~TIOCM_RTS;
	ioctl(fd, TIOCMSET, &stat);
	fprintf(stderr, "Serial port initialized.\n");
	while(1) {
		if(write(fd, "D", 1) == -1) perror("met");
		siz = read(fd, buffer, 15);
		now = time(NULL);
		tim = *localtime(&now);
		if(siz == 14) printf("%d:%d.%d: %s\n", tim.tm_hour,
				tim.tm_min, tim.tm_sec, buffer);
		bzero(buffer, 16);
		sleep(1);
	}
	return 0;
}
sig_t* than(int s)
{
	close(fd);
	exit(0);
}
