#include <iostream>
#include <string_view>
#include <vector>
#include <algorithm>
#include "file_raii_wrapper.h"

using namespace std;

std::string readFile(fs::path path)
{
    try
    {
        // Open the stream to 'lock' the file.
        FileRaiiWrapper<FileOpenMode::READ_MODE> file(path);

        // Obtain the size of the file.
        const auto sz = fs::file_size(path);

        // Create a buffer.
        std::string result(sz, '\0');

        // Read the whole file into the buffer.
        file.f.read(result.data(), sz);

        return result;
    }
    catch (const std::ios_base::failure& e)
    {
        std::cout << e.what() << '\n';
        return "";
    }
}

// trim from start (in place)
static inline void ltrim(std::string_view &s) {
    auto pos = std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    });
    s.remove_prefix(std::distance(s.begin(), pos));
}

// trim from end (in place)
static inline void rtrim(std::string_view &s) {
    auto pos = std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base();
    s.remove_suffix(s.size() - std::distance(s.begin(), pos));
}

// trim from both ends (in place)
static inline void trim(std::string_view &s) {
    rtrim(s);
    ltrim(s);
}

std::string str_tolower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c)
        {
            return std::tolower(c); 
        });
    return s;
}

using LineWordTags = std::vector<std::pair<std::string, std::string>>;

std::vector<LineWordTags> parseFileContents(std::string_view contents)
{
    std::vector<LineWordTags> result;
    std::vector<std::pair<std::string, std::string>> word_tags;
    while (true)
    {
        // No content left for processing
        if (contents.empty())
        {
            break;
        }
        auto pos = contents.find_first_of("\r\n");
        auto sub = contents.substr(0, pos);
        trim(sub);
        if (sub.empty())
        {
            contents.remove_prefix(pos + 1);
        }
        else
        {
            if (!sub.starts_with('#'))
            {
                std::stringstream ss {std::string{sub.begin(), sub.end()}};
                std::string field;
                std::vector<std::string> fields;
                while (std::getline(ss, field, '\t'))
                {
                    fields.push_back(field);
                }
                if (!fields.empty())
                {
                    word_tags.emplace_back(fields[1], fields[3]);
                }
            }
            else if (!word_tags.empty())
            {
                result.push_back(word_tags);
                word_tags.clear();
            }
            contents.remove_prefix(pos + 1);
        }
    }
    if (!word_tags.empty())
    {
        result.push_back(word_tags);
        word_tags.clear();
    }

    return result;
}

int main()
{
    auto file_contents = readFile("data.txt");
    if (file_contents.empty())
    {
        return 1;
    }

    try
    {
        FileRaiiWrapper<FileOpenMode::WRITE_MODE> lines_with_words("lines_with_words.txt");
        FileRaiiWrapper<FileOpenMode::WRITE_MODE> lines_with_tags("lines_with_tags.txt");

        for (const auto& v : parseFileContents(file_contents))
        {
            bool is_atleast_one_word_written = false;
            for (const auto& [word, tag] : v)
            {
                if (word.starts_with("RT")
                        || word.starts_with("@USER")
                        || word.starts_with("URL")
                        || word.starts_with(':')
                        || word.ends_with(':')
                        || tag == "EMOTICON" 
                        || word.starts_with('#'))
                {
                    continue;
                }
                lines_with_words.f << str_tolower(word) << ' ';
                lines_with_tags.f << tag << ' ';
                is_atleast_one_word_written = true;
            }
            if (is_atleast_one_word_written)
            {
                lines_with_words.f << '\n';
                lines_with_tags.f << '\n';
            }
        }
    }
    catch (const std::ios_base::failure& e)
    {
        std::cout << e.what() << '\n';
        return 1;
    }

    return 0;
}
