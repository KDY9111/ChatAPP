#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>

using namespace std;


SOCKADDR_IN addr;

SOCKET sConnect;
struct Buffer
{
	int ID;
	char Message[256];
};

int ClientThread()
{
	Buffer sbuffer;

	char buffer[sizeof(sbuffer)] = {0};

	for(;; Sleep(10))
	{
		if(recv(sConnect, buffer, sizeof(sbuffer), NULL))
		{
			memcpy(&sbuffer, buffer, sizeof(sbuffer));
			cout << "<User " << sbuffer.ID << ":> " << sbuffer.Message <<endl;
		}
	}

	return 0;
}

int main()
{
	system("cls");

	int RetVal = 0;

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2,1);
	RetVal = WSAStartup(DllVersion, &wsaData);
	if(RetVal != 0)
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	sConnect = socket(AF_INET, SOCK_STREAM, NULL);

	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port        = htons(1234);
	addr.sin_family      = AF_INET;

	cout << "Connect to Masterserver? Press the [ENTER] button" <<endl;
	getchar();
	RetVal = connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));

	if(RetVal != 0)
	{
		MessageBoxA(NULL, "Could not connect to server", "Error", MB_OK | MB_ICONERROR);
		main();
	}
	else
	{
		int ID;
		char* cID = new char[64];
		ZeroMemory(cID, 64);

		recv(sConnect, cID, 64, NULL);
		ID = atoi(cID);

		cout << "Connected" <<endl;
		cout << "You are User No: " << ID <<endl;

        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) ClientThread, NULL, NULL, NULL);
		
		for(;; Sleep(10))
		{
			char* buffer = new char[256];
			ZeroMemory(buffer, 256);

			cin >> buffer;
			getchar();
			
			send(sConnect, buffer, 256, NULL);
		}
	}

	return 0;
}