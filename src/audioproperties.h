#ifndef TAGIO_AUDIOPROPERTIES_H
#define TAGIO_AUDIOPROPERTIES_H

#include <node/node.h>
#include <taglib/audioproperties.h>

namespace TagIO {
    class AudioProperties {
    public:
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::AudioProperties *audioProperties);
    };
}


#endif //TAGIO_AUDIOPROPERTIES_H
