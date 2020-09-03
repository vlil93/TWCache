#pragma once
#include <atlstr.h>
#include "resource.h"

namespace ComboboxOptions
{
	enum Choice
	{
		Add = 0,
		Cancel = 1,
		Query = 2
	};

	enum Query
	{
		WasTraded = 0,
		GreaterThan = 1,
		TotalQuantity = 2,
		BuySellDiff = 3
	};
}

enum class Function
{
	AddOrder = 0,
	CancelOrder = 1,
	QueryWasTraded = 2,
	QueryGreaterThan = 3,
	QueryTotalQuantity = 4,
	QueryBuySellDiff = 5,
	Unidentified
};

enum class TWErr
{
	NoError = 0,
	HelperNotInitialized = IDS_ERR_HELPER_NOT_INIT,
	IdCollision = IDS_ERR_ID_COLLISION,
	ItemNotFound = IDS_ERR_NOT_FOUND,
	InvalidInput = IDS_ERR_INVALID_INPUT,
	NotComparisonOperator = IDS_ERR_NOT_COMPARE_OP,
	NotArithOperator = IDS_ERR_NOT_ARITH_OP,
	MinQuantity = IDS_ERR_MIN_QUANTITY
};

namespace Direction
{
	const CString Buy = _T("B");
	const CString Sell = _T("S");
	const CString Both = _T("BS");
};

enum class Operator
{
	GreaterThan = 0,
	LessThan = 1,
	GreaterOrEqualTo = 2,
	LessOrEqualTo = 3,
	Sum = 4,
	Difference = 5
};