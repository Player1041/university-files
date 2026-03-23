#pragma once
#include <string>

class Question
{
	int QUESTION_NUMBER;
	enum TYPE {
		TIMES_TABLES,
		ARITHMETIC,
		DIVISION,
		ALPHABET,
		SPELLING
	};

	string QUESTION_NAME;
	string QUESTION_DESCRIPTION;
};

