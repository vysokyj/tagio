#ifndef TAGIO_TAG_H
#define TAGIO_TAG_H

#include <nan.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>

struct GenericTag {
    TagLib::String title;
    TagLib::String album;
    TagLib::String artist;
    TagLib::uint track;
    TagLib::uint year;
    TagLib::String genre;
    TagLib::String comment;
};

void ExportTag(GenericTag *tag, v8::Object *object) ;
void ExportTag(TagLib::Tag *tag, v8::Object *object);
void ImportTag(v8::Object *object, TagLib::Tag *tag);
void ImportTag(v8::Object *object, GenericTag *tag);


#endif //TAGIO_TAG_H
