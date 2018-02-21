

# Network Library

```
해당 라이브러리의 저작권은 본인에게 있습니다.

C/C++을 통해 개발되었으며, RapidJson과 MySQL C Connector을 사용하였습니다.
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



