#include <algorithm>
#include "pch.h"
#include "TWCache.h"

TWCache::TWCache():
	m_CurrentId(1)
{

}

CString TWCache::GetNextId()
{
	CString str;
	str.Format(_T("ID%d"), m_CurrentId++);
	return str;
}

TWErr TWCache::Add(const CString& bondId, const CString& direction, int quantity, CString& insertedOrderId)
{
	return Add((insertedOrderId = GetNextId()), bondId, direction, quantity);
}

TWErr TWCache::Add(const CString& orderId, const CString& bondId, const CString& direction, int quantity)
{
	return Add(TWOrder(orderId, bondId, direction, quantity));
}

TWErr TWCache::Add(const TWOrder& order)
{
	if (order.m_Quantity < 1)
	{
		return TWErr::MinQuantity;
	}
	if (std::find(m_Cache.begin(), m_Cache.end(), order) != m_Cache.end())
	{
		return TWErr::IdCollision;
	}
	else
	{
		m_Cache.push_back(order);
	}
	return TWErr::NoError;
}

TWErr TWCache::Remove(const TWOrder& order)
{
	return Remove(order.m_OrderId);
}

TWErr TWCache::Remove(const CString& orderId)
{
	std::vector<TWOrder>::iterator order = std::find(m_Cache.begin(), m_Cache.end(), orderId);
	if (order != m_Cache.end())
	{
		m_Cache.erase(order);
	}
	else
	{
		return TWErr::ItemNotFound;
	}
	return TWErr::NoError;
}

TWOrder TWCache::At(int i)
{
	return m_Cache.at(i);
}

int TWCache::Size()
{
	return m_Cache.size();
}

bool TWCache::HasBondBeenTraded(const CString& bondId)
{
	return std::find_if(m_Cache.begin(), m_Cache.end(), 
		[&bondId](TWOrder order) { return order.m_BondId == bondId; }) != m_Cache.end();
}


std::map<CString, int> TWCache::BondTotalTrNumber(const CString& direction,
	const std::vector<CString>& bondIdList)
{
	if (direction == Direction::Both)
	{
		TWErr err;
		return BondTotalTrNumber(Operator::Sum, err, bondIdList);
	}
	std::map<CString, int> totalTrPerBond;
	for (TWOrder order : m_Cache)
	{
		if (order.m_Direction != direction 
			|| ( !bondIdList.empty() 
				&& std::find(bondIdList.begin(), bondIdList.end(), order.m_BondId) == bondIdList.end()
			)
		)
		{
			continue;
		}

		auto it = totalTrPerBond.find(order.m_BondId);
		if (it != totalTrPerBond.end())
		{
			it->second += order.m_Quantity;
		}
		else
		{
			totalTrPerBond.insert(std::pair<CString, int>(
				order.m_BondId, order.m_Quantity));
		}
	}
	return totalTrPerBond;
}

std::map<CString, int> TWCache::BondTotalTrNumber(const Operator& op,
	TWErr& err, const std::vector<CString>& bondIdList)
{
	err = TWErr::NoError;
	std::map<CString, int> totalTrPerBond;
	if (op != Operator::Sum && op != Operator::Difference)
	{
		err = TWErr::NotArithOperator;
		return totalTrPerBond;
	}
	for (TWOrder order : m_Cache)
	{
		if (!bondIdList.empty() &&
			std::find(bondIdList.begin(), bondIdList.end(), order.m_BondId) == bondIdList.end())
		{
			continue;
		}
		int quantity = order.m_Direction == Direction::Sell && op == Operator::Difference ?
			-order.m_Quantity : order.m_Quantity;

		auto it = totalTrPerBond.find(order.m_BondId);
		if (it != totalTrPerBond.end())
		{
			it->second += quantity;
		}
		else
		{
			totalTrPerBond.insert(std::pair<CString, int>(
				order.m_BondId, quantity));
		}
	}
	return totalTrPerBond;
}

std::vector<CString> TWCache::BondTrNumberQuery(int quantity, const CString& direction, 
	const Operator& op, TWErr& err, const std::vector<CString>& bondIdList)
{
	std::vector<CString> result;
	std::map<CString, int> totalTrPerBond;
	err = TWErr::NoError;

	auto compareFunc = GetCompareFunction(op, err);
	if (err == TWErr::NoError)
	{
		if (direction == Direction::Both)
		{
			totalTrPerBond = BondTotalTrNumber(Operator::Sum, err, bondIdList);
		}
		else
		{
			totalTrPerBond = BondTotalTrNumber(direction, bondIdList);
		}
		for (std::pair<CString, int> pair : totalTrPerBond)
		{
			if (compareFunc(pair.second, quantity))
			{
				result.push_back(pair.first);
			}
		}
	}
	return result;
}

compare_function TWCache::GetCompareFunction(const Operator& op, TWErr& err)
{
	switch (op)
	{
		case Operator::GreaterThan:
		{
			return [](int a, int b) { return a > b; };
		}
		case Operator::LessThan:
		{
			return [](int a, int b) { return a < b; };
		}
		case Operator::GreaterOrEqualTo:
		{
			return [](int a, int b) { return a >= b; };
		}
		case Operator::LessOrEqualTo:
		{
			return [](int a, int b) { return a <= b; };
		}
		default:
		{
			err = TWErr::NotComparisonOperator;
			return [](int a, int b) { return false; };
		}
	}
}

