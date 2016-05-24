#ifndef TAGIO_CONFIGURATION_H
#define TAGIO_CONFIGURATION_H

#include <node.h>
#include <string>
#include <taglib/tstring.h>

namespace TagIO {

    class Configuration {
    public:
        static Configuration &Get() {
            static Configuration instance;
            return instance;
        }
        static v8::Local<v8::Object> New(v8::Isolate *isolate);
        static void Set(v8::Isolate *isolate, v8::Object *object);

        enum BinaryDataMethodTypes {
            IGNORE = 1,       // IGNORE BINARY FILES
            FILENAME = 2,     // JSON contains just the filename -> somefile.ext
            ABSOLUTE_URL = 3, // JSON contains compete file URL -> file://somepath/somefile.ext
            PREFIXED_URL = 4  // JSON contains file URL with given prefix -> /somepath/somefile.ext
        };

        // Accessor methods
        int BinaryDataMethod()   { return binaryDataMethod; }
        const char *BinaryDataDirectory()     { return binaryDataDirectory.c_str(); }
        const char *BinaryDataUrlPrefix()     { return binaryDataUrlPrefix.c_str(); }
        bool                 APESave()        { return apeSave; }
        bool                 ID3V1Save()      { return id3v1Save; }
        TagLib::String::Type ID3V1Encoding()  { return id3v1Encoding; }
        bool                 ID3V2Save()      { return id3v2Save; }
        uint32_t             ID3V2Version()   { return id3v2Version; }
        TagLib::String::Type ID3V2Encoding()  { return id3v2Encoding; }
        bool        ID3V2UseFrameEncoding()   { return id3v2UseFrameEncoding; }


    protected:
        ~Configuration() {}
    private:
        Configuration() {}
        Configuration(Configuration const&)   = delete;
        void operator=(Configuration const&)  = delete;

        // Helper methods
        static int StringToBinaryDataMethod(TagLib::String string);
        static TagLib::String BinaryDataMethodToString(int binaryDataMethod);

        // Base configuration
        int binaryDataMethod = BinaryDataMethodTypes::FILENAME; // how to process binary attachments and images
        std::string binaryDataDirectory = ".";    // default directory for exporting and importing files
        std::string binaryDataUrlPrefix = "";     // relative URL prefix for BinaryDataMethod::RELATIVE_URL
        bool apeSave = false;
        bool id3v1Save = false;
        TagLib::String::Type id3v1Encoding = TagLib::String::UTF8;
        bool id3v2Save = true;
        uint32_t id3v2Version = 4;
        TagLib::String::Type id3v2Encoding = TagLib::String::UTF8;
        bool id3v2UseFrameEncoding = false;
    };
}



#endif //TAGIO_CONFIGURATION_H
