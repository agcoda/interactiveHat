#ifndef CTRLR_H
#define CTRLR_H

#include<string>

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
		//for joysticks/triggers vals go from-32768 to 32767
		//for joysticks the code is 0-5 but it also sends a full zero code btwn each
		int DEADZONE = 150; //they don't quite reset to zero so only send the val if its outside this range
	public:
		std::string name(const Logitech310&);
};

#endif
