#ifndef TAGIO_ID3V1_TAG_H
#define TAGIO_ID3V1_TAG_H

#include <node.h>
#include <taglib/id3v1tag.h>
#include <taglib/tstring.h>
#include <taglib/tbytevector.h>

namespace TagIO {

    class StringHandler : public TagLib::ID3v1::StringHandler {
        virtual TagLib::String parse(const TagLib::ByteVector &data) const;
        virtual TagLib::ByteVector render(const TagLib::String &s) const;
    };

    class ID3v1Tag {
    public:
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::ID3v1::Tag *tag);
        static void Set(v8::Isolate *isolate, v8::Object *object, TagLib::ID3v1::Tag *tag);
    };
}


#endif //TAGIO_ID3V1_TAG_H
