#include "pch.h"
#include "Acceptor.h"
#include "ServiceListener.h"

/*
 * AcceptEx을 통해 미리 소켓을 N개 만큼 만들고, .Post해둔다. 
 * 연결 요청이 들어오면 GQCS를 통해 완료 통지를 해준다.
 * 
 * Accept 
 *
 *
 *	1. Listen 소켓 생성, IOCP 연결
 *	 - Listen 함수 호출 전, 소켓 옵션 SO_CONDITIONAL_ACCEPT 수행 한다. -> AcceptEx()가 동작중이지 않아도 Listen이 발생하지 않음.
 *  2. AcceptEx 통해 Listen Pool 생성
 *  3. Accept 과정 수행.
 *
 */
namespace MuteNet
{

}
