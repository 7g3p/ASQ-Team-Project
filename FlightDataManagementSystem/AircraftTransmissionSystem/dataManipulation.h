#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

// Structs
typedef struct DataPacketFormat
{
	char* aircraftTailNumber;
	unsigned int packetSequenceNumber;
	char* aircraftData;
	int checksum;
}DataPacket;

using namespace std;


class DataManipulation
{
private:

	typedef struct flightData
	{
		char* aircraftNumber;
		char dateTime[20];
		double x;
		double y;
		double z;
		double weight;
		double alt;
		double pitch;
		double bank;
	}FlightData;

	FlightData static ParseData(char* data, char* aircraftNum);

public:
	DataPacket static PrepForTransmission(FlightData data, unsigned int sequenceNumber);
	int static ParseFromInput(char* fileName);
};

