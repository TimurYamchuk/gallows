#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <windows.h>  // Подключение библиотеки Windows для Beep.

using namespace std;

class HangmanGame {
public:
    HangmanGame(const vector<string>& loadedWords) : words(loadedWords), maxTime(120), maxAttempts(5) {}

    void Run() {
        // Выбираем случайное слово для угадывания и инициализируем время начала игры.
        wordToGuess = ChooseWord();
        startTime = time(NULL);
        time_t currentTime;
        int timeLeft;
        int attemptsLeft = maxAttempts;
        int tone = 1000;  // Начальная тональность для Beep.

        while (attemptsLeft > 0) {
            currentTime = time(NULL);
            timeLeft = maxTime - difftime(currentTime, startTime);

            // Если время вышло, игра завершается с звуковым сигналом и предложением сыграть ещё раз.
            if (timeLeft <= 0) {
                Beep(tone, 300);
                DisplayGameResultTimeout();
                if (!PlayAgain()) {
                    return;
                }
                wordToGuess = ChooseWord();
                guessedLetters.clear();
                startTime = time(NULL);
                attemptsLeft = maxAttempts;
                tone = 1000;  // Сброс тональности.
            }

            DisplayWord();  // Отображение угадываемого слова с учетом отгаданных букв.
            cout << "Оставшееся время: " << timeLeft << " секунд." << endl;
            cout << "Попыток осталось: " << attemptsLeft << endl;
            cout << "Введите букву: ";
            string input;
            cin >> input;

            // Обработка ввода игрока.
            if (input == "hint" && timeLeft > maxTime / 2) {
                cout << "Подсказка: " << GetHint() << endl;
            } else if (input.length() == 1 && isalpha(input[0])) {
                char guess = tolower(input[0]);

                // Проверка, была ли уже отгадана эта буква.
                if (IsGuessed(guess)) {
                    Beep(tone, 300);
                    cout << "Вы уже вводили эту букву." << endl;
                } else {
                    guessedLetters.push_back(guess);
                    // Если игрок выиграл, играется мелодия победы и предлагается начать заново.
                    if (IsWinner()) {
                        Beep(3000, 300);
                        Beep(100, 300);
                        Beep(3000, 300);
                        DisplayGameResultWin();
                        if (!PlayAgain()) {
                            return;
                        }
                        wordToGuess = ChooseWord();
                        guessedLetters.clear();
                        startTime = time(NULL);
                        attemptsLeft = maxAttempts;
                        tone = 1000;  // Сброс тональности.
                    } else if (!IsLetterInWord(guess)) {
                        Beep(tone, 300);
                        cout << "Этой буквы нет в слове." << endl;
                        attemptsLeft--;
                        tone -= 300;  // Уменьшение тональности с каждой ошибкой.
                    }
                }
            } else {
                Beep(tone, 300);
                cout << "Введите одну букву." << endl;
            }
        }

        // Если закончились попытки, выводится сообщение о проигрыше и предлагается начать заново.
        DisplayGameResultLose();
        if (PlayAgain()) {
            wordToGuess = ChooseWord();
            guessedLetters.clear();
            startTime = time(NULL);
            attemptsLeft = maxAttempts;
        }
    }

private:
    vector<string> words;
    string wordToGuess;
    vector<char> guessedLetters;
    time_t startTime;
    int maxTime;
    int maxAttempts;

    // Выбор случайного слова из списка.
    string ChooseWord() const {
        srand(static_cast<unsigned>(time(NULL)));
        return words[rand() % words.size()];
    }

    // Проверка, была ли уже отгадана буква.
    bool IsGuessed(char guess) const {
        for (char letter : guessedLetters) {
            if (guess == letter) {
                return true;
            }
        }
        return false;
    }

    // Проверка, выиграл ли игрок, отгадав все буквы слова.
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

    // Проверка, присутствует ли буква в загаданном слове.
    bool IsLetterInWord(char letter) const {
        for (char l : wordToGuess) {
            if (l == letter) {
                return true;
            }
        }
        return false;
    }

    // Генерация подсказки на основе отгаданных букв.
    string GetHint() const {
        string hint;
        for (int i = 0; i <= guessedLetters.size(); ++i) {
            hint += wordToGuess[i];
        }
        return hint;
    }

    // Отображение угадываемого слова с подчеркиваниями для неотгаданных букв.
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

    // Вывод сообщения о победе.
    void DisplayGameResultWin() const {
        cout << "Поздравляем, вы выиграли! Загаданное слово: " << wordToGuess << endl;
        cout << "Желаете сыграть ещё раз? (да/нет): ";
    }

    // Вывод сообщения о времени, которое закончилось.
    void DisplayGameResultTimeout() const {
        cout << "Время вышло! Загаданное слово: " << wordToGuess << endl;
        cout << "Желаете сыграть ещё раз? (да/нет): ";
    }

    // Вывод сообщения о проигрыше (закончились попытки).
    void DisplayGameResultLose() const {
        cout << "У вас закончились попытки! Загаданное слово: " << wordToGuess << endl;
        cout << "Желаете сыграть ещё раз? (да/нет): ";
    }

    // Запрос у игрока на повторную игру.
    bool PlayAgain() const {
        string playAgain;
        cin >> playAgain;
        return playAgain == "да";
    }
};

// Загрузка списка слов из файла.
vector<string> LoadWords(const string& filename) {
    vector<string> loadedWords;
    ifstream file(filename);

    // Проверка на успешное открытие файла. В случае неудачи проигрывается звуковой сигнал.
    if (!file.is_open()) {
        Beep(400, 300);
        cerr << "Ошибка: Не удается открыть файл с загруженными словами." << endl;
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
