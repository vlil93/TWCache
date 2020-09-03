#include "pch.h"
#include "FunctionHelper.h"
#include "TWCacheDlg.h"

FunctionHelper::FunctionHelper(TWCacheDlg* dlg)
{
	m_Dlg = dlg;
}
void FunctionHelper::SetFuntion(const Function& function)
{
	m_SelectedFunction = function;
}
CString FunctionHelper::ExecuteFunction(TWErr& err)
{
	err = TWErr::NoError;
	if (m_Dlg == nullptr)
	{
		return SetErrAndReturnErrString(err, TWErr::HelperNotInitialized);
	}
	if (m_Dlg->GetEdit1String().GetLength() < 1 
		&& m_SelectedFunction != Function::QueryTotalQuantity
		&& m_SelectedFunction != Function::QueryBuySellDiff
	)
	{
		return SetErrAndReturnErrString(err, TWErr::InvalidInput);
	}
	switch (m_SelectedFunction)
	{
		case Function::AddOrder:
		{
			return HandleAddOrder(err);
		}
		case Function::CancelOrder:
		{
			return HandleCancelOrder(err);
		}
		case Function::QueryWasTraded:
		{
			return HandleWasTraded(err);
		}
		case Function::QueryGreaterThan:
		{
			return HandleGreaterThan(err);
		}
		case Function::QueryTotalQuantity:
		{
			return HandleTotalQuantity(err);
		}
		case Function::QueryBuySellDiff:
		{
			return HandleBuySellDiff(err);
		}
		default:
		{
			return _T("Undefined Funciton");
			break;
		}
	}
	return _T("");
}

CString FunctionHelper::GetErrString(const TWErr& errValue)
{
	CString stringBuffer;
	stringBuffer.LoadStringW(static_cast<int>(errValue));
	return stringBuffer;
}

CString FunctionHelper::SetErrAndReturnErrString(TWErr& err, const TWErr& errValue)
{
	err = errValue;
	return GetErrString(errValue);
}

CString FunctionHelper::HandleAddOrder(TWErr& err)
{
	CString result;
	if (m_Dlg->GetEdit2String().GetLength() < 1 ||
		!(m_Dlg->IsBuyChecked() ^ m_Dlg->IsSellChecked()))
	{
		return SetErrAndReturnErrString(err, TWErr::InvalidInput);
	}
	int quantity = _tstoi(m_Dlg->GetEdit2String());
	CString orderId;
	err = m_Dlg->GetCache().Add(
		m_Dlg->GetEdit1String(),
		m_Dlg->IsBuyChecked() ? Direction::Buy : Direction::Sell,
		quantity,
		orderId);
	if (err != TWErr::NoError)
	{
		return GetErrString(err);
	}
	else
	{
		CString buffer;
		buffer.LoadStringW(IDS_ORDER_ADDED);
		result.Format(buffer, orderId);
		return result;
	}
}
CString FunctionHelper::HandleCancelOrder(TWErr& err)
{
	CString result;
	err = m_Dlg->GetCache().Remove(m_Dlg->GetEdit1String());
	if (err != TWErr::NoError)
	{
		return GetErrString(err);
	}
	else
	{
		CString buffer;
		buffer.LoadStringW(IDS_ORDER_CANCELED);
		result.Format(buffer, m_Dlg->GetEdit1String());
		return result;
	}
	return result;
}

CString FunctionHelper::HandleWasTraded(TWErr& err)
{
	CString buffer;
	bool result = m_Dlg->GetCache().HasBondBeenTraded(m_Dlg->GetEdit1String());
	buffer.LoadStringW(result ? IDS_YES : IDS_NO);
	return buffer;
}

