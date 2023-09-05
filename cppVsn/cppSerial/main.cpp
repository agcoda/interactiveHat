
#include <stdio.h>
#include <string.h>


#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int main(void){
//Open a Serial port on Raspi
	int serial_port = open("/dev/tty1",O_RDWR);

	if(serial_port<0){
		printf("Error %i from open: %s\n",errno, strerror(errno));
	}

	struct termios tty;
/*
termios structure
c_iflag //in 
c_oflag //out 
c_cflag //ctrl mode
c_lflag //local mode
c_line //line discipline
c_cc[NCCS] // ctrl chars
*/

	//Set Serial vals
	//set cflags
	tty.c_cflag &= ~PARENB; //disables parity bit
	tty.c_cflag &= ~CSTOPB; //clear stop bit so only one stop bit used
	tty.c_cflag &= ~CSIZE; //clear size bits	
	tty.c_cflag |= CS8; //set bits/Byte 8
	tty.c_cflag &= ~CRTSCTS; //disable hardware flow control
	tty.c_cflag |= CREAD | CLOCAL; // Read on, ignore ctrl lines

	//set lflags
	tty.c_lflag &= ~ICANON; //disbale canonical mode (process input when newline received
	tty.c_lflag &= ~ECHO;
	tty.c_lflag &= ~ECHOE;
	tty.c_lflag &= ~ECHONL; //these 3 echo sent chars back after sending
	tty.c_lflag &= ~ISIG; //disables INTR QUIT and SUSP

	//set iflags
	//disable all the special things
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

	//set oflags
	//again just disabling special things
	tty.c_oflag &= ~OPOST;
	tty.c_oflag &= ~ONLCR;
	//dont need ~oxtabs or oneot in linux

	//set ccc
	//VMIN = 0 and VTIME >0 blocks until  read or timeout

	tty.c_cc[VTIME] = 10; //wait 1s and return anything received
	tty.c_cc[VMIN] = 0;
	//vmin = 0 and vtime = 0 would return when anything called
	//vmin>0 and vtime = 0 would block until received

	//set the baud
	//ard should be 9600
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);

	
	//save above setings
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0){
		printf("Error %i from open: %s\n",errno, strerror(errno));
	}

//Send information over that serial port
	unsigned char msg[] = {'B', 'A', '\r'};

	write(serial_port, msg, sizeof(msg));


	return 0;
}
