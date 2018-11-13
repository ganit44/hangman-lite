#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <memory>
#include <stdexcept>
#include <algorithm>
#define MAX_WRONG_ATTEMPTS 5


std::string exec(const char* cmd) 
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) 
        throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) 
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

int main()
{
    std::string user_input;
    int attempt, matched_chars;
    char user_char_input;
    bool isMatchedChar;
    std::string selected_word;
    std::vector<bool> matched;
    std::size_t found;

    std::cout << "Welcome to HANGMAN Lite.\n";
    std::cout << "Do you want play this game ? [Y/N] : ";

    std::cin >> user_input;

    std::transform(user_input.begin(), user_input.end(), 
                    user_input.begin(), ::tolower);

    while(user_input.compare("y") == 0)
    {
        while(true)
        {
            selected_word  = exec("shuf -n1  /usr/share/dict/words");
       
            selected_word.erase(
                    std::remove(selected_word.begin(), selected_word.end(), '\n'), 
                    selected_word.end());  

            selected_word.erase(
                    std::remove(selected_word.begin(), selected_word.end(), ' '), 
                    selected_word.end()); 

            if(std::isalpha(selected_word.at(selected_word.length()-2)) != false
               && selected_word.length() > 5)
                break;
        }
        
        matched.clear();
        matched.resize(selected_word.length());

        attempt = 0;


        for(std::string::size_type i = 0; i < selected_word.length(); ++i)
        {
            std::cout << "_" << " ";
        }

        while(true)
        {
            std::cout << "\nEnter a character : ";
            std::cin >> user_char_input;
            std::cout << "\n";

            isMatchedChar = false;
            matched_chars = 0;

            for(std::string::size_type i = 0; i < selected_word.length(); ++i) 
            {
                if(std::tolower(selected_word[i]) == std::tolower(user_char_input))
                {
                    matched[i] = true;
                    ++matched_chars;
                    isMatchedChar = true;
                    std::cout << selected_word[i] << " ";
                }
                else if(matched[i] == true)
                {
                    ++matched_chars;
                    std::cout << selected_word[i] << " ";
                }
                else
                {
                    std::cout << "_" << " ";
                }
            }
            
            if(isMatchedChar == false)
            {
                ++attempt;
                std::cout << "\t\tChar Not Found. Only " << MAX_WRONG_ATTEMPTS - attempt << " attempt(s) remaining;\n";
            }

            if(matched_chars == selected_word.length())
            {
                std::cout << "\nCongratulations! You have won. \n";
                break;
            }

            if(attempt == MAX_WRONG_ATTEMPTS)
            {
                std::cout << "You lost! Correct word was " << selected_word << ".\n";
                break;
            }
        }

        std::cout << "\nDo you want to play again ? [Y/N] : ";

        std::cin >> user_input;

        std::transform(user_input.begin(), user_input.end(), 
                        user_input.begin(), ::tolower);

    }

    std::cout << "Thanks for playing Hangman Lite.\n";
}


