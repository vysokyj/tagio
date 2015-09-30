#ifndef TAGIO_CONFIGURATION_H
#define TAGIO_CONFIGURATION_H

#include <node/node.h>
#include <v8.h>
#include <string>
#include <taglib/tstring.h>

namespace TagIO {

    class Configuration {
    public:
        enum BinaryDataMethod {
            IGNORE,       // IGNORE BINARY FILES
            FILENAME,     // JSON contains just the filename -> somefile.ext
            ABSOLUTE_URL, // JSON contains compete file URL -> file://somepath/somefile.ext
            PREFIXED_URL  // JSON contains file URL with given prefix -> /somepath/somefile.ext
        };
        static Configuration &Get() {
            static Configuration instance;
            return instance;
        }
        static v8::Local<v8::Object> New(v8::Isolate *isolate);
        static void Set(v8::Isolate *isolate, v8::Object *object);

        // Accessor methods
        BinaryDataMethod BinaryDataMethod()   { return binaryDataMethod; }
        const char *BinaryDataDirectory()     { return binaryDataDirectory.c_str(); }
        const char *BinaryDataUrlPrefix()     { return binaryDataUrlPrefix.c_str(); }
        bool                 APESave()        { return apeSave; }
        bool                 ID3V1Save()      { return id3v1Save; }
        bool                 ID3V2Save()      { return id3v2Save; }
        uint32_t             ID3V2Version()   { return id3v2Version; }
        TagLib::String::Type ID3V2Encoding()  { return id3v2Encoding; }


    protected:
        ~Configuration() {}
    private:
        Configuration() {}
        Configuration(Configuration const&)   = delete;
        void operator=(Configuration const&)  = delete;

        // Base configuration
        enum BinaryDataMethod binaryDataMethod = FILENAME; // how to process binary attachments and images
        std::string binaryDataDirectory = ".";    // default directory for exporting and importing files
        std::string binaryDataUrlPrefix = "";   // relative URL prefix for BinaryDataMethod::RELATIVE_URL
        bool apeSave = false;
        bool id3v1Save = false;
        bool id3v2Save = true;
        uint32_t id3v2Version = 4;
        TagLib::String::Type id3v2Encoding = TagLib::String::UTF8;
    };
}



#endif //TAGIO_CONFIGURATION_H
