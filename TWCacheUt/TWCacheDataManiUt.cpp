#include "pch.h"
#include "CppUnitTest.h"
#include "Specialization.h"
#include "TWCache.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TWCacheUt
{
	TEST_CLASS(TWCacheDataManiUt)
	{
	public:
		//Test that the OrderId auto-increments after each call
		TEST_METHOD(TestGetNextId)
		{
			TWCache cache;
			Assert::AreEqual(_T("ID1"), cache.GetNextId());
			Assert::AreEqual(_T("ID2"), cache.GetNextId());
			Assert::AreEqual(_T("ID3"), cache.GetNextId());
		}

		//Test that adding new entries without giving a specific OrderId
		//happens successfully and the correct generatedId is returned
		TEST_METHOD(TestAddGenerateId)
		{
			TWCache cache;
			CString generatedId;
			cache.Add(_T("BOND1"), Direction::Sell, 5, generatedId);
			Assert::AreEqual(1, (int)cache.Size());
			ValidateOrderFields(cache.At(0), generatedId, _T("BOND1"), Direction::Sell, 5);
		}

		//Test that adding entries with specifying all fields happens successfully
		TEST_METHOD(TestAdd)
		{
			TWCache cache;
			cache.Add(_T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
			Assert::AreEqual(1, (int)cache.Size());
			ValidateOrderFields(cache.At(0), _T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
		}

		//Test that adding a TWOrder type object to the cache happens sucessfully
		TEST_METHOD(TestAddOrder)
		{
			TWCache cache;
			TWOrder order(_T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
			cache.Add(order);
			Assert::AreEqual(1, (int)cache.Size());
			ValidateOrderFields(cache.At(0), _T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
		}

		//Test that inserting multiple objects in the cache happens successfully
		TEST_METHOD(TestMultipleAdd)
		{
			TWCache cache;
			CString generatedId;
			cache.Add(_T("BOND1"), Direction::Buy, 1, generatedId);
			Assert::AreEqual(1, (int)cache.Size());
			ValidateOrderFields(cache.At(0), generatedId, _T("BOND1"), Direction::Buy, 1);
			cache.Add(_T("BOND1"), Direction::Sell, 2, generatedId);
			Assert::AreEqual(2, (int)cache.Size());
			ValidateOrderFields(cache.At(1), generatedId, _T("BOND1"), Direction::Sell, 2);
			cache.Add(_T("BOND2"), Direction::Buy, 3, generatedId);
			Assert::AreEqual(3, (int)cache.Size());
			ValidateOrderFields(cache.At(2), generatedId, _T("BOND2"), Direction::Buy, 3);
		}

		//Test that inserting two objects with the same Id generaed an error
		TEST_METHOD(TestAddIdCollision)
		{
			TWCache cache;
			TWOrder order(_T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
			cache.Add(order);
			Assert::AreEqual(1, (int)cache.Size());
			ValidateOrderFields(cache.At(0), _T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
			TWErr err = cache.Add(order);
			Assert::AreEqual((int)TWErr::IdCollision, (int)err);
			Assert::AreEqual(1, (int)cache.Size());
			err = cache.Add(_T("INPUT_ID1"), _T("BOND2"), Direction::Buy, 10);
			Assert::AreEqual((int)TWErr::IdCollision, (int)err);
			Assert::AreEqual(1, (int)cache.Size());
		}

		//Test to add an order with a negative quantity of bonds
		TEST_METHOD(TestAddNegativeQuantity)
		{
			TWCache cache;
			CString generatedId;
			TWErr err = cache.Add(_T("BOND1"), Direction::Buy, -2, generatedId);
			Assert::AreEqual((int)TWErr::MinQuantity, (int)err);
			Assert::AreEqual(0, (int)cache.Size());
			err = cache.Add(_T("BOND2"), Direction::Sell, -2, generatedId);
			Assert::AreEqual((int)TWErr::MinQuantity, (int)err);
			Assert::AreEqual(0, (int)cache.Size());
		}

		//Test that removing an object when providing the OrderId
		//happens successfully
		TEST_METHOD(TestRemoveById)
		{
			TWCache cache;
			TWOrder order1(_T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
			TWOrder order2(_T("INPUT_ID2"), _T("BOND2"), Direction::Buy, 6);
			cache.Add(order1);
			cache.Add(order2);
			Assert::AreEqual(2, (int)cache.Size());
			cache.Remove(order1.m_OrderId);
			Assert::AreEqual(1, (int)cache.Size());
			Assert::AreEqual(order2.m_OrderId, cache.At(0).m_OrderId);
			cache.Remove(order2.m_OrderId);
			Assert::AreEqual(0, (int)cache.Size());
		}

		//Test that removing an object when providing the Order
		//happens successfully
		TEST_METHOD(TestRemoveByObject)
		{
			TWCache cache;
			TWOrder order1(_T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
			TWOrder order2(_T("INPUT_ID2"), _T("BOND2"), Direction::Buy, 6);
			cache.Add(order1);
			cache.Add(order2);
			Assert::AreEqual(2, (int)cache.Size());
			cache.Remove(order1);
			Assert::AreEqual(1, (int)cache.Size());
			Assert::AreEqual(order2.m_OrderId, cache.At(0).m_OrderId);
			cache.Remove(order2);
			Assert::AreEqual(0, (int)cache.Size());
		}

		//Test to remove an ID that does not exist in the cache
		TEST_METHOD(TestRemoveNonexistentId)
		{
			TWCache cache;
			TWOrder order(_T("INPUT_ID1"), _T("BOND1"), Direction::Sell, 5);
			cache.Add(order);
			Assert::AreEqual(1, (int)cache.Size());
			TWErr err = cache.Remove(_T("INPUT_ID2"));
			Assert::AreEqual((int)TWErr::ItemNotFound, (int)err);
			Assert::AreEqual(1, (int)cache.Size());
			order.m_OrderId = _T("INPUT_ID2");
			err = cache.Remove(order);
			Assert::AreEqual((int)TWErr::ItemNotFound, (int)err);
			Assert::AreEqual(1, (int)cache.Size());
		}

		void ValidateOrderFields(TWOrder order, CString orderId,
			CString bondId, CString direction, int quantity)
		{
			Assert::AreEqual(orderId, order.m_OrderId);
			Assert::AreEqual(bondId, order.m_BondId);
			Assert::AreEqual(direction, order.m_Direction);
			Assert::AreEqual(quantity, order.m_Quantity);
		}
	};
}
