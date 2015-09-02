#include "id3v2frame.h"
#include "wrapper.h"

#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/commentsframe.h>
#include <taglib/generalencapsulatedobjectframe.h>
#include <taglib/ownershipframe.h>
#include <taglib/popularimeterframe.h>
#include <taglib/privateframe.h>
#include <taglib/relativevolumeframe.h>
#include <taglib/textidentificationframe.h>
#include <taglib/uniquefileidentifierframe.h>
#include <taglib/unknownframe.h>
#include <taglib/unsynchronizedlyricsframe.h>
#include <taglib/urllinkframe.h>

using namespace TagIO;
using namespace v8;
using namespace std;

static map<uint, TagLib::ID3v2::AttachedPictureFrame::Type> APIC = {
        {0x00, TagLib::ID3v2::AttachedPictureFrame::Other},
        {0x01, TagLib::ID3v2::AttachedPictureFrame::FileIcon},
        {0x02, TagLib::ID3v2::AttachedPictureFrame::OtherFileIcon},
        {0x03, TagLib::ID3v2::AttachedPictureFrame::FrontCover},
        {0x04, TagLib::ID3v2::AttachedPictureFrame::BackCover},
        {0x05, TagLib::ID3v2::AttachedPictureFrame::LeafletPage},
        {0x06, TagLib::ID3v2::AttachedPictureFrame::Media},
        {0x07, TagLib::ID3v2::AttachedPictureFrame::LeadArtist},
        {0x08, TagLib::ID3v2::AttachedPictureFrame::Artist},
        {0x09, TagLib::ID3v2::AttachedPictureFrame::Conductor},
        {0x0A, TagLib::ID3v2::AttachedPictureFrame::Band},
        {0x0B, TagLib::ID3v2::AttachedPictureFrame::Composer},
        {0x0C, TagLib::ID3v2::AttachedPictureFrame::Lyricist},
        {0x0D, TagLib::ID3v2::AttachedPictureFrame::RecordingLocation},
        {0x0E, TagLib::ID3v2::AttachedPictureFrame::DuringRecording},
        {0x0F, TagLib::ID3v2::AttachedPictureFrame::DuringPerformance},
        {0x10, TagLib::ID3v2::AttachedPictureFrame::MovieScreenCapture},
        {0x11, TagLib::ID3v2::AttachedPictureFrame::ColouredFish},
        {0x12, TagLib::ID3v2::AttachedPictureFrame::Illustration},
        {0x13, TagLib::ID3v2::AttachedPictureFrame::BandLogo},
        {0x14, TagLib::ID3v2::AttachedPictureFrame::PublisherLogo}
};

Local<Object> ID3v2Frame::New(Isolate *isolate, TagLib::ID3v2::Frame *frame) {
    EscapableHandleScope handleScope(isolate);
    Local<Object> object = Object::New(isolate);
    Wrapper o(isolate, *object);
    TagLib::ByteVector idBytes = frame->frameID();
    string id = string(idBytes.data(), idBytes.size());
    o.SetString("id", TagLib::String(id));
    if (id.at(0) == 'T') {
        auto *f = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(frame);
        o.SetString("text", f->toString());
    } else if (id.at(0) == 'W') {
        auto *f = dynamic_cast<TagLib::ID3v2::UrlLinkFrame *>(frame);
        o.SetString("url", f->url());
        o.SetString("text", f->toString());
    } else if (id.compare("COMM") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::CommentsFrame *>(frame);
        o.SetString("text", f->toString());
    } else if (id.compare("APIC") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame);
        o.SetString("mimeType", f->mimeType());
        o.SetString("description", f->description());
        o.SetUint32("type", f->type());
        o.SetString("text", f->toString());
        o.SetBinary("file", f->picture(), f->mimeType());
    } else if (id.compare("GEOB") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::GeneralEncapsulatedObjectFrame *>(frame);
        o.SetString("mimeType", f->mimeType());
        o.SetString("fileName", f->fileName());
        o.SetBinary("file", f->object(), f->mimeType());
    } else if (id.compare("PRIV") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::PrivateFrame *>(frame);
        o.SetString("owner", f->owner());
    } else if (id.compare("RVA2") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::RelativeVolumeFrame *>(frame);
        TagLib::List<TagLib::ID3v2::RelativeVolumeFrame::ChannelType> channels = f->channels();
        Local<Array> channelArray = Array::New(isolate, channels.size());
        for (unsigned int i = 0; i < channels.size(); i++)
            channelArray->Set(i, Integer::New(isolate, channels[i]));
        object->Set(String::NewFromUtf8(isolate, "channels"), channelArray);
    } else if (id.compare("UFID") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::UniqueFileIdentifierFrame *>(frame);
        TagLib::String mimeType("data/bin", TagLib::String::UTF8); //TODO: Mime type
        o.SetString("owner", f->owner());
        o.SetBinary("file", f->identifier(), mimeType);
    } else if (id.compare("USLT") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frame);
        o.SetString("description", f->description());
        o.SetString("text", f->toString());
    } else {
        // fallback
        o.SetString("text", frame->toString());
    }
    return handleScope.Escape(object);
}

