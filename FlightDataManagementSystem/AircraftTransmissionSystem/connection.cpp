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
	server.sin_port = htons(54000); // Used to have htons(port)
	server.sin_addr.S_un.S_addr = INADDR_ANY;
	
	bind(sock, (sockaddr*)&server, sizeof(server));

	listen(sock, SOMAXCONN);

	FD_ZERO(&master);

	FD_SET(sock, &master);
}

TCPConnection::~TCPConnection()
{
	closesocket(sock);
	WSACleanup();
}

void TCPConnection::ConnectToSocket()
{
	// Variables
	SOCKET client = NULL;

	while (IsConnected == false)
	{
		if (INVALID_SOCKET != (client = accept(sock, nullptr, nullptr)))
		{
			IsConnected = true;
			FD_SET(client, &master);
		}
	}
}

int TCPConnection::SendData(DataPacket data)
{
	// Variables
	char msg[sizeof(data)] = "";

	sprintf(msg, "%s,%s,%s,%s", data.aircraftTailNumber, to_string(data.packetSequenceNumber).c_str(), data.aircraftData, to_string(data.checksum).c_str());

	if (SOCKET_ERROR == send(master.fd_array[1], msg, sizeof(msg) + 1, 0))
	{
		printf("Failed to send data. Error code: %d", WSAGetLastError());
		return -1;
	}

	return 0;
}