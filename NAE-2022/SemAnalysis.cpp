#include "stdafx.h"
#include "LT.h"
#include "SemAnalysis.h"
#include "Log.h"
namespace Semantic
{
	string arr[100];
	int l = 0;
	bool Analyze(Lex::LEX& tables, Log::LOG& log)
	{
		bool sem_ok = true;


		for (int i = 0; i < tables.lextable.size; i++)
		{
			switch (tables.lextable.table[i].lexema)
			{
			case LEX_EQUAL:
			{

				if (tables.idtable.table[tables.lextable.table[i - 1].idxTI].iddatatype != tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype)
				{
					sem_ok = false;
					Log::WriteError(log, Error::geterrorin(312, tables.lextable.table[i].line, -1));
				}
				break;

			}
			

			case LEX_LET:
			{
				/*for (int j = i + 1; tables.lextable.table[j].lexema == LEX_SEMICOLON; j++)
				{
					if (tables.lextable.table[i].lexema == LEX_LET)
					{
						arr[l] = tables.idtable.table[tables.lextable.table[i + 2].idxTI].id;
						l++;
						for (int k = 0; k < l; k++)
							cout << arr[k] << endl;
					}

				}*/
				
				if (tables.lextable.table[i + 1].lexema != LEX_INT)
				{
					sem_ok = false;
					Log::WriteError(log, Error::geterrorin(306, tables.lextable.table[i].line, -1));
					//std::cout << "fdfd";
					break;
				}
			}

			
			case LEX_INT:
			{
				/*if (tables.lextable.table[i + 1].lexema == LEX_FUNCTION)
				{
					for (int k = 0; i < tables.lextable.size; k++)
					{
					if (tables.idtable.table[tables.lextable.table[i + 2].idxTI].id == tables.idtable.table[tables.lextable.table[k].idxTI].id)
					{
						sem_ok = false;
						Log::WriteError(log, Error::geterrorin(313, tables.lextable.table[i].line, -1));
						break;
					}
					}
					
				}*/
				if (tables.lextable.table[i + 1].lexema == LEX_ID && tables.lextable.table[i - 1].lexema != LEX_LET && tables.idtable.table[tables.lextable.table[i + 1].idxTI].idtype != IT::P)
				{
					sem_ok = false;
					Log::WriteError(log, Error::geterrorin(304, tables.lextable.table[i].line, -1));
					//std::cout << "ff";
					break;
				}
			}

			case LEX_ID:
			{
				if (tables.lextable.table[i - 1].lexema == LEX_FUNCTION)
				{
					if (tables.lextable.table[i + 1].lexema == LEX_LEFTTHESIS && tables.lextable.table[i - 1].lexema == LEX_FUNCTION)
					{
						for (int j = i + 1 ; tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
						{
							if (tables.lextable.table[j].lexema == LEX_ID && tables.lextable.table[j - 1].lexema == LEX_INT)
							{
								tables.idtable.table[tables.lextable.table[i].idxTI].countOfPar++;
								tables.idtable.table[tables.lextable.table[i].idxTI].types[tables.idtable.table[tables.lextable.table[i].idxTI].countOfPar - 1] = tables.idtable.table[tables.lextable.table[j].idxTI].iddatatype;
							}
						}
					}
					IT::IDDATATYPE ftype = tables.idtable.table[tables.lextable.table[i].idxTI].iddatatype;
					int j = i + 1;
					while (tables.lextable.table[j].lexema != LEX_GET)
						j++;
					if (ftype != tables.idtable.table[tables.lextable.table[j + 1].idxTI].iddatatype)
					{
						sem_ok = false;
						Log::WriteError(log, Error::geterrorin(314, tables.lextable.table[i].line, -1));
					}
				}
				if (tables.lextable.table[i + 1].lexema == LEX_LEFTTHESIS && tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
				{
					int parms = 0;
					for (int j = i + 1; tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
					{
						if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL)
						{
							parms++;
							//cout << parms;
						}
					}
					if (parms != tables.idtable.table[tables.lextable.table[i].idxTI].countOfPar)
					{
						sem_ok = false;
						Log::WriteError(log, Error::geterrorin(317, tables.lextable.table[i].line, -1));
					}
				}
				if (tables.lextable.table[i + 1].lexema == LEX_LEFTTHESIS && tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
				{
					if(tables.idtable.table[tables.lextable.table[i - 1].idxTI].id == "srtcpy" || tables.idtable.table[tables.lextable.table[i - 1].idxTI].id == "concat")
					{
						break;
					}
					int parms = 0;
					for (int j = i + 1; tables.lextable.table[j].lexema != LEX_RIGHTTHESIS; j++)
					{
						if (tables.lextable.table[j].lexema == LEX_ID || tables.lextable.table[j].lexema == LEX_LITERAL)
						{
							parms++;
							IT::IDDATATYPE ctype = tables.idtable.table[tables.lextable.table[j].idxTI].iddatatype;
							if (ctype != tables.idtable.table[tables.lextable.table[i].idxTI].types[parms - 1])
							{
								sem_ok = false;
								Log::WriteError(log, Error::geterrorin(309, tables.lextable.table[i].line, -1));
							}
						}
					}
				}
				break;
			}
			case LEX_DIRSLASH:
			{

				if (tables.lextable.table[i + 1].lexema == 'v')
				{
					sem_ok = false;
					Log::WriteError(log, Error::geterrorin(310, tables.lextable.table[i].line, -1));
				}
				else if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::SGN|| tables.idtable.table[tables.lextable.table[i + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
				{
					sem_ok = false;
					Log::WriteError(log, Error::geterrorin(310, tables.lextable.table[i].line, -1));
				}
				else if (tables.lextable.table[i].op == 3)
				{
					if (tables.idtable.table[tables.lextable.table[i + 1].idxTI].value.vint == 0)
					{
						sem_ok = false;
						Log::WriteError(log, Error::geterrorin(311, tables.lextable.table[i].line, -1));
					}
				}
				break;
			}
			}
			
		}
		return sem_ok;
	}
};