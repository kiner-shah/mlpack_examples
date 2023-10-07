#include <iostream>
#include <set>
#include <algorithm>
#include "file_raii_wrapper.h"

std::vector<std::string> read_unique_strings_from_file(fs::path path)
{
    std::vector<std::string> result;
    try
    {
        FileRaiiWrapper<FileOpenMode::READ_MODE> file(path);
        std::set<std::string> words;

        std::string word;
        while (file.f >> word)
        {
            words.insert(word);
        }

        result.resize(words.size());
        result.assign(words.begin(), words.end());

        return result;
    }
    catch (const std::ios_base::failure& e)
    {
        std::cout << e.what() << '\n';
        return {};
    }
}

int binary_search(const std::vector<std::string>& v, const std::string& key)
{
    auto ret = std::lower_bound(v.cbegin(), v.cend(), key);
    if (ret == v.cend() || key < *ret)
    {
        return -1;
    }
    return std::distance(v.cbegin(), ret);
}
