#pragma once

/*
	Task는 작업의 단위이지만,
	스레드 속성이 들어가 있지않는다.

	Task는 TaskAgent에 의해 만들어지고
	TaskAgent는 TaskManager에 의해 만들어진다.

	Task는 람다를 담는다.

	지금은 중요한 기능만 가지고 있는 뼈밖에 없는 Task 클래스

*/

using dolamda = std::function<void()>;

class Task
{
public:
	Task();
	~Task();

	GET_SET_ATTRIBUTE(dolamda&, work);

	void Work();
private:
	dolamda  _work;

};

