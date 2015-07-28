#ifndef TAGIO_GENERIC_H
#define TAGIO_GENERIC_H

#include <node/node.h>
#include <node/node_object_wrap.h>

#include <string>
#include <iostream>
#include <fstream>
#include <taglib/fileref.h>

#include "base.h"

namespace TagIO {
    class GENERIC : public Base, public node::ObjectWrap {
    public:
        static void Init(v8::Handle<v8::Object> exports);
    private:
        explicit GENERIC(const char *path);
        ~GENERIC();

        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void Save(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetPath(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetConfiguration(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetTag(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void SetTag(const v8::FunctionCallbackInfo<v8::Value>& args);

        // members
        static v8::Persistent<v8::Function> constructor;
        TagLib::FileRef *file;

    };
}


#endif //TAGIO_GENERIC_H
