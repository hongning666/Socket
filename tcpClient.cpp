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
		WSACleanup();
		return -1;
	}
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		printf("socket failed\n");
		WSACleanup();
		return -1;
	}
	SOCKADDR_IN addrClient;
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(6000);
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int ret = connect(sClient, (sockaddr*)&addrClient, sizeof(sockaddr));
	if (ret == SOCKET_ERROR)
	{
		printf("connect failed\n");
		WSACleanup();
		closesocket(sClient);
		return -1;
	}
	char szBuf[1024];
	memset(szBuf, 0, 1024);
	sprintf(szBuf, "Hello World");
	ret = send(sClient, szBuf, sizeof(szBuf), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("send failed\n");
		WSACleanup();
		closesocket(sClient);
		return -1;
	}
	ret = recv(sClient, szBuf, sizeof(szBuf), 0);
	if (ret == SOCKET_ERROR)
	{
		printf("recv failed\n");
		WSACleanup();
		closesocket(sClient);
		return -1;
	}
	printf("接收到消息[%s]\n", szBuf);
	return 0;
}