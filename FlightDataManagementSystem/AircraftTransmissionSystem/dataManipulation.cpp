#include "dataManipulation.h"
#include "genHeader.h"


DataManipulation::FlightData DataManipulation::ParseData(char* data)
{
	// Variables
	FlightData retData = { retData.dateTime = NULL, retData.x = NULL, retData.y = NULL, retData.z = NULL, retData.weight = NULL, retData.alt = 0.0, retData.pitch = 0.0, retData.bank = 0.0};
	char* tempToken;
	int counter = 0;
	char* str;

	tempToken = strtok(data, ",");
	counter++;

	// Continously loop through the string parsing it into tokens until the end of the string is reached
	while (tempToken != NULL)
	{
		// Save to the private struct for later parsing and checksum calculation
		switch (counter)
		{
		case 1:
			strcpy(retData.dateTime, tempToken);
			break;
		case 2:
			strcpy(retData.x, tempToken);
			break;
		case 3:
			strcpy(retData.y, tempToken);
			break;
		case 4:
			strcpy(retData.z, tempToken);
			break;
		case 5:
			strcpy(retData.weight, tempToken);
			break;
		case 6:
			retData.alt = strtod(tempToken, &str);
			break;
		case 7:
			retData.pitch = strtod(tempToken, &str);
			break;
		case 8:
			retData.bank = strtod(tempToken, &str);
			break;
		default:

			break;
		}

		// Seperate the new token from the string
		tempToken = strtok(data, ",");
		counter++;
	}


	return retData;
}


int DataManipulation::ParseFromInput(char* fileName)
{
	// Variables
	FILE* stream = NULL;
	char tempLine[MAX_CHAR] = "";
	bool done = false;

	// Check that the file connects
	if (NULL == (stream = fopen(fileName, "r")))
	{
		return FAIL_TO_CONNECT;
	}

	while (done == false)
	{
		// Check that the EOF has not been reached
		if (NULL != fgets(tempLine, MAX_CHAR, stream))
		{
			// Call a parsing function
			printf("%s\n\n", tempLine);

			// Clear the string
			memset(tempLine, 0, sizeof(tempLine));
		}
		else
		{
			// Check if the NULL result is due to and error or EOF reached
			if (feof(stream))
			{
				break;
			}
			else if (0 != ferror(stream))
			{
				return FAIL_TO_READ;
			}
		}
	}

	if (0 != fclose(stream))
	{
		return FAIL_TO_CLOSE;
	}

	return NO_ERRORS;
}