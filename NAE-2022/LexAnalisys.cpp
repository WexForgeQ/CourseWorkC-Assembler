#include "stdafx.h"
#include "LT.h"
#define DIV			'|'
#define SPACE		' '
#define PLUS		'+'
#define MINUS		'-'
#define STAR		'*'
#define DIRSLASH	'/'
#define MOD			'%'
#define MORE		'>'
#define LESS		'<'
#define EQU			'~'
#define NEQU		'!'
#define EQUAL		'='
#define EMORE		'^'
#define ELESS		'_'


namespace Lex {

	void cleanup(char source[], int size, Log::LOG logfile) {
		char Separators[] = { " ,;(){}=+-*/|" };
		bool findLiteral = false;
		int count = NULL;
		int count2 = NULL;

		for (int i = 0; i < size; i++) {
			if (source[i] == '\'') {
				findLiteral = !findLiteral;
				count++;
			}


			if ((source[i] == ' ' || source[i] == '\t') && !findLiteral) {
				for (int j = 0; j < sizeof(Separators) - 1; j++) {
					if (source[i + 1] == Separators[j] || source[i - 1] == Separators[j] || i == 0) {
						for (int k = i; k < size; k++) {
							source[k] = source[k + 1];
						}
						i--;
						break;
					}
				}
			}
		}

		if (count % 2 != 0)
			Log::WriteError(logfile, Error::geterror(300));
	}

	char** divideText(char source[], int size)
	{
		char** word = new char* [max_word];
		for (int i = 0; i < max_word; i++) {
			word[i] = new char[size_word] {NULL};
		}

		bool findSeparator, findLiteral = false;
		int j = 0;
		char Separators[] = { " ,;(){}=+-*/|" };
		for (int i = 0, k = 0; i < size - 1; i++, k++) {
			findSeparator = false;
			if (source[i] == '\'') findLiteral = !findLiteral;
			for (int t = 0; t < sizeof(Separators) - 1; t++) {
				if (source[i] == Separators[t] && !findLiteral) {
					findSeparator = true;
					if (word[j][0] != NULL) {
						word[j++][k] = '\0';
						k = 0;
					}
					if (Separators[t] == ' ') {
						k = -1;
						break;
					}
					word[j][k++] = Separators[t];
					word[j++][k] = '\0';
					k = -1;
					break;
				}
			}

			if (!findSeparator) word[j][k] = source[i];
		}

		word[j] = NULL;
		for (int i = 0; i < j; i++) {
			if (!strcmp((char*)word[i], "")) return NULL;
		}

		return word;
	}





