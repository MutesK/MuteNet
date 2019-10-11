

# 네트워크 라이브러리 개발(To Framework Level)

>  라이브러리 저작권은 LICENSE에 따라 Open Source 입니다.
>
> 해당 라이브러리는 자가개발(개발 능력 향상)을 위해 계속 손대고 있는 실험적 (Experience) Level 입니다.

## To-Do
- [ ] TcpSocket, Socket 클래스 존속 고민
- [ ] AcceptEx, ConnectEx, GetAcceptExSockAddrs 에 대해 해당 함수포인터를 Init할때 사용할 소켓(Service Provider에 종속적인 소켓)에 대한 처리 방법 연구
- [ ] THREAD_SHUTDOWN_FLAG() 를 사용하는 모든 클래스에 대해 Event로 처리하도록 변경. (해당 이벤트는 condition_variable로 생성.)
- [ ] TaskManager에서 사용하는 Task 전달에 대해 Non-Copy로 변경.
- [ ] File 클래스 생성.
