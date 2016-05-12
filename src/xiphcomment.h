#ifndef TAGIO_XIPH_COMMENT_H
#define TAGIO_XIPH_COMMENT_H

#include <node.h>
#include <taglib/xiphcomment.h>

namespace TagIO {

    class XiphComment {
    public:
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::Ogg::XiphComment *tag);
        static void Set(v8::Isolate *isolate, v8::Object *object, TagLib::Ogg::XiphComment *tag);
    };
}


#endif //TAGIO_XIPH_COMMENT_H
