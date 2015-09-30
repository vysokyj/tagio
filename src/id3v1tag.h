#ifndef TAGIO_ID3V1_TAG_H
#define TAGIO_ID3V1_TAG_H

#include <v8.h>
#include <taglib/id3v1tag.h>

namespace TagIO {
    class ID3v1Tag {
    public:
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::ID3v1::Tag *tag);
        static void Set(v8::Isolate *isolate, v8::Object *object, TagLib::ID3v1::Tag *tag);
    };
}


#endif //TAGIO_ID3V1_TAG_H
