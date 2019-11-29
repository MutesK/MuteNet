#pragma once

/*
	Task는 작업의 단위이지만,
	스레드 속성이 들어가 있지않는다.

	Task는 TaskAgent에 의해 만들어지고
	TaskAgent는 TaskManager에 의해 만들어진다.

	Task는 람다를 담는다.

	지금은 중요한 기능만 가지고 있는 뼈밖에 없는 Task 클래스

*/

namespace Util
{
	using DoLambda = std::function<void()>;
	using CompleteNotifyDoLambda = std::function<void()>;

	class Task final
	{
	private:
		DoLambda _DoLambda;
		CompleteNotifyDoLambda _CompleteLambda;

		bool _IsCheckedCompleteLambda{};

		std::string _CallerFunction;
		std::string _CallerFile;
		size_t		_CallerFileLine{};
	public:
		Task(DoLambda doLambda, const char* callerFunction,
			const char* callerFilename, const size_t& callerFileLine);

		Task(DoLambda doLambda, CompleteNotifyDoLambda completeLambda,
			const char* callerFunction, const char* callerFilename,
			const size_t& callerFileLine);

		Task() = default;
		~Task() = default;

		void SetLambda(DoLambda dolamda, const char* callerfunction,
			const char* callerfilename, const size_t& callerfileline);
		void SetCompleteLambda(DoLambda _dolamda, CompleteNotifyDoLambda completelambda,
			const char* callerfunction, const char* callerfilename,
			const size_t& callerfileline);
		bool IsEmptyCompleteLambda() const;

		void Do() const;
		void CompleteDo() const;
	};

}