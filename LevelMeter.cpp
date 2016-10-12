// LevelMeter.cpp : 实现文件
//

#include "stdafx.h"
#include <time.h>
#include <exception>
#include "LevelMeter.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// CLevelMeter

const UINT CLevelMeter::m_nDefaultMsgID = ::RegisterWindowMessage(_T("CLevelMeter_DefaultMsgID"));

// 开启Socket服务器
void CLevelMeter::Start(UINT nPort, HWND hwndDest, UINT nMsgID, LPARAM lParam)
{
    m_nPort = nPort;
    m_hwndDest = hwndDest;
    m_nMsgID = (nMsgID == WM_NULL) ? m_nDefaultMsgID : nMsgID;
    m_lParam = lParam;
    m_nExit  = FALSE;

    DWORD dwThreadId = 0;
    HANDLE hThread = ::CreateThread(0,0,ProcListen,LPVOID(this),0,&dwThreadId);
    if( dwThreadId == 0 ) {
        throw runtime_error("Socket Server Thread Create Faild!");
    }
}

// 关闭Socket并退出其线程
void CLevelMeter::StopSocketThread(void)
{
    m_nExit = TRUE;
    for( SOCKET_ITR i = m_lstSocket.begin(); i != m_lstSocket.end(); ++i )
    {
        shutdown(*i, SD_BOTH);
        Sleep(20);
    }
}

// Thread Function for Level connection
DWORD WINAPI CLevelMeter::ProcListen(LPVOID lpArg)
{
    CLevelMeter* pThis = reinterpret_cast<CLevelMeter*>(lpArg);
    return pThis->ProcListen();
}
DWORD CLevelMeter::ProcListen(void)
{
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);   // 创建Socket
    if( INVALID_SOCKET == sockSrv ) {
        throw runtime_error("Socket Create Faild!");
    }

    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrSrv.sin_family           = AF_INET;
    addrSrv.sin_port             = htons(m_nPort);      // 绑定Port
    if( SOCKET_ERROR == bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) ) {
        throw runtime_error("Socket Bind Faild!");
    }

    if( SOCKET_ERROR == listen(sockSrv, 5) ) {          // 监听
        throw runtime_error("Socket Listen Faild!");
    }

    SOCKET_ITR itrSock = ListInsert(sockSrv);           // 插入Socket队列

    while( !m_nExit )
    {
        SOCKADDR_IN addrClient;
        int len = sizeof(SOCKADDR);                     // 接受客户端连接
        SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
        if( INVALID_SOCKET == sockConn ) {
            continue;
        }

        LPVOID* lpVoid = new LPVOID[2];
        lpVoid[0] = reinterpret_cast<LPVOID>(this);
        lpVoid[1] = reinterpret_cast<LPVOID>(sockConn);
        DWORD dwThreadId = 0;                           // 创建接收线程
        HANDLE hThread = ::CreateThread(0,0,ProcMeter,LPVOID(lpVoid),0,&dwThreadId);
        if( dwThreadId == 0 ) {
            throw runtime_error("Socket Receive Thread Create Faild!");
        }
    }

    ListRemove(itrSock);                                // 从Socket队列删除
    closesocket(sockSrv);
    return( 0 );
}


// Thread Function for Level Meter
DWORD WINAPI CLevelMeter::ProcMeter(LPVOID lpArg)
{
    LPVOID*      lpVoid   = reinterpret_cast<LPVOID*>(lpArg);
    CLevelMeter* pThis    = reinterpret_cast<CLevelMeter*>(lpVoid[0]);
    SOCKET       sockConn = reinterpret_cast<SOCKET>(lpVoid[1]);
    delete[]     lpVoid;
    return pThis->ProcMeter(sockConn);
}
DWORD CLevelMeter::ProcMeter(SOCKET sockConn)
{
    char sendBuf[6];
    char recvBuf[256];
    SOCKET_ITR itrSock = ListInsert(sockConn);  // 插入Socket队列

    int timeout = 10000;
    if( setsockopt(sockConn,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout)) ) {
        goto RETURN;                            // 设置Socket接收超时
    }

    if( recv(sockConn, recvBuf, sizeof(recvBuf), 0) <= 0 ) {
        goto RETURN;                            // 接收注册数据
    }
    if( strncmp(recvBuf, "液位计编号", 10) != 0 ) {
        goto RETURN;
    }
    UINT nLevelID = recvBuf[10] - '0';
    float level;

    for(; !m_nExit;)
    {
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        DataCmd1(sendBuf);                  // 发送命令1
        if( send(sockConn, sendBuf, sizeof(sendBuf), 0) != sizeof(sendBuf) ) {
            break;
        }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }

        DataCmd2(sendBuf);                  // 发送命令2
        if( send(sockConn, sendBuf, sizeof(sendBuf), 0) != sizeof(sendBuf) ) {
            break;
        }

        if( recv(sockConn, recvBuf, sizeof(recvBuf), 0) <= 0 ) {
            break;                          // 接收传感器回传数据
        }
        if( (level = DataRev1(recvBuf)) < 0 ) {
            continue;                       // 传感器回传数据错误
        }

        LevelMsg* msg = new LevelMsg;
        time(&msg->m_time);
        msg->m_LevelID  = nLevelID;
        msg->m_LevelVal = level;
        if( !SendMsg(msg) ) { delete msg; } // 发送液位消息到窗口
        
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
        Sleep(10);  if( m_nExit ) { break; }    Sleep(10);  if( m_nExit ) { break; }
    }

  RETURN:
    ListRemove(itrSock);                        // 从Socket队列删除
    closesocket(sockConn);
    return( 0 );
}

