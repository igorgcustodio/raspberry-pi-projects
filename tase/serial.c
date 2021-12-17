#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>


int main(int argc, char** argv) {
 	//system("sudo systemctl stop serial-getty@serial0.service");
 	int sfd = open("/dev/serial0", O_RDWR | O_NOCTTY); 
 	if (sfd == -1) 
 	{
 		printf("Error no is : %d\n", errno);
  		printf("Error description is : %s\n", strerror(errno));
  		return (-1);
 	};
 	struct termios options;
 	tcgetattr(sfd, &options);

	cfsetspeed(&options, B9600);
	cfmakeraw(&options);
	options.c_cflag &= ~CSTOPB;
	options.c_cflag |= CLOCAL;
	options.c_cflag |= CREAD;
	options.c_cc[VTIME]=1; 
	options.c_cc[VMIN]=100;
	tcsetattr(sfd, TCSANOW, &options);
	char buf[] = "hello world";
	char buf2[100];
	int count = write(sfd, buf,strlen(buf));

	usleep(100000);
	int bytes;
	ioctl(sfd, FIONREAD, &bytes);
	if(bytes!=0)
		count = read(sfd, buf2, 100);
	
	printf("%s\n\r", buf2);
	close(sfd);
	return (EXIT_SUCCESS);
}