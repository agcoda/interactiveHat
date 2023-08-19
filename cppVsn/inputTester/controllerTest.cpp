#ifndef CTRLR_H
#define CTRLR_H

#include<string>
#include<vector>

//for testing
#include<iostream>

//basic alg of main
/*
setup the object for the ctrlr that matches whats plugged in
open proper communication with communicator

Get an event from evdev
check if that event is a btn type or range type
	if btn type get the code and translate that to a button name
	get if btn up or down
	if range type get the code and if the value is + or -
hand off the btn name to the communicator
	it translates that button to the proper form
	it then sends it off to the proper other module
	gets a confirmation message
return to get next event
if next is same btn (a release) just ignore it
if next is range type and same direction ignore it
*/

//remote system programming notes
/*
Btns take a single action when received
range actions repeat until a change or 0 is received.

*/

//every controller will have range and button inputs
//more advanced may have touch and motion, but we dont need that here
class Ctrlr{
	protected:
		
		std::string pressed_name = "n";
		char pressed_type = 'n';
		char pressed_dir = 'n';
		int last_pressed_i = 0;
		std::vector<int> btn_codes;

		//standard list of buttons most controllers have
		std::vector<std::string> btn_names{ "a", "b", "x", 
		"y", "tl", "tr", "select", "start", "home", 
		"l3", "r3"};


	public:
		//three attributes to events type, code, value
		//each input will  be a b(utton) or a r(ange)
		//type 1 =btn, type 3=range
		void setType(int type_code){
			switch(type_code){
				case 1: //btn type
					pressed_type = 'b';
				case 3: //rng type
					pressed_type = 'r';
			}
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

		void lastPressedUpdate(int i){
			last_pressed_i = i;
		}

		//when a button is pressed call this to update all info on that
		void buttonPressed(){

		}


};

class Logitech310 : Ctrlr {
	private:
		std::string nameStr = "Logitech310";

		//specific codes for the Logitech310 gotten from testing
		std::vector<int> btn_codes{304, 305, 307,
			308, 310, 311, 314, 315, 316, 
			317, 318};
/*		// replaced direct definitions with vectors for searching
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
		int last_pressed = 0; //index of last pressed

		//for joysticks/triggers vals go from-32768 to 32767
		//for joysticks the code is 0-5 but it also sends a full zero code btwn each
		int DEADZONE = 150; //they don't quite reset to zero so only send the val if its outside this range
		Logitech310
	
	public:
		std::string getName(){
			return nameStr;
		}
		
		//useful to store previous btn index to avoid doubling commands

		
		//input a code and get a string representing the button pressed
		//these btn strings will be sent to the translator
		void setBtnName(int btn_code){
			for(int i=0; i< btn_names.size();i++){
				if(btn_code == btn_codes[i]){
					pressed_name = btn_names[i];
				}
			}
		}
		

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
	
	public:
		char translate_to_serial(Logitech310 &logitech){
			*logitech.
		}
			 
};
#endif


//test driver
int main(){
	Logitech310 ctrlr1;
	std::cout << ctrlr1.getName();

	return 0;
}