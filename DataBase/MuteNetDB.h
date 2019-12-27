#pragma once


#include "../Foundation/foundation.h"
#include "../Foundation/MacroFunction.h"

#include "../Foundation/ASyncIOTask.h"
#include "../Foundation/ASyncQueue.h"

#if DEBUG
#pragma comment(lib,"MariaDB/debug/libmariadb.lib")
#pragma comment(lib,"MariaDB/debug/mariadbclient.lib")
#else
#pragma comment(lib,"MariaDB/release/libmariadb.lib")
#pragma comment(lib,"MariaDB/release/mariadbclient.lib")
#endif

#include <mysql.h>


/*
	Redesign ÇÊ¼ö
*/