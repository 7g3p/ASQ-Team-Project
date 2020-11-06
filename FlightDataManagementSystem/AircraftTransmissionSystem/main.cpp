#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <ctype.h>
#include "genHeader.h"

// Prototypes
int checkAircraftNumber(char* dest, char* airNum);
void getString(char* retString);

int main(int argc, char** argv)
{
	// Variables
	bool cont = false;
	char userInput[INPUT_CHAR] = "";
	char aircraftFile[MAX_CHAR] = "";
	DataManipulation temp;
	TCPConnection *conn = new TCPConnection();

	try
	{
		printf("Waiting for a Connection...\n");
		conn->ConnectToSocket();
	}
	catch (int retError)
	{
		if (retError != 0)
		{
			return -4;
		}
	}
	
	
	do
	{
		do
		{
			// Get the aircraftTailNumber from the user
			printf("Enter the Aircraft Tail Number (Exact match. E.g. 'C-FGAX'): ");
			getString(userInput);

			// Check that the aircraft number in argv[argc - 1] is one of the currently obtained telemetry data files; else notify the user of an incorrect aircraft number and exit the program
			if (NO_ERRORS != checkAircraftNumber(aircraftFile, userInput))
			{
				printf("Invalid Aircraft number. Telemetry Data does not exist for that number.\n");
				cont = true;
			}
			else
			{
				cont = false;
			}

		} while (cont);


		temp.ParseFromInput(aircraftFile, *conn);

		// Check if the user wishes to exit
		printf("Press 'q' to Quit.\\nn");
	} while ('q' != getch());

	return 0;
}

/*
* FUNCTION : checkAircraftNumber
* DESCRIPTION :
*	This function checks that the inputed airNum is valid and can connect to a telemetry data file
* PARAMETERS :
*	char* dest   : The destination string to hold the converted airNum to its filename
*	char* airNum : The aircraftNumber to be checked
* RETURNS :
*		int : Ret code. -1 is failure; 0 is success
*/
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

/*
* FUNCTION : getString
* DESCRIPTION :
*	This function gets a string from the user
* PARAMETERS :
*	N/A
* RETURNS :
*	N/A
*/
void getString(char* retString)
{
	// Variables
	char temp[INPUT_CHAR] = "";

	fgets(temp, INPUT_CHAR, stdin);

	strcpy(retString, (const char*)strtok(temp, "\n"));
}