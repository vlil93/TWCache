#pragma once
#include <atlstr.h>
class TWOrder
{
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="orderId">Unique id of the order</param>
		/// <param name="bondId">Id of the traded bond</param>
		/// <param name="direction">Direction of the trade</param>
		/// <param name="quantity">Quantity traded</param>
		TWOrder(CString orderId, CString bondId, CString direction, int quantity);
		/// <summary>
		/// Unique id of the order
		/// </summary>
		CString m_OrderId;
		/// <summary>
		/// Id of the traded bond
		/// </summary>
		CString m_BondId;
		/// <summary>
		/// Direction of the trade
		/// </summary>
		CString m_Direction;
		/// <summary>
		/// Quantity traded
		/// </summary>
		int m_Quantity;
		bool operator==(const TWOrder& order);
		bool operator==(const CString& orderId);
};