void ID3v2Frame::Set(Isolate *isolate, Object *object, TagLib::ID3v2::Tag *tag) {
    Wrapper o(isolate, object);
    String::Utf8Value idValue(object->Get(String::NewFromUtf8(isolate, "id"))->ToString());
    string id(*idValue);
    if (id.at(0) == 'T') {
        auto *frame= new TagLib::ID3v2::TextIdentificationFrame(*idValue, TagLib::String::UTF8);
        frame->setText(o.GetString("text"));
        tag->addFrame(frame);
    } else if (id.at(0) == 'W') {
        auto *frame = new TagLib::ID3v2::UrlLinkFrame(TagLib::ByteVector(*idValue, 4));
        frame->setUrl(o.GetString("url"));
        tag->addFrame(frame);
    } else if (id.compare("COMM") == 0) {
        auto *frame = new TagLib::ID3v2::CommentsFrame();
        frame->setText(o.GetString("text"));
        tag->addFrame(frame);
    } else if (id.compare("APIC") == 0) {
        uint32_t type(object->Get(String::NewFromUtf8(isolate, "type"))->Uint32Value());
        auto *frame = new TagLib::ID3v2::AttachedPictureFrame();
        frame->setMimeType(o.GetString("mimeType"));
        if (APIC.count(type)) frame->setType(APIC[type]);
        else frame->setType(TagLib::ID3v2::AttachedPictureFrame::Other);
        frame->setDescription(o.GetString("description"));
        frame->setPicture(o.GetBinary("file"));
        tag->addFrame(frame);
    } else if (id.compare("GEOB") == 0) {
        auto *frame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
        frame->setMimeType(o.GetString("mimeType"));
        frame->setFileName(o.GetString("fileName"));
        frame->setObject(o.GetBinary("file"));
        tag->addFrame(frame);
    } else if (id.compare("PRIV") == 0) {
        auto *frame = new TagLib::ID3v2::PrivateFrame();
        frame->setOwner(o.GetString("owner"));
    } else if (id.compare("RVA2") == 0) {
//            Local<Array> channelArray = Local<Array>::Cast(object->Get(String::NewFromUtf8(isolate, "text")));
//            auto *frame = new RelativeVolumeFrame();
//            for (unsigned int j = 0; j < channelArray->Length(); j++) {
//                Local<Object> channelObject = channelArray->Get(j)->ToObject();
//            }
//            tag->addFrame(frame);
        cout << "RVA2 not yet supported!" << endl;
    } else if (id.compare("UFID") == 0) {
        auto *frame = new TagLib::ID3v2::UniqueFileIdentifierFrame(o.GetString("owner"),
                                                                   TagLib::ByteVector(*idValue, 4));
        frame->setIdentifier(o.GetBinary("identifier"));
    } else if (id.compare("USLT") == 0) {
        auto *frame = new TagLib::ID3v2::UrlLinkFrame(TagLib::ByteVector(*idValue, 4));
        frame->setUrl(o.GetString("url"));
        frame->setText(o.GetString("text"));
        tag->addFrame(frame);
    } else { // fallback
        auto *frame = new TagLib::ID3v2::TextIdentificationFrame(*idValue, TagLib::String::UTF8);
        frame->setText(o.GetString("text"));
        tag->addFrame(frame);
    }
}
