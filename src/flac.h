#ifndef TAGIO_FLAC_H
#define TAGIO_FLAC_H

#include <node.h>
#include <node_object_wrap.h>

#include <string>
#include <iostream>
#include <fstream>
#include <taglib/flacfile.h>

#include "base.h"
#include "wrapper.h"

namespace TagIO {
    class FLAC : public Base, public node::ObjectWrap {
    public:
        static void Init(v8::Handle<v8::Object> exports);
    private:
        explicit FLAC(const char *path);
        ~FLAC();

        // Generic API
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void Save(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetPath(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetAudioProperties(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetTag(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void SetTag(const v8::FunctionCallbackInfo<v8::Value>& args);

        // FLAC API

        // members
        static v8::Persistent<v8::Function> constructor;
        TagLib::FLAC::File *file;
        bool saved = false;
    };
}


#endif //TAGIO_FLAC_H
