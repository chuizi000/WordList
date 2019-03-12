#pragma once
#ifndef FINDCHAIN_H
#define FINDCHAIN_H
#include <cstring>
#include <vector>
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
	
	int canGo[26][26];
	std::vector<Wordedge> edges[26][26];
	size_t useEdge[26][26];
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







#endif