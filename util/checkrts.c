#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
int main(int argc, char* argv[])
{
	int stat, fd;
	if(argc != 2) {
		fprintf(stderr, "Usage %s <device>\n", argv[0]);
		exit(-1);
	}
	fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror(argv[0]);
		exit(1);
	}
	ioctl(fd, TIOCMGET, &stat);
	if(stat & TIOCM_RTS) printf("Not asserted\n"); else printf("Asserted\n");
	return 0;
}
