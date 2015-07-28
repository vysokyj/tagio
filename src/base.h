#ifndef TAGIO_BASE_H
#define TAGIO_BASE_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <v8.h>

#include <taglib/fileref.h>
#include <taglib/tstring.h>
#include <taglib/tbytevector.h>
#include "md5.h"

namespace TagIO {

    class Base {

    enum BinaryDataMethod {
        FILE, // just file name
        FILE_URL, // complete url - file protocol file://somepath/somefile.ext
        RELATIVE_URL // relative url - used binaryDataRelativeUrl property
    };

    public:
        const char *GetFilePath() { return this->path.c_str(); }
        static void SetTagByObject(v8::Isolate *isolate, v8::Object *object, TagLib::Tag *tag);
        static void SetObjectByTag(v8::Isolate *isolate, v8::Object *object, TagLib::Tag *tag);

        // helper methods
        static void SetBaseConfiguration(v8::Isolate *isolate, v8::Object *object, Base *base);
        //static TagLib::ByteVector GetBinary(v8::Isolate *isolate, v8::Object *object, const char *key);
        //static void SetBinary(v8::Isolate *isolate, v8::Object *object, const char *key, TagLib::ByteVector);
        static TagLib::uint GetUint32(v8::Isolate *isolate, v8::Object *object, const char *key);
        static void SetUint32(v8::Isolate *isolate, v8::Object *object, const char *key, const TagLib::uint value);
        static TagLib::String GetString(v8::Isolate *isolate, v8::Object *object, const char *key);
        static void SetString(v8::Isolate *isolate, v8::Object *object, const char *key, const TagLib::String value);
    protected:
        Base(const char *path);
        ~Base();
        TagLib::String ExportFile(TagLib::ByteVector byteVector, TagLib::String mimeType);
        TagLib::ByteVector ImportFile(TagLib::String path);

        static const char *FixPath(const char *path);
        static std::string FixPath(std::string path);
        static const char *FixContext(const char *context);
        static std::string FixContext(std::string context);
        static std::string NewPath(std::string directory, std::string fileName);
        static std::string NewRelativeUrl(std::string relativeUrl, std::string fileName);
        static std::string CountMD5(TagLib::ByteVector byteVector);
        static std::string NewFileName(TagLib::ByteVector byteVector, std::string mimeType);


    private:
        std::string path;

        // binary data props
        std::string binaryDataDirectory = "."; // export to current directory at default
        std::string binaryDataRelativeUrl; // UR
        BinaryDataMethod binaryDataMethod = FILE; // JSON contains just file name


    };
}

#endif //TAGIO_BASE_H
