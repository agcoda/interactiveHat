
#Function: 
#   Take input from handheld controller
#   Pass input to next module

######################################
#Unit 1: Take input from handheld controller
#Logitech 310 PC Controller
#Controller has
#   omni D pad, 2 joysticks, XYAB buttons, RL trigger, RL bumper

#sys for exit, time for general sleep
import sys
import time

#will write to arduino over serial
import serial

#bring in all scancodes for buttons on desired controller
from logitech310 import *

#initialize the serial communication
ser = serial.Serial(
    port='/dev/ttyS0', #Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
    baudrate = 9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

#get the inputs from that controlfile
from evdev import InputDevice, categorize, ecodes, KeyEvent
gamepad = InputDevice('/dev/input/'+CONTROLFILE)
#just look for the scancodes in ctrlr file and save which button was pressed
btn_pressed = 'none'
#this is the main loop we stay in until home button is pressed
for event in gamepad.read_loop():
    if event.type == ecodes.EV_KEY:
        keyevent = categorize(event)
        #raspi currently running 3.9, cant use match, case yet
        if keyevent.keystate == KeyEvent.key_down:
            if keyevent.scancode == btn_b:
                btn_pressed = 'B'
            if keyevent.scancode == btn_a:
                btn_pressed = 'A'
            if keyevent.scancode == btn_x:
                btn_pressed = 'X' 
            if keyevent.scancode == btn_y:
                btn_pressed = 'Y'     
            if keyevent.scancode == btn_tl:
                btn_pressed = 'TL' 
            if keyevent.scancode == btn_tr:
                btn_pressed = 'TR' 
            if keyevent.scancode == btn_select:
                btn_pressed = 'select' 
            if keyevent.scancode == btn_start:
                btn_pressed = 'start'     
            if keyevent.scancode == btn_home:
                btn_pressed = 'home'
            if keyevent.scancode == btn_l3:
                btn_pressed = 'L3' 
            if keyevent.scancode == btn_r3:
                btn_pressed = 'R3' 
        print(btn_pressed)
        if btn_pressed == 'home':
            sys.exit("Home button closes program")

    ##################################
    #Unit 2: output 
    #once a button is pushed we need to pack and send it to the next device

    #pyserial can't take str,encode first to utf8 or ascii
    ser.write(btn_pressed.encode("utf-8"))

    #get response for integrity, if one sent
    while ser.in_waiting:
        action = ser.readline()
        action = action.decode("utf-8")
        print(action)
        #clear out the rest of the buffer
        action = ser.readline()


