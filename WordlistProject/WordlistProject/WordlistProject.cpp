#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <queue>
#include "WordlistProject.h"

#define NUM 26
#define MAX 100000

void error()
{
	return;
}
bool w = false, c = false, r = false;
char h = '\0', t = '\0';
char *file;

void parse(int argc, char *argv[])
{
	

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0 && !h)
		{
			i++;
			h = argv[i][0];
		}
		else if (strcmp(argv[i], "-t") == 0 && !t)
		{
			i++;
			t = argv[i][0];
		}
		else if (strcmp(argv[i], "-r") == 0 && !r)
		{
			r = true;
		}
		else if (strcmp(argv[i], "-w") == 0 && !w)
		{
			w = true;
			i++;
			file = argv[i];
		} 
		else if (strcmp(argv[i], "-c") == 0 && !c)
		{
			c = true;
			i++;
			file = argv[i];
		}
		else
		{
			error();
		}
	}
	if (w && c) error();
	if (!w && !c) error();
	if (h && !isalpha(h)) error();
	if (t && !isalpha(t)) error();
}


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

std::vector<Wordedge> edges[26][26];
int canGo[26][26];

void BuildWordMap(char* words[], int len, bool isWord)
{
	for (int i = 0; i < len; i++)
	{
		int llen = strlen(words[i]);
		int key = llen;
		if (isWord) key = 1;
		char* newWord = words[i];
		edges[newWord[0] - 'a'][newWord[llen - 1] - 'a'].push_back({ newWord,key });
	}
	for (int i = 0; i < NUM; i++)
		for (int j = 0; j < NUM; j++)
		{
			std::sort(edges[i][j].begin(), edges[i][j].end());
			std::vector<Wordedge>::iterator it;
			it = std::unique(edges[i][j].begin(), edges[i][j].end());
			edges[i][j].resize(std::distance(edges[i][j].begin(), it));
			std::unique(edges[i][j].begin(), edges[i][j].end());
		}

	for (int i = 0; i < NUM; i++)
		for (int j = 0; j < NUM; j++)
		{
			if (i == j) continue;
			if (edges[i][j].size() > 0)
			canGo[i][++canGo[i][0]] = j;
		}
}

bool hasRing()
{
	int inDegree[NUM];
	int n = 0;
	bool p[NUM];

	for (int i = 0; i < NUM; i++) inDegree[i] = 0;
	for (int i = 0; i < NUM; i++)
		for (int j = 0; j < NUM; j++)
		{
			std::vector<Wordedge> &e = edges[i][j];
			if (i == j && e.size() > 1)
				return true;
			if (e.size() > 0)
			{
				inDegree[j]++;
				p[i] = p[j] = true;
			}
		}
	for (int i = 0; i < NUM; i++) n += p[i];

	std::queue<int> Q;
	for (int i = 0; i < NUM; i++)
		if (inDegree[i] == 0 && p[i])
		{
			Q.push(i);
		}
	int cnt = 0;
	while (!Q.empty())
	{
		int newP = Q.front();
		Q.pop();
		cnt++;
		for (int i = 0; i < NUM; i++)
		{
			if (edges[newP][i].size() == 0) continue;
			inDegree[i]--;
			if (inDegree[i] == 0)
			{
				Q.push(i);
			}
		}
	}
	if (cnt == n)
		return false;
	return true;
}


int GetWordChain_NoRing(char head, char tail)// char* result[],)
{
	int dist[27];
	int use[26];
	int rd[26];
	int parent[26];
	std::queue<int> Q;
	memset(dist, 0, sizeof(dist));
	memset(rd, 0, sizeof(rd));
	memset(use, 0, sizeof(use));
	memset(parent, -1, sizeof(parent));
	bool hasHead = 0;
	bool hasTail = 0;
	if (head != '\0')  hasHead = 1;
	if (tail != '\0') hasTail = 1;

	for (int i=0;i<26;i++)
		for(int j=0;i<=26;i++)
			if (i!=j)
			for (size_t k = 0; k < edges[i][j].size(); k++)
				rd[j]++;
		
	for (int i = 0; i < NUM; i++)
		if (rd[i] == 0)
		{
			if (hasHead && (head-'a')!=i) continue;
			Q.push(i);
			use[i] = 1;
			parent[i] = i;
		}

	while (!Q.empty())
	{
		int top = Q.front();
		for (int j = 0;j<NUM;j++)
		if (edges[top][j].size()>0 && j!=top)
		{
			int newdis = dist[top] + edges[top][j][0].key;
			if (edges[top][top].size() > 0) 
				newdis += edges[top][top][0].key;
			if (dist[j] < newdis)
			{
				dist[j] = newdis;
				parent[j] = top;
				if (use[j] == 0) Q.push(j);
			}
		}
		use[top] = 0;
		Q.pop();
	}
	int ans = 26;
	for (int i = 0; i < NUM; i++)
		if (dist[ans] < dist[i])
		{
			if (hasTail && (tail - 'a') != i) continue;
			ans = i;
		}
	if (ans == 26) return dist[ans];
	int road[100];
	int i = 0, child = ans;
	road[i++] = ans;
	if (edges[ans][ans].size() > 0) road[i++] = ans;
	while (1)
	{
		int pa = parent[child];
		if (pa == child) break;
		road[i++] = pa;
		if (edges[pa][pa].size() > 0) road[i++] = pa;
		child = pa;
	}
	for (i--; i > 0 ; i--)
	{
		int u = road[i], v = road[i - 1];
		printf("%s\n", edges[u][v][0].word);
	}
	return dist[ans];
}