CString FunctionHelper::HandleGreaterThan(TWErr& err)
{
	int quantity = _tstoi(m_Dlg->GetEdit1String());
	bool buy = m_Dlg->IsBuyChecked();
	bool sell = m_Dlg->IsSellChecked();
	CString direction;

	if (buy && sell)
	{
		direction = Direction::Both;
	}
	else if(buy)
	{
		direction = Direction::Buy;
	}
	else if(sell)
	{
		direction = Direction::Sell;
	}
	else
	{
		err = TWErr::InvalidInput;
		return GetErrString(err);
	}
	std::vector<CString> result;
	result = m_Dlg->GetCache()
		.BondTrNumberQuery(quantity, direction, Operator::GreaterThan, err);
	if (err != TWErr::NoError)
	{
		return GetErrString(err);
	}
	CString buffer;
	if (result.size() > 0)
	{
		buffer.LoadString(IDS_BONDS_TRADED_MORE_THAN);
		for (auto it = result.begin(); it < result.end() - 1; it++)
		{
			buffer += *it + _T(",");
		}
		buffer += *(result.end() - 1);
	}
	else
	{
		buffer.LoadStringW(IDS_NO_BONDS_MATCH);
	}
	return buffer;
}

CString FunctionHelper::HandleTotalQuantity(TWErr& err)
{
	std::vector<CString> bondId;
	if (m_Dlg->GetEdit1String().GetLength() > 0)
	{
		bondId.push_back(m_Dlg->GetEdit1String());
	}
	bool buy = m_Dlg->IsBuyChecked();
	bool sell = m_Dlg->IsSellChecked();
	std::map<CString, int> result;
	CString direction;
	if (buy && sell)
	{
		result = m_Dlg->GetCache().BondTotalTrNumber(Operator::Sum, err,  bondId );
		direction = Direction::Both;
	}
	else if (buy)
	{
		result = m_Dlg->GetCache().BondTotalTrNumber(Direction::Buy, bondId);
		direction = Direction::Buy;
	}
	else if (sell)
	{
		result = m_Dlg->GetCache().BondTotalTrNumber(Direction::Sell, bondId);
		direction = Direction::Sell;
	}
	else
	{
		err = TWErr::InvalidInput;
	}
	if (err != TWErr::NoError)
	{
		return GetErrString(err);
	}
	return FormatTransactionNumberString(result, direction);
}

CString FunctionHelper::HandleBuySellDiff(TWErr& err)
{
	std::vector<CString> bondId;
	if (m_Dlg->GetEdit1String().GetLength() > 0)
	{
		bondId.push_back(m_Dlg->GetEdit1String());
	}
	std::map<CString, int> result;
	result = m_Dlg->GetCache().BondTotalTrNumber(Operator::Difference, err, bondId);
	return FormatTransactionDifferenceString(result);
}

CString FunctionHelper::FormatTransactionNumberString(std::map<CString, int> trNrMap, CString direction)
{
	CString format, buffer, action, result;

	if (trNrMap.size() == 0)
	{
		buffer.LoadStringW(IDS_NO_TRANSACTON_FOUND);
		return buffer;
	}

	if (direction == Direction::Both)
	{
		action.LoadStringW(IDS_TRANSACTIONED);
	}
	else if(direction == Direction::Buy)
	{
		action.LoadStringW(IDS_BOUGHT);
	}
	else
	{
		action.LoadStringW(IDS_SOLD);
	}

	format.LoadStringW(IDS_BOND_WAS_TRANSACTONED);
	for (auto trNr : trNrMap)
	{
		buffer.Format(format, trNr.first, action, trNr.second);
		result += buffer + _T("\n");
	}
	return result;
}

CString FunctionHelper::FormatTransactionDifferenceString(std::map<CString, int> trNrMap)
{
	CString format, buffer, result;

	if (trNrMap.size() == 0)
	{
		buffer.LoadStringW(IDS_NO_TRANSACTON_FOUND);
		return buffer;
	}
	format.LoadStringW(IDS_TR_NR_DIFFERENCE);
	for (auto trNr : trNrMap)
	{
		buffer.Format(format, trNr.first, trNr.second);
		result += buffer + _T("\n");
	}
	return result;
}