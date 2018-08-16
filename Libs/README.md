## Lock Free 알고리즘
 > 명시적인 동기화 객체를 사용하지 않고 Thread-Safe한 자료구조를 말합니다.
> 쓰레드간 동기화를 위해 Atomic 연산을 조합해서 구현합니다.
 ### Lock Free를 사용한 이유
 > 락프리 기본 알고리즘은 Busy Waiting이고, Lock이 가능한 조건이 될때까지 Sleep하지 않으며 Context Switching을 거의 하지않게 됩니다.
> Spin Lock과 비슷하지만, 최악의 경우에는 자원소유를 한뒤 해당 스레드가 문맥교환에 들어가버리는 경우, maxtime이 늘어나게 됩니다.
> Lock Free와 Spin Lock을 비교할때, Avg Time에 대해서는 락프리 Avg >= Spin Avg이고, MaxTime인 경우 락프리 Avg <= Spin Avg이게 됩니다.
> 즉 락프리 알고리즘은 Data Race가 많은 곳에서 사용하면 효율적입니다. 하지만 Overall Performance 상은 Spin Lock이 좋습니다.
 ## LanServer
 ```
클라이언트 유저와 서버간의 통신 모듈이 아닌 내부 네트워크의 서버와 서버간의 통신을 목적으로 함.
```
 1. 대규모의 접속을 처리하지않아도 된다.
2. 최대한 간단한 프로토콜을 사용하고, 보안은 신경쓰지 않는다.
3. 간단한 사용법을 가지도록 한다.
4. 서버간의 통신이지만, 클라이언트 서버 모델
5. 유저간 통신 모듈을 사용하며, 유저처럼 서버를 연결하여 유저와 똑같이 처리하는 것도 가능
6. 서버와의 통신은 유저통신과 별도로 분리시키는 것이 관리면, 반응면에서 좋다.
 + 서버간의 통신은 최대한 빠르고, 안전하게 유저수에 영향 받지않고 영향을 주지않는 것이 좋다.
+ 서버간의 통신은 별도의 랜카드를 사용하여 일반 유저접속 IP와는 다르게 가는것이 일반적
+ 유저간 통신 모듈은 덩치가 크며 프로토콜도 무거우므로, 서버간 통신은 프로토콜도 개별적
 ```cpp
	 bool Start(...) 오픈 IP / 포트 / 워커스레드 수 / 나글옵션 / 최대접속자 수
	 void Stop(...)
	 bool Disconnect(ClientID)  / SESSION_ID
	 SendPacket(ClientID, Packet *)   / SESSION_ID
 	virtual void OnClientJoin(Client 정보 / ClientID / 기타등등) = 0;   < Accept 후 접속처리 완료 후 호출.
	virtual void OnClientLeave(ClientID) = 0;   	            < Disconnect 후 호출
	virtual bool OnConnectionRequest(ClientIP,Port) = 0;        < accept 직후 
			return false; 시 클라이언트 거부.
			return true; 시 접속 허용	
 	virtual void OnRecv(ClientID, CPacket *) = 0;              < 패킷 수신 완료 후
	virtual void OnSend(ClientID, int sendsize) = 0;           < 패킷 송신 완료 후
 	virtual void OnWorkerThreadBegin() = 0;                    < 워커스레드 GQCS 바로 하단에서 호출
	virtual void OnWorkerThreadEnd() = 0;                      < 워커스레드 1루프 종료 후
 	virtual void OnError(int errorcode, wchar *) = 0;
 ```
 ## NetServer
 ```
LanServer 와 다른점이란 단지 패킷 송수신 암호화 복호화이며, 일반유저와 접속시키는 클래스.
```
 ## LanServer와 NetServer의 내부 동기화 해결
