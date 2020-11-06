/*
* FILE : connection.cpp
* PROJECT : SENG3020 - Milestone #2
* PROGRAMMER : Stephen Perrin, Kieran Campbell, Zena Potts, Karson Lai
* FIRST VERSION : 2020-11-05
* DESCRIPTION :
* This file contains the methods for the TCPConnection class.
*/
#include "genHeader.h"


/*
* FUNCTION : TCPConnection
* DESCRIPTION :
*	This method is the constructor method that runs on object instantiation.
* PARAMETERS :
*	N/A
* RETURNS :
*	N/A
*/
TCPConnection::TCPConnection()
{
	// Instatiate the connection address and port
	ipAddress = "127.0.0.1";
	port = 8080;

	// Initialize winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		throw - 1;
	}

	// Create a socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		throw - 2;
	}

	// Fill in a hint structure
	server.sin_family = AF_INET;
	server.sin_port = htons(54000); // Used to have htons(port)
	server.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(sock, (sockaddr*)&server, sizeof(server));

	listen(sock, SOMAXCONN);

	FD_ZERO(&master);

	FD_SET(sock, &master);
}

/*
* FUNCTION : ~TCPConnection
* DESCRIPTION :
*	This method is the destructor method that runs on object disposal.
* PARAMETERS :
*	N/A
* RETURNS :
*	N/A
*/
TCPConnection::~TCPConnection()
{
	closesocket(sock);
	WSACleanup();
}

/*
* FUNCTION : ConnectToSocket
* DESCRIPTION :
*	This method connects to the previously instatiated socket and waits for a connection.
* PARAMETERS :
*	N/A
* RETURNS :
*	N/A
*/
void TCPConnection::ConnectToSocket()
{
	// Variables
	SOCKET client = NULL;

	// Check if a connection has been made. If not, loop until a connection is made
	while (IsConnected == false)
	{
		if (INVALID_SOCKET != (client = accept(sock, nullptr, nullptr)))
		{
			IsConnected = true;
			FD_SET(client, &master);
		}
	}
}

/*
* FUNCTION : SendData(DataPacket data)
* DESCRIPTION :
*	This method converts the prepared data into a data string and sends it over the connection to the client
* PARAMETERS :
*		DataPacket data : The prepared telemetry and aircraft data to be sent
* RETURNS :
*	N/A
*/
int TCPConnection::SendData(DataPacket data)
{
	// Variables
	char msg[sizeof(data)] = "";

	// Converts into a string with ',' delimiters
	sprintf(msg, "%s,%s,%s,%s", data.aircraftTailNumber, to_string(data.packetSequenceNumber).c_str(), data.aircraftData, to_string(data.checksum).c_str());

	// Checks if it was able to send properly else notify the user and return a fail code
	if (SOCKET_ERROR == send(master.fd_array[1], msg, sizeof(msg) + 1, 0))
	{
		printf("Failed to send data. Error code: %d", WSAGetLastError());
		return -1;
	}

	return 0;
}