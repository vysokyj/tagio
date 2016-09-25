#include "audioproperties.h"
#include "wrapper.h"

void ExportAudioProperties(TagLib::AudioProperties *audioProperties, v8::Object *object) {
    TagLibWrapper o(object);
    o.SetInt32("length", audioProperties->length());
    o.SetInt32("bitrate", audioProperties->bitrate());
    o.SetInt32("sampleRate", audioProperties->sampleRate());
    o.SetInt32("channels", audioProperties->channels());
}
