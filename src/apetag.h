#ifndef TAGIO_APETAG_H
#define TAGIO_APETAG_H

#include <node.h>
#include <taglib/apetag.h>

void ExportAPETag(TagLib::APE::Tag *tag, v8::Object *object);
void ImportAPETag(v8::Object *object, TagLib::APE::Tag *tag);

#endif //TAGIO_APETAG_H
