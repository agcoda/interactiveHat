#ifndef COMM_H
#define COMM_H

#include<string>
#include<vector>

//for testing
#include<iostream>

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
		char msg_out[3] = {'N','N','N'};
	
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
		}

		void getMsg(){
			//read from serial buffer
		}
			 
};
}
#endif