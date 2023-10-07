#pragma once

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

enum class FileOpenMode : uint8_t
{
    READ_MODE,
    WRITE_MODE
};

template<FileOpenMode mode>
struct FileRaiiWrapper
{
    std::fstream f;

    FileRaiiWrapper(fs::path path)
    {
        if constexpr (mode == FileOpenMode::READ_MODE)
        {
            f.open(path, std::ios::in | std::ios::binary);
            if (!f.good())
            {
                std::stringstream ss;
                ss << "Couldn't open file for reading: " << path;
                throw std::ios_base::failure(ss.str());
            }
        }
        else
        {
            f.open(path, std::ios::out);
            if (!f.good())
            {
                std::stringstream ss;
                ss << "Couldn't open file for writing: " << path;
                throw std::ios_base::failure(ss.str());
            }
        }
    }
    ~FileRaiiWrapper()
    {
        if (f.is_open())
        {
            f.close();
        }
    }

    FileRaiiWrapper(const FileRaiiWrapper&) = delete;
    FileRaiiWrapper(FileRaiiWrapper&&) = delete;
    FileRaiiWrapper& operator=(const FileRaiiWrapper&) = delete;
    FileRaiiWrapper& operator=(FileRaiiWrapper&&) = delete;
};
