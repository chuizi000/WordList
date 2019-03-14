#include "stdafx.h"
#include "CppUnitTest.h"
#include "../WordlistProject/FindChain.h"
#include "../WordlistProject/Initialization.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestFindChain
{		
	TEST_CLASS(UnitTestBuildMap)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			char* words[10] = { "ab","bc","cd", "cd", "de", "dee" };

			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 0, 0);
			Assert::AreEqual(findchain.edges[0][1][0].key,1);
			Assert::AreEqual(findchain.edges[2][3][0].word,"cd");
			Assert::AreEqual(findchain.edges[3][4][1].word, "de");


			Assert::AreEqual(findchain.canGo[3][0], 1);
			Assert::AreEqual(findchain.canGo[3][1], 4);
		}
		TEST_METHOD(TestMethod2)
		{
			char* words[10] = {"ab","bc","cd", "cd", "de", "dee" };

			FindChain findchain;
			findchain.BuildMap(words, 6, 0, 0, 0);
			Assert::AreEqual(findchain.edges[0][1][0].key, 2);
			Assert::AreEqual(findchain.edges[2][3][0].word, "cd");
			Assert::AreEqual(findchain.edges[3][4][1].word, "de");


			Assert::AreEqual(findchain.canGo[3][0], 1);
			Assert::AreEqual(findchain.canGo[3][1], 4);
		}
	};
	
	TEST_CLASS(UnitTesthasRing)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			char* words[10] = { "ab","bc","cd", "cd", "de", "dee" };

			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 0, 0);
			bool ret = findchain.hasRing();
			Assert::IsFalse(ret);

		}
		TEST_METHOD(TestMethod2)
		{
			char* words[10] = { "ab","bc","cd", "cd", "de", "dea" };

			FindChain findchain;
			findchain.BuildMap(words, 6, 0, 0, 0);
			bool ret = findchain.hasRing();
			Assert::IsTrue(ret);
		}
		TEST_METHOD(TestMethod3)
		{
			char* words[10] = { "aa","ba", "cd", "de", "dea" };

			FindChain findchain;
			findchain.BuildMap(words, 5, 0, 0, 0);
			bool ret = findchain.hasRing();
			Assert::IsFalse(ret);
		}
		TEST_METHOD(TestMethod4)
		{
			char* words[10] = { "aa","bb", "cc", "dd", "ee" };

			FindChain findchain;
			findchain.BuildMap(words, 5, 0, 0, 0);
			bool ret = findchain.hasRing();
			Assert::IsFalse(ret);
		}
	};

	TEST_CLASS(UnitTestGetWordChain_NoRing)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			char* words[10] = { "ab","bc","cd", "cd", "de", "dee" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 0, 0);
			int len = findchain.GetWordChain_NoRing(result);
			Assert::AreEqual(len,4);
			for (int i = 0; i < 3; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen-1]==result[i+1][0]);
			}
		}
		TEST_METHOD(TestMethod2)
		{
			char* words[10] = { "abb","acc","bc", "cd", "de", "dd" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 0, 0);
			int len = findchain.GetWordChain_NoRing(result);
			Assert::AreEqual(len, 5);
			for (int i = 0; i < 4; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
		}
		TEST_METHOD(TestMethod3)
		{
			char* words[10] = { "ab","cd","ed", "ef", "rw", "tw" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 0, 0);
			int len = findchain.GetWordChain_NoRing(result);
			Assert::AreEqual(len, 1);
		}
		TEST_METHOD(TestMethod4)
		{
			char* words[10] = { "ab","acccccccccccccc","bc", "cd", "de", "dd" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 0, 0, 0);
			int len = findchain.GetWordChain_NoRing(result);
			Assert::AreEqual(len, 4);
			for (int i = 0; i < 3; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
		}
		TEST_METHOD(TestMethod5)
		{
			char* words[10];
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 0, 0, 0, 0);
			int len = findchain.GetWordChain_NoRing(result);
			Assert::AreEqual(len, 0);
		}
		TEST_METHOD(TestMethod6)
		{
			char* words[10] = { "abb","acc","bc", "cd", "de", "dd" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 'a','c');
			int len = findchain.GetWordChain_NoRing(result);
			Assert::AreEqual(len, 2);
		}
		TEST_METHOD(TestMethod7)
		{
			char* words[10] = { "aa","cc","bb", "dd", "ee", "rr" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 'c', 0);
			int len = findchain.GetWordChain_NoRing(result);
			Assert::AreEqual(len, 1);
		}
		TEST_METHOD(TestMethod8)
		{
			char* words[10] = { "abb","bc", "cd", "de", "drrrrr","rrrrr" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 0, 0);
			int len = findchain.GetWordChain_NoRing(result);
			Assert::AreEqual(len, 5);
			for (int i = 0; i < 4; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
		}
	};



	TEST_CLASS(UnitTestGetWordChain_Ring)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			char* words[10] = { "ab","bc","ca", "cd", "de", "dee" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 1, 0, 0);
			int len = findchain.GetWordChain_Ring(result);
			Assert::AreEqual(len, 5);
			for (int i = 0; i < 4; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
		}

		TEST_METHOD(TestMethod2)
		{
			char* words[10] = { "ab","bc","ca", "cd", "de", "dee" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 0,'a', 0);
			int len = findchain.GetWordChain_Ring(result);
			Assert::AreEqual(len, 4);
			for (int i = 0; i < 3; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
			Assert::AreEqual(result[len - 1], "dee");
		}
		TEST_METHOD(TestMethod3)
		{
			char* words[10] = { "ab","bc","ca", "cd", "de", "dee" };
			char* result[105];
			FindChain findchain;
			findchain.BuildMap(words, 6, 0, 'c', 'c');
			int len = findchain.GetWordChain_Ring(result);
			Assert::AreEqual(len, 3);
			for (int i = 0; i < 2; i++)
			{
				int slen = strlen(result[i]);
				Assert::IsTrue(result[i][slen - 1] == result[i + 1][0]);
			}
			Assert::AreEqual(result[0], "ca");
			Assert::AreEqual(result[len - 1], "bc");
		}
	};

}