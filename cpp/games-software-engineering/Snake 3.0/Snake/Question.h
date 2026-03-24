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

	std::string QUESTION_NAME;
	std::string QUESTION_DESCRIPTION;
};

