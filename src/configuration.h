#ifndef TAGIO_CONFIGURATION_H
#define TAGIO_CONFIGURATION_H

#include <node/node.h>
#include <v8.h>
#include <string>

namespace TagIO {

    class Configuration {
    public:
        enum BinaryDataMethod {
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

        BinaryDataMethod GetBinaryDataMethod() { return binaryDataMethod; }
        const char *GetBinaryDataDirectory() { return binaryDataDirectory.c_str(); }
        const char *GetBinaryDataUrlPrefix() { return binaryDataUrlPrefix.c_str(); }
        bool GetSaveID3v1Tag() { return  saveID3v1Tag; }
        bool GetSaveID3v2Tag() { return  saveID3v2Tag; }
        bool GetSaveApeTag() { return  saveApeTag; }
    protected:
        ~Configuration() {}
    private:
        Configuration() {}
        Configuration(Configuration const&)   = delete;
        void operator=(Configuration const&)  = delete;

        static Configuration *instance;

        // Base configuration
        BinaryDataMethod binaryDataMethod = FILENAME; // how to process binary attachments and images
        std::string binaryDataDirectory = ".";    // default directory for exporting and importing files
        std::string binaryDataUrlPrefix = "";   // relative URL prefix for BinaryDataMethod::RELATIVE_URL
        bool saveID3v1Tag = false;
        bool saveID3v2Tag = true;
        bool saveApeTag = false;
    };
}



#endif //TAGIO_CONFIGURATION_H
