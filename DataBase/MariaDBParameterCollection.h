#pragma once

namespace Util
{
	namespace DB
	{
		struct MariaDBParameterInformation
		{
			my_bool is_null;
			unsigned long length;
			my_bool error;
		};

		constexpr uint32_t MaxParameter = 128;

		class MariaDBParameterCollection abstract
		{
		protected:
			std::vector<MariaDBParameterInformation> _BindingInformations;
			MYSQL_BIND*					 _pArrParameters;
			uint32_t _numberOfParameters;
			uint32_t _paramterPosition;
		public:
			MariaDBParameterCollection();
			virtual ~MariaDBParameterCollection();

			void SetNumberOfParameter(const uint32_t numberOfParameters);
			void Reset();
			void AddParameter(const enum_field_types type,
				void* OUT pBuffer, uint32_t* const OUT pOutLength,
				const uint32_t length, const uint32_t maxlength,
				const bool isunsigned);
		};

		inline void MariaDBParameterCollection::SetNumberOfParameter(const uint32_t numberOfParameters)
		{
			_numberOfParameters = numberOfParameters;
		}

		inline void MariaDBParameterCollection::Reset()
		{
			_paramterPosition = 0;
		}

		class MariaDBInputParameterCollection final : public MariaDBParameterCollection
		{
		public:
			MariaDBInputParameterCollection() = default;
			virtual ~MariaDBInputParameterCollection() = default;

			void Bind(MYSQL_STMT* const pStmt);
		};

		class MariaDBOutputParameterCollection final : public MariaDBParameterCollection
		{
		public:
			MariaDBOutputParameterCollection() = default;
			virtual ~MariaDBOutputParameterCollection() = default;

			void Bind(MYSQL_STMT* const pStmt);
		};
	}
}

