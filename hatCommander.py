#Function: 
#   Take input from handheld controller
#   Pass input to next module

#Unit 1: Take input from handheld controller
#Logitech 310 PC Controller
#Controller has
#   omni D pad, 2 joysticks, XYAB buttons, RL trigger, RL bumper

#sys for exit
import sys

#Get input from device
#define which event corresponds to the controller
CONTROLFILE = 'event1'
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



#get the inputs from that controlfile
from evdev import InputDevice, categorize, ecodes
gamepad = InputDevice('/dev/input/'+CONTROLFILE)
#just look for the scancodes
for event in gamepad.read_loop():
    if event.type == ecodes.EV_KEY:
        keyevent = categorize(event)
        if keyevent.keystate == KeyEvent.key_down:
            if keyevent.scancode == btn_b:
                print('B')
            if keyevent.scancode == btn_a:
                print('A')
            if keyevent.scancode == btn_x:
                print('X')
            if keyevent.scancode == btn_y:
                print('Y')    
            if keyevent.scancode == btn_tl:
                print('TL')
            if keyevent.scancode == btn_tr:
                print('TR')
            if keyevent.scancode == btn_select:
                print('select')
            if keyevent.scancode == btn_start:
                print('start')    
            if keyevent.scancode == btn_home:
                sys.exit("Home button closes program")
            if keyevent.scancode == btn_l3:
                print('L3')
            if keyevent.scancode == btn_r3:
                print('R3')
