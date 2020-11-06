#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "wsock32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "dataManipulation.h"

// Structs


// Class
class TCPConnection
{
private:
	string ipAddress;
	int port;
	WSADATA wsa;
	SOCKET sock;
	sockaddr_in server;
	HOSTENT* host;
	bool IsConnected;
public:
	TCPConnection();
	~TCPConnection();
	void ConnectToSocket();
	int SendData(DataPacket data);

};