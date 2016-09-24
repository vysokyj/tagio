#include "id3v2tag.h"
#include "id3v2frame.h"

using namespace TagIO;
using namespace v8;
using namespace std;

Local<Array> ID3v2Tag::New(Isolate *isolate, TagLib::ID3v2::Tag *tag) {
    EscapableHandleScope handleScope(isolate);
    TagLib::ID3v2::FrameList frameList = tag->frameList();
    Local<Array> array = Array::New(isolate, frameList.size());
    for (unsigned int i = 0; i < frameList.size(); i++) {
        TagLib::ID3v2::Frame *frame = frameList[i];
        Local<Object> object = ID3v2Frame::New(isolate, frame);
        array->Set(i, object);
    }
    return handleScope.Escape(array);
}

void ID3v2Tag::Set(Isolate *isolate, Array *frames, TagLib::ID3v2::Tag *tag) {
    Clear(tag);
    for (unsigned int i = 0; i < frames->Length(); i++) {
        Local<Object> object = frames->Get(i)->ToObject();
        ID3v2Frame::Set(isolate, *object, tag);
    }
}

void ID3v2Tag::Clear(TagLib::ID3v2::Tag *tag) {
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
