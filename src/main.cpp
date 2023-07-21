#include <cctype>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <sys/ioctl.h>
#include <unistd.h>

//READ ME: This program is translated from "hangman.py". The orginal creator of "hangman.py" is https://github.com/ThomasGrafNumerics.

// Declaration of Functions
void play_hangman(std::string word); //core game
std::string random_word(void); //gets a random number from the file "helvetismen_ascii.txt" and assigns it to the variable "word"
std::string show_guess_state(std::string word, std::string guessed_letters); // Shows the already guessed letters. The non guessed ones are replaced with an "_". Example: show_guess_state("HANGMAN", "HGA"); => HA_G_A_
void gallow_ascii(int state); // prints the state of the gallow using ascii characters.

// Change the filename here to change the files used for the words.
std::string filename = "helvetismen_ascii.txt";


std::string random_word(void) {

    std::vector<std::string> wordlist; // Creating Vector for the list
    std::ifstream file(filename); // opening the file in which the words are listed
    std::string current_line;

    if (file) { // Checking if file is successfully opened
        while (std::getline(file, current_line)) { 
            wordlist.push_back(current_line);
        }
    } else { // Provides a useful error message if the file isn't opened successfully.
        std::cout << "Couldn't open file, please make sure that it is named " << filename << ". Also make sure that it is in the same directory as the executable." << std::endl;
    }

    std::cout << "Successfully imported file" << std::endl;
    file.close();

    
    // Get random element of the vector using random index, Why is this so hard 😳

    std::random_device rd;
    int maxsize = wordlist.size();
    maxsize--; //maxsize is one less than wordlist size, because wordlist begins with 0 not 1, therefore the max element is size -1
    std::uniform_int_distribution<int> dist(0, maxsize);
    int random_index = dist(rd);
    std::string word = wordlist[random_index];
    

    // Convert it to all Upper case

    for (char& c : word) {
        c = std::toupper(c);
    }

    word = word.substr(0, word.length() - 1); //Removing "New line" at the end of the word.


    return word;
}


std::string show_guess_state(std::string word, std::string guessed_letters) {

    std::string guessed_state = "";

    for (char c : word) {

        size_t found = guessed_letters.find(c);

        if (found != std::string::npos) {
           guessed_state += c;
        } else {
            guessed_state += "_";
        }
    }


    return guessed_state;
}


void gallow_ascii(int state) {

    std::string ascii[] = {

        "    +---+\n"
            "    |   |\n"
            "        |\n"
            "        |\n"
            "        |\n"
            "        |\n"
            "   =========\n"
            ,
        "    +---+\n"
            "    |   |\n"
            "    O   |\n"
            "        |\n"
            "        |\n"
            "        |\n"
            "   =========\n"
            ,
        "    +---+\n"
            "    |   |\n"
            "    O   |\n"
            "    |   |\n"
            "        |\n"
            "        |\n"
            "   =========\n"
            ,
        "    +---+\n"
            "    |   |\n"
            "    O   |\n"
            "   /|   |\n"
            "        |\n"
            "        |\n"
            "   =========\n"
            ,
        "    +---+\n"
            "    |   |\n"
            "    O   |\n"
            "   /|\\  |\n"
            "        |\n"
            "        |\n"
            "   =========\n"
            ,
        "    +---+\n"
            "    |   |\n"
            "    O   |\n"
            "   /|\\  |\n"
            "   /    |\n"
            "        |\n"
            "   =========\n"
            ,
        "    +---+\n"
            "    |   |\n"
            "    O   |\n"
            "   /|\\  |\n"
            "   / \\  |\n"
            "        |\n"
            "========="

    };


    std::cout << ascii[state] << std::endl;
}

void play_hangman(std::string word){

        //Creating a blank page
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        int rows = size.ws_row;

        for (int i = 0; i < rows; i++) {

            std::cout << "\n";


        }

    std::string welcome = R"(_       __     __                             __           __  __                                      
| |     / /__  / /________  ____ ___  ___     / /_____     / / / /___ _____  ____ _____ ___  ____ _____ 
| | /| / / _ \/ / ___/ __ \/ __ `__ \/ _ \   / __/ __ \   / /_/ / __ `/ __ \/ __ `/ __ `__ \/ __ `/ __ \
| |/ |/ /  __/ / /__/ /_/ / / / / / /  __/  / /_/ /_/ /  / __  / /_/ / / / / /_/ / / / / / / /_/ / / / /
|__/|__/\___/_/\___/\____/_/ /_/ /_/\___/   \__/\____/  /_/ /_/\__,_/_/ /_/\__, /_/ /_/ /_/\__,_/_/ /_/ 
                                                                          /____/                        )";

    std::cout << welcome << std::endl; // If you have a really low resulution or want to run the script in small terminals then comment out this line

    bool guess = false;
    int wrong_guesses = 0;
    std::string guessed_letters = "";
    

    std::cout << "Current state: " << show_guess_state(word, guessed_letters) << std::endl;
    gallow_ascii(wrong_guesses);

    while (!guess && wrong_guesses < 6) {

        char current_letter;

        std::cout << "Your guess: " << std::endl;
        std::cin >> current_letter;
        current_letter = std::toupper(current_letter);

        //Creating a blank page
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        int rows = size.ws_row;

        for (int i = 0; i < rows; i++) {

            std::cout << "\n";


        }
        if (guessed_letters.find(current_letter) != std::string::npos) {

            std::cout << "Please pick another letter, because " << current_letter << " was already used" << std::endl;


        } else if(word.find(current_letter) == std::string::npos){

            std::cout << "The letter " << current_letter << " is not in the word." << std::endl;
            wrong_guesses++;
            guessed_letters += current_letter;


        } else {

            std::cout << "The letter " << current_letter << " is in the word. Good Job!" << std::endl;
            guessed_letters += current_letter;

            (show_guess_state(word, guessed_letters).find("_") == std::string::npos ? guess = true : guess = false);

        }

        gallow_ascii(wrong_guesses);
        std::cout << "Current state: " << show_guess_state(word, guessed_letters) << std::endl;
        std::cout << "You alredy guessed: " << guessed_letters << std::endl;

    }

    (guess == true ? std::cout << "Good Job, you guessed the word " << word << " with " << wrong_guesses << " wrong guesses" << std::endl : std::cout << "You died, the word was: " << word << std::endl);

}

int main() {

    std::string word = random_word();
    play_hangman(word);
}
