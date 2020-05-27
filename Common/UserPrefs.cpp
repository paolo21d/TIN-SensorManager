#include "UserPrefs.h"

using namespace std;

UserPrefs::UserPrefs()
{
    load();
}

UserPrefs &UserPrefs::getInstance()
{
    static UserPrefs instance;
    return instance;
}

void UserPrefs::save()
{
    ofstream file(filename.c_str());
    if (file.good())
    {
        for (const auto value : values)
        {
            file << value.first << ":" << value.second << endl;
        }
    }
    file.close();
}

void UserPrefs::load()
{
    values.clear();

    ifstream file(filename.c_str());
    if (file.good())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::size_t found = line.find_first_of(":");
            if (found!=std::string::npos)
            {
                string key = line.substr(0, found);
                string val = line.substr(found + 1, line.size());

                if (!checkKey(key) || !checkString(val))
                    continue;

                values[key] = val;
            }
        }
    }
    file.close();
}

void UserPrefs::setInt(std::string key, int value)
{
    values[key] = intToStr(value);
}

void UserPrefs::setFloat(std::string key, float value)
{
    values[key] = floatToStr(value);
}

void UserPrefs::setString(std::string key, std::string value)
{
    values[key] = value;
}

int UserPrefs::getInt(std::string key)
{
    return stoi(values[key]);
}

float UserPrefs::getFloat(std::string key)
{
    return stof(values[key]);
}

std::string UserPrefs::getString(std::string key)
{
    return values[key];
}

bool UserPrefs::checkString(std::string str)
{
    for (const auto c : str)
        if (c == '\n')
            return false;

    return true;
}

bool UserPrefs::checkKey(std::string key)
{
    if (!checkString(key))
        return false;

    for (const auto c : key)
        if (!isalpha(c) && !isdigit(c) && c != '_')
            return false;

    return true;
}

std::string UserPrefs::intToStr(int x)
{
    stringstream ss;
    ss << x;
    return ss.str();
}

std::string UserPrefs::floatToStr(float x)
{
    stringstream ss;
    ss << x;
    return ss.str();
}