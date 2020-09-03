#include "pch.h"
#include "CppUnitTest.h"
#include "TestcaseDataStruct.h"
#include "TWCache.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TWCacheUt
{
	TEST_CLASS(TWCacheQueryUt)
	{
	public:

		TEST_METHOD(WasBondTraded)
		{
			TWCache cache;
			cache.Add(_T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
			Assert::IsTrue(cache.HasBondBeenTraded(_T("BOND1")));
			Assert::IsFalse(cache.HasBondBeenTraded(_T("BOND2")));
		}

		//Test the fetching of total number of transactions based on direction
		TEST_METHOD(TotalNrOfTransactionsByDirection)
		{
			std::vector<TotalTrNrByDirectionTC>  testCases =
			{
				//Basic Test
				{ "0",	Direction::Buy,		{"BOND1"},	{ std::pair<CString, int>("BOND1", 1) } },
				//Test with different value for bondId
				{ "1",	Direction::Buy,		{"BOND2"},	{ std::pair<CString, int>("BOND2", 2) } },
				//Test with different direction
				{ "2",	Direction::Sell,	{"BOND1"},	{ std::pair<CString, int>("BOND1", 3) } },
				{ "3",	Direction::Both,	{"BOND1"},	{ std::pair<CString, int>("BOND1", 4) } },
				//Test fetch multiple bonds for all directions
				{ "4",	Direction::Buy,		{"BOND1", "BOND2" },
					{ std::pair<CString, int>("BOND1", 1), std::pair<CString, int>("BOND2", 2) } },
				{ "5",	Direction::Sell,	{"BOND1", "BOND3" },
					{ std::pair<CString, int>("BOND1", 3), std::pair<CString, int>("BOND3", 3) } },
					//BOND1 has buy and sell operations, BOND2 only buy and BOND6 only sell
				{ "6",	Direction::Both,	{"BOND1", "BOND2", "BOND6" },
					{ 
						std::pair<CString, int>("BOND1", 4), 
						std::pair<CString, int>("BOND2", 2),
						std::pair<CString, int>("BOND6", 5),
					} 
				},
				//Test missing bond
				{ "7",	Direction::Buy,		{"BONDX"},	{ } },
				//Test both missing and existing bonds in same list
				{ "8",	Direction::Buy,		{"BOND1", "BONDX", "BOND3", "BONDY"},	
					{
						std::pair<CString, int>("BOND1", 1),
						std::pair<CString, int>("BOND3", 3)
					}
				},
				//Test bonds which exist but do not have transactions
				{ "9",	Direction::Buy,		{"BOND6"},	{ } },
				{ "10",	Direction::Sell,	{"BOND2"},	{ } },
				//Test bonds which exist but do not have transactions 
				//along with bonds that do have in the same list
				{ "11",	Direction::Buy,		{"BOND1", "BOND3", "BOND6"},
					{
						std::pair<CString, int>("BOND1", 1),
						std::pair<CString, int>("BOND3", 3)
					}
				},
				{ "12",	Direction::Sell,	{"BOND1", "BOND2", "BOND3"},
					{
						std::pair<CString, int>("BOND1", 3),
						std::pair<CString, int>("BOND3", 3)
					}
				},
				//Test that all results are fetched when no list is provided
				{ "13",	Direction::Buy,		{ },
					{
						std::pair<CString, int>("BOND1", 1),
						std::pair<CString, int>("BOND2", 2),
						std::pair<CString, int>("BOND3", 3),
						std::pair<CString, int>("BOND4", 4),
						std::pair<CString, int>("BOND5", 5)
					}
				},
				{ "14",	Direction::Sell,	{ },
					{
						std::pair<CString, int>("BOND1", 3),
						std::pair<CString, int>("BOND3", 3),
						std::pair<CString, int>("BOND5", 3),
						std::pair<CString, int>("BOND6", 5)
					}
				},
				{ "15",	Direction::Both,	{ },
					{
						std::pair<CString, int>("BOND1", 4),
						std::pair<CString, int>("BOND2", 2),
						std::pair<CString, int>("BOND3", 6),
						std::pair<CString, int>("BOND4", 4),
						std::pair<CString, int>("BOND5", 8),
						std::pair<CString, int>("BOND6", 5)
					}
				}
			};
			//Test
			TWCache cache;
			GenerateInputData(cache);
			for (auto testCase : testCases)
			{
				auto result = cache.BondTotalTrNumber(testCase.m_Direction, testCase.m_BondIdList);
				Assert::IsTrue(testCase.m_ExpectedResult == result,
					_T("TestCase ") + testCase.m_TestName + _T(" has failed\nExpected: ")
					+ ResultToString(testCase.m_ExpectedResult)
					+ "\nActual: " + ResultToString(result)
				);
			}
		}

		//Test the fetching of total number and apply operation
		//between buy and sell transactions
		TEST_METHOD(TotalNrOfTransactionsWithOperation)
		{
			std::vector<TotalTrNrWithOperationTC>  testCases =
			{
				//Basic Test
				{ "0",	Operator::Sum,			{"BOND1"}, TWErr::NoError,	
					{ std::pair<CString, int>("BOND1", 4) } },
				//Test for diff
				{ "1",	Operator::Difference,	{"BOND1"}, TWErr::NoError,	
					{ std::pair<CString, int>("BOND1", -2) } },
				//Test multiple bonds
				{ "2",	Operator::Sum,			{"BOND1", "BOND3"}, TWErr::NoError,	
					{ std::pair<CString, int>("BOND1", 4), std::pair<CString, int>("BOND3", 6) } },
				{ "3",	Operator::Difference,	{"BOND1", "BOND3"}, TWErr::NoError,	
					{ std::pair<CString, int>("BOND1", -2) , std::pair<CString, int>("BOND3", 0) } },
				//Test bonds with only buy or sell operations
				{ "4",	Operator::Sum,			{"BOND2", "BOND6"}, TWErr::NoError,
					{ std::pair<CString, int>("BOND2", 2) , std::pair<CString, int>("BOND6", 5)	} },
				{ "5",	Operator::Difference,	{"BOND2", "BOND6"}, TWErr::NoError,
					{ std::pair<CString, int>("BOND2", 2) , std::pair<CString, int>("BOND6", -5) } },
				//Test missing bond
				{ "6",	Operator::Sum,			{"BONDX"}, TWErr::NoError, { } },
				//Test both missing and existing bonds in same list
				{ "7",	Operator::Sum,			{"BOND1", "BONDX", "BOND3", "BONDY"}, TWErr::NoError,
					{
						std::pair<CString, int>("BOND1", 4),
						std::pair<CString, int>("BOND3", 6)
					}
				},
				//Test that all results are fetched when no list is provided
				{ "8",	Operator::Sum,			{ }, TWErr::NoError,
					{
						std::pair<CString, int>("BOND1", 4),
						std::pair<CString, int>("BOND2", 2),
						std::pair<CString, int>("BOND3", 6),
						std::pair<CString, int>("BOND4", 4),
						std::pair<CString, int>("BOND5", 8),
						std::pair<CString, int>("BOND6", 5)
					}
				},
				{ "9",	Operator::Difference,	{ }, TWErr::NoError,
					{
						std::pair<CString, int>("BOND1", -2),
						std::pair<CString, int>("BOND2", 2),
						std::pair<CString, int>("BOND3", 0),
						std::pair<CString, int>("BOND4", 4),
						std::pair<CString, int>("BOND5", 2),
						std::pair<CString, int>("BOND6", -5)
					}
				},
				//Test wrong type of operator
				{ "10",	Operator::GreaterThan,	{"BOND1"}, TWErr::NotArithOperator, { } },
			};
			//Test
			TWCache cache;
			TWErr err;
			GenerateInputData(cache);
			for (auto testCase : testCases)
			{
				auto result = cache.BondTotalTrNumber(testCase.m_Operation, err, testCase.m_BondIdList);
				Assert::AreEqual((int)(testCase.m_ExpectedError), (int)err,
					_T("TestCase ") + testCase.m_TestName + _T(" has failed"));
				Assert::IsTrue(testCase.m_ExpectedResult == result,
					_T("TestCase ") + testCase.m_TestName + _T(" has failed\nExpected: e")
					+ ResultToString(testCase.m_ExpectedResult)
					+ "\nActual: " + ResultToString(result)
				);
			}
		}

		//Test applying a logical operator to a total number of traded bonds
		//and fetching the ones which satisfy the criteria
		TEST_METHOD(TestBondTrNumberQuery)
		{
			std::vector<BondTrNumberQueryTC>  testCases =
			{
				//Basic operations
				{ "0",	Operator::GreaterThan,		3,	Direction::Both,	{"BOND1"},	TWErr::NoError, {"BOND1"} },
				{ "1",	Operator::GreaterThan,		4,	Direction::Both,	{"BOND1"},	TWErr::NoError, { } },
				{ "2",	Operator::GreaterOrEqualTo,	3,	Direction::Both,	{"BOND1"},	TWErr::NoError, {"BOND1"} },
				{ "3",	Operator::GreaterOrEqualTo,	4,	Direction::Both,	{"BOND1"},	TWErr::NoError, {"BOND1"} },
				{ "4",	Operator::GreaterOrEqualTo,	5,	Direction::Both,	{"BOND1"},	TWErr::NoError, { } },
				{ "5",	Operator::LessThan,			5,	Direction::Both,	{"BOND1"},	TWErr::NoError, {"BOND1"} },
				{ "6",	Operator::LessThan,			4,	Direction::Both,	{"BOND1"},	TWErr::NoError, { } },
				{ "7",	Operator::LessOrEqualTo,	5,	Direction::Both,	{"BOND1"},	TWErr::NoError, {"BOND1"} },
				{ "8",	Operator::LessOrEqualTo,	4,	Direction::Both,	{"BOND1"},	TWErr::NoError, {"BOND1"} },
				{ "9",	Operator::LessOrEqualTo,	3,	Direction::Both,	{"BOND1"},	TWErr::NoError, { } },
				//Test with multiple return IDs
				{ "10",	Operator::GreaterThan,		1, Direction::Both,		{"BOND1", "BOND2", "BOND3"},
					TWErr::NoError, {"BOND1", "BOND2", "BOND3"} },
				{ "11",	Operator::GreaterThan,		3,	Direction::Both,	{"BOND1", "BOND2", "BOND3"},
					TWErr::NoError, {"BOND1", "BOND3"} },
				{ "12",	Operator::GreaterThan,		5,	Direction::Both,	{"BOND1", "BOND2", "BOND3"},
					TWErr::NoError, {"BOND3"} },
				{ "13",	Operator::GreaterThan,		7,	Direction::Both,	{"BOND1", "BOND2", "BOND3"}, TWErr::NoError, { } },
				//Test missing bond
				{ "14",	Operator::GreaterThan,		0,	Direction::Both,	{"BONDX"}, TWErr::NoError, { } },
				//Test both missing and existing bonds in same list
				{ "15",	Operator::GreaterThan,		0,	Direction::Both,	{"BOND1", "BONDX", "BOND3", "BONDY"},
					TWErr::NoError, {"BOND1", "BOND3"} },
				//Test that all results are fetched when no list is provided
				{ "16",	Operator::GreaterThan,		0,	Direction::Both,	{ }, TWErr::NoError,
					{ "BOND1", "BOND2", "BOND3", "BOND4", "BOND5", "BOND6" }
				},
				{ "17",	Operator::GreaterThan,		4,	Direction::Both,	{ }, 
					TWErr::NoError, { "BOND3", "BOND5", "BOND6" } },
				//Test with different directions
				{ "18",	Operator::GreaterThan,		3,	Direction::Buy,		{ },
					TWErr::NoError, { "BOND4", "BOND5" } },
				{ "19",	Operator::GreaterThan,		4,	Direction::Sell,	{ },
					TWErr::NoError, { "BOND6" } },
				//Test use of invalid operators
				{ "20",	Operator::Sum,				0,	Direction::Both,	{ }, 
					TWErr::NotComparisonOperator, { } },
				{ "21",	Operator::Difference,		0,	Direction::Both,	{ }, 
					TWErr::NotComparisonOperator, { } }
			};
			TWCache cache;
			TWErr err;
			GenerateInputData(cache);
			std::vector<CString> result;
			for (auto testCase : testCases)
			{
				result = cache.BondTrNumberQuery(testCase.m_Quantity, testCase.m_Direction,
					testCase.m_Operation, err, testCase.m_BondIdList);
				Assert::AreEqual((int)(testCase.m_ExpectedError), (int)err,
					_T("TestCase ") + testCase.m_TestName + _T(" has failed"));
				Assert::IsTrue(testCase.m_ExpectedResult == result,
					_T("TestCase ") + testCase.m_TestName + _T(" has failed\nExpected: ")
					+ ResultToString(testCase.m_ExpectedResult)
					+ "\nActual: " + ResultToString(result)
				);
			}
		}

		void GenerateInputData(TWCache& cache)
		{
			CString buffer;
			for (int i = 1; i < 6; i++)
			{
				buffer.Format(_T("%d"), i);
				cache.Add("BOND" + buffer, Direction::Buy, i, buffer);
			}
			cache.Add("BOND1", Direction::Sell, 3, buffer);
			cache.Add("BOND3", Direction::Sell, 3, buffer);
			cache.Add("BOND5", Direction::Sell, 3, buffer);
			cache.Add("BOND6", Direction::Sell, 5, buffer);
		}

		CString ResultToString(std::map<CString, int> results)
		{
			if (results.size() == 0)
			{
				return "Empty";
			}
			CString buffer;
			for (auto result : results)
			{
				buffer.Format(buffer + result.first + ":%d, ", result.second);
			}
			buffer.Delete(buffer.GetLength() - 2, 2);
			return buffer;
		}

		CString ResultToString(std::vector<CString> results)
		{
			if (results.size() == 0)
			{
				return "Empty";
			}
			CString buffer;
			for (CString result : results)
			{
				buffer += result + ", ";
			}
			buffer.Delete(buffer.GetLength() - 2, 2);
			return buffer;
		}
	};
}