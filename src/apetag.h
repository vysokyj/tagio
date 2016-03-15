#ifndef TAGIO_APETAG_H
#define TAGIO_APETAG_H

#include <node/node.h>
#include <taglib/apetag.h>

namespace TagIO {
    class APETag {
    public:
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::APE::Tag *tag);
        static void Set(v8::Isolate *isolate, v8::Object *object, TagLib::APE::Tag *tag);
    };
}


#endif //TAGIO_APETAG_H
