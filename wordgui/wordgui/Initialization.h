#ifndef INITIALIZATION_H
#define INITIALIZATION_H


class Init
{
public:

	bool w, c, r;
	char h, t;
	char *file;
	char*  words[10000];
	int n;

	Init();
	void parse(int argc, char *argv[]);
	void InitWord(char* text);
	void ReadFile(char* text);
};
#endif