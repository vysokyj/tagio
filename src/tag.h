#ifndef TAGIO_TAG_H
#define TAGIO_TAG_H

#include <nan.h>
#include <taglib/tag.h>



v8::Local<v8::Object> ExportTag(TagLib::Tag *tag);
void ImportTag(v8::Object *object, TagLib::Tag *tag);


#endif //TAGIO_TAG_H