- 각 플레이어를 담은 공간은 배열을 사용함.
- 세션에 대한 동기화를 IO Count을 통해 누군가가 사용중이라면 릴리즈 될수 없게 만듬.
 ## MMOServer
 ```
본 구조에는 클라이언트의 처리 및 컨텐츠 처리를 위한 스레드가 Auth, Game 2개로 나누어져 있다.
 CMMOServer 클래스는 MMO 게임서버에 특화된 네트워크 클래스로 내부에 게임 컨텐츠 처리를 위한 스레드와, 공정한 패킷 처리 기능을 담고 있다.
다만 기본적으로 CPU 사용률이 높으며, 멀티코어 환경에 효율적이지 않다.
 네트워크 처리 부는 CNetSession에 들어가며, 이를 상속받은 CPlayer에는 각 세션에 대한 게임상의 플레이어 컨텐츠가 들어간다.
 CNetSession = 네트워크 모듈 부
CPlayer = 게임서버 컨텐츠 부
 각 새션은 모드상태 플래그를 가지고있으며, 상속자 처리 가상함수를 통해 콜백하고 있다.
```
 1. 세션의 모드상태
	1. MODE_NONE
	2. MODE_AUTH
	3. MODE_AUTH_TO_GAME
	4. MODE_GAME
	5. MODE_LOGOUT_IN_AUTH
	6. MODE_LOGOUT_IN_GAME
	7. MODE_WAIT_LOGOUT
2. 상속자 처리용 핸들러
```cpp
	virtual	bool		OnAuth_SessionJoin(void) = 0;
	virtual	bool		OnAuth_SessionLeave(bool bToGame = false) = 0;
	virtual	bool		OnAuth_Packet(CPacketBuffer *pPacket) = 0;
	virtual	void		OnAuth_Timeout(void) = 0;
 	virtual	bool		OnGame_SessionJoin(void) = 0;
	virtual	bool		OnGame_SessionLeave(void) = 0;
	virtual	bool		OnGame_Packet(CPacketBuffer *pPacket) = 0;
	virtual	void		OnGame_Timeout(void) = 0;
 	virtual bool		OnGame_SessionRelease(void) = 0;
	virtual void		OnError(int ErrorCode, WCHAR *szStr) = 0;
```
 ```
Auth, Game, Send 스레드 가 항시 루프를 돌기 때문에, CPU 사용율이 높다.
멀티코어, 멀티 CPU에 특화되지 않은 설계
Auth / Game 스레드에 지정된 프레임에 따라 패킷처리속도가 결정됨.
 ```
 ## Dump System
 ```
APIHook을 이용하여 프로그램이 Crash가 발생하였을때 고유 핸들러로 우회시켜서 minidump을 이용하여 풀덤프를 뜨게 만듭니다.
```
 ## System Log
 ```
타입과 로그레벨과 로그메세지와 로그인덱스 그리고 시간을 파일로써 남기게 합니다.
중요한 메세지나 유저가 이상한 행동 등에 대한 로그를 남길수 있습니다.
```
 ## Profiler
 ```
구간별 처리소요 시간을 측정하여 스레드별로 정리하여 파일로 출력해줍니다.
전처리기 플래그를 사용하고 있습니다.
```
 ```cpp
// 프로파일러 체크 플래그는 전처리기로 처리하고 있습니다.
#ifdef PROFILE_CHECK
#define PRO_BEGIN(x)  CProfile::GetInstance()->ProfileBegin(x)
#define PRO_END(x)  CProfile::GetInstance()->ProfileEnd(x)
#define PRO_TEXT(x)	CProfile::GetInstance()->ProfileOutText(x)
#else
#define PRO_BEGIN(x)
#define PRO_END(x)
#define PRO_TEXT(x)	
#endif
```
 ## Packet Buffer
 ```
객체 직렬화 버퍼의 다른말로 연산자 오버로딩을 통해 편하게 넣고 뺄수있게 만들어졌습니다.
현재 메모리풀을 이용하여 패킷풀로 사용하고있습니다.
```
 ## Stream Buffer
 ```
링 버퍼의 다른말로 현재 연속된 패킷을 받아서 처리하게끔 만들어져있습니다.
```
 ## Memory Pool
 ```
Free-List에 가까운 메모리 풀입니다.
Lock Free 구조로 만들어져 있으며, 이를 TLS에 활용하여 사용중입니다.
```
 ## Parser
 ```
하드코딩을 하지않게끔 만들기 위해 만들어졌습니다.
서버 설정 파일을 읽고 돌아갈수 있게 만들어졌습니다.
```
 ## DBConnector
 ```
MySQL Connector을 이용해서 손쉽게 접속하고 실패시 로그를 남기게 해줍니다.
이 또한 빠르게 처리하기 위해 TLS을 활용하고 있습니다.
```
