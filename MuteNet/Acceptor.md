# AcceptEx 고민

1. 소켓 재사용성이 들어가면서 소켓의 TIME_WAIT을 대비해야 됨.
2. 지속적으로 AcceptEx 호출해줌으로써 Server Handle 에게 Accept IO Complete Callback을 주게 할것인지. 아니면, Server Handle에서 Enable, Disable 함수를 두어서 수동적으로 On, Off 를 주게 할것인지. 등에 대한 동작 고민.
3. 소켓 풀의 고민. 단순히 reactor -> proactor로 전환하여 사용할것인지. 일정 수 Socket Pool 생성후, 그 Socket Pool Size 만큼만 사용하게 할것인지?

> 2,3 번은 Flag Option 처리로 빼게 한다.

> Acceptor::Listen 함수에  Work Mechanism Flag (MANUAL_MODE, AUTOMATIC_MODE), Using Receive Packet Mode Flag (RECEIVE_PACKET, NORMAL ), Socket Array Size, void* AdditionalKey

## Work Mechanism Flag

> AUTOMATIC Mode의 경우엔 Socket Array Size 정의는 필수로 하자. Automatic 모드는, 예전에 내가 작업한 IOCP 서버처럼 Array Session 처럼 Accept 동작
하도록 한다. Automatic Mode의 경우, 소켓 재사용성이 불가능하도록 설계.

> 매뉴얼 모드는 Server Handle로 부터 요청이 오지 않은 한은, 계속 AcceptEx 호출을 하게 하고, 이때에는 소켓 재사용성을 사용한다. 
최대한  TIME_WAIT이 발생하지 않도록 만들어야됨 !!.

## Using Receive Packet Mode Flag

> AcceptEx에서는 수신과 동시에, Packet을 Recv할수 있음. 

> NORMAL 모드에서는, 해당 패킷을 받지 않는다. RECEIVE_PACKET에서는 해당 패킷을 받아서 처리한다.

> RECEIVE_PACKET에서 처리할려면 Accept OVERLAPPED 구조체를 확장하고, 안에 Buffer를 만들어야됨. -> Callback은 Link::Callback::OnReceivedData로 하도록 하자.

# TIME_WAIT 

> 보통 TIME_WAIT 상태는, Active Close Side에서 발생한다. lingeroption으로 임시로 TIME_WAIT 풀수는 있지만, 안전하지 않음

> 내 기억으로는 예전에 이걸 처리하기 위해서, Session의 IOCount가 0보다 크고, Recv Size 를 0을 받았을때, Shutdown 처리를 해준뒤, IO Completion이 해당 Session 으로 오면서, IOCount가 점점 줄고 == 0이 되면 날렸었음.

> Recv를 받아서 종료되는식이 된다면, Active Close Side가 Client가 된다. 이후 TIME_WAIT 상태에서 클라이언트가 정상적으로 패킷을 전송해준다고 해도, 이는 이미 서버에서는 무시한다. -> (이미 Shutdown을 통해 IO는 막혀있고 남아있는 Send Buffer는 알아서 다 보낸다.)

-> Shutdown을 통해 소켓 참조 카운터를 무시하고 TCP 종료 프로세스를 먼저 타게 하고, 나중에 closesocket을 할때 TIME_WAIT이 안남게 하면될듯.

[유령 세션](https://ozt88.tistory.com/19)
[closesocket, shutdown](https://pencil1031.tistory.com/95)
