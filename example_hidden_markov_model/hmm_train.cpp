#include <iostream>
#include <mlpack/methods/hmm.hpp>
#include "file_raii_wrapper.h"
#include "utils.h"

int main()
{
    std::vector<std::string> unique_words = read_unique_strings_from_file("lines_with_words.txt");
    std::vector<std::string> unique_tags = read_unique_strings_from_file("lines_with_tags.txt");

    if (unique_words.empty() || unique_tags.empty())
    {
        std::cout << "No unique words or tags found\n";
        return 1;
    }

    // data sequence:
    // vector {
    //  mat {
    //      {seq0_o00, seq0_o10, seq0_o20, ... },
    //      {seq0_o01, seq0_o11, seq0_o21, ... },
    //  },
    //  mat {
    //      {seq1_o00, seq1_o10, seq1_o20, ... },
    //      {seq1_o01, seq1_o11, seq1_o21, ... },
    //  }
    // }
    //
    // In our case, each matrix will contain only one row containing indices of words in a sentence

    // state sequence:
    // vector {
    //  row {seq0_state0, seq0_state1, seq0_state2, ... }
    //  row {seq1_state0, seq1_state1, seq1_state2, ... }
    // }
    //
    // In our case, each row will contain parts-of-speech tags for input words

    using namespace mlpack;

    HMM hmm { unique_tags.size(), DiscreteDistribution(unique_words.size()) };
    try
    {
        FileRaiiWrapper<FileOpenMode::READ_MODE> word_file("lines_with_words.txt");
        
        std::vector<arma::mat> data_seq;
        std::string line;
        while (std::getline(word_file.f, line))
        {

            std::vector<double> indices;

            std::string word;
            std::stringstream line_stream(line);
            while (line_stream >> word)
            {
                int index = binary_search(unique_words, word);
                indices.push_back(index);
            }
            arma::rowvec word_indices { indices };
            data_seq.emplace_back(word_indices);
        }

        FileRaiiWrapper<FileOpenMode::READ_MODE> tag_file("lines_with_tags.txt");
        std::vector<arma::Row<size_t>> state_seq;
        while (std::getline(tag_file.f, line))
        {
            std::vector<size_t> tag_indices;

            std::string tag;
            std::stringstream tag_stream(line);
            while (tag_stream >> tag)
            {
                int index = binary_search(unique_tags, tag);
                tag_indices.push_back(index);
            }
            state_seq.emplace_back(tag_indices);
        }

        hmm.Train(data_seq, state_seq);

        if (!data::Save("hmm.xml", "hmm", hmm))
        {
            std::cout << "Failed to save model to hmm.xml\n";
        }
    }
    catch(const std::ios_base::failure& e)
    {
        std::cout << e.what() << '\n';
        return 1;
    }
}
