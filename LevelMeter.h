#pragma once

#include <list>
using namespace std;

#define ON_WM_LEVEL_MSG(memberFxn)  ON_REGISTERED_MESSAGE(CLevelMeter::m_nDefaultMsgID,memberFxn)

// Message for Level Meter
struct LevelMsg
{
    time_t      m_time;                     // Time  for Level Meter
    UINT        m_LevelID;                  // ID    for Level Meter
    float       m_LevelVal;                 // Value for Level Meter
};


// Class for Level Meter
class CLevelMeter : public CObject
{
public:
    CLevelMeter() : m_hwndDest(NULL), m_nMsgID(WM_NULL), m_lParam(0), m_nExit(FALSE)
    {
        InitializeCriticalSection(&m_hLock);
    }
    virtual ~CLevelMeter()
    {
        Stop();
    }

public:
    void Start(UINT nPort, HWND hwndDest = NULL, UINT nMsgID = WM_NULL, LPARAM lParam = 0);
    void Stop(void)
    {
        ::EnterCriticalSection(&m_hLock);
        StopSocketThread();
        ::LeaveCriticalSection(&m_hLock);
    }
protected:
    void StopSocketThread(void);

protected:                                  // Thread Function for Level connection
    DWORD               ProcListen(void);   
    static DWORD WINAPI ProcListen(LPVOID lpArg);
protected:                                  // Thread Function for Level Meter
    DWORD               ProcMeter(SOCKET sockConn);    
    static DWORD WINAPI ProcMeter(LPVOID lpArg);

protected:
    list<SOCKET>        m_lstSocket;        // Socket Arry for Level Meter
    CRITICAL_SECTION    m_hLock;
    int                 m_nExit;            // Socket Server Exit?
public:
    static const UINT   m_nDefaultMsgID;    // MsgID for Level notification(Default)
protected:
    UINT                m_nMsgID;           // MsgID for Level notification
    LPARAM              m_lParam;
    HWND                m_hwndDest;         // HWND  for Level notification
    UINT                m_nPort;            // Socket Server Listen Port

protected:
    typedef list<SOCKET>::iterator SOCKET_ITR;
protected:
    SOCKET_ITR ListInsert(const SOCKET& x)
    {
        ::EnterCriticalSection(&m_hLock);
        SOCKET_ITR itr = m_lstSocket.insert(m_lstSocket.begin(), x);
        ::LeaveCriticalSection(&m_hLock);
        return( itr );
    }
    void ListRemove(SOCKET_ITR itr)
    {
        ::EnterCriticalSection(&m_hLock);
        m_lstSocket.erase(itr);
        ::LeaveCriticalSection(&m_hLock);
    }
    BOOL SendMsg(LevelMsg *msg)
    {
        if(m_hwndDest != NULL) {
            return( ::PostMessage(m_hwndDest, m_nMsgID, WPARAM(msg), LPARAM(m_lParam)) );
        }
        return( FALSE );
    }

protected:
    char DataCheck(char *buff)
    {                                   // 数据校验
        return( buff[0] + buff[1] + buff[2] + buff[3] + buff[4] );
    }
    void DataCmd1(char *buff)
    {
        buff[0] = (char)0xFC;
        buff[1] = (char)0x01;
        buff[2] = (char)0xB2;           // 传感器工作指令
        buff[3] = (char)0x00;
        buff[4] = (char)0x00;
        buff[5] = DataCheck(buff);
    }
    void DataCmd2(char *buff)
    {
        buff[0] = (char)0xFC;
        buff[1] = (char)0x01;
        buff[2] = (char)0xC3;           // 传感器回传指令
        buff[3] = (char)0x00;
        buff[4] = (char)0x00;
        buff[5] = DataCheck(buff);
    }
    float DataRev1(char *buff)
    {
        if(   (buff[0] != (char)0xFC) 
            | (buff[1] != (char)0x01)
            | (buff[2] != (char)0xD4)   // 传感器回传信息
            | (buff[5] != DataCheck(buff))
          )
        {
            return( -1 );
        }
        return( ((UINT)buff[3] * 100 + (UINT)buff[4]) / (float)1000 );
    }
};

