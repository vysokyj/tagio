#ifndef TAGIO_XIPH_COMMENT_H
#define TAGIO_XIPH_COMMENT_H

#include <nan.h>
#include <taglib/xiphcomment.h>

void ExportXiphComment(TagLib::Ogg::XiphComment *tag, v8::Array);
void ImportXiphComment(v8::Array *array, TagLib::Ogg::XiphComment *tag);

#endif //TAGIO_XIPH_COMMENT_H
