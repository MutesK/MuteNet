#pragma once


namespace Util
{
	class MemoryStream abstract
	{
	protected:
		virtual void Serialize(void* inData, uint32_t inByteCount) = 0;

		template <typename Type>
		void Serialize(Type& Data);

		virtual bool IsInput() = 0;

		std::recursive_mutex   _mutex;
	};

	template <typename Type>
	void MemoryStream::Serialize(Type& Data)
	{
		Serialize(&Data, sizeof(Type));
	}

}
