
#Function: 
#   Take input from handheld controller
#   Write to screen for viewing

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
for event in gamepad.read_loop():
    if event.type == evdev.ecodes.EV_KEY:
        keyevent = evdev.categorize(event)
        #raspi currently running 3.9, cant use match, case yet

        print(keyevent)



