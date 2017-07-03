#include<stdio.h>
#include<winsock2.h>
#include<windows.h>
#pragma comment(lib,"ws2_32.lib")

DWORD WINAPI clientProc(LPARAM lparam);

int main()
{
	WSAData wsa;
	//��ʼ�����绵��
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup failed\n");
		return -1;
	}
	//�����׽���
	SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		printf("socket failed\n");
		WSACleanup();
		closesocket(sServer);
		return -1;
	}
	//������ַ
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(6000);
	addrServ.sin_addr.S_un.S_addr = INADDR_ANY;
	//���׽��ֺ͵�ַ
	int ret = bind(sServer, (sockaddr*)&addrServ, sizeof(sockaddr));
	if (ret == SOCKET_ERROR)
	{
		printf("bind failed\n");
		WSACleanup();
		closesocket(sServer);
		return -1;
	}
	//����󶨳ɹ������м���
	ret = listen(sServer, 5);
	if (ret == SOCKET_ERROR)
	{
		printf("listen failed\n");
		WSACleanup();
		closesocket(sServer);
		return -1;
	}
	//�����ɹ�
	SOCKADDR_IN addrClient;
	int addrClientLen = sizeof(sockaddr_in);
	while (TRUE)
	{
		SOCKET *sClient = new SOCKET;
		*sClient = accept(sServer, (sockaddr*)&addrClient, &addrClientLen);
		if (INVALID_SOCKET == *sClient)
		{
			printf("accept failed\n");
			WSACleanup();
			closesocket(sServer);
			return -1;
		}
		CreateThread(0, 0, LPTHREAD_START_ROUTINE(clientProc), (LPVOID)*sClient, 0, 0);
	}
	closesocket(sServer);
	WSACleanup();

	return 0;
}
DWORD WINAPI clientProc(LPARAM lparam)
{
	SOCKET sockClient = (SOCKET)lparam;
	char buf[1024];
	while (TRUE)
	{
		memset(buf, 0, 1024);
		int ret = recv(sockClient, buf, sizeof(buf), 0);
		if (ret == SOCKET_ERROR)
		{
			printf("recv failed\n");
			closesocket(sockClient);
			return -1;
		}
		if (ret == 0)
		{
			printf("client close the connection\n");
			closesocket(sockClient);
			return -1;
		}
		printf("�յ��ͻ��˷�������Ϣ[%s]\n", buf);
		ret = send(sockClient, buf, sizeof(buf), 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send failed\n");
			closesocket(sockClient);
			return -1;
		}
		closesocket(sockClient);
		return 0;
	}
}