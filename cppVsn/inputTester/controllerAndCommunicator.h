#ifndef CTRLR_H
#define CTRLR_H

#include<string>
#include<vector>

//for testing
#include<iostream>

//every controller will have range and button inputs
//more advanced may have touch and motion, but we dont need that here
namespace input
{
class Ctrlr{
	protected:
		bool new_pressed = true;
		char pressed_r_in_range = 'd';//three zones n egative, d eadzone, p ositive
		int pressed_i = 0;
		//{type, code, value}
		int pressed_trip[3] = {0,0,0};
		
		std::string pressed_name = "n";
		char pressed_type = 'n';
		char pressed_dir = 'n';

		int num_btns = 11;

		//standard list of buttons most controllers have
		std::vector<std::string> btn_names{ "a", "b", "x", 
		"y", "tl", "tr", "select", "start", "home", 
		"l3", "r3"};

		//each controller will need to overwrite these with the vals it sends for them
		std::vector<int> btn_codes{0,0,0,
		0,0,0,0,0,0,
		0,0};

		//for joysticks/triggers vals go from-32768 to 32767
		//for joysticks the code is 0-5 but it also sends a full zero code btwn each
		//each ctrlr needs update deadzone on const
		int deadzone = 0; //they don't quite reset to zero so only send the val if its outside this range


	public:
		bool getIfIsNew(){
			return new_pressed;
		}

		//three attributes to events type, code, value
		//each input will  be a b(utton) or a r(ange)
		//type 1 =btn, type 3=range
		char determineType(int type_code){
			switch(type_code){
				case 1: //btn type
					return 'b';
					break;
				case 3: //rng type
					return 'r';
					break;
			}

		}


		void setType(int type_code){
			pressed_type = determineType(type_code);
		}

		//for btns value =1 for d(own), 0 for u(p)
		void setBtnDir(int value_code){
			if(pressed_type == 'b'){
				if(value_code == 1){
					pressed_dir = 'd';
				}
				else{
					pressed_dir = 'u';

				}
			}
		}

		//translates the value to a char


		//returns the char for "u p or d own"
		char getPressedDir(){
			return pressed_dir;
		}

		void setPressedName(int code){
			for(int i=0; i< btn_names.size();i++){
				if(code == btn_codes[i]){
					pressed_name = btn_names[i];
					lastPressedUpdate(i);
				}
			}

			pressed_name = std::to_string(code);
			//pressed_trip[1] = code;
		}
		std::string getPressedName(){
			return pressed_name;
		}


		void lastPressedUpdate(int i){
			pressed_i = i;
		}

		//requires caller to understand or figure out whats in each location
		int getPressedIndex(){
			return pressed_i;
		}

		char determineRange(int value){
			if(value < -deadzone){
				return 'n';
			}
			else if(value > deadzone){
				return 'p';
			}
			else {
				return 'z';
			}
		}
		void setPressedRange(int value){
			pressed_r_in_range = determineRange(value);
		}

		void setRngDir(int value){
			//only want to update if its + or - and outside the deadzone
			//u(p) for positive dir, d(own) for neg z for z ero
			char val_char = determineRange(value);
			switch(val_char){
				case 'p':
					pressed_dir = 'u';
					break;
				case 'n':
					pressed_dir = 'd';
					break;
				case 'z':
					pressed_dir = 'z';
					break;
			}


		}



		//takes the triplet type,code,value and stores it as a pressed button
		void updatePressed(int type, int code, int value){
			char rngTest;
			//first check if this is a repeated press
			if(code != pressed_trip[1]){
				new_pressed = true;
				//if its a new button, update pressed

				pressed_trip[0] = type;
				pressed_trip[1] = code;
				pressed_trip[2] = value;

				setType(type);
				setPressedName(code);
				setPressedRange(value);
				//maybe overload this with bool? worth?
				if(pressed_type == 'b'){
					setBtnDir(value);
				}
				else if(pressed_type == 'r'){
					setRngDir(value);
				}
				else {
					//unknown type found, log it
				}
			}
			else{
				//if its a repeated button ignore it
				new_pressed = false;
				//unless if its a repeated range and dir is different update
				//different dir includes entering/exiting deadzone
				rngTest = determineRange(value);

				if(determineType(type) == 'r' && (determineRange(value) != pressed_r_in_range)
				){
					setRngDir(value);
					new_pressed = true;
				}
			}




		}




};

class Logitech310 : public Ctrlr {
	private:
		std::string nameStr = "Logitech310";
		
		/*	
		//specific codes for the Logitech310 gotten from testing
		std::vector<int> btn_codes{304, 305, 307,
			308, 310, 311, 314, 315, 316, 
			317, 318};	
		//replaced direct definitions with vectors for searching
		int btn_a = 304;
		int btn_b = 305;
		int btn_x = 307;
		int btn_y = 308;
		int btn_tl = 310;
		int btn_tr = 311;
		int btn_select = 314;
		int btn_start = 315;
		int btn_home = 316;
		int btn_l3 = 317;
		int btn_r3 = 318;
		*/
		
	
	public:

		Logitech310(){
			deadzone = 150;
			btn_codes = {304, 305, 307,
			308, 310, 311, 314, 315, 316, 
			317, 318};
		}

		std::string getName(){
			return nameStr;
		}
		
		//useful to store previous btn index to avoid doubling commands

		
		//input a code and get a string representing the button pressed
		//these btn strings will be sent to the translator

		

};


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
		void translateToSerial(Logitech310 &logitech){
			//first char is button pushed
			msg_out[0] = output_chars[logitech.getPressedIndex()];
			//second char is up or down
			msg_out[1] = logitech.getPressedDir();
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