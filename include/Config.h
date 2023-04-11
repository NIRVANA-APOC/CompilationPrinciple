#pragma once

#include <string>
#include <vector>
#include <filesystem>

class Path
{
private:
    std::vector<std::string> path;

public:
    Path();
    Path(std::string);
    std::string getPath();
    Path *joinpath(std::string);
    bool mkdir();
    Path operator=(const Path &);
};

class Config
{
public:
    Path *root_dir;
    Path *input_dir;
    Path *output_dir;

    Config()
    {
        root_dir = new Path("..");
        input_dir = root_dir->joinpath("input");
        output_dir = root_dir->joinpath("output");

        root_dir->mkdir();
        input_dir->mkdir();
        output_dir->mkdir();
    }
};