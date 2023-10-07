#include <mlpack/methods/hmm.hpp>
#include <iostream>
#include <vector>
#include <sstream>

#include "utils.h"

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
        {
            return std::tolower(c);
        });
    return s;
}

int main()
{
    using namespace mlpack;

    HMM hmm;
    if (!data::Load("hmm.xml", "hmm", hmm))
    {
        std::cout << "Couldn't load hmm.xml\n";
        return 1;
    }
    std::vector<std::string> unique_words = read_unique_strings_from_file("lines_with_words.txt");
    std::vector<std::string> unique_tags = read_unique_strings_from_file("lines_with_tags.txt");

    if (unique_words.empty() || unique_tags.empty())
    {
        std::cout << "No unique words or tags found\n";
        return 1;
    }

    std::cout << "Enter a sentence:\n";
    std::string sentence;
    if (!std::getline(std::cin, sentence))
    {
        std::cout << "Problem during user input\n";
        return 1;
    }

    std::stringstream ss { sentence };
    std::string word;
    std::vector<double> data_seq;
    std::vector<std::string> words;
    while (ss >> word)
    {
        int index = binary_search(unique_words, str_tolower(word));
        data_seq.push_back(index);
        words.push_back(word);
    }
    arma::rowvec data_row { data_seq };
    arma::mat input_data { data_row };

    arma::Row<size_t> predicted_states;
    double log_likelihood = hmm.Predict(input_data, predicted_states);
    std::cout << "Sentence tagged with most probable states:\n";
    for (int i = 0; i < words.size(); i++)
    {
        std::cout << words[i] << '/' << unique_tags[predicted_states(i)] << ' ';
    }
    std::cout << "\nPredicted with log likelihood = " << std::fixed << log_likelihood << '\n';
    
}
