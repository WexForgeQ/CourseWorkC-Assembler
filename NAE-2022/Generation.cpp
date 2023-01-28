#include "Generation.h"

using namespace std;

namespace Gen {
	Generator::Generator(LT::LexTable plexT, IT::IdTable pidT, wchar_t pout[])
	{
		lexT = plexT;
		idT = pidT;
		out = ofstream(pout, ios_base::out);

		Head();
		Const();
		Data();
		Code();
	}

	void Generator::Head() {
		out << ".586\n";
		out << ".model flat, stdcall\n";

		out << "includelib libucrt.lib\n";
		out << "includelib kernel32.lib\n";
		out << "ExitProcess PROTO :DWORD\n\n";

		out << "EXTRN concat: proc\n";
		out << "EXTRN strcp: proc\n";
		out << "EXTRN outnum: proc\n";
		out << "EXTRN outstr: proc\n";
		out << "EXTRN outchar: proc\n";
		out << "EXTRN outnumln: proc\n";
		out << "EXTRN outstrln: proc\n";
		out << "EXTRN outcharln: proc\n";

		out << "\n.stack 4096\n\n";
	}

	void Generator::Const() {
		out << ".const\n";
		for (int i = 0; i < idT.size; i++) {
			if (idT.table[i].idtype == IT::L) {
				out << "\t" << idT.table[i].id;
				if (idT.table[i].iddatatype == IT::STR)
					out << " BYTE \"" << idT.table[i].value.vstr.str << "\", 0";
				if(idT.table[i].iddatatype == IT::SGN)
					out << " BYTE \"" << idT.table[i].value.vsgn.sgn << "\", 0";
				if (idT.table[i].iddatatype == IT::INT)
					out << " SDWORD " << idT.table[i].value.vint;
				out << "\n";
			}
		}
	}

	void Generator::Data() {
		out << "\n.data\n";
		out << "\tbuffer BYTE 256 dup(0)\n";
		for (int i = 0; i < lexT.size; i++) {
			if (lexT.table[i].lexema == LEX_LET) {
				if (idT.table[lexT.table[i + 2].idxTI].idtype == IT::V) {
					out << "\t" << idT.table[lexT.table[i + 2].idxTI].id;
					if (idT.table[lexT.table[i + 2].idxTI].iddatatype == IT::STR || idT.table[lexT.table[i + 2].idxTI].iddatatype == IT::SGN)
						out << " DWORD ?\n";
					if (idT.table[lexT.table[i + 2].idxTI].iddatatype == IT::INT)
						out << " SDWORD 0\n";
				}
			}
		}
	}

	void Generator::Code() {
		stack<char*> stk;
		int num_of_points = 0,
			num_of_ret = 0,
			num_of_ends = 0,
			num_of_cycles = 0,
			countParm = 0;
		string strret = string(),
			cycle_code = string(),
			func_name = string();
		bool flag_func = false,
			flag_ret = false,
			flag_body = false,
			flag_if = false,
			flag_then = false,
			flag_else = false,
			flag_proc = false,
			flag_callfunc = false,
			flag_condition = false,
			flag_cycle = false;

		int result_position;

		out << "\n.code\n\n";
		for (int i = 0; i < lexT.size; i++) {
			switch (lexT.table[i].lexema) {
			case LEX_FUNCTION:
				while (lexT.table[i].lexema != LEX_RIGHTTHESIS) {
					if (lexT.table[i].lexema == LEX_ID && idT.table[lexT.table[i].idxTI].idtype == IT::F) {
						out << (func_name = string((char*)idT.table[lexT.table[i].idxTI].id)) << " PROC ";
					}

					if (lexT.table[i].lexema == LEX_ID && idT.table[lexT.table[i].idxTI].idtype == IT::P) {
						out << idT.table[lexT.table[i].idxTI].id << " : ";
						if (idT.table[lexT.table[i].idxTI].iddatatype == IT::INT)
							out << "SDWORD";
						else
							out << "DWORD";
					}

					if (lexT.table[i].lexema == LEX_COMMA)
						out << ", ";

					i++;
				}
				flag_func = true;
				out << "\n";
				break;

			case LEX_MAIN:
				flag_body = true;
				out << "main PROC\n";
				break;

			case LEX_EQUAL:
				result_position = i - 1;
				while (lexT.table[i].lexema != LEX_SEMICOLON) {
					switch (lexT.table[i].lexema) {
					case LEX_ID:
					case LEX_LITERAL:
						if (idT.table[lexT.table[i].idxTI].idtype == IT::F)
							flag_callfunc = true;
						if (idT.table[lexT.table[i].idxTI].iddatatype == IT::INT) {
							out << "\tpush " << idT.table[lexT.table[i].idxTI].id << "\n";
							stk.push(idT.table[lexT.table[i].idxTI].id);
							break;
						}
						if (idT.table[lexT.table[i].idxTI].iddatatype == IT::STR || idT.table[lexT.table[i].idxTI].iddatatype == IT::SGN) {
							char* s;
							if (idT.table[lexT.table[i].idxTI].idtype == IT::L) {
								 out<<"\tpush offset "<<idT.table[lexT.table[i].idxTI].id << "\n";
								s = new char[8]{ "offset " };
							}
							else {
								out << "\tpush " << idT.table[lexT.table[i].idxTI].id << "\n";
								s = new char[1]{ "" };
							}

							size_t len1 = strlen((char*)s);
							size_t len2 = strlen((char*)idT.table[lexT.table[i].idxTI].id);
							char* result = (char*)malloc(len1 + len2 + 1);
							memcpy(result, s, len1);
							memcpy(result + len1, (char*)idT.table[lexT.table[i].idxTI].id, len2 + 1);
							stk.push(result);
							break;
						}

					case LEX_OPERATOR:
						switch (lexT.table[i].op) {
						case LT::OMUL:
							out << "\tpop eax\n\tpop ebx\n";
							out << "\tmul ebx\n\tpush eax\n";
							break;
						case LT::OPLUS:
							out << "\tpop eax\n\tpop ebx\n";
							out << "\tadd eax, ebx\n\tpush eax\n";
							break;
						case LT::OMINUS:
							out << "\tpop ebx\n\tpop eax\n";
							out << "\tsub eax, ebx\n\tpush eax\n";
							break;
						case LT::ODIV:
							out << "\tpop ebx\n\tpop eax\n";
							out << "\tcdq\n\tidiv ebx\n\tpush eax\n";
							break;
						case LT::OMOD:
							out << "\tpop ebx\n\tpop eax\n";
							out << "\tcdq\n\tidiv ebx\n\tpush edx\n";
							break;
						}
						break;

					case '@':
						countParm = (char)lexT.table[i + 1].lexema - '0';

						for (int j = 1; j <= countParm; j++)
							out << "\tpop edx\n";

						for (int j = 1; j <= countParm; j++) {
							out << "\tpush " << stk.top() << "\n";
							stk.pop();
						}
						out << "\tcall " << idT.table[lexT.table[i].idxTI].id << "\n\tpush eax\n";
						flag_callfunc = false;
						break;
					}

					i++;
				}

				out << "\tpop " << idT.table[lexT.table[result_position].idxTI].id << "\n";
				break;

			case '@':
				countParm = (char)lexT.table[i + 1].lexema - '0';
				for (int j = 1; j <= countParm; j++) {
					if (idT.table[lexT.table[i - j].idxTI].iddatatype == IT::INT)
						out << "\tpush " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					else {
						if (idT.table[lexT.table[i - j].idxTI].idtype == IT::L)
							out << "\tpush offset " << idT.table[lexT.table[i - j].idxTI].id << "\n";
						else
							out << "\tpush " << idT.table[lexT.table[i - j].idxTI].id << "\n";
					}
				}

				out << "\tcall " << idT.table[lexT.table[i - countParm - 1].idxTI].id << "\n";
				break;

			case LEX_GET:
				if(idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::INT)
					out << "\tpush ";
				else
					out<< "\tpush offset ";
				if (idT.table[lexT.table[i + 1].idxTI].idtype == IT::L)
				{
					if(idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::INT)
						out << idT.table[lexT.table[i + 1].idxTI].value.vint << "\n";
					else if(idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::STR)
						out << idT.table[lexT.table[i + 1].idxTI].id << "\n";
					else if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::STR)
						out << idT.table[lexT.table[i + 1].idxTI].id
						<< "\n";
				}
				else
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n";
				if (flag_func) {
					out << "\tjmp local" << num_of_ret << "\n";
					flag_ret = true;
				}
				if (flag_body) {
					out << "\t\tjmp theend\n";
					flag_ret = true;
				}
				break;

			case LEX_BRACELET:
				if (flag_body && !flag_then && !flag_else && !flag_func) {
					if (flag_ret) {
						out << "theend:\n";
						flag_ret = false;
					}
					out << "\tcall ExitProcess\nmain ENDP\nend main";
				}
				if (flag_func && !flag_then && !flag_else) {
					if (flag_ret) {
						out << "local" << num_of_ret++ << ":\n";
						out << "\tpop eax\n\tret\n";
						flag_ret = false;
					}
					if (flag_proc) {
						out << "\tret\n";
						flag_proc = false;
					}
					out << func_name << " ENDP\n\n";
					flag_func = false;
				}
				if (flag_then) {
					flag_then = false;
					if (flag_else) {
						out << "\tjmp e" << num_of_ends << "\n";
						flag_else = false;
					}
					out << "m" << num_of_points++ << ":\n";
				}
				if (flag_else) {
					flag_else = false;
					out << "e" << num_of_ends++ << ":\n";
				}
				break;

			case LEX_IF:
				flag_if = true;
				break;

			case LEX_ELSE:
				flag_else = true;
				break;

			case LEX_LEFTTHESIS:
				if (flag_if) {
					out << "\tmov eax, " << idT.table[lexT.table[i + 1].idxTI].id << "\n";
					out << "\tcmp eax, " << idT.table[lexT.table[i + 3].idxTI].id << "\n";

					if (lexT.table[i + 2].op == LT::OMORE) {
						out << "\tjg m" << num_of_points << "\n";
						out << "\tjl m" << num_of_points + 1 << "\n";
					}
					else if (lexT.table[i + 2].op == LT::OLESS) {
						out << "\tjl m" << num_of_points << "\n";
						out << "\tjg m" << num_of_points + 1 << "\n";
					}
					else if (lexT.table[i + 2].op == LT::OEQU) {
						out << "\tjz m" << num_of_points << "\n";
						out << "\tjnz m" << num_of_points + 1 << "\n";
					}
					else if (lexT.table[i + 2].op == LT::ONEQU) {
						out << "\tjnz m" << num_of_points << "\n";
						out << "\tjz m" << num_of_points + 1 << "\n";
					}
					else if (lexT.table[i + 2].op == LT::OEMORE) {
						out << "\tjge m" << num_of_points << "\n";
						out << "\tjle m" << num_of_points + 1 << "\n";
					}
					else if (lexT.table[i + 2].op == LT::OELESS) {
						out << "\tjle m" << num_of_points << "\n";
						out << "\tjge m" << num_of_points + 1 << "\n";
					}
					out << "\tje m" << num_of_points + 1 << "\n";
					int j = i;
					while (lexT.table[j++].lexema != LEX_BRACELET) {
						if (lexT.table[j + 1].lexema == LEX_ELSE) {
							flag_else = true;
							break;
						}
					}
				}
				break;

			case LEX_RIGHTTHESIS:
				if (lexT.table[i + 1].lexema == LEX_LEFTBRACE && flag_if) {
					flag_then = true;
					out << "m" << num_of_points++ << ":\n";
					flag_if = false;
				}
				break;

			case LEX_OUT:
				if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::INT)
					out << "\tpush " << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall outnum\n";
				else if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::STR) {
					if (idT.table[lexT.table[i + 1].idxTI].idtype == IT::L)
						out << "\tpush offset ";
					else
						out << "\tpush ";
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall outstr\n";
				}
				else if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::SGN) {
					if (idT.table[lexT.table[i + 1].idxTI].idtype == IT::L)
						out << "\tpush offset ";
					else
						out << "\tpush ";
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall outchar\n";
				}
				break;

			case LEX_OUTLN:
				if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::INT)
					out << "\tpush " << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall outnumln\n";
				else if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::STR) {
					if (idT.table[lexT.table[i + 1].idxTI].idtype == IT::L)
						out << "\tpush offset ";
					else
						out << "\tpush ";
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall outstrln\n";
				}
				else if (idT.table[lexT.table[i + 1].idxTI].iddatatype == IT::SGN) {
					if (idT.table[lexT.table[i + 1].idxTI].idtype == IT::L)
						out << "\tpush offset ";
					else
						out << "\tpush ";
					out << idT.table[lexT.table[i + 1].idxTI].id << "\n\tcall outcharln\n";
				}
				break;
			}
		}
	}
}