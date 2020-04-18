#ifndef BytesParser_h
#define BytesParser_h

#include <vector>

class BytesParser
{
public:
    template <class T>
    static T parse(std::vector<unsigned char> bytes, int offset = 0);

    template <class T>
    static std::vector<unsigned char> toBytes(T value);

    template <class T>
    static std::vector<unsigned char> appendBytes(std::vector<unsigned char> &bytes, T value);
};

template <class T>
T BytesParser::parse(std::vector<unsigned char> bytes, int offset)
{
    T result;
    memcpy(&result, &(bytes[offset]), sizeof(result));
    return result;
}

template <class T>
std::vector<unsigned char> BytesParser::toBytes(T value)
{
    std::vector<unsigned char> byteArray(sizeof(value), 0);
    memcpy(byteArray.data(),(const char *)&value,sizeof(value));
    return byteArray;
}

template <class T>
std::vector<unsigned char> BytesParser::appendBytes(std::vector<unsigned char> &bytes, T value)
{
    std::vector<unsigned char> temp = toBytes(value);
    bytes.insert(bytes.end(), make_move_iterator(temp.begin()), make_move_iterator(temp.end()));
    return bytes;
}

#endif /* BytesParser_h */
