#include "id3v2tag.h"
#include "id3v2frame.h"

using namespace std;

using v8::Local;
using v8::Object;
using v8::Array;
using Nan::HandleScope;
using Nan::New;

static void Clear(TagLib::ID3v2::Tag *tag) {
    typedef list<TagLib::ByteVector> ByteVectorList;
    TagLib::ID3v2::FrameList frameList = tag->frameList();
    ByteVectorList list;
    for (uint32_t i = 0; i < frameList.size(); i++) {
        TagLib::ID3v2::Frame *frame = frameList[i];
        TagLib::ByteVector vector = frame->frameID();
        string id = string(vector.data(), vector.size());
        if (id.at(0) == 'T' ||
            id.at(0) == 'W' ||
            id.compare("COMM") == 0 ||
            id.compare("APIC") == 0 ||
            id.compare("GEOB") == 0 ||
            id.compare("POPM") == 0 ||
            id.compare("PRIV") == 0 ||
            id.compare("RVA2") == 0 ||
            id.compare("UFID") == 0 ||
            id.compare("USLT") == 0) {
            list.push_back(vector);
        }
    }
    for (ByteVectorList::const_iterator ci = list.begin(); ci != list.end(); ++ci)
        tag->removeFrames(*ci);

    list.clear();
}

void ExportID3v2Tag(TagLib::ID3v2::Tag *tag, v8::Array *frames, Configuration *conf) {
    HandleScope scope;
    TagLib::ID3v2::FrameList frameList = tag->frameList();
    for (unsigned int i = 0; i < frameList.size(); i++) {
        TagLib::ID3v2::Frame *frame = frameList[i];
        Local<Object> object = New<Object>();
        ExportID3v2Frame(frame, *object, conf);
        frames->Set(i, object);
    }
}

void ImportID3v2Tag(v8::Array *frames, TagLib::ID3v2::Tag *tag, Configuration *conf) {
    Clear(tag);
    for (unsigned int i = 0; i < frames->Length(); i++) {
        Local<Object> object = frames->Get(i)->ToObject();
        ImportID3v2Frame(*object, tag, conf);
    }
}

