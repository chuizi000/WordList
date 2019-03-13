#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\WordlistProject\Core.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestCore
{		
	TEST_CLASS(UnitTestinit_word)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Core core;
			char* argv[10] = { "program","-w","../WordlistProject/atest.txt" };
			Init* init = core.init_word(3, argv);
			Assert::IsTrue(init->w);
			Assert::AreEqual(init->file, "../WordlistProject/atest.txt");
		}

		TEST_METHOD(TestMethod2)
		{
			Core core;
			char* argv[10] = { "program","-w","a.txt","-r","-t","q","-h","w" };
			Init* init = core.init_word(8, argv);
			Assert::IsTrue(init->w);
			Assert::IsFalse(init->c);
			Assert::AreEqual(init->file, "a.txt");
			Assert::AreEqual(init->t,'q');
			Assert::IsTrue(init->r);
			Assert::AreEqual(init->h,'w');
		}


	};

	TEST_CLASS(UnitTestbuild_map)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			Core core;
			char text[1000];
			char* argv[10] = { "program","-w","../WordlistProject/atest.txt" };
			Init* init = core.init_word(3, argv);
			Assert::IsTrue(init->w);
			Assert::AreEqual(init->file, "../WordlistProject/atest.txt");
			core.read_file(init,text);
			core.build_map(init,text);
			Assert::AreEqual(init->n, 5);
			Assert::AreEqual(init->words[0], "ab");
			Assert::AreEqual(init->words[1], "bc");
			Assert::AreEqual(init->words[2], "cd");
			Assert::AreEqual(init->words[3], "de");
			Assert::AreEqual(init->words[4], "dee");
		}
	};

	TEST_CLASS(UnitTestgen_chain_word)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			Core core;
			char* words[10] = { "ab","bc","cd", "cd", "de", "dee" };
			char* result[105];
			int len = core.gen_chain_word(words, 6, result, 0, 0, 0);
			Assert::AreEqual(len, 4);
			for (int i = 0; i < 3; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
		}
		TEST_METHOD(TestMethod2)
		{
			Core core;
			char* words[10] = { "ab","bc","cd", "da", "abb", "bcc" };
			char* result[105];
			int len = core.gen_chain_word(words, 6, result, 0, 0, 1);
			Assert::AreEqual(len, 6);
			for (int i = 0; i < 5; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
		}
	};

	TEST_CLASS(UnitTestgen_chain_char)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			Core core;
			char* words[10] = { "ab","bc","cd", "cd", "de", "dee" };
			char* result[105];
			int len = core.gen_chain_char(words, 6, result, 0, 0, 0);
			Assert::AreEqual(len, 4);
			for (int i = 0; i < 3; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
		}

		TEST_METHOD(TestMethod2)
		{
			Core core;
			char* words[10] = { "ab","bc","cd", "da", "abb", "bcc" };
			char* result[105];
			int len = core.gen_chain_char(words, 6, result, 0, 0, 1);
			Assert::AreEqual(len, 6);
			for (int i = 0; i < 5; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
		}

	};
}

//Core core;
//Init init = core.init_word(argc, argv);
//core.read_file(init, text);
//core.build_map(init, text);
//if (init.w) result_len = core.gen_chain_word(init.words, init.n, result, init.h, init.t, init.r);
//else result_len = core.gen_chain_char(init.words, init.n, result, init.h, init.t, init.r);
//core.printf_chain(result, result_len);