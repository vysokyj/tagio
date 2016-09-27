#ifndef TAGIO_ID3V2TAG_H
#define TAGIO_ID3V2TAG_H

#include <nan.h>
#include <taglib/id3v2tag.h>

#include "configuration.h"

void ExportID3v2(TagLib::ID3v2::Tag *tag, v8::Array *frames, Configuration *conf);
void ImportID3v2(v8::Array *frames, TagLib::ID3v2::Tag *tag, Configuration *conf);

#endif //TAGIO_ID3V2TAG_H
