#ifndef Questions

#include <iostream>
#include "random.h"
#include <vector>
#include <string>
#include <string_view>
#include <cassert>
#include <cctype> // for std::isalpha()
#include <limits> // for std::numeric_limits

namespace WordList
{
	using sv = std::string_view;
	const std::vector<sv> words{ "mystery", "broccoli", "account", "almost"
		, "spaghetti", "opinion", "beautiful", "distance", "luggage" };

	std::string_view getRandomWord()
	{
		if (words.empty()) return "Vector empty";
		return words.at(Random::get<std::size_t>(0u, (words.size() - 1)));
	}
}

class Session
{
private:
	std::string_view m_randomWord{ WordList::getRandomWord() };
	std::vector<char> m_guessed{};
	bool m_alreadyGuessed{ false };
	bool won{ false };
	std::vector<char> m_wordNote{};
	int m_hearts{ 4 };

public:
	std::string_view getRandomWord() const { return m_randomWord; }
	void Introduce()
	{
		std::cout << "Welcome to C++man (a variant of Hangman)\n"
			<< "To win : guess the word.To lose : run out of pluses.\n\n";
	}

	void initializeWordNote()
	{
		for (std::size_t i{ 0 }; i < m_randomWord.size(); i++)
		{
			m_wordNote.push_back('_');
		}
	}

	void track_guessed(char s)
	{
		m_alreadyGuessed = false;
		for (const auto& i : m_guessed)
		{
			if (i == s)
			{
				m_alreadyGuessed = true;
				std::cout << "You already guessed that. Try again.\n\n";
				return;
			}
		}
		m_guessed.push_back(s);

		bool found_in_word = false;
		for (char c : m_randomWord)
		{
			if (c == s)
			{
				found_in_word = true;
				break;
			}
		}
		if (!found_in_word)
		{
			m_hearts--;
		}

	} // if string_view it crashes cuz it doesn't preserve.

	void updateGameState()
	{
		if (m_guessed.empty()) return;

		char latest_guess = m_guessed.back();
		int matched_count = 0;

		if (!m_alreadyGuessed)
		{
			std::cout << "Guessed so far: ";
			for (std::size_t index{ 0 }; const auto& i : m_randomWord)
			{
				for (const auto& j : m_guessed)
				{
					if (i == j)
					{
						if (m_wordNote[index] == '_') { m_wordNote[index] = i; }
					}
				}
				index++;
			}


			for (char c : m_wordNote)
			{
				if (c != '_') { matched_count++; }
			}

			m_alreadyGuessed = false;

			for (const auto& words : getWordNote()) { std::cout << words; }
			std::cout << '\n';
			if (static_cast<std::size_t>(matched_count) == m_randomWord.size()) { won = true; return; }


			bool latest_guess_correct = false;
			for (char c : m_randomWord)
			{
				if (c == latest_guess)
				{
					latest_guess_correct = true;
					break;
				}
			}

			if (latest_guess_correct)
			{
				std::cout << "Yes, " << latest_guess << " is in the word!\n";
			}
			else
			{
				std::cout << "No, " << latest_guess << " is not in the word!\n";
			}
			std::cout << '\n';
		}
	}

	constexpr int getHearts() const { return m_hearts; };
	const std::vector<char>& getWordNote() const { return m_wordNote; }
	constexpr bool winLoseStats() const { return won; }
	const std::vector<char>& getWrongGueses() const { return m_guessed; }

	void loseMessage() { std::cout << "Game Over!\n"; }
	void winMessage() { std::cout << "\nYou won~~~.\nYOKOSO Watashino -- - -- :__: Society YE! Kok~\n"; };
	std::string takeGameInput();
};

std::string Session::takeGameInput()
{
	std::cout << "\t\t\tHearts Remaining:   " << getHearts() << '\n';
	std::string c{};
	while (true)
	{
		std::cout << "Enter your next letter: ";
		if (std::cin >> c)
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (c.length() == 1 && std::isalpha(c[0]))
			{
				return c;
			}
		}
		else
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
}

void runGame(Session& session)
{
	session.Introduce();
	session.initializeWordNote();

	while (true)
	{
		std::string input = session.takeGameInput();
		if (!input.empty())
		{
			char guess = input[0];
			session.track_guessed(guess);
		}
		session.updateGameState();
		if (session.getHearts() == 0) { session.loseMessage(); break; };
		if (session.winLoseStats()) { session.winMessage(); break; };
	}
}

int main()
{
	Session game{};
	std::cout << game.getRandomWord() << '\n'; //printing game word otherwise u stand no chance.

	runGame(game);
	return 0;
}

#endif