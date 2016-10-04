#ifndef TAGIO_ID3V2FRAME_H
#define TAGIO_ID3V2FRAME_H


#include "configuration.h"

#include <nan.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>


void ExportID3v2Frame(TagLib::ID3v2::Frame *frame, v8::Object *object, Configuration *conf);
void ImportID3v2Frame(v8::Object *object, TagLib::ID3v2::Tag *tag, std::map<uintptr_t, std::string> *fmap, Configuration *conf);


#endif //TAGIO_ID3V2FRAME_H
