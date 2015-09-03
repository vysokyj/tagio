#ifndef TAGIO_ID3V2FRAME_H
#define TAGIO_ID3V2FRAME_H

#include <v8.h>
#include <taglib/id3v2frame.h>

namespace TagIO {
    class ID3v2Frame {
    public:
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::ID3v2::Frame *frame);
        static void Set(v8::Isolate *isolate, v8::Object *object, TagLib::ID3v2::Tag *tag);
    };
}



#endif //TAGIO_ID3V2FRAME_H
