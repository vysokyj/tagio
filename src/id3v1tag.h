#ifndef TAGIO_ID3V1_TAG_H
#define TAGIO_ID3V1_TAG_H

#include <nan.h>
#include <taglib/id3v1tag.h>

#include "configuration.h"

void ExportID3v1Tag(TagLib::ID3v1::Tag *tag, v8::Object *object);
void ImportID3v1Tag(v8::Object *object, TagLib::ID3v1::Tag *tag, Configuration *conf);

#endif //TAGIO_ID3V1_TAG_H
