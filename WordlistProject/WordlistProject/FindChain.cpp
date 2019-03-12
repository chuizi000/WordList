#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
#include "FindChain.h"
#include "Error.h"




FindChain::FindChain()
{

	head = 0; tail = 0;
	ansLen = ansNum = 0;
	memset(canGo, 0, sizeof(canGo));
	memset(useEdge, 0, sizeof(useEdge));
	memset(ansWords, 0, sizeof(ansWords));
	memset(nowWords, 0, sizeof(nowWords));

	for (int i = 0; i <26; i++)
		for (int j = 0; j<26;j++)
			edges[i][j].resize(0);

}



void FindChain::BuildMap(char * words[], int len, bool isWord, char h, char t)
{
	head = h; tail = t;
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

bool FindChain::hasRing()
{
	int inDegree[NUM];
	int n = 0;
	bool p[NUM];

	for (int i = 0; i < NUM; i++) inDegree[i] = p[i] = 0;

	for (int i = 0; i < NUM; i++)
		for (int j = 0; j < NUM; j++)
		{
			std::vector<Wordedge> &e = edges[i][j];
			if (i == j && e.size() > 1)
				return true;
			if (i == j) continue;
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

int FindChain::GetWordChain_NoRing(char * result[])// char* result[],)
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

	for (int i = 0; i < 26; i++)
		for (int j = 0; i <= 26; i++)
			if (i != j)
				for (size_t k = 0; k < edges[i][j].size(); k++)
					rd[j]++;

	for (int i = 0; i < NUM; i++)
		if (rd[i] == 0)
		{
			if (hasHead && (head - 'a') != i) continue;
			Q.push(i);
			use[i] = 1;
			parent[i] = i;
		}

	while (!Q.empty())
	{
		int top = Q.front();
		for (int j = 0; j < NUM; j++)
			if (edges[top][j].size() > 0 && j != top)
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
	if (ans == 26) return 0;
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
	int ans_len = 0;
	for (i--; i > 0; i--)
	{
		int u = road[i], v = road[i - 1];
		result[ans_len] = edges[u][v][0].word;
		ans_len++;
	}
	return ans_len;
}

void FindChain::dfs(int node, int dist, int num)
{
	//printf("%d,%d,%d\n",node,dist,num);
	char nowtail = 0;
	if (nowWords[num] != NULL)
		nowtail = nowWords[num][strlen(nowWords[num]) - 1];
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
			nowWords[num + i + 1] = edges[node][node][i].word;
		}
		dfs(node, dist + adddist, num + edges[node][node].size());
		useEdge[node][node] = 0;
		return;
	}

	for (int i = 1; i <= canGo[node][0]; i++)
	{
		int x = canGo[node][i];
		size_t& curIndex = useEdge[node][x];
		std::vector<Wordedge>& edge = edges[node][x];
		if (curIndex == edge.size()) continue;
		curIndex++;
		nowWords[num + 1] = edge[curIndex - 1].word;
		dfs(x, dist + edge[curIndex - 1].key, num + 1);
		curIndex--;
	}
}

void FindChain::updateNode(bool node[])
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

int FindChain::GetWordChain_Ring(char * result[])
{
	bool hasHead = 0;
	bool hasTail = 0;
	if (head != '\0')  hasHead = 1;
	if (tail != '\0') hasTail = 1;
	if (hasHead) dfs(head - 'a', 0, 0);
	else
	{
		bool node[NUM];
		for (int i = 0; i < NUM; i++) node[i] = false;
		for (int i = 0; i < NUM; i++)
		{
			if (node[i]) continue;
			//printf("$node %d\n",i);
			dfs(i, 0, 0);
			updateNode(node);
		}
	}
	for (int i = 0;i<ansNum;i++)
		result[i] = ansWords[i+1];
	return ansNum;
}