	LEX LexAnaliz(Log::LOG log, In::IN in) {
		LEX lex;
		LT::LexTable lextable = LT::Create(LT_MAXSIZE);
		IT::IdTable idtable = IT::Create(TI_MAXSIZE);

		cleanup(in.text, in.size, log);

		char** word = new char* [max_word];
		for (int i = 0; i < max_word; i++)
			word[i] = new char[size_word] {NULL};

		do {
			word = divideText(in.text, in.size);
		} while (word == NULL);

		int indexLex = 0;
		int literalCounter = 1;
		int line = 1;
		int position = 0;
		bool findMain = false;
		int mainCounter = 0;
		bool findDeclaration = false;
		bool findType = false;
		bool findProc = false;
		int is_cycle = 0;

		bool findMath = false;
		bool findString = false;

		stack<string> functions;
		char* bufprefix = new char[10]{ "" };
		char* L = new char[2]{ "L" };
		char* bufL = new char[TI_STR_MAXSIZE];
		char* nameLiteral = new char[10]{ "" };
		char* charclit = new char[10]{ "" };

		bool findFunc = false;
		bool findParm = false;
		bool findSD = false;
		IT::Entry entryIT;

		for (int i = 0; word[i] != NULL; i++, indexLex++) {
			bool findSameID = false;
			position += strlen(word[i]);
			if (FST::execute(FST::FST(word[i], FST_LET))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findDeclaration = true;
			}
			else if (FST::execute(FST::FST(word[i], FST_SNUM))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_INT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findType = true;
				entryIT.iddatatype = IT::INT;
			}
			else if (FST::execute(FST::FST(word[i], FST_SIGN))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_SIGN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findType = true;
				entryIT.iddatatype = IT::SGN;
				//entryIT.value.vsgn.sgn + '\0';
			}
			else if (FST::execute(FST::FST(word[i], FST_STRING))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_STRING, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findType = true;
				entryIT.iddatatype = IT::STR;
				strcpy(entryIT.value.vstr.str, "");
			}
			else if (FST::execute(FST::FST(word[i], FST_FUNCTION))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_FUNCTION, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findFunc = true;
			}
			else if (FST::execute(FST::FST(word[i], FST_GET))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_GET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_OUT))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_OUT, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_OUTLN))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_OUTLN, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_MAIN))) {
				strcpy(entryIT.id, word[i]);
				entryIT.idxFirstLE = indexLex;
				entryIT.idtype = IT::F;
				entryIT.iddatatype = IT::INT;
				IT::Add(idtable, entryIT);
				entryIT = {};

				LT::Entry entryLT = WriteEntry(entryLT, LEX_MAIN, IT::IsId(idtable, word[i]), line);
				LT::Add(lextable, entryLT);
				functions.push(word[i]);
				findMain = true;
				mainCounter++;
			}
			else if (FST::execute(FST::FST(word[i], FST_IF))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_IF, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				is_cycle++;
			}
			else if (FST::execute(FST::FST(word[i], FST_ELSE))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_ELSE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				is_cycle++;
			}
			else if (FST::execute(FST::FST(word[i], FST_CONCAT)) || FST::execute(FST::FST(word[i], FST_STRCP))) {
				if (int idx = IT::IsId(idtable, word[i]) == TI_NULLIDX) {
					entryIT.idtype = IT::F;
					entryIT.iddatatype = IT::STR;
					entryIT.idxFirstLE = indexLex;
					entryIT.types[0] = IT::STR;
					entryIT.types[1] = IT::STR;
					entryIT.countOfPar = 2;
					strcpy(entryIT.id, word[i]);
					IT::Add(idtable, entryIT);
					entryIT = {};
				}

				LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, IT::IsId(idtable, word[i]), line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_INTLIT))) {
				int value = atoi((char*)word[i]);
				if (value > SHRT_MAX)
					throw ERROR_THROW_IN(315, line, position);
				for (int k = 0; k < idtable.size; k++) {
					if (idtable.table[k].value.vint == value && idtable.table[k].idtype == IT::L && idtable.table[k].iddatatype == IT::INT) {
						LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						findSameID = true;
						break;
					}
				}

				if (findSameID) continue;
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::INT;
				entryIT.value.vint = value;
				entryIT.idxFirstLE = indexLex;
				_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				word[i] = strcat(bufL, charclit);
				strcpy(entryIT.id, word[i]);
				IT::Add(idtable, entryIT);
				entryIT = {};

				LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, IT::IsId(idtable, word[i]), line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_STRLIT))) {
				int length = strlen(word[i]);
				for (int k = 0; k < length; k++)
					word[i][k] = word[i][k + 1];
				word[i][length - 2] = 0;

				for (int k = 0; k < idtable.size; k++) {
					if (!(strcmp(idtable.table[k].value.vstr.str, word[i]))) {
						findSameID = true;
						LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
						LT::Add(lextable, entryLT);
						break;
					}
				}
				if (findSameID) continue;

				strcpy(entryIT.value.vstr.str, word[i]);
				entryIT.value.vstr.len = length - 2;
				entryIT.idtype = IT::L;
				entryIT.iddatatype = IT::STR;
				entryIT.idxFirstLE = indexLex;

				_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
				strcpy(bufL, L);
				nameLiteral = strcat(bufL, (char*)charclit);
				strcpy(entryIT.id, nameLiteral);
				IT::Add(idtable, entryIT);
				entryIT = {};

				int k = 0;
				for (k = 0; k < idtable.size; k++) {
					if (!(strcmp(idtable.table[k].value.vstr.str, word[i])))
						break;
				}
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_SGNLIT))) {
			int length = strlen(word[i]);
			if (length > 3)
				throw ERROR_THROW_IN(316, line, position);
			for (int k = 0; k < length; k++)
				word[i][k] = word[i][k + 1];
			word[i][length - 2] = 0;

			for (int k = 0; k < idtable.size; k++) {
				if (!(strcmp(idtable.table[k].value.vstr.str, word[i]))) {
					findSameID = true;
					LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
					LT::Add(lextable, entryLT);
					break;
				}
			}
			if (findSameID) continue;

			strcpy(entryIT.value.vstr.str, word[i]);
			entryIT.value.vstr.len = length - 2;
			entryIT.idtype = IT::L;
			entryIT.iddatatype = IT::SGN;
			entryIT.idxFirstLE = indexLex;

			_itoa_s(literalCounter++, charclit, sizeof(char) * 10, 10);
			strcpy(bufL, L);
			nameLiteral = strcat(bufL, (char*)charclit);
			strcpy(entryIT.id, nameLiteral);
			IT::Add(idtable, entryIT);
			entryIT = {};

			int k = 0;
			for (k = 0; k < idtable.size; k++) {
				if (!(strcmp(idtable.table[k].value.vstr.str, word[i])))
					break;
			}
			LT::Entry entryLT = WriteEntry(entryLT, LEX_LITERAL, k, line);
			LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_ID))) {
			if (findParm)
			{
					entryIT.idtype = IT::P;
				}
					
				else if (findFunc) {
					entryIT.idtype = IT::F;
					functions.push(word[i]);
				}
				else {
					entryIT.idtype = IT::V;
					int idx = IT::IsId(idtable, word[i]);
					if (idx != TI_NULLIDX) {
						LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, idx, line);
						LT::Add(lextable, entryLT);
						entryIT = { };
						continue;
					}
					if (entryIT.iddatatype == IT::INT)
						entryIT.value.vint = TI_INT_DEFAULT;
					else if (entryIT.iddatatype == IT::STR) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vstr.str, TI_STR_DEFAULT, sizeof(char));
					}
					else if (entryIT.iddatatype == IT::SGN) {
						entryIT.value.vstr.len = 0;
						memset(entryIT.value.vsgn.sgn, TI_SGN_DEFAULT, sizeof(char));
					}
				}

				entryIT.idxFirstLE = indexLex;
				if (entryIT.idtype != IT::F && !functions.empty()) {
					strcpy(bufprefix, functions.top().c_str());
					word[i] = strcat(bufprefix, word[i]);
				}
				strcpy(entryIT.id, word[i]);
				int idx = IT::IsId(idtable, word[i]);
				if (idx == TI_NULLIDX)
					IT::Add(idtable, entryIT);
				LT::Entry entryLT = WriteEntry(entryLT, LEX_ID, IT::IsId(idtable, word[i]), line);
				LT::Add(lextable, entryLT);
				entryIT = { };
			}
			else if (FST::execute(FST::FST(word[i], FST_OPERATOR))) {
				strcpy(entryIT.id, word[i]);
				entryIT.idxFirstLE = indexLex;
				entryIT.idtype = IT::OP;
				if (IT::IsId(idtable, word[i]) == TI_NULLIDX)
					IT::Add(idtable, entryIT);
				entryIT = { };

				LT::Entry entryLT = WriteEntry(entryLT, LEX_OPERATOR, IT::IsId(idtable, word[i]), line);
				switch (word[i][0]) {
				case MORE:
					entryLT.priority = 0;
					entryLT.op = LT::operations::OMORE;
					break;
				case EMORE:
					entryLT.priority = 0;
					entryLT.op = LT::operations::OEMORE;
					break;
				case ELESS:
					entryLT.priority = 0;
					entryLT.op = LT::operations::OELESS;
					break;
				case LESS:
					entryLT.priority = 0;
					entryLT.op = LT::operations::OLESS;
					break;
				case EQU:
					entryLT.priority = 0;
					entryLT.op = LT::operations::OEQU;
					break;
				case NEQU:
					entryLT.priority = 0;
					entryLT.op = LT::operations::ONEQU;
					break;
				case PLUS:
					entryLT.priority = 2;
					entryLT.op = LT::operations::OPLUS;
					break;
				case MINUS:
					entryLT.priority = 2;
					entryLT.op = LT::operations::OMINUS;
					break;
				case DIRSLASH: 
					entryLT.priority = 3;
					entryLT.op = LT::operations::ODIV;
					break;
				case STAR:
					entryLT.priority = 3;
					entryLT.op = LT::operations::OMUL;
					break;
				case MOD:
					entryLT.priority = 3;
					entryLT.op = LT::operations::OMOD;
				}
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_SEMICOLON))) {
				if (findDeclaration && findFunc && findType)
					functions.pop();
				LT::Entry entryLT = WriteEntry(entryLT, LEX_SEMICOLON, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
				findDeclaration = findFunc = findType = findParm = false;
				entryIT = { };
			}
			else if (FST::execute(FST::FST(word[i], FST_COMMA))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_COMMA, LT_TI_NULLIDX, line);
				entryLT.priority = 1;
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_LEFTBRACE))) {
				findType = findDeclaration = findFunc = findParm = findProc = false;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LEFTBRACE, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_BRACELET))) {
				if (!functions.empty() && is_cycle == 0)
					functions.pop();
				else if (is_cycle != 0) is_cycle--;
				LT::Entry entryLT = WriteEntry(entryLT, LEX_BRACELET, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_LEFTTHESIS))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_LEFTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				LT::Add(lextable, entryLT);
				if (findFunc || findProc)
				{
					findParm = true;
				}
					
			}
			else if (FST::execute(FST::FST(word[i], FST_RIGHTTHESIS))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_RIGHTTHESIS, LT_TI_NULLIDX, line);
				entryLT.priority = 0;
				findParm = false;
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_EQUAL))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_EQUAL, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
			}
			else if (FST::execute(FST::FST(word[i], FST_EQU))) {
				LT::Entry entryLT = WriteEntry(entryLT, LEX_EQU, LT_TI_NULLIDX, line);
				LT::Add(lextable, entryLT);
			}
			else if (word[i][0] == DIV) {
				line++;
				position = 0;
				indexLex--;
			}
			else
				throw ERROR_THROW_IN(205, line, position);
		}

		if (!findMain) throw ERROR_THROW(302);
		if (mainCounter > 1) throw ERROR_THROW(301);

		lex.idtable = idtable;
		lex.lextable = lextable;
		return lex;
	}

	void Synchronization(Lex::LEX& lex) {
		bool* is_changed = new bool[lex.idtable.size]{ false };
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].idxTI != -1 && !is_changed[lex.lextable.table[i].idxTI]) {
				lex.idtable.table[lex.lextable.table[i].idxTI].idxFirstLE = i;
				is_changed[lex.lextable.table[i].idxTI] = true;
			}
		}
		delete[] is_changed;
	}
}