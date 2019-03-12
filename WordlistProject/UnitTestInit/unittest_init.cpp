#include "stdafx.h"
#include "CppUnitTest.h"
#include "../WordlistProject/Initialization.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestInit
{		
	TEST_CLASS(UnitTestParse)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			char* argv[10] = { "program","-w","a.txt" };
			Init init;
			init.parse(3, argv);
			Assert::IsTrue(init.w);
			Assert::IsFalse(init.c);
			Assert::AreEqual(init.file,"a.txt");
		}
		TEST_METHOD(TestMethod2)
		{
			char* argv[10] = { "program","-c","a.txt" };
			Init init;
			init.parse(3, argv);
			Assert::IsTrue(init.c);
			Assert::IsFalse(init.w);
			Assert::AreEqual(init.file, "a.txt");
		}
		TEST_METHOD(TestMethod3)
		{
			char* argv[10] = { "program","-w","a.txt","-h","w"};
			Init init;
			init.parse(5, argv);
			Assert::IsTrue(init.w);
			Assert::AreEqual(init.file, "a.txt");
			Assert::AreEqual(init.h,'w');
		}
		TEST_METHOD(TestMethod4)
		{
			char* argv[10] = { "program","-w","a.txt","-t","q"};
			Init init;
			init.parse(5, argv);
			Assert::IsTrue(init.w);
			Assert::AreEqual(init.file, "a.txt");
			Assert::AreEqual(init.t, 'q');
		}
		TEST_METHOD(TestMethod5)
		{
			char* argv[10] = { "program","-w","a.txt","-r","-t","q"};
			Init init;
			init.parse(6, argv); 
			Assert::IsTrue(init.w);
			Assert::IsFalse(init.c);
			Assert::AreEqual(init.file, "a.txt");
			Assert::AreEqual(init.t, 'q');
			Assert::IsTrue(init.r);
		}
		TEST_METHOD(TestMethod6)
		{
			char* argv[10] = { "program","-w","a.txt","-r","-t","q","-h","w"};
			Init init;
			init.parse(8, argv);
			Assert::IsTrue(init.w);
			Assert::IsFalse(init.c);
			Assert::AreEqual(init.file, "a.txt");
			Assert::AreEqual(init.t, 'q');
			Assert::IsTrue(init.r);
			Assert::AreEqual(init.h, 'w');
		}
	};


	TEST_CLASS(UnitTestInitWord)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			char text[1000];
			char* argv[10] = { "program","-w","../WordlistProject/atest.txt" };
			Init init;
			init.parse(3, argv);			
			Assert::IsTrue(init.w);
			Assert::AreEqual(init.file, "../WordlistProject/atest.txt");
			init.ReadFile(text);
			init.InitWord(text);
			Assert::AreEqual(init.n,5);
			Assert::AreEqual(init.words[0], "ab");
			Assert::AreEqual(init.words[1], "bc");
			Assert::AreEqual(init.words[2], "cd");
			Assert::AreEqual(init.words[3], "de");
			Assert::AreEqual(init.words[4], "dee");
		}
	};


}