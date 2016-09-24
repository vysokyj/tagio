#ifndef TAGIO_AUDIOPROPERTIES_H
#define TAGIO_AUDIOPROPERTIES_H

#include <nan.h>
#include <taglib/audioproperties.h>

v8::Local<v8::Object> ExportAudioProperties(TagLib::AudioProperties *audioProperties);


#endif //TAGIO_AUDIOPROPERTIES_H
