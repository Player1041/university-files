#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <algorithm>

using namespace std;
using namespace string_literals;

int main() {

    string words[100] = {
        "stark", "catch", "buses", "ralph", "halls", "quest", "virus", "lusts", "heels", "welsh",
        "nacho", "truer", "rants", "piper", "rapid", "pesky", "lunar", "upper", "taxes", "heavy",
        "heaps", "mount", "poles", "laser", "upset", "blond", "pools", "gemma", "fixed", "birds",
        "couch", "thick", "baton", "slack", "deity", "pains", "climb", "mills", "rhino", "wacko",
        "luton", "hedge", "peels", "smile", "brill", "talks", "lures", "goods", "hello", "churn",
        "heals", "props", "privy", "snarl", "quack", "fails", "dined", "swell", "flank", "repay",
        "tummy", "elect", "blues", "guide", "prick", "parts", "genes", "party", "manly", "armor",
        "poles", "wheat", "coral", "goons", "outdo", "moons", "proms", "shall", "smith", "ariel",
        "rocky", "swiss", "smith", "meats", "rehab", "waltz", "cures", "upset", "reset", "viper",
        "soars", "gusto", "floor", "paint", "lambs", "crank", "motto", "crush", "vroom", "vicar"
    };

    string fore_white = "\u001b[37;1m";
    string fore_black = "\u001b[30m";

    string back_black = "\u001b[40m";
    string back_yellow = "\u001b[43m";
    string back_green = "\u001b[42m";
    string back_white = "\u001b[47m";

    string word = words[rand() % 100];
    string word_letters[5];
    int display;
    string used;
    int tries;
    string guess;

    for (char& letter : word) {
		word_letters.push_back(letter);
    }

    while (true) {

        cout << word << endl;
        cout << "Please guess the word: ";
        cin >> guess;

        if (guess.length() != 5) {
            if (guess.empty() || std::all_of(guess.begin(), guess.end(), [](char c) {return std::isspace(c); })) {
                cout << "Please enter a valid 5 letter word." << endl;
            };
            cout << "Please enter a valid 5 letter word." << endl;
        };

		cout << word_letters[0] << word_letters[1] << word_letters[2] << word_letters[3] << word_letters[4] << endl;

        /*for index, letter in enumerate(guess) :
            if letter in used and word.count(letter) == 1 :
                display += f"{fore_white}{back_black}{letter}{reset}"
                elif letter == word[index] :
                display += f"{fore_black}{back_green}{letter}{reset}"
                used.append(letter)
                elif letter in word and letter != word[index] :
                display += f"{fore_black}{back_yellow}{letter}{reset}"
                used.append(letter)
            else:
        display += f"{fore_white}{back_black}{letter}{reset}"*/
    }
    return 0;
}