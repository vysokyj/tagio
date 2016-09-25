#ifndef TAGIO_TAG_H
#define TAGIO_TAG_H

#include <nan.h>
#include <taglib/tag.h>



void ExportTag(TagLib::Tag *tag, v8::Object *object);
void ImportTag(v8::Object *object, TagLib::Tag *tag);


#endif //TAGIO_TAG_H
