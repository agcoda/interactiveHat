
#Function: 
#   Take input from handheld controller
#   Pass input to next module

######################################
#Unit 1: Take input from handheld controller
#Logitech 310 PC Controller
#Controller has
#   omni D pad, 2 joysticks, XYAB buttons, RL trigger, RL bumper

#evdev allows for simple controller interfacing
import evdev

#sys for exit, time for general sleep
import sys
import time

#will write to arduino over serial
import serial

#bring in all scancodes for buttons on desired controller
from logitech310 import *


#need to find the ControllerName in /dev/input since its not guaranteed consistent
    #get a list of all devices
devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
for device in devices:
    if device.name == CONTROLLERNAME:
        CONTROLFILE = device.path

#get the inputs from that controlfile

gamepad = evdev.InputDevice(CONTROLFILE)
#just look for the scancodes in ctrlr file and save which button was pressed
btn_pressed = 'none'
#this is the main loop we stay in until home button is pressed




def main():

    serialComm = SerialComm() #this init will start serial comms

    action = CtrlrAction()

    #main loop provided by evdev
    for event in gamepad.read_loop():
        if event.type == evdev.ecodes.EV_KEY:
            keyevent = evdev.categorize(event)
            if keyevent.keystate == evdev.KeyEvent.key_down:
                btn_pressed = action.btnInt(keyevent)

    
            

    ##################################
    #Unit 2: output 
    #once a button is pushed we need to pack and send it to the next device





class CtrlrAction:
    #handles button presses that are just off or on, like AB etc
    def btnInt(self, keyevent: str) -> str:
        #raspi currently running 3.9, cant use match, case yet
        if keyevent.scancode == btn_b:
            btn_pressed = 'B'
        if keyevent.scancode == btn_a:
            btn_pressed = 'A'
        if keyevent.scancode == btn_x:
            btn_pressed = 'X' 
        if keyevent.scancode == btn_y:
            btn_pressed = 'Y'     
        if keyevent.scancode == btn_tl:
            btn_pressed = 'L' 
        if keyevent.scancode == btn_tr:
            btn_pressed = 'R' 
        if keyevent.scancode == btn_select:
            btn_pressed = 'C' 
        if keyevent.scancode == btn_start:
            btn_pressed = 'S'     
        if keyevent.scancode == btn_home:
            btn_pressed = 'H'
        if keyevent.scancode == btn_l3:
            btn_pressed = 'Q' 
        if keyevent.scancode == btn_r3:
            btn_pressed = 'W' 
        print(btn_pressed)
        if btn_pressed == 'H':
            sys.exit("Home button closes program")
        return btn_pressed
    
    #for joysticks/triggers that have a range of being pushed
    def btnDec(self, keyevent):
        if keyevent.keystate == evdev.KeyEvent.key_down:
            btnPressed = keyevent.scancode
            print(btnPressed)
        return btn_pressed

class SerialComm():
    def __init__(self):
        
        self.ser = serial.Serial(
            port='/dev/ttyS0', #Replace ttyS0 with ttyAM0 for Pi1,Pi2,Pi0
            baudrate = 9600,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=1
        )

    def writeToArd(self, ):    
        #pyserial can't take str,encode first to utf8 or ascii
        self.ser.flush
        self.ser.write(btn_pressed.encode("utf-8"))
        self.ser.flush

    def readFromArd(self):
        #get response for integrity, if one sent
        while self.ser.in_waiting:
            action = self.ser.readline()
            #action = action.decode("utf-8")
            #print("received:" +action +"\n")
            print(action)
            #clear out the rest of the buffer
            while self.ser.in_waiting:
                trash = self.ser.readline()

main()