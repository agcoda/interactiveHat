#include "controller.h"


#include<string>

char Ctrlr :: determineType(int type_code){
	switch(type_code){
		case 1: //btn type
			return 'b';
			break;
		case 3: //rng type
			return 'r';
			break;
		default:
			return 'n';
			break;
	}
	return 'n';
}

