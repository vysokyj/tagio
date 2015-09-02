#ifndef TAGIO_MPEG_H
#define TAGIO_MPEG_H

#include <node/node.h>
#include <node/node_object_wrap.h>

#include <string>
#include <iostream>
#include <fstream>
#include <taglib/mpegfile.h>
#include <taglib/apetag.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/commentsframe.h>
#include <taglib/generalencapsulatedobjectframe.h>
#include <taglib/ownershipframe.h>
#include <taglib/popularimeterframe.h>
#include <taglib/privateframe.h>
#include <taglib/relativevolumeframe.h>
#include <taglib/textidentificationframe.h>
#include <taglib/uniquefileidentifierframe.h>
#include <taglib/unknownframe.h>
#include <taglib/unsynchronizedlyricsframe.h>
#include <taglib/urllinkframe.h>

#include "base.h"
#include "wrapper.h"

namespace TagIO {
    class MPEG : public Base, public node::ObjectWrap {
    public:
        static void Init(v8::Handle<v8::Object> exports);
        void SetID3v1TagEnabled(bool value) { this->saveID3v1Tag = value; }
        void SetID3v2TagEnabled(bool value) { this->saveID3v2Tag = value; }
        void SetApeTagEnabled(bool value) { this->saveApeTag = value; }
    private:
        explicit MPEG(const char *path);
        ~MPEG();

        // Generic API
        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void Save(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetPath(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetAudioProperties(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetTag(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void SetTag(const v8::FunctionCallbackInfo<v8::Value>& args);

        // MPEG API
        static void HasAPETag(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void HasID3v1Tag(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void HasID3v2Tag(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void GetID3v2Tag(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void SetID3v2Tag(const v8::FunctionCallbackInfo<v8::Value>& args);

        // members
        static v8::Persistent<v8::Function> constructor;
        TagLib::MPEG::File *file;
        bool saveID3v1Tag = false;
        bool saveID3v2Tag = true;
        bool saveApeTag = false;
    };
}


#endif //TAGIO_MPEG_H
