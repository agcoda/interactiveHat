#include "controller.h"
#include "communicator.h"

#include <iostream>
#include <fcntl.h>
//#include <libevdev-1.0/libevdev/libevdev.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

using namespace std;

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


//test driver proto
void pressedEmulator(int test_type,int test_code,int test_value);


int main(){
	//first need to find the matching controller

	int test_type, test_code, test_value = 0;

	//driver
	/*
	//emulate a pushed button B (type, code, value)
	test_type = 1;
	test_code = 305;
	test_value = 0;
	//when button pushed update
	pressedEmulator(test_type,test_code,test_value);
	*/
	//emulate a pushed button lstick (type, code, value)
	test_type = 3;
	test_code = 311;
	test_value = -300;
	//when button pushed update
	pressedEmulator(test_type,test_code,test_value);
	//emulate a pushed button lstick (type, code, value)
	test_type = 3;
	test_code = 311;
	test_value = 0;
	//when button pushed update
	pressedEmulator(test_type,test_code,test_value);

/*	
	struct libevdev *dev = NULL;
 	int fd;
 	int rc = 1;
	
	fd = open("/dev/input/event26", O_RDONLY|O_NONBLOCK);
 	
	rc = libevdev_new_from_fd(fd, &dev);
 	if (rc < 0) {
        	fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
         	exit(1);
 	}
 	printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
 	printf("Input device ID: bus %#x vendor %#x product %#x\n",
        	libevdev_get_id_bustype(dev),
        	libevdev_get_id_vendor(dev),
        	libevdev_get_id_product(dev));
 	//if (!libevdev_has_event_type(dev, EV_REL) ||
     	//	!libevdev_has_event_code(dev, EV_KEY, BTN_LEFT)) {
         //	printf("This device does not look like a mouse\n");
         //	exit(1);
 	//}

	do {
        	struct input_event ev;
        	rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
         	if (rc == 0)
                	printf("Event: %s %s %d\n",
                        	libevdev_event_type_get_name(ev.type),
                        	libevdev_event_code_get_name(ev.type, ev.code),
                        	ev.value);
 	} while (rc == 1 || rc == 0 || rc == -EAGAIN);		
	//get the inputs and print them in simple form for testing
	

	//simple form will eventually be sent down serial comms
*/
	return 0;
}




//test driver
void pressedEmulator(int test_type, int test_code, int test_value){
	static input::Logitech310 ctrlr1;
	static comms::piUART comm1;

	ctrlr1.updatePressed(test_type, test_code, test_value);

	//check if the ctrlr foudn this to be a new button, if so
	//translate it with comms then send it
	if(ctrlr1.getIfIsNew()){
		//updates the msg array[4] held in comm1
		comm1.translateToSerial(ctrlr1.getPressedIndex(), ctrlr1.getPressedDir());
		//sends the full msg[4]
		comm1.sendMsg();
	}
}