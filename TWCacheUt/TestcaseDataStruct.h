#pragma once
#include "Enums.h"
#include <atlstr.h>
#include <map>
#include <vector>

struct TestCase
{
	TestCase(const CString& testCaseNr) : m_TestName(testCaseNr) { };
	CString m_TestName;
};

struct TotalTrNrByDirectionTC : TestCase
{
	TotalTrNrByDirectionTC(const CString& testCaseNr, const CString& direction,
		std::initializer_list<CString> bondIds,
		std::initializer_list<std::pair<const CString, int>> expectedResult) :
		TestCase(testCaseNr),
		m_Direction(direction),
		m_BondIdList(bondIds),
		m_ExpectedResult(expectedResult)
	{ }
	CString m_Direction;
	std::vector<CString> m_BondIdList;
	std::map<CString, int> m_ExpectedResult;
};

struct TotalTrNrWithOperationTC : TestCase
{
	TotalTrNrWithOperationTC(const CString& testCaseNr, const Operator& op,
		std::initializer_list<CString> bondIds,
		TWErr expectedError,
		std::initializer_list<std::pair<const CString, int>> expectedResult) :
		TestCase(testCaseNr),
		m_Operation(op),
		m_BondIdList(bondIds),
		m_ExpectedError(expectedError),
		m_ExpectedResult(expectedResult)
	{ }
	Operator m_Operation;
	std::vector<CString> m_BondIdList;
	TWErr m_ExpectedError;
	std::map<CString, int> m_ExpectedResult;
};

struct BondTrNumberQueryTC : TestCase
{
	BondTrNumberQueryTC(const CString& testCaseNr, 
		const Operator& op, 
		int quantity,
		const CString& direction,
		std::initializer_list<CString> bondIds,
		TWErr expectedError,
		std::initializer_list<CString> expectedResult) :
		TestCase(testCaseNr),
		m_Operation(op),
		m_Quantity(quantity),
		m_Direction(direction),
		m_BondIdList(bondIds),
		m_ExpectedError(expectedError),
		m_ExpectedResult(expectedResult)
	{ }
	Operator m_Operation;
	int m_Quantity;
	CString m_Direction;
	std::vector<CString> m_BondIdList;
	TWErr m_ExpectedError;
	std::vector<CString> m_ExpectedResult;
};