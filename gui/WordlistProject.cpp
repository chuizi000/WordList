#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include "Initialization.h"
#include "FindChain.h"
#include "Core.h"

char * result[105];
char text[1000000];
int result_len = 0;

int main(int argc, char *argv[])
{
	
	Core core;
	Init init = core.init_word(argc,argv);
	core.read_file(init,text);
	core.build_map(init,text);
	if (init.w) result_len = core.gen_chain_word(init.words,init.n,result,init.h,init.t,init.r);
	else result_len = core.gen_chain_char(init.words, init.n, result, init.h, init.t, init.r);
	core.printf_chain(result, result_len);

	return 0;
}
