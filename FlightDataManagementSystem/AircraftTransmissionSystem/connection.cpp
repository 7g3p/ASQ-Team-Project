#include "genHeader.h"


TCPConnection::TCPConnection()
{
	ipAddress = "127.0.0.1";
	port = 8080;

	// Initialize winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		throw -1;
	}

	// Create a socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		throw -2;
	}

	// Fill in a hint structure
	server.sin_family = AF_INET;
	server.sin_port = 8080; // Used to have htons(port)
	inet_pton(AF_INET, ipAddress.c_str(), &server.sin_addr);

}

TCPConnection::~TCPConnection()
{
	closesocket(sock);
	WSACleanup();
}

void TCPConnection::ConnectToSocket()
{
	// Check that it isn't already connected
	if (IsConnected != true)
	{
		// Connect to server
		if (SOCKET_ERROR == connect(sock, (sockaddr*)&server, sizeof(server)))
		{
			printf("Can't connect to server. Error code: %d", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			IsConnected = false;
			throw - 3;
		}
		else
		{
			IsConnected = true;
		}
	}
}

int TCPConnection::SendData(DataPacket data)
{
	// Variables
	char msg[sizeof(data)] = "";

	sprintf(msg, "%s,%s,%s,%s", data.aircraftTailNumber, to_string(data.packetSequenceNumber).c_str(), data.aircraftData, to_string(data.checksum).c_str());

	if (SOCKET_ERROR == send(sock, msg, sizeof(msg) + 1, 0))
	{
		printf("Failed to send data. Error code: %d", WSAGetLastError());
		return -1;
	}

	return 0;
}