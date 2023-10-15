#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

class HangmanGame {
public:
    HangmanGame(const vector<string>& loadedWords) : words(loadedWords), maxTime(120) {}

    void Run() {
        wordToGuess = ChooseWord();
        startTime = time(NULL);
        time_t currentTime;
        int timeLeft;

        while (true) {
            currentTime = time(NULL);
            timeLeft = maxTime - difftime(currentTime, startTime);
            if (timeLeft <= 0) {
                DisplayGameResultTimeout();
                if (!PlayAgain()) {
                    return;
                }
                wordToGuess = ChooseWord();
                guessedLetters.clear();
                startTime = time(NULL);
            }

            DisplayWord();
            cout << "Оставшееся время: " << timeLeft << " секунд." << endl;
            cout << "Введите букву: ";
            string input;
            cin >> input;

            if (input == "hint" && timeLeft > maxTime / 2) {
                cout << "Подсказка: " << GetHint() << endl;
            } else if (input.length() == 1 && isalpha(input[0])) {
                char guess = tolower(input[0]);
                if (IsGuessed(guess)) {
                    cout << "Вы уже вводили эту букву." << endl;
                } else {
                    guessedLetters.push_back(guess);
                    if (IsWinner()) {
                        DisplayGameResultWin();
                        if (!PlayAgain()) {
                            return;
                        }
                        wordToGuess = ChooseWord();
                        guessedLetters.clear();
                        startTime = time(NULL);
                    }
                }
            } else {
                cout << "Введите одну букву." << endl;
            }
        }
    }

private:
    vector<string> words;
    string wordToGuess;
    vector<char> guessedLetters;
    time_t startTime;
    int maxTime;

    string ChooseWord() const {
        srand(static_cast<unsigned>(time(NULL)));
        return words[rand() % words.size()];
    }

    bool IsGuessed(char guess) const {
        for (char letter : guessedLetters) {
            if (guess == letter) {
                return true;
            }
        }
        return false;
    }

    bool IsWinner() const {
        for (char letter : wordToGuess) {
            bool found = false;
            for (char guess : guessedLetters) {
                if (letter == guess) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }

    string GetHint() const {
        string hint;
        for (int i = 0; i <= guessedLetters.size(); ++i) {
            hint += wordToGuess[i];
        }
        return hint;
    }

    void DisplayWord() const {
        for (char letter : wordToGuess) {
            bool found = false;
            for (char guess : guessedLetters) {
                if (letter == guess) {
                    found = true;
                    break;
                }
            }
            if (found) {
                cout << letter << " ";
            } else {
                cout << "_ ";
            }
        }
        cout << endl;
    }

    void DisplayGameResultWin() const {
        cout << "Поздравляем, вы выиграли! Загаданное слово: " << wordToGuess << endl;
        cout << "Желаете сыграть ещё раз? (да/нет): ";
    }

    void DisplayGameResultTimeout() const {
        cout << "Время вышло! Загаданное слово: " << wordToGuess << endl;
        cout << "Желаете сыграть ещё раз? (да/нет): ";
    }

    bool PlayAgain() const {
        string playAgain;
        cin >> playAgain;
        return playAgain == "да";
    }
};

vector<string> LoadWords(const string& filename) {
    vector<string> loadedWords;
    ifstream file(filename);

    if (!file) {
        cerr << "Не удалось открыть файл с словами." << endl;
        exit(1);
    }

    string word;
    while (getline(file, word)) {
        loadedWords.push_back(word);
    }

    return loadedWords;
}

int main() {
    vector<string> words = LoadWords("words.txt");
    HangmanGame game(words);
    game.Run();
    return 0;
}
