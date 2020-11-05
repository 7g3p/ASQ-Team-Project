#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable: 4996)

#define NO_ERRORS			0
#define FAIL_TO_CONNECT		-1
#define FAIL_TO_READ		-2
#define FAIL_TO_CLOSE		-3
#define MAX_CHAR			100

// Namespace
using namespace std;

// Global Structs
typedef struct DataPacketFormat
{
	char* aircraftTailNumber;
	unsigned int packetSequenceNumber;
	char* aircraftData;
	int checksum;
}DataPacket;