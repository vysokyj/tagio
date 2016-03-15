#ifndef TAGIO_ID3V2TAG_H
#define TAGIO_ID3V2TAG_H


#include <node/node.h>
#include <taglib/id3v2tag.h>

namespace TagIO {
    class ID3v2Tag {
    public:
        static v8::Local<v8::Array> New(v8::Isolate *isolate, TagLib::ID3v2::Tag *tag);
        static void Set(v8::Isolate *isolate, v8::Array *frames, TagLib::ID3v2::Tag *tag);
    protected:
        static void Clear(TagLib::ID3v2::Tag *tag);
    };
}


#endif //TAGIO_ID3V2TAG_H
