#pragma once
#include "stdafx.h"

namespace Polish {
	void startPolish(Lex::LEX& lex);
	bool polishNotation(int i, Lex::LEX& lex);
}