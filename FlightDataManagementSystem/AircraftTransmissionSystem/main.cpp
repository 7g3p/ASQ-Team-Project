#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "genHeader.h"
#include "dataManipulation.h"


int checkAircraftNumber(char* dest, char* airNum);

int main(int argc, char** argv)
{
	// Variables
	char aircraftFile[MAX_CHAR] = "";
	DataManipulation temp;

	// Check that the aircraft number in argv[argc - 1] is one of the currently obtained telemetry data files; else notify the user of an incorrect aircraft number and exit the program
	if ( NO_ERRORS != checkAircraftNumber(aircraftFile, argv[argc - 1 ]))
	{
		printf("Invalid Aircraft number. Telemetry Data does not exist for that number.\n");
		return -1;
	}


	temp.ParseFromInput(aircraftFile);
	


	return 0;
}

int checkAircraftNumber(char* dest, char* airNum)
{
	// Variables
	char temp[MAX_CHAR] = "";

	// Change the aircraft number into a file name
	sprintf(temp, "..//%s.txt", airNum);

	// Check that the file with the aircraft number exists
	if (NULL == fopen(temp, "r"))
	{
		return FAIL_TO_CONNECT;
	}

	// Copy converted aircraft number to filename to the destination string address referenced by char* dest
	strcpy(dest, temp);

	return NO_ERRORS;
}