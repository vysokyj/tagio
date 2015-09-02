#include "audioproperties.h"
#include "wrapper.h"

using namespace TagIO;
using namespace v8;

Local<Object> AudioProperties::New(Isolate *isolate, TagLib::AudioProperties *audioProperties) {
    EscapableHandleScope handleScope(isolate);
    Local<Object> object = Object::New(isolate);
    Wrapper o(isolate, *object);
    o.SetInt32("length", audioProperties->length());
    o.SetInt32("bitrate ", audioProperties->bitrate());
    o.SetInt32("sampleRate", audioProperties->sampleRate());
    o.SetInt32("channels", audioProperties->channels());
    return handleScope.Escape(object);
}

