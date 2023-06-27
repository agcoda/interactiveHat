
#define which event corresponds to the controller
CONTROLLERNAME = "Logitech Gamepad F310"

#scancodes gathered from manual testing of inputs
btn_a = 304
btn_b = 305
btn_x = 307
btn_y = 308
btn_tl = 310
btn_tr = 311
btn_select = 314
btn_start = 315
btn_home = 316
btn_l3 = 317
btn_r3 = 318

#event.codes for joysticks and triggers
stick_l_x = 0
stick_l_y = 1
stick_r_x = 3
stick_r_y = 4
trig_l = 2
trig_r = 5

#three attributes to events type, code, value
#type 1 =btn, type 3=joystick/trigger
#for btns value =1 for press, 0 for release

#for joysticks/triggers vals go from-32768 to 32767
#for joysticks the code is 0-5 but it also sends a full zero code btwn each
