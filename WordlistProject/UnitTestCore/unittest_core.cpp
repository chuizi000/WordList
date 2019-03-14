#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\WordlistProject\Core.h"
#include <windows.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef int(*p_gen_chain_word)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
typedef int(*p_gen_chain_char)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
typedef int(*p_build_map)(char* words[], char* text);

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
		TEST_METHOD(InitError1)
		{
			Core core;
			char* argv[10] = { "program", "-w" };
			try
			{
				Init* init = core.init_word(2, argv);
				Assert::Fail();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("Missing parameters", msg);
			}
		}
		TEST_METHOD(InitError2)
		{
			Core core;
			char* argv[10] = { "program", "-w","a.txt","-c","a.txt" };
			try
			{
				Init* init = core.init_word(5, argv);
				Assert::Fail();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("Command line arguments include both -w and -c", msg);
			}
		}
		TEST_METHOD(InitError3)
		{
			Core core;
			char* argv[10] = { "program" };
			try
			{
				Init* init = core.init_word(1, argv);
				Assert::Fail();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("Command line parameters do not contain - w or -c", msg);
			}
		}
		TEST_METHOD(InitError4)
		{
			Core core;
			char* argv[10] = { "program", "-w", "a.txt", "-h", "0" };
			try
			{
				Init* init = core.init_word(5, argv);
				Assert::Fail();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("The parameter should be a letter", msg);
			}
		}
		TEST_METHOD(InitError5)
		{
			Core core;
			char* argv[10] = { "program", "w" };
			try
			{
				Init* init = core.init_word(2, argv);
				Assert::Fail();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("Incorrect command line parameters!", msg);
			}
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
		TEST_METHOD(BuildError1)
		{
			Core core;
			char text[1000];
			char* argv[10] = { "program","-w","../WordlistProject/zzy.txt" };
			Init* init = core.init_word(3, argv);
			try
			{
				core.read_file(init, text);
				Assert::Fail();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("File not found", msg);
			}
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
		TEST_METHOD(LoopError1)
		{
			HINSTANCE CoreDLL = LoadLibrary("Core.dll");
			p_gen_chain_word gen_chain_word = (p_gen_chain_word)GetProcAddress(CoreDLL, "gen_chain_word");
			Core core;
			char* words[10] = { "ab","bc","cd", "da", "abb", "bcc" };
			char* result[105];
			try
			{
				int len = gen_chain_word(words, 6, result, 0, 0, 0);
				Assert::Fail();
			}
			catch (const char* msg)
			{
				Assert::AreEqual("There is a ring in the word list", msg);
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