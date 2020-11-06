/*
* FILE : dataManipulation.cpp
* PROJECT : SENG3020 - Milestone #2
* PROGRAMMER : Stephen Perrin, Kieran Campbell, Zena Potts, Karson Lai
* FIRST VERSION : 2020-11-05
* DESCRIPTION :
* This file contains the methods for the DataManipulation class.
*/
#include "genHeader.h"

/*
* FUNCTION : ParseData
* DESCRIPTION :
*	This function parses the raw telemetry data into understandable tokens that will be packaged and sent
* PARAMETERS :
*	char* data   : The raw telemetry data
*	char* aircraftNum : The aircraftNumber (in filename form)
* RETURNS :
*		FlightData : The parsed tokens of the telemetry data
*/
DataManipulation::FlightData DataManipulation::ParseData(char* data, char* aircraftNum)
{
	// Variables
	FlightData retData;
	char* tempToken;
	int counter = 0;
	char* str;

	// Parse the first token
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
			retData.x = strtod(tempToken, &str);
			break;
		case 3:
			retData.y = strtod(tempToken, &str);
			break;
		case 4:
			retData.z = strtod(tempToken, &str);
			break;
		case 5:
			retData.weight = strtod(tempToken, &str);
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
			retData.aircraftNumber = strtok(aircraftNum, "..//.txt");		// Convert the telemetry data filename (which has the tail number in it) into just the tail number and save to FlightData
			break;
		}

		// Seperate the new token from the string
		tempToken = strtok(NULL, ",\n");
		counter++;
	}


	return retData;
}

/*
* FUNCTION : PrepForTransmission
* DESCRIPTION :
*	This function prepares the parsed telemetry data for sending by converting to the dataPacketFormat struct
* PARAMETERS :
*	FlightData data   : The parsed telemetry data
*	unsigned int sequenceNumber : The sequence number of the telemetry data to maintain order
* RETURNS :
*		DataPacket : The prepared telemetry data in the DataPacket format
*/
DataPacket DataManipulation::PrepForTransmission(FlightData data, unsigned int sequenceNumber)
{
	// Variables
	DataPacket packet;

	strcpy(packet.aircraftTailNumber, data.aircraftNumber);																									// Copy the aircraftTailNumber from FlightData data
	packet.packetSequenceNumber = sequenceNumber;																											// Copy the sequenceNumber from the referenced sequence number
	sprintf(packet.aircraftData, "%s,%f,%f,%f,%f,%f,%f,%f", data.dateTime, data.x, data.y, data.z, data.weight, data.alt, data.pitch, data.bank);			// Parse the FlightData into a single string (',' as the delimiter) and save
	packet.checksum = (int)(data.alt + data.pitch + data.bank) / 3;																							// Calculate the checksum and save


	return packet;
}

/*
* FUNCTION : ParseFromInput
* DESCRIPTION :
*	This function parses the raw telemetry data from the file into individual data points
* PARAMETERS :
*	char* filename   : The telemetry data filename
* RETURNS :
*		int : The ret code. 0 is success otherwise is failure
*/
int DataManipulation::ParseFromInput(char* fileName, TCPConnection conn)
{
	// Variables
	FlightData data;
	DataPacket packet;
	FILE* stream = NULL;
	char tempLine[MAX_CHAR] = "";
	bool done = false;
	unsigned int lineNumber = 0;

	// Check that the file connects
	if (NULL == (stream = fopen(fileName, "r")))
	{
		return FAIL_TO_CONNECT;
	}

	// Loop read the file until completion (When the "break;" is hit/the EOF)
	while (done == false)
	{
		// Check that the EOF has not been reached
		if (NULL != fgets(tempLine, MAX_CHAR, stream))
		{
			// Check that the end has been reached but not the EOF
			if (3 < strlen(tempLine))
			{
				lineNumber++;													// Increment the line number to remember the sequence

				data = ParseData(tempLine, fileName);							// Call parsing function to parse into token data
				packet = PrepForTransmission(data, lineNumber);					// Call prep function to prepare the parsed data into the DataPacketFormat to be sent
				Sleep(1000);
				if (0 != conn.SendData(packet))									// Send packet through connection
				{
					return FAIL_TO_SEND;
				}

				memset(tempLine, 0, sizeof(tempLine));							// Clear the string
			}
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

	// Check that the stream was able to close properly
	if (0 != fclose(stream))
	{
		return FAIL_TO_CLOSE;
	}

	return NO_ERRORS;
}