
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

    prev_btn = "none"
    prev_val = "none"

    #main loop provided by evdev
    for event in gamepad.read_loop():
        dec_pressed = []
        #if its a button type (1 or 0) we just need to decifer the scancode
        if event.type == BTNTYPE:
            btn_pressed: str = action.btnInt(event.code)
            serialComm.writeToArd(btn_pressed)
            serialComm.readFromArd()

        #if its a decimal type we also need to send the value
        #starting with just if its + or -
        if event.type == DYNTYPE:
            dec_pressed: tuple[str,int]= action.btnDec(event.code, event.value)
            #dyn types send a ton of keypresses, just send the first one
            if dec_pressed[0] != prev_btn and dec_pressed[1] !=prevVal:
                [serialComm.writeToArd(i) for i in dec_pressed]
            serialComm.readFromArd()
            prev_btn = dec_pressed[0] #0 is the associated stick
            prev_val = dec_pressed[1]

    
            

    ##################################
    #Unit 2: output 
    #once a button is pushed we need to pack and send it to the next device





class CtrlrAction:
    #handles button presses that are just off or on, like AB etc
    def btnInt(self, code: str) -> str:
        #raspi currently running 3.9, cant use match, case yet
        if code == btn_b:
            btn_pressed = 'B'
        if code == btn_a:
            btn_pressed = 'A'
        if code == btn_x:
            btn_pressed = 'X' 
        if code == btn_y:
            btn_pressed = 'Y'     
        if code == btn_tl:
            btn_pressed = 'L' 
        if code == btn_tr:
            btn_pressed = 'R' 
        if code == btn_select:
            btn_pressed = 'C' 
        if code == btn_start:
            btn_pressed = 'S'     
        if code == btn_home:
            btn_pressed = 'H'
        if code == btn_l3:
            btn_pressed = 'Q' 
        if code == btn_r3:
            btn_pressed = 'W' 
        print(btn_pressed)
        if btn_pressed == 'H':
            sys.exit("Home button closes program")
        return btn_pressed
    
    #for joysticks/triggers that have a range of being pushed
    def btnDec(self, code:str, val:int) ->tuple[str]:
        dec_pressed = []
        #first char sent is which stick/trig
        if code == stick_l_x:
            dec_pressed.append('Z')
        elif code == stick_l_y:
            dec_pressed.append('C')

        #second is if pos or negative

        if val >=0:
            dec_pressed.append('P')
        elif val <0:
            dec_pressed.append('N')

        #could upgrade to send the full value but not necessary 
        # since speed is controlle by other btns
        return dec_pressed

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

    def writeToArd(self, btn_pressed):    
        #i=0 #db
        #pyserial can't take str,encode first to utf8 or ascii
        print("Pressed: ")
        print(btn_pressed)
        self.ser.flush
        self.ser.write(btn_pressed.encode("utf-8"))
        self.ser.flush
        #print("sent" + btn_pressed + "for" + str(i) +"time\n")

    def readFromArd(self):
        #get response for integrity, if one sent
        while self.ser.in_waiting:
            action = self.ser.readline()
            #action = action.decode("utf-8")
            #print("received:" +action +"\n")
            print("Received: ")
            print(action)
            #clear out the rest of the buffer
            while self.ser.in_waiting:
                trash = self.ser.readline()

main()