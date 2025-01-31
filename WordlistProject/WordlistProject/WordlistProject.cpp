#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Error.h"
#include "Initialization.h"
#include "FindChain.h"
#include "Core.h"
#include <exception>  
#include <iostream>
#include <windows.h>

typedef int(*p_gen_chain_word)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
typedef int(*p_gen_chain_char)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
typedef int(*p_build_map)(char* words[], char* text);
char * result[105];

char * words[10010];
char text[1000010];
int result_len = 0;

int main(int argc, char *argv[])
{
	try
	{
		HINSTANCE CoreDLL = LoadLibrary("Core.dll");
		if (CoreDLL == NULL)
		{
			throw "File Core.dll not found";
		}


		p_gen_chain_word gen_chain_word = (p_gen_chain_word)GetProcAddress(CoreDLL, "gen_chain_word");
		p_gen_chain_char gen_chain_char = (p_gen_chain_char)GetProcAddress(CoreDLL, "gen_chain_char");
		p_build_map build_map = (p_build_map)GetProcAddress(CoreDLL, "build_map");
		if (gen_chain_word == NULL || gen_chain_char == NULL || build_map == NULL)
		{
			throw "File Core.dll not found";

		}

		Core core;
		Init* init = core.init_word(argc, argv);
		core.read_file(init, text);

		int n = build_map(words, text);
		if (init->w) result_len = gen_chain_word(words, n, result, init->h, init->t, init->r);
		else result_len = gen_chain_char(words, n, result, init->h, init->t, init->r);
		core.printf_chain(result, result_len);
	}
	catch (const char* msg)
	{
		printf("%s", msg);
		return 1;
	}
	return 0;
}

