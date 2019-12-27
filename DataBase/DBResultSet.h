#pragma once

namespace Util
{
	namespace DB
	{
		class DBResultSet abstract
		{
		public:
			DBResultSet() =default;
			virtual ~DBResultSet() = default;

			void GetNextResultSet();
			void GetNext();
			void Skip();

			virtual void BindImpl() = 0;
			virtual void ResetImpl() = 0;
			virtual bool NextResultSetImpl() = 0;
			virtual bool NextImpl() = 0;
			
			virtual void Bind(uint8_t& OUT value) = 0;
			virtual void Bind(uint16_t& OUT value) = 0;
			virtual void Bind(uint32_t& OUT value) = 0;
			virtual void Bind(uint64_t& OUT value) = 0;
			virtual void Bind(int8_t& OUT value) = 0;
			virtual void Bind(int16_t& OUT value) = 0;
			virtual void Bind(int32_t& OUT value) = 0;
			virtual void Bind(int64_t& OUT value) = 0;
			virtual void Bind(double& OUT value) = 0;
			virtual void Bind(char* OUT value, uint32_t& OUT length, uint32_t maxLength) = 0;

			virtual void GetResult(int16_t& OUT result) = 0;
			virtual uint64_t GetNumberOfRows() const = 0;
 		};
	}
}
