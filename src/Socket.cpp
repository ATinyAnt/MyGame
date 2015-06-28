#include "Socket.h"


CFightSocket::CFightSocket()
{
	isInit=false;
	memset(heroid,0,sizeof(heroid));
	//memset(heroid,0,sizeof(heroid));
	//memset(&pro,'\0',sizeof(pro));
	//recvMsg=new char[1024];
	//sendMsg=new char[1024];
	//isSetSkill=false;
}

bool CFightSocket::InitSocket(char *ipaddr,int port)
{
	if (isInit)
	{
		return isInit;
	}
	WSADATA Data;  
	  
	SOCKADDR_IN destSockAddr;  
	unsigned long destAddr;  
	int status;  
	//int numsnt;  
	//char *toSendtxt="Test String";  
	//fight_port=port;
// 	memcpy(ip_addr,ipaddr,strlen(ipaddr));
// 	ip_addr[strlen(ipaddr)]='\0';
	status=WSAStartup(MAKEWORD(1, 1), &Data);  

	 
	destAddr=inet_addr(ipaddr);  
	 
	memcpy(&destSockAddr.sin_addr, &destAddr, sizeof(destAddr));  
	  
	destSockAddr.sin_port=htons(port);  
	
	destSockAddr.sin_family=AF_INET;  

	destSocket=socket(AF_INET, SOCK_STREAM, 0);  
	if (destSocket == INVALID_SOCKET)  
	{  
		MessageBox(NULL,"初始化socket失败","消息",MB_OK);
		status=WSACleanup();  
		
		return(1);  
	}  

	unsigned long ul = 1;

	
	status=connect(destSocket, (LPSOCKADDR)&destSockAddr, sizeof(destSockAddr));  
	if (status == SOCKET_ERROR)  
	{ 
		MessageBox(NULL,"不能连接","消息",MB_OK);
		closesocket(destSocket);  
		WSACleanup();  
  
		return(1);  
	} 
	ioctlsocket(destSocket, FIONBIO, (unsigned long*)&ul);
	isInit=true;
	return isInit;
}


void CFightSocket::SendMsg(char * msg)
{
	int len=send(destSocket, msg, strlen(msg),0);
	if (len<=0)
	{
		MessageBox(NULL,"（send）连接丢失","消息",MB_OK);
		closesocket(destSocket);  
		WSACleanup();  
	}
}


//sizeof(msg)这里是4，因为msg是地址
int CFightSocket::RecvMsg(char * msg)
{
	int len=recv(destSocket, msg, 1024, 0);
	if (len>0)
	{
		if (len>1023)
		{
			len=1023;
		}
		msg[len]='\0';
	}
	//当len为0时，即服务端已经关闭
 	if (len==0)
	{
		MessageBox(NULL,"（recv）连接丢失","消息",MB_OK);
		closesocket(destSocket);  
		WSACleanup();
		//PostMessage(m_hSWnd, WM_QUIT, 0, 0);
	}
	return len;
}


void CFightSocket::SendFinghtInfo()
{
	if (sendTime.ReachTime(500) || sendPro.skill!=NOSKILL)
	{
		static Ptotocal pro;
		memcpy(pro.flag,"FIGHTINFO\0",sizeof(pro.flag));
		memcpy(pro.msg,&sendPro,sizeof(sendPro));
		memcpy(sendMsg,&pro,sizeof(pro));
		if (sendPro.skill=='Z')
		{
			memcpy(sendMsg+sizeof(pro),&P_Coord,sizeof(P_Coord));
			//sendlen+=sizeof(P_Coord);

		}
		int len=send(destSocket, sendMsg, sizeof(sendMsg),0);
		if (len<=0)
		{
			MessageBox(NULL,"（send）连接丢失","消息",MB_OK);
			closesocket(destSocket);  
			WSACleanup();  
		}
		sendTime.SetTimenow();
		return;
		//char msg[1024];
		char msg[256]="FIGHTINFO#";
		
		memcpy(sendMsg,&sendPro,sizeof(sendPro));
		sendMsg[sizeof(sendPro)]='\0';
		
		memcpy(msg+sizeof("FIGHTINFO#")-1,sendMsg,sizeof(sendPro));
		//strcat(msg,sendMsg);
		//因为sizeof("FIGHTINFO#"),实际是sizeof("FIGHTINFO#\0"),因此要减1
		int sendlen=sizeof("FIGHTINFO#")+sizeof(sendPro)-1;
		if (sendPro.skill=='Z')
		{
			memcpy(msg+sendlen,&P_Coord,sizeof(P_Coord));
			sendlen+=sizeof(P_Coord);

		}
		len=send(destSocket, msg, sendlen,0);
		if (len<=0)
		{
			MessageBox(NULL,"（send）连接丢失","消息",MB_OK);
			closesocket(destSocket);  
			WSACleanup();  
		}
		sendTime.SetTimenow();
	}
	
}




void CFightSocket::InitProtocal()
{
	memset(&sendPro,NOSKILL,sizeof(sendPro));
	sendPro.skill=NOSKILL;
}


// FightProtocal * CFightSocket::CharToStruct(char *msg)
//{
// 	FightProtocal *temp=new FightProtocal;
// 	memcpy(temp,msg,sizeof(temp));
// 	return temp;
//}

//char * CFightSocket::StructToChar(FightProtocal *pro)
//{
// 	char *temp=new char[1024];
// 	memcpy(temp,pro,sizeof(pro));
// 	return temp;
//}