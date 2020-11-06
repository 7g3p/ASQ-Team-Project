#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "wsock32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable: 4996)

#define NO_ERRORS			0
#define FAIL_TO_CONNECT		-1
#define FAIL_TO_READ		-2
#define FAIL_TO_CLOSE		-3
#define FAIL_TO_SEND		-4
#define MAX_CHAR			110
#define INPUT_CHAR			10
#define CHAR_LIM 100

// Namespace
using namespace std;

// Struct
typedef struct DataPacketFormat
{
	char aircraftTailNumber[CHAR_LIM];
	unsigned int packetSequenceNumber;
	char aircraftData[CHAR_LIM];
	int checksum;
}DataPacket;

// Classes
class TCPConnection
{
private:
	string ipAddress;
	int port;
	WSADATA wsa;
	SOCKET sock;
	sockaddr_in server;
	HOSTENT* host;
	fd_set master;
	bool IsConnected;
public:
	TCPConnection();
	~TCPConnection();
	void ConnectToSocket();
	int SendData(DataPacket data);
};

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
	int static ParseFromInput(char* fileName, TCPConnection conn);
};
