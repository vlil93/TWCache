#pragma once
#include <map>
#include <vector>
#include "Enums.h"
#include "TWOrder.h"

typedef bool (*compare_function)(int, int);

class TWCache
{
	public:
		TWCache();
		///<summary>Get next value of auto-incremented ID</summary>
		///<returns>A string which auto-increments on each function call</returns>
		CString GetNextId();

		/// <summary>
		/// Adds a transaction(order) of the bond with the specified id
		/// </summary>
		/// <param name="bondId">Bond ID</param>
		/// <param name="direction">Direction(Buy/Sell)</param>
		/// <param name="quantity">Quantity</param>
		/// <param name="insertedOrderId">The auto-generated id is returned</param>
		/// <returns>
		/// <see cref="TWErr::MinQuantity"/> if the quantity is less than 0
		/// <see cref="TWErr::NoError"/> if the insert was successful
		/// </returns>
		TWErr Add(const CString& bondId, const CString& direction,
			int quantity, CString& insertedOrderId);

		/// <summary>
		/// Adds a transaction(order) of the bond with the specified id
		/// </summary>
		/// <param name="orderId">Order ID</param>
		/// <param name="bondId">Bond ID</param>
		/// <param name="direction">Direction(Buy/Sell)</param>
		/// <param name="quantity">Quantity</param>
		/// <returns>
		/// <see cref="TWErr::IdCollision"/> if the provided Order ID already exists
		/// <see cref="TWErr::MinQuantity"/> if the quantity is less than 0
		/// <see cref="TWErr::NoError"/> if the insert was successful
		/// </returns>
		TWErr Add(const CString& orderId, const CString& bondId,
			const CString& direction, int quantity);

		/// <summary>
		/// Adds a transaction(order) to the cache
		/// </summary>
		/// <param name="order">Order</param>
		/// <returns>
		/// <see cref="TWErr::IdCollision"/> if the ID of the order already exists
		/// <see cref="TWErr::MinQuantity"/> if the quantity is less than 0
		/// <see cref="TWErr::NoError"/> if the insert was successful
		/// </returns>
		TWErr Add(const TWOrder& order);

		/// <summary>
		/// Removes the transaction identified by the given Order ID
		/// </summary>
		/// <param name="orderId">Order ID</param>
		/// <returns>
		/// <see cref="TWErr::ItemNotFound"/> if no item with the given ID was found
		/// <see cref="TWErr::NoError"/> if the removal was successful
		/// </returns>
		TWErr Remove(const CString& orderId);

		/// <summary>
		/// Removes the transaction identified by the given ID of the provided order
		/// </summary>
		/// <param name="order">Order</param>
		/// <returns>
		/// <see cref="TWErr::ItemNotFound"/> if no item with the given ID was found
		/// <see cref="TWErr::NoError"/> if the removal was successful
		/// </returns>
		TWErr Remove(const TWOrder& order);

		/// <summary>
		/// Returns the element at the given position from cache
		/// </summary>
		/// <param name="i">Position</param>
		/// <returns>Order which is present in the cache at the specified position</returns>
		TWOrder At(int i);

		/// <summary>
		/// Returns the number of items from the cache
		/// </summary>
		/// <returns>Number of items in cache</returns>
		int Size();

		/// <summary>
		/// Determines whether a bond with the given id has ever been traded
		/// </summary>
		/// <param name="bondId">Bond ID</param>
		/// <returns>
		///   <c>true</c> if the bond has been traded otherwise, <c>false</c>.
		/// </returns>
		bool HasBondBeenTraded(const CString& bondId);

		/// <summary>
		/// Calculates the total quantity of trades for each bond
		/// provided in the list for the specified direction (Buy or Sell)
		/// </summary>
		/// <param name="direction">Direction(Buy/Sell)</param>
		/// <param name="bondIdList">List of Bond IDs (if empty, all bonds will be considered)</param>
		/// <returns>
		/// A map for which each entry is a pair of (BondId, TotalQuantity). 
		/// If a bond provided in the list is not found, no entry will be created
		/// </returns>
		std::map<CString, int> BondTotalTrNumber(const CString& direction,
			const std::vector<CString>& bondIdList = std::vector<CString>());

		/// <summary>
		/// Calculates the total quantity of trades for each bond
		/// provided in the list and applies the operator given as input
		/// between the total number of buy and total number of sell quantities.
		/// </summary>
		/// <param name="op">Operator to be applied between the buy and sell quantities</param>
		/// <param name="err">
		/// If an error has occured it will be provided in this parameter. Potential errors:
		/// <see cref="TWErr::NotArithOperator"/> if the operator provided is not an aithmetical one
		/// </param>
		/// <param name="bondIdList">List of Bond IDs (if empty, all bonds will be considered)</param>
		/// <returns>
		/// A map for which each entry is a pair of (BondId, CalculatedQuantity). 
		/// If a bond provided in the list is not found, no entry will be created
		/// </returns>
		std::map<CString, int> BondTotalTrNumber(const Operator& op, TWErr& err,
			const std::vector<CString>& bondIdList = std::vector<CString>());

		/// <summary>
		/// Calculate the total(Buy, Sell or both) quantity of trades for each bond,
		/// applies the logical operator between the calculated quantity and the one 
		/// received as input and returns a list with all bonds for which the result is true
		/// </summary>
		/// <param name="quantity">Quantity</param>
		/// <param name="direction">Direction of the transactions that are considered for the total</param>
		/// <param name="op">Operator to be applied between calculated quantity and input quantity</param>
		/// <param name="err">
		/// If an error has occured it will be provided in this parameter. Potential errors:
		/// <see cref="TWErr::NotComparisonOperator"/> if the operator provided is not a logical one
		/// </param>
		/// <param name="bondIdList">List of Bond IDs (if empty, all bonds will be considered)</param>
		/// <returns>A vector containing all Bond IDs that match the criteria</returns>
		std::vector<CString> BondTrNumberQuery(int quantity, const CString& direction, const Operator& op,
			TWErr& err,	const std::vector<CString>& bondIdList = std::vector<CString>());

	private:
		std::vector<TWOrder> m_Cache;
		int m_CurrentId;

		/// <summary>
		/// Gets a function pointer to the function
		/// corresponding to the operator provided as input
		/// </summary>
		/// <param name="op">Operator</param>
		/// <param name="err">
		/// If an error has occured it will be provided in this parameter. Potential errors:
		/// <see cref="TWErr::NotComparisonOperator"/> if the operator provided is not a logical one
		/// </param>
		/// <returns></returns>
		compare_function GetCompareFunction(const Operator& op, TWErr& err);
};

