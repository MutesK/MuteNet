

# 네트워크 라이브러리 개발(To Framework Level)

>  라이브러리 저작권은 LICENSE에 따라 Open Source 입니다.
>
> 해당 라이브러리는 자가개발(개발 능력 향상)을 위해 계속 손대고 있는 실험적 (Experience) Level 입니다.
>
> 현재 Windows System에서만 동작가능합니다.

## Complete 

1. Object Pool  TLS is possible to return the smart pointer
   1. 스마트 포인터의 RAII 기능과 자체적인 Reference Counting에 의해 , GC 같은 기능을 하게 됨. 
2. Task Manager 개발 
   1. Task 단위로 동작하는 스레드 풀
3. Mini-Dump
   1. 전에 사용하던 미니덤프 간소화 작업
4. Property  Template Class
   1. 매크로 함수를 줄이기 위해서 만들어졌지만, 선택적 기능.



## TO-DO

- [x] Libs 폴더를 Old Branch로 빼고, Base Develop을 New Branch로 분리
- [ ] Delay Detection 추가
- [ ] 네트워크 파트는 소켓과 소켓 입출력 모드를 Plug-in 하여 처리할수 있는 모양새로 변경
- [ ] Lock - Free Algorithm Windows System Dependency 제거 - 리눅스에서도 사용가능하게 변경
- [ ] Object Pool , Object Pool Windows System Dependency 제거  - 리눅스에서도 사용가능하게 변경
- [ ] ODBC 를 이용한 DB 연결 정책 수용 , ODBC Api를 간편하게 사용가능할수 있는 클래스 제작
- [ ] Foundation 기능 추가 - IPC, Pipe, Frame(Timer), Application Factory
- [ ] 모던 C++ Task 문법 적용한 State-Machine 제공 예정
- [ ] Type-Safe C# Stytle Logger