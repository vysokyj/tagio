#ifndef TAGIO_FILESYSTEM_H
#define TAGIO_FILESYSTEM_H

#include <taglib/tstring.h>
#include <taglib/tbytevector.h>

namespace TagIO {
    class ByteVector {
    public:
        static TagLib::String Export(TagLib::ByteVector byteVector, TagLib::String mimeType);
        static TagLib::ByteVector Import(TagLib::String path);
    private:
        static TagLib::String ExportFile(TagLib::ByteVector byteVector, TagLib::String mimeType);
        static TagLib::ByteVector ImportFile(TagLib::String path);
        static std::string NewPath(std::string directoryPath, std::string fileName);
        static std::string NewRelativeUrl(std::string relativeUrl, std::string fileName);
        static std::string CountMD5(TagLib::ByteVector byteVector);
        static std::string NewFileName(TagLib::ByteVector byteVector, std::string mimeType);
    };
}


#endif //TAGIO_FILESYSTEM_H
