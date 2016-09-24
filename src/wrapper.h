#ifndef TAGIO_WRAPPER_H
#define TAGIO_WRAPPER_H


#include <nan.h>
#include <string>
#include <taglib/tlist.h>
#include <taglib/tstring.h>
#include <taglib/tstringlist.h>
#include <taglib/tbytevector.h>
#include "md5.h"

// Wrap v8 object and convert properties for taglib.



class TagLibWrapper {

public:
    TagLibWrapper(v8::Object *object);
    bool GetBoolean(const char *key);
    void SetBoolean(const char *key, bool value);
    double GetNumber (const char *key);
    void SetNumber(const char *key, double value);
    int GetInt32(const char *key);
    void SetInt32(const char *key, const int value);
    TagLib::uint GetUint32(const char *key);
    void SetUint32(const char *key, const TagLib::uint value);
    TagLib::String GetString(const char *key);
    void SetString(const char *key, const TagLib::String value);
    TagLib::StringList GetStringList(const char *key);
    void SetStringList(const char *key, const TagLib::StringList value);
//        TagLib::List<TagLib::String> GetStringArray(const char *key);
//        void SetStringArray(const char *key, const TagLib::List<TagLib::String>);
    //TagLib::ByteVector GetBytes(const char *key);
    //void SetBytes(const char *key, const TagLib::ByteVector value, TagLib::String mimeType);
    TagLib::String::Type GetEncoding(const char *key);
    void SetEncoding(const char *key, const TagLib::String::Type value);
    TagLib::ByteVector GetLanguage(const char *key);
    void SetLanguage(const char *key, const TagLib::ByteVector value);
    ~TagLibWrapper();
private:
    v8::Object *object;
};



#endif //TAGIO_WRAPPER_H
