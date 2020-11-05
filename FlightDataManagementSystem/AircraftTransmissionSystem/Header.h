#include <iostream>
#include <string.h>
#include <stdio.h>

// Constants
const int MAX_CHAR = 110;

// Global Structs
typedef struct DataPacketFormat
{
	char* aircraftTailNumber;
	unsigned int packetSequenceNumber;
	char* aircraftData;
	int checksum;
}DataPacket;