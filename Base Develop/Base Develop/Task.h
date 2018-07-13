#pragma once

/*
	Task�� �۾��� ����������,
	������ �Ӽ��� �� �����ʴ´�.

	Task�� TaskAgent�� ���� ���������
	TaskAgent�� TaskManager�� ���� ���������.

	Task�� ���ٸ� ��´�.

	������ �߿��� ��ɸ� ������ �ִ� ���ۿ� ���� Task Ŭ����

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

