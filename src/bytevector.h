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
        static std::string NewPath(std::string directoryPath, std::string fileName);
        static std::string NewRelativeUrl(std::string relativeUrl, std::string fileName);
        static std::string NewAbsoluteUrl(std::string filePath);
        static std::string CountMD5(TagLib::ByteVector byteVector);
        static std::string NewFileName(TagLib::ByteVector byteVector, std::string mimeType);
        static std::string PathToString(std::string filePath, std::string fileName);
        static std::string StringToPath(std::string str);
        static bool FileExist(const std::string& name);
    };
}


#endif //TAGIO_FILESYSTEM_H
