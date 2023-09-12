#ifndef COMM_H
#define COMM_H

#include<string>
#include<vector>

//for testing
#include<iostream>

//for serial comms
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>


//every controller will have range and button inputs
//more advanced may have touch and motion, but we dont need that here
namespace comms
{
//takes code inputs from some source and outputs it to a specified type
class Communicator{
	private:
		std::string output_type ="None";
	public:
		void setOutputType(std::string newType){
			output_type = newType;
		}
};

//Serial output type is one char at a time.
//Associate any buttons to single characters
class Serial : Communicator{
	private:
		std::vector<char> output_chars{'A', 'B', 'X',
			'Y', 'L', 'R', 'S', 'T', 'H',
			'Q', 'W'};
		//type, direction, null
		char msg_out[4] = {'N','N','N','\r'};
	
	public:
		//assign the index of the action in ctrlr to one of the available chars in serial
		
		/********
		requires output_chars to be at least the same size
		change to take button names and translate them instead
		should have this method just find the closest matching serial char
		*********/
		void translateToSerial(int index, char dir){
			//first char is button pushed
			msg_out[0] = output_chars[index];
			//second char is up or down
			msg_out[1] = dir;
			//third char is not used yet 
			//stays 'N' (For finding endline?)
		}

		//sends a triplet of type, dir, null 
		void sendMsg(){
			//using cout for testing in diff environ
			for(int i=0;i<3;i++){
				std::cout << msg_out[i];
				//send the serial bit
			}

			write(serial_port, msg_out, sizeof(msg));
		}

		void getMsg(){
			//read from serial buffer
		}
			 
			
};

//constructor for UART communication on raspi
class piUART : Serial {
	public:
		piUART(){
			//Open a Serial port on Raspi
			//ttyS04 is pin 13/14 UART on pi 4
			int serial_port = open("/dev/ttyS0",O_RDWR);

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
		}
};

}
#endif