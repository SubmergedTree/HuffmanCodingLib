#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(RAWSTRING1)
		{
			HuffmanCoding::Encoder* encoder;
		
			encoder = new HuffmanCoding::Encoder("test", HuffmanCoding::EncoderInputFlags::rawString);
	
			std::string res;
			try
			{
				res = encoder->getFromTable('t');
			}
			catch(std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("0"), res);

			try
			{
				res = encoder->getFromTable('e');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("10"), res);

			try
			{
				res = encoder->getFromTable('s');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("11"), res);
		}



		TEST_METHOD(RAWSTRING2)
		{
			HuffmanCoding::Encoder encoder("MISSISIPI", HuffmanCoding::EncoderInputFlags::rawString);
			
			std::string res;
			try
			{
				res = encoder.getFromTable('M');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("100"), res);

			try
			{
				res = encoder.getFromTable('I');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("0"), res);

			try
			{
				res = encoder.getFromTable('S');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("11"), res);

			try
			{
				res = encoder.getFromTable('P');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("101"), res);
		}


		TEST_METHOD(STRINGFROMFILE1)
		{
			HuffmanCoding::Encoder* encoder;
			try
			{
				encoder = new HuffmanCoding::Encoder("STRINGFROMFILE1.txt", HuffmanCoding::EncoderInputFlags::filename);
			}
			catch(std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			std::string res;
			try
			{
				res = encoder->getFromTable('M');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("100"), res);

			try
			{
				res = encoder->getFromTable('I');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("0"), res);

			try
			{
				res = encoder->getFromTable('S');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("11"), res);

			try
			{
				res = encoder->getFromTable('P');
			}
			catch (std::exception& e)
			{
				Logger::WriteMessage(e.what());
				Assert::Fail();
			}
			Assert::AreEqual(std::string("101"), res);
		}

	};
}