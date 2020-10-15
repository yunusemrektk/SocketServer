#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment(lib,"ws2_32.lib")

using namespace std;
void sendProduct();
void uploadProduct();

void main()
{
	//Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);

	if (wsOk != 0) {
		cerr << "Can't Initialize winsock!" << endl;
		WSACleanup();
		return;
	}

	//Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr << "Can't create a socket! " << endl;
		WSACleanup();
		return;
	}

	//Bind 
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(9900);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	int iResult = bind(listening, (sockaddr*)&hint, sizeof(hint));
	if (iResult == SOCKET_ERROR) {
		cout << "Bind error!" << endl;
		WSACleanup();
		return;
	}

	//Listen
	iResult= listen(listening, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		cout << "Bind error!" << endl;
		WSACleanup();
		return;
	}

	//Wait connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];
	char service[NI_MAXHOST];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		cout << host << " connected on port " << service << endl;

	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	//Close listtening
	closesocket(listening);

	//Accpet and echo message

	string message = "Hello from server!\r\n";
	int bytesReceived;
	int sendResult;

	
	do{
		vector<char> buf(4096);

		buf.clear();
		bytesReceived = recv(clientSocket, buf.data(), 4096, 0);
		if (bytesReceived > 0) {
			const char *sendCmd = "SEND";
			const char* getCmd = "UPLOAD";

			cout << buf.data() << endl;

	
			if (strcmp(buf.data(),sendCmd)==0) {

				sendResult = send(clientSocket, message.c_str(), (int)strlen(message.c_str()), 0);
				if (sendResult == SOCKET_ERROR) {
					cout << "send failed !" << endl;
					closesocket(clientSocket);
					WSACleanup();
					return;
				}

				sendProduct();
			}
			else if (strcmp(buf.data(),getCmd)==0) {
				uploadProduct();
			}


		}
		else if (bytesReceived == 0) {
			cout << "Client disconnected" << endl;
			return;
			}
		else {
			cout << "Error in recv!" << endl;
			closesocket(clientSocket);
			WSACleanup();
			return;
		}
	

	} while (bytesReceived > 0);

	sendResult = shutdown(clientSocket, SD_SEND);
	if (sendResult == SOCKET_ERROR) {
		closesocket(clientSocket);
		WSACleanup();
		return;
	}

	closesocket(clientSocket);
	WSACleanup();


}

void sendProduct() {
	cout << "SEND FUNC CALLED" << endl;

}

void uploadProduct() {
	cout << "GET FUNC CALLED" << endl;
}