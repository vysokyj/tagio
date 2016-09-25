#ifndef TAGIO_AUDIOPROPERTIES_H
#define TAGIO_AUDIOPROPERTIES_H

#include <nan.h>
#include <taglib/audioproperties.h>

void ExportAudioProperties(TagLib::AudioProperties *audioProperties, v8::Object *object);


#endif //TAGIO_AUDIOPROPERTIES_H
