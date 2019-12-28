#include "MuteNetDB.h"
#include "MariaDBParameterCollection.h"

namespace Util
{
	namespace DB
	{
		MariaDBParameterCollection::MariaDBParameterCollection()
			:_numberOfParameters(0),
			_paramterPosition(0),
			_pArrParameters(nullptr)
		{
			_BindingInformations.resize(MaxParameter);
			_pArrParameters = new MYSQL_BIND[MaxParameter];
			_pIndicator = new char[MaxParameter];

			memset(_pIndicator, STMT_INDICATOR_IGNORE, MaxParameter);
		}

		MariaDBParameterCollection::~MariaDBParameterCollection()
		{
			delete[] _pArrParameters;
			delete[] _pIndicator;
		}

		void MariaDBParameterCollection::AddParameter(const enum_field_types type,
			void* OUT pBuffer, uint32_t* const OUT pOutLength,
			const uint32_t length, const uint32_t maxlength,
			const bool isunsigned)
		{
			if (MaxParameter <= _paramterPosition)
			{
				throw;
			}

			auto info = _BindingInformations[_paramterPosition];
			info.is_null = false;
			info.length = length;
			info.error = 0;

			auto bind = &_pArrParameters[_paramterPosition];
			memset(bind, 0, sizeof(MYSQL_BIND));
	
			bind->buffer_type = type;
			bind->buffer = pBuffer;
			bind->u.indicator = _pIndicator;

			++_paramterPosition;

			_numberOfParameters = _paramterPosition;
		}
		void MariaDBInputParameterCollection::Bind(MYSQL_STMT* const pStmt)
		{
			if (_numberOfParameters != _paramterPosition)
			{
				throw;
			}

			const my_bool result = ::mysql_stmt_bind_param(pStmt, &_pArrParameters[0]);

			if (0 != result)
			{
				auto errorCode = ::mysql_stmt_errno(pStmt);
				auto errorStr = ::mysql_stmt_error(pStmt);

				throw;
			}
		}

		void MariaDBOutputParameterCollection::Bind(MYSQL_STMT* const pStmt)
		{
			if (_numberOfParameters != _paramterPosition)
			{
				throw;
			}

			const my_bool result = ::mysql_stmt_bind_param(pStmt, &_pArrParameters[0]);

			if (false != result)
			{
				throw;
			}
		}
	}
}
