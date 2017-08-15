#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			HuffmanCoding::Encoder encoder("test", HuffmanCoding::EncoderInputFlags::rawString);
			
			std::string res;
			try
			{
				res = encoder.getFromTable('t');
			}
			catch(HuffmanCoding::NotInTableException& e)
			{
				Assert::Fail();
			}
			//Assert::AreEqual(2, HelloWorld::getTwo());
			Logger::WriteMessage(res.c_str());
		}

	};
}