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
        FILENAME,     // JSON contains just the filename -> somefile.ext
        ABSOLUTE_URL, // JSON contains compete file URL -> file://somepath/somefile.ext
        PREFIXED_URL  // JSON contains file URL with given prefix -> /somepath/somefile.ext
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

        // Base configuration
        BinaryDataMethod binaryDataMethod = FILENAME; // how to process binary attachments and images
        std::string binaryDataDirectory = ".";    // default directory for exporting and importing files
        std::string binaryDataUrlPrefix = "";   // relative URL prefix for BinaryDataMethod::RELATIVE_URL

    };
}

#endif //TAGIO_BASE_H
