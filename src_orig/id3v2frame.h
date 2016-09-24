#ifndef TAGIO_ID3V2FRAME_H
#define TAGIO_ID3V2FRAME_H

#include "wrapper.h"
#include <node.h>
#include <taglib/id3v2frame.h>

namespace TagIO {
    class ID3v2Frame {
    public:
        static v8::Local<v8::Object> New(v8::Isolate *isolate, TagLib::ID3v2::Frame *frame);
        static void Set(v8::Isolate *isolate, v8::Object *object, TagLib::ID3v2::Tag *tag);
    private:
        static void GetTXXX(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetTXXX(Wrapper &o, TagLib::ID3v2::Tag *tag);
        static void GetTYYY(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetTYYY(Wrapper &o, TagLib::ID3v2::Tag  *tag, const TagLib::ByteVector &id);
        static void GetWXXX(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetWXXX(Wrapper &o, TagLib::ID3v2::Tag  *tag);
        static void GetWYYY(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetWYYY(Wrapper &o, TagLib::ID3v2::Tag  *tag, const TagLib::ByteVector &id);
        static void GetCOMM(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetCOMM(Wrapper &o, TagLib::ID3v2::Tag  *tag);
        static void GetAPIC(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetAPIC(Wrapper &o, TagLib::ID3v2::Tag  *tag);
        static void GetGEOB(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetGEOB(Wrapper &o, TagLib::ID3v2::Tag  *tag);
        static void GetPOPM(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetPOPM(Wrapper &o, TagLib::ID3v2::Tag  *tag);
        static void GetPRIV(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetPRIV(Wrapper &o, TagLib::ID3v2::Tag  *tag);
        static void GetRVA2(Wrapper &o, TagLib::ID3v2::Frame *frame, v8::Isolate *isolate, v8::Object *object);
        static void SetRVA2(Wrapper &o, TagLib::ID3v2::Tag  *tag, v8::Isolate *isolate, v8::Object *object);
        static void GetUFID(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetUFID(Wrapper &o, TagLib::ID3v2::Tag  *tag, const TagLib::ByteVector &id);
        static void GetUSLT(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetUSLT(Wrapper &o, TagLib::ID3v2::Tag  *tag, const TagLib::ByteVector &id);
        static void GetNONE(Wrapper &o, TagLib::ID3v2::Frame *frame);
        static void SetNONE(Wrapper &o, TagLib::ID3v2::Tag  *tag, const TagLib::ByteVector &id);
    };
}



#endif //TAGIO_ID3V2FRAME_H