size_t useEdge[NUM][NUM];
int ansChar;
int ansLen;
int ansNum;
char* ansWords[1000];
char* nowWords[1000];
int maxAns[NUM];
int headnow;

void dfs(int node, int dist,int num,char tail)
{
	char nowtail = 0;
	if (nowWords[num]!=NULL)
	nowtail = nowWords[num][strlen(nowWords[num]) - 1];
	if (dist > maxAns[headnow])
	{
		if (tail != '\0' &&  nowtail != (tail));
		else
		{
			maxAns[headnow] = dist;
		}
	}
	if (dist > ansLen)
	{
		if (tail != '\0' &&  nowtail != (tail));
		else
		{
			ansLen = dist;
			for (int i = 1; i <= num; i++)
				ansWords[i] = nowWords[i];
			ansNum = num;
		}
	}
	if (edges[node][node].size() > useEdge[node][node])
	{
		useEdge[node][node] = edges[node][node].size();
		int adddist = 0;
		for (size_t i = 0; i < edges[node][node].size(); i++)
		{
			adddist += edges[node][node][i].key;
			nowWords[num + i+1] = edges[node][node][i].word;
		}
		dfs(node,dist+adddist,num+edges[node][node].size(),tail);
		useEdge[node][node] = 0;
		return;
	}

	for (int i = 1; i <= canGo[node][0]; i++)
	{
		int x = canGo[node][i];
		size_t& curIndex = useEdge[node][x];
		std::vector<Wordedge>& edge = edges[node][x];
		if (curIndex == edge.size()) continue;
		//if (headnow > x)
		//	if (edge[curIndex].key + dist + maxAns[x] < ansLen) continue;                   
		curIndex++;
		nowWords[num + 1] = edge[curIndex - 1].word;
		dfs(x,dist+edge[curIndex-1].key,num+1,tail);
		curIndex--;
	}
}

void updateNode(bool node[])
{
	int cnt[NUM];
	memset(cnt, 0, sizeof(cnt));
	for (int i = 1; i <= ansNum; i++)
	{
		cnt[ansWords[i][0] - 'a'] ++;
	}
	for (int i = 0; i < NUM; i++)
	{
		//printf("i = %d ", cnt[i]);
		for (int j = 0; j < NUM; j++)
		{
			cnt[i] -= edges[i][j].size();
		}
		if (cnt[i] == 0) node[i] = true;
		//printf(",  i = %d \n", cnt[i]);
	}
}

int GetWordChain_Ring(char head, char tail)
{
	bool hasHead = 0;
	bool hasTail = 0;
	if (head != '\0')  hasHead = 1;
	if (tail != '\0') hasTail = 1;
	if (hasHead) dfs(head-'a', 0,0,tail);
	else
	{
		bool node[NUM];
		for (int i = 0; i < NUM; i++) node[i] = false;
		for (int i = 0; i < NUM; i++)
		{
			//if (node[i]) continue;
			//printf("^^%d\n", i);
			headnow = i;
			dfs(i, 0, 0, tail);
			//updateNode(node);
		}
	}
	
	printf("!!!%d\n", ansLen);
	for (int i = 1; i <= ansNum; i++)
	{
		printf("%s\n",ansWords[i]);
	}

	return ansLen;
}

void Init(char *words[],int &n,char *file)
{
	FILE *pf = fopen(file, "r");
	char text[100000];
	size_t size = 0;
	while ((text[size] = fgetc(pf)) != EOF) size++;
	text[size] = '\0';
	char stream[100];
	size_t j = 0, i = 0;
	while (1)
	{
		if (text[i] == '\0')
		{
			if (j > 0)
			{
				stream[j] = '\0';
				words[n] = (char*)malloc(j + 1);
				memcpy(words[n], stream, j + 1);
				n++;
				j = 0;
			}
			break;
		}
		
		if (isalpha(text[i]))
		{
			stream[j] = tolower(text[i]);
			j++;
		}
		else if (j > 0)
		{
			stream[j] = '\0';
			words[n] = (char*)malloc(j + 1);
			memcpy(words[n], stream, j + 1);
			n++;
			j = 0;
		}
		i++;
	}
}

	char*  wordlist[10000];
	int n = 0;

int main(int argc, char *argv[])
{
	parse(argc, argv);
	
	Init(wordlist,n,file);
	int isWord = 0;
	if (w) isWord = 1;
	BuildWordMap(wordlist, n, isWord);
	if (r)
	{
		GetWordChain_Ring(h, t);
	}
	else
	{
		if (hasRing()) error();
		GetWordChain_NoRing(h, t);
	}

	return 0;
}
