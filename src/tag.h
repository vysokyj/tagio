#ifndef TAGIO_TAG_H
#define TAGIO_TAG_H

#include <v8.h>
#include <taglib/tag.h>

namespace TagIO {
    class Tag {
    public:
        /**
         * Create new V8 Object with tag properties.
         */
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::Tag *tag);

        /**
         * Set TagLib tag by properties from V8 Object.
         */
        static void Set(v8::Isolate *isolate, v8::Object *object, TagLib::Tag *tag);
    };
}


#endif //TAGIO_TAG_H
