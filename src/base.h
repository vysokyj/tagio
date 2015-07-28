#ifndef TAGIO_BASE_H
#define TAGIO_BASE_H

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <v8.h>
#include <taglib/fileref.h>
#include <taglib/tbytevector.h>
#include "md5.h"

namespace TagIO {

    class Base {
    public:
        std::string GetFilePath() { return this->path; }
        std::string GetAttachmentsDir() { return this->attachmentsDir; }
        void SetAttachmentsDir(std::string attachmentsDir) { this->attachmentsDir = FixPath(attachmentsDir); }
        std::string GetAttachmetnsCtx() { return this->attachmentsCtx; }
        void SetAttachmentsCtx(std::string attachmentsCtx) { this->attachmentsCtx = FixContext(attachmentsCtx); };

        static void SetTagByObject(v8::Isolate *isolate, v8::Object *object, TagLib::Tag *tag);
        static void SetObjectByTag(v8::Isolate *isolate, v8::Object *object, TagLib::Tag *tag);

        // helper methods
        static TagLib::uint GetUint32(v8::Isolate *isolate, v8::Object *object, const char *key);
        static void SetUint32(v8::Isolate *isolate, v8::Object *object, const char *key, const TagLib::uint value);
        static TagLib::String GetString(v8::Isolate *isolate, v8::Object *object, const char *key);
        static void SetString(v8::Isolate *isolate, v8::Object *object, const char *key, const TagLib::String value);
    protected:
        Base(std::string path);
        ~Base();
        TagLib::String ExportFile(TagLib::ByteVector byteVector, TagLib::String mimeType);
        TagLib::ByteVector ImportFile(TagLib::String path);
    private:
        std::string attachmentsDir;
        std::string attachmentsCtx;
        std::string path;
        static std::string FixPath(std::string path);
        static std::string FixContext(std::string context);
        static std::string NewPath(std::string directory, std::string fileName);
        static std::string CountMD5(TagLib::ByteVector byteVector);
        std::string NewFileName(TagLib::ByteVector byteVector, std::string mimeType);
    };
}

#endif //TAGIO_BASE_H
