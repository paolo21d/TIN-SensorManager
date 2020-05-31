#ifndef USERPREFS_H
#define USERPREFS_H

#include <fstream>
#include <map>
#include <sstream>

class UserPrefs
{
private:
    UserPrefs();

public:
    static UserPrefs &getInstance();

    void save();

    void setInt(std::string key, int value);
    void setFloat(std::string key, float value);
    void setString(std::string key, std::string value);

    int getInt(std::string key);
    float getFloat(std::string key);
    std::string getString(std::string key);

private:
    std::string filename = "config.txt";
    std::map<std::string, std::string> values;

    void load();

    bool checkString(std::string str);
    bool checkKey(std::string key);

    std::string intToStr(int x);
    std::string floatToStr(float x);
};

#endif //USER_PREFS_H
