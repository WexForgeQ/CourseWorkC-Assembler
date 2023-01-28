#include "stdafx.h"
using namespace std;

namespace Lex {

	#define max_word 1000
	#define size_word 20

	struct LEX 
	{
		LT::LexTable lextable;
		IT::IdTable idtable;
		
	};

	LEX LexAnaliz(Log::LOG log, In::IN in);
	void Synchronization(Lex::LEX& lex);


	char** divideText(char source[], int size);
	void cleanup(char source[], int size, Log::LOG logfile);

}