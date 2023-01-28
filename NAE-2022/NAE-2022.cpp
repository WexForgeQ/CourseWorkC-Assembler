#include "stdafx.h"
#include "MFST.h"
#include "Polish.h"
#include "SemAnalysis.h"
#include "Generation.h"

int _tmain(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "Rus");

    Log::LOG log = Log::INITLOG;
    Out::OUT out = Out::INITOUT;
    try
    {
        Parm::PARM parm = Parm::getparm(argc, argv);
        log = Log::getlog(parm.log);
        out = Out::getout(parm.out);
        Log::WriteLine(log, "Тест: ", "без ошибок ", "");
        Log::WriteLine(log, L"Тест: ", L"без ошибок\n ", L"");
        Log::WriteLog(log);
        Log::WriteParm(log, parm);
        In::IN in = In::getin(parm);
        Log:WriteIn(log, in);
        Out::WriteIn(out, in);
        Lex::LEX lex = Lex::LexAnaliz(log, in);
        Lex::Synchronization(lex);
        if (!Semantic::Analyze(lex, log)) {
            std::cout << "Найдены семантические ошибки. Проверьте лог файл";
            exit(-1);
        }
        MFST::check_syntax(lex, log, *log.stream);
        Polish::startPolish(lex);
        Log::WriteLexTableLog(lex.lextable, *log.stream);
        LT::ShowTable(lex.lextable, *log.stream);
        IT::ShowTable(lex.idtable, *log.stream);
        Gen::Generator Gener(lex.lextable, lex.idtable, parm.out);
        Log::Close(log);
        Out::Close(out);
    }
    catch (Error::ERROR e)
    {
        Log::WriteError(log, e);
        Out::WriteError(out, e);
        cout << "Найдены ошбики. За подробностями првоерьте лог файл." << endl;
    }
    system("pause");
    return 0;
}
