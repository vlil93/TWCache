#pragma once
#include <atlstr.h>
#include <map>
#include "Enums.h"

class TWCacheDlg;

class FunctionHelper
{
	public:
		FunctionHelper(TWCacheDlg* dlg);
		void SetFuntion(const Function& function);
		CString ExecuteFunction(TWErr& err);
		CString GetErrString(const TWErr& errValue);
		CString SetErrAndReturnErrString(TWErr& err, const TWErr& errValue);
	private:
		Function m_SelectedFunction;
		TWCacheDlg* m_Dlg;
		CString HandleAddOrder(TWErr& err);
		CString HandleCancelOrder(TWErr& err);
		CString HandleWasTraded(TWErr& err);
		CString HandleGreaterThan(TWErr& err);
		CString HandleTotalQuantity(TWErr& err);
		CString HandleBuySellDiff(TWErr& err);
		CString FormatTransactionNumberString(std::map<CString, int> trNrMap, CString direction);
		CString FormatTransactionDifferenceString(std::map<CString, int> trNrMap);
};

