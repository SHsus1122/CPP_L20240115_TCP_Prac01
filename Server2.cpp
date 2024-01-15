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
	// 1. 소켓 연결 준비
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// 2. 소켓 생성 및 설정
	SOCKET ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 3. 소켓 주소 생성 및 설정
	//		- 소켓 주소 생성
	//		- 소켓 초기값 0 으로 초기화
	//		- 주소 체계를 IPv4 로 설정
	//		- IPv4 방식, IP 주소를 "127.0.0.1" 으로, 네트워크 주소 정보 저장
	//		- 소켓 주소의 포트를 "5001" 번으로 지정
	struct sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &(ServerSockAddr.sin_addr.s_addr));
	ServerSockAddr.sin_port = htons(5001);

	// 4. 소켓 연결
	connect(ServerSocket, (struct sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));

	while (true)
	{
		// 5. 정보를 받기 위한 준비 작업
		char Buffer[1024] = { 0, };
		int RecvByte = recv(ServerSocket, Buffer, 1024, 0);
		if (RecvByte <= 0)
		{
			break;
		}

		// 받을 데이터를 앞서 만든 변경된 패킹방식이 적용된 구조체에 담습니다.
		Data Packet;
		memcpy(&Packet, Buffer, sizeof(Packet));

		long long Result = 0;

		cout << Packet.FirstNumber << " ";

		// 6. 받아온 구조체를 통해서 계산을 진행합니다.
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

		// 7. 결과를 담을 배열을 생성하고 계산 결과(Result)를 담아줍니다.
		char Message[8] = { 0, };
		memcpy(Message, &Result, sizeof(Result));

		// 8. 이후 결과를 보냅니다.
		send(ServerSocket, Message, (u_int)sizeof(Message), 0);
	}
	// 9. 소켓 연결 닫기
	closesocket(ServerSocket);

	// 10. Winsock 2 DLL(Ws2_32.dll) 사용 종료, 원속 라이브러리 사용 종료
	WSACleanup();

	return 0;
}