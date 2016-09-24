#include "audioproperties.h"
#include "wrapper.h"

using Nan::EscapableHandleScope;
using Nan::New;
using v8::Local;
using v8::Object;

Local<Object> ExportAudioProperties(TagLib::AudioProperties *audioProperties) {
    EscapableHandleScope scope;
    Local<Object> object = New<Object>();
    TagLibWrapper o( *object);
    o.SetInt32("length", audioProperties->length());
    o.SetInt32("bitrate", audioProperties->bitrate());
    o.SetInt32("sampleRate", audioProperties->sampleRate());
    o.SetInt32("channels", audioProperties->channels());
    return scope.Escape(object);
}

