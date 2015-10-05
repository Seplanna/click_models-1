#ifndef BINARY_READER
#define BINARY_READER

#include <memory>
#include "Tools.h"

using std::shared_ptr;

namespace FileManager
{

template <class _Reader>
class BinaryReader
{
public:
    typedef _Reader Reader;

    BinaryReader(shared_ptr<Reader> reader,
        std::locale readerLocale);
    Reader& GetReader();
    BinaryReader& operator >>(int& data);
    BinaryReader& operator >>(short& data);
    BinaryReader& operator >>(double& data);
    BinaryReader& operator >>(float& data);
    BinaryReader& operator >>(size_t& data);
    BinaryReader& operator >>(char& data);
    BinaryReader& operator >>(wchar_t& data);
    BinaryReader& operator >>(string& data);
    BinaryReader& operator >>(wstring& data);
private:
    shared_ptr<Reader> reader;
    std::locale readerLocale;
};

template <class Reader>
BinaryReader<Reader>::BinaryReader(
    shared_ptr<Reader> reader,
    std::locale readerLocale)
    : reader(reader)
    , readerLocale(readerLocale)
{
}

template <class Reader>
typename BinaryReader<Reader>::Reader&
BinaryReader<Reader>::GetReader()
{
    return *reader;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(int& data)
{
    reader->read((char*)&data, sizeof(int));
    return *this;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(short& data)
{
    reader->read((char*)&data, sizeof(short));
    return *this;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(double& data)
{
    reader->read((char*)&data, sizeof(double));
    return *this;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(float& data)
{
    reader->read((char*)&data, sizeof(float));
    return *this;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(size_t& data)
{
    reader->read((char*)&data, sizeof(size_t));
    return *this;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(char& data)
{
    reader->read((char*)&data, sizeof(char));
    return *this;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(wchar_t& data)
{
    reader->read((char*)&data, sizeof(wchar_t));
    return *this;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(string& data)
{
    short size;
    (*this) >> size;
    if (size > 0) {
        data.resize(size);
        reader->read((char*)data.c_str(), size * sizeof(char));
    } else {
        data = "";
    }
    return *this;
}

template <class Reader>
BinaryReader<Reader>&
BinaryReader<Reader>::operator >>(wstring& data)
{
    string converted;
    *this >> converted;
    data = Tools::ConvertUTF8StringToWstring(
        converted, readerLocale);
    return *this;
}


}

#endif // BINARY_READER
