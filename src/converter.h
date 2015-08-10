#ifndef TAGIO_CONVERTER_H
#define TAGIO_CONVERTER_H


#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <v8.h>

#include <taglib/fileref.h>
#include <taglib/tstring.h>
#include <taglib/tbytevector.h>
#include "md5.h"

// Wrap v8 object and convert properties for taglib.

namespace TagIO {

    class Converter {

    public:
        Converter(v8::Isolate *isolate, v8::Object *object);
        bool GetBoolean(const char *key);
        void SetBoolean(const char *key, bool);
        int GetInt32(const char *key);
        void SetInt32(const char *key, const int value);
        TagLib::uint GetUint32(const char *key);
        void SetUint32(const char *key, const TagLib::uint value);
        TagLib::String GetString(const char *key);
        void SetString(const char *key, const TagLib::String value);
        ~Converter();
    private:
        v8::Isolate *isolate;
        v8::Object *object;

    };
}


#endif //TAGIO_CONVERTER_H
