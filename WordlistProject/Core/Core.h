#ifdef CoreDll
#define CoreAPI _declspec(dllexport)
#else
#define CoreAPI  _declspec(dllimport)
#endif


#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>


#define NUM 26
#define MAX 100000

void error(int e_code);

class Init
{
public:

	bool w, c, r;
	char h, t;
	char *file;

	Init();
	void parse(int argc, char *argv[]);
	int InitWord(char* words[], char* text);
	void ReadFile(char* text);
};

struct Wordedge
{
	char* word;
	int key;
	bool operator < (const Wordedge &w) const
	{
		if (key == w.key)
		{
			return strcmp(word, w.word) > 0;
		}
		return key > w.key;
	}

	bool operator == (const Wordedge &w) const
	{
		return strcmp(word, w.word) == 0;
	}
};

class FindChain
{
public:

	int canGo[30][30];
	std::vector<Wordedge> edges[30][30];
	size_t useEdge[30][30];
	int ansLen;
	int ansNum;
	char* ansWords[105];
	char* nowWords[105];


	char head;
	char tail;

	FindChain();
	void BuildMap(char* words[], int len, bool isWord, char h, char t);
	bool hasRing();
	int GetWordChain_NoRing(char * result[]);
	void dfs(int node, int dist, int num);
	int GetWordChain_Ring(char * result[]);
	void updateNode(bool node[]);

};


extern "C" CoreAPI int build_map(char* words[], char* text);
extern "C" CoreAPI int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
extern "C" CoreAPI int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
