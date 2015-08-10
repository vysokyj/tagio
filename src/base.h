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
#include "converter.h"

namespace TagIO {

    class Base {

    enum BinaryDataMethod {
        FILENAME,     // JSON contains just the filename -> somefile.ext
        ABSOLUTE_URL, // JSON contains compete file URL -> file://somepath/somefile.ext
        PREFIXED_URL  // JSON contains file URL with given prefix -> /somepath/somefile.ext
    };

    public:
        const char *GetFilePath() { return this->path.c_str(); }
        static void SetObjectByAudioProperties(v8::Isolate *isolate, v8::Object *object, TagLib::AudioProperties *audioProperties);
        static void SetTagByObject(v8::Isolate *isolate, v8::Object *object, TagLib::Tag *tag);
        static void SetObjectByTag(v8::Isolate *isolate, v8::Object *object, TagLib::Tag *tag);

        // helper methods
        static void SetBaseConfiguration(v8::Isolate *isolate, v8::Object *object, Base *base);
        static void GetBaseConfiguration(v8::Isolate *isolate, v8::Object *object, Base *base);

    protected:
        Base(const char *path);
        ~Base();
        TagLib::String ExportFile(TagLib::ByteVector byteVector, TagLib::String mimeType);
        TagLib::ByteVector ImportFile(TagLib::String path);

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
