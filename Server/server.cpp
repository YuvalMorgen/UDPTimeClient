#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"


int main() {
	programInit();

	SOCKET m_socket = createSocket();
	executeProgram(m_socket);

	endProgram(m_socket);

	return 0;
}

void programInit() {
	// Initialize Winsock (Windows Sockets).

	// Create a WSADATA object called wsaData.
	// The WSADATA structure contains information about the Windows
	// Sockets implementation.
	WSAData wsaData;

	// Call WSAStartup and return its value as an integer and check for errors.
	// The WSAStartup function initiates the use of WS2_32.DLL by a process.
	// First parameter is the version number 2.2.
	// The WSACleanup function destructs the use of WS2_32.DLL by a process.
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		string msg("Time Server: Error at WSAStartup()\n");
		cout << msg;
	}
}

void executeProgram(SOCKET& m_socket) {
	// Waits for incoming requests from clients.

	// Send and receive data.
	sockaddr client_addr;
	int client_addr_len = sizeof(client_addr);
	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[255];
	char recvBuff[255];

	cout << "Time Server: Wait for clients' requests.\n";

	while (true) {
		bytesRecv = receiveData(m_socket, recvBuff, client_addr, client_addr_len);
		cout << "Time Server: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";

		char request[255], cityName[255];

		readFormatRequest(recvBuff, request, cityName);
		// Answer client's request.
		getOutputByAction(sendBuff, request, cityName);
		bytesSent = sendDataTo(m_socket, sendBuff, client_addr, client_addr_len);
		cout << "Time Server: Sent. " << bytesSent << "\\" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
	}
}

void endProgram(SOCKET& m_socket) {
	// Closing connections and Winsock.
	cout << "Time Server: Closing Connection.\n";
	closesocket(m_socket);
	WSACleanup();
}
