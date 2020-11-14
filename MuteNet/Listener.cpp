#include "pch.h"
#include "Acceptor.h"
#include "ServiceListener.h"

/*
 * AcceptEx�� ���� �̸� ������ N�� ��ŭ �����, .Post�صд�. 
 * ���� ��û�� ������ GQCS�� ���� �Ϸ� ������ ���ش�.
 * 
 * Accept 
 *
 *
 *	1. Listen ���� ����, IOCP ����
 *	 - Listen �Լ� ȣ�� ��, ���� �ɼ� SO_CONDITIONAL_ACCEPT ���� �Ѵ�. -> AcceptEx()�� ���������� �ʾƵ� Listen�� �߻����� ����.
 *  2. AcceptEx ���� Listen Pool ����
 *  3. Accept ���� ����.
 *
 */
namespace MuteNet
{

}
