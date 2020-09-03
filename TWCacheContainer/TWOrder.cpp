#pragma once
#include "pch.h"
#include "TWOrder.h"

TWOrder::TWOrder(CString orderId, CString bondId, CString direction, int quantity) :
	m_OrderId(orderId),
	m_BondId(bondId),
	m_Direction(direction),
	m_Quantity(quantity)
{ }

bool TWOrder::operator==(const TWOrder& order)
{
	return m_OrderId == order.m_OrderId;
}

bool TWOrder::operator==(const CString& orderId)
{
	return m_OrderId == orderId;
}