#ifndef TAGIO_FLAC_PICTURE_H
#define TAGIO_FLAC_PICTURE_H

#include <node.h>
#include <node_object_wrap.h>

#include <string>
#include <iostream>
#include <fstream>
#include <taglib/flacpicture.h>

#include "base.h"
#include "wrapper.h"

namespace TagIO {
    class FLACPictures {
    public:
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::List<TagLib::FLAC::Picture *> list);
        static void Set(v8::Isolate *isolate, v8::Object *object, TagLib::List<TagLib::FLAC::Picture *> list);
    };
}

#endif // TAGIO_FLAC_PICTURE_H
