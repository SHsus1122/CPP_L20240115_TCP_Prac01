#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32")

using namespace std;

int main()
{
	// 1. Socket ���� �۾�
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (Result != 0)
	{
		cout << "Winsock dll error : " << GetLastError() << endl;
		exit(-1);
	}

	// 2. ���� ����
	SOCKET ListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "INVALID SOCKET : " << GetLastError() << endl;
		exit(-1);
	}

	// 3. ���� �ּҰ� ���� �� �ʱ�ȭ
	SOCKADDR_IN ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));

	// 4. ���� �ּ� �� ����
	// AF_INET6 �� IPv4 �� IPv6 ��� ��� �����մϴ�.
	ListenSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET6, "127.0.0.1", &(ListenSockAddr.sin_addr.s_addr));
	ListenSockAddr.sin_port = htons(5001);

	// 5. ���� ���� �۾�
	Result = bind(ListenSocket, (SOCKADDR*)&ListenSockAddr, sizeof(ListenSockAddr));
	if (Result == SOCKET_ERROR)
	{
		cout << "can't bind : " << GetLastError() << endl;
		exit(-1);
	}

	// 6. ���� ���� ����
	Result = listen(ListenSocket, 5);
	if (Result == SOCKET_ERROR)
	{
		cout << "can't listen : " << GetLastError() << endl;
		exit(-1);
	}

	// 7. Ŭ���̾�Ʈ�� ���� �ּ� ���� �� �ʱ�ȭ
	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSockAddrLength = sizeof(ClientSockAddr);

	// 8. Ŭ���̾�Ʈ ���� ���� �� ���� �۾�
	SOCKET ClientSocket = accept(ListenSocket, (SOCKADDR*)&ClientSockAddr, &ClientSockAddrLength);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accpet fail : " << GetLastError() << endl;
		exit(-1);
	}

	// 9. ������ ����(Buffer) �ۼ�
	const char Message[] = "show me the money.";

	// 10. ������ �۾� ����
	int SentByte = send(ClientSocket, Message, (int)strlen(Message) + 1, 0);
	if (SentByte < 0)
	{
		cout << "Error : " << GetLastError() << endl;
		exit(-1);
	}
	else if (SentByte == 0)
	{
		cout << "Disconnected : " << GetLastError() << endl;
		exit(-1);
	}
	else
	{
		cout << "Sent Byte : " << SentByte << ", " << Message << endl;
	}

	// 11. �޴� ����(Buffer) �ۼ� �� �۾� ����
	char RecvBuffer[1024] = { 0, };
	int RecvByte = recv(ClientSocket, RecvBuffer, sizeof(RecvBuffer), 0);
	if (RecvByte < 0)
	{
		cout << "Error : " << GetLastError() << endl;
		exit(-1);
	}
	else if (RecvByte == 0)
	{
		cout << "Disconnected : " << GetLastError() << endl;
		exit(-1);
	}
	else
	{
		cout << "Recv Byte : " << RecvByte << ", " << RecvBuffer << endl;
	}

	// 12. ���� ���� �� ����
	closesocket(ClientSocket);
	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}