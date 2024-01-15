#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32")
#pragma pack(push, 1)

typedef struct _Data
{
	int FirstNumber;
	int SecondNumber;
	char Operator;
} Data;

typedef struct _Header
{
	u_short Code;
	u_short Size;
} Header;

#pragma pack(pop)

int main()
{
	// 1. ���� ���� �غ�
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 2. ���� ���� �� ����
	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 3. ���� �ּ� ���� �� ����
	//		- ���� �ּ� ����
	//		- ���� �ʱⰪ 0 ���� �ʱ�ȭ
	//		- �ּ� ü�踦 IPv4 �� ����
	//		- IPv4 ���, IP �ּҸ� "127.0.0.1" ����, ��Ʈ��ũ �ּ� ���� ����
	//		- ���� �ּ��� ��Ʈ�� "5001" ������ ����
	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(ServerSockAddr.sin_addr.s_addr));
	ServerSockAddr.sin_port = htons(5001);

	// 4. ���� ����
	connect(ServerSocket, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));

	while (true)
	{
		// 5. ������ �ޱ� ���� �غ� �۾�
		char Buffer[1024] = { 0, };
		int RecvByte = recv(ServerSocket, Buffer, 1024, 0);
		if (RecvByte <= 0)
		{
			break;
		}

		// ���� �����͸� �ռ� ���� ����� ��ŷ����� ����� ����ü�� ����ϴ�.
		Data Packet;
		memcpy(&Packet, Buffer, sizeof(Packet));

		long long Result = 0;

		cout << Packet.FirstNumber << " ";

		// 6. �޾ƿ� ����ü�� ���ؼ� ����� �����մϴ�.
		switch (Packet.Operator)
		{
		case 0:
			Result = Packet.FirstNumber + Packet.SecondNumber;
			cout << " + ";
			break;
		case 1:
			Result = Packet.FirstNumber - Packet.SecondNumber;
			cout << " - ";
			break;
		case 2:
			Result = Packet.FirstNumber * Packet.SecondNumber;
			cout << " * ";
			break;
		case 3:
			Result = Packet.FirstNumber / Packet.SecondNumber;
			cout << " / ";
			break;
		case 4:
			Result = Packet.FirstNumber % Packet.SecondNumber;
			cout << " % ";
			break;
		default:
			Result = Packet.FirstNumber + Packet.SecondNumber;
			cout << " + ";
			break;
		}
		cout << Packet.SecondNumber << " = ";
		cout << Result << endl;

		// 7. ����� ���� �迭�� �����ϰ� ��� ���(Result)�� ����ݴϴ�.
		char Message[8] = { 0, };
		memcpy(Message, &Result, sizeof(Result));

		// 8. ���� ����� �����ϴ�.
		send(ServerSocket, Message, (u_int)sizeof(Message), 0);
	}
	// 9. ���� ���� �ݱ�
	closesocket(ServerSocket);

	// 10. Winsock 2 DLL(Ws2_32.dll) ��� ����, ���� ���̺귯�� ��� ����
	WSACleanup();

	return 0;
}