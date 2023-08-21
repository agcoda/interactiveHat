#ifndef CTRLR_H
#define CTRLR_H

#include<string>
#include<vector>

//for testing
#include<iostream>

class Ctrlr{
	private:
		//three attributes to events type, code, value
		//type 1 =btn, type 3=joystick/trigger
		//for btns value =1 for press, 0 for release
		int BTNTYPE = 1;
		int DYNTYPE = 3;
};
class Logitech310 : Ctrlr {
	private:
		std::string nameStr = "Logitech310";
		//parallel arrays for matching codes to buttons
		std::vector<std::string> btns{ "a", "b", "x",
	 		"y", "tl", "tr", "select", "start", "home", 
			"l3", "r3"};
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
		int last_pressed_loc 0;
		//for joysticks/triggers vals go from-32768 to 32767
		//for joysticks the code is 0-5 but it also sends a full zero code btwn each
		int DEADZONE = 150; //they don't quite reset to zero so only send the val if its outside this range
	public:
		std::string name(Logitech310 &this){
			return *this.nameStr;
		}
		
		//useful to store previous button to avoid doubling commands
		void lastPressedUpdate(int i){
			*this.last_pressed_loc= i
		}
		
		//input a code and get a string representing the button pressed
		//these btn strings will be sent to the translator
		std::string find_btn_name(Logitech310 &this,int btn_code){
			for(int i=0; i< *this.btns.size();i++){
				if(btn_code == btn_codes[i]){
					return btns[i];
				}
			}
		}
		

};


//takes code inputs from some source and outputs it to a specified type
class Translator{
	private:
		std::string outputType ="None";
	public:
		change_output_type(std::string newType){
			outputType = newType;
		}
};

//Serial output type is one char at a time.
//Associate any buttons to single characters
class Serial : Translator{
	private:
		vector<char> output_chars{'A', 'B', 'X',
			'Y', 'L', 'R', 'S', 'T', 'H',
			'Q', 'W');
	
	public:
		char translate_to_serial(Logitech logitech , std::string btn){
			 

#endif


//test driver
int main(){
	Logitech310 ctrlr1;
	cout << ctrlr1.name();
}