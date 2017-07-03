#include<stdio.h>
#include<winsock2.h>
#include<windows.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}
	SOCKET sClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sClient == INVALID_SOCKET)
	{
		printf("socket failed\n");
		WSACleanup();
		return -1;
	}
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6000);
	addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	char buf[] = "hello I'm client";
	int dwSend = sendto(sClient, buf, sizeof(buf), 0, (sockaddr*)&addrServer, sizeof(sockaddr));
	if (dwSend == 0)
	{
		printf("send failed\n");
		WSACleanup();
		closesocket(sClient);
		return -1;
	}
	printf("send msg[%s] to server\n", buf);
	char recvBuf[1024];
	memset(recvBuf, 0, 1024);
	int serverLen = sizeof(sockaddr);
	int dwRecv = recvfrom(sClient, recvBuf, sizeof(recvBuf), 0, (sockaddr*)&addrServer, &serverLen);
	printf("recv msg [%s] from server\n", recvBuf);
	return 0;
}