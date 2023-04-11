#include "Config.h"

Path::Path()
{
    this->path.clear();
}

Path::Path(std::string path)
{
    this->path.push_back(path);
}

Path *Path::joinpath(std::string path)
{
    this->path.push_back(path);
    return this;
}

std::string Path::getPath()
{
    std::string path;
    if (this->path.empty())
    {
        return path;
    }
    for (int i = 0; i < (int)this->path.size() - 1; i++)
    {
        path += this->path[i] + "/";
    }
    path += *this->path.end();
    return path;
}

bool Path::mkdir()
{
    return std::filesystem::create_directory(getPath());
}
