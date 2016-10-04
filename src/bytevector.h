#ifndef TAGIO_FILESYSTEM_H
#define TAGIO_FILESYSTEM_H

#include <taglib/tstring.h>
#include <taglib/tbytevector.h>
#include "configuration.h"

TagLib::String ExportByteVector(TagLib::ByteVector byteVector, TagLib::String mimeType, Configuration *conf);
TagLib::ByteVector ImportByteVector(TagLib::String path, Configuration *conf);
TagLib::ByteVector ImportByteVector(std::string path, Configuration *conf);

#endif //TAGIO_FILESYSTEM_H
