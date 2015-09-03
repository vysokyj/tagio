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

static map<uint, TagLib::ID3v2::RelativeVolumeFrame::ChannelType> RVA2 = {
    {0x00, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::Other},
    {0x01, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::MasterVolume},
    {0x02, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::FrontRight},
    {0x03, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::FrontLeft},
    {0x04, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::BackRight},
    {0x05, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::BackLeft},
    {0x06, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::FrontCentre},
    {0x07, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::BackCentre},
    {0x08, TagLib::ID3v2::RelativeVolumeFrame::ChannelType::Subwoofer}
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
    } else if (id.compare("COMM") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::CommentsFrame *>(frame);
        o.SetString("text", f->toString());
    } else if (id.compare("APIC") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame);
        o.SetString("mimeType", f->mimeType());
        o.SetString("description", f->description());
        o.SetUint32("type", f->type());
        //o.SetString("text", f->toString());
        o.SetBytes("picture", f->picture(), f->mimeType());
    } else if (id.compare("GEOB") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::GeneralEncapsulatedObjectFrame *>(frame);
        o.SetString("mimeType", f->mimeType());
        o.SetString("fileName", f->fileName());
        o.SetBytes("object", f->object(), f->mimeType());
    } else if (id.compare("PRIV") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::PrivateFrame *>(frame);
        o.SetString("owner", f->owner());
    } else if (id.compare("RVA2") == 0) {
        //TODO: Test RVA2 usability
        auto *f = dynamic_cast<TagLib::ID3v2::RelativeVolumeFrame *>(frame);
        TagLib::List<TagLib::ID3v2::RelativeVolumeFrame::ChannelType> channels = f->channels();
        Local<Array> channelArray = Array::New(isolate, channels.size());
        for (unsigned int i = 0; i < channels.size(); i++) {
            TagLib::ID3v2::RelativeVolumeFrame::ChannelType channelType = channels[i];
            Local<Object> channelObject = Object::New(isolate);
            Wrapper co(isolate, *channelObject);
            TagLib::ID3v2::RelativeVolumeFrame::PeakVolume peakVolume = f->peakVolume(channelType);
            co.SetUint32("channelType", channelType); //numeric
            co.SetNumber("volumeAdjustment", (double) f->volumeAdjustment(channelType));
            co.SetString("bitsRepresentingPeak", (char) peakVolume.bitsRepresentingPeak);
            co.SetString("peakVolume", TagLib::String(peakVolume.peakVolume));
            channelArray->Set(i, channelObject);
        }
        object->Set(String::NewFromUtf8(isolate, "channels"), channelArray);
    } else if (id.compare("UFID") == 0) {
        auto *f = dynamic_cast<TagLib::ID3v2::UniqueFileIdentifierFrame *>(frame);
        TagLib::String mimeType("data/bin", TagLib::String::UTF8); //TODO: Mime type
        o.SetString("owner", f->owner());
        o.SetBytes("file", f->identifier(), mimeType);
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
        auto *f= new TagLib::ID3v2::TextIdentificationFrame(*idValue, TagLib::String::UTF8);
        f->setText(o.GetString("text"));
        tag->addFrame(f);
    } else if (id.at(0) == 'W') {
        auto *f = new TagLib::ID3v2::UrlLinkFrame(TagLib::ByteVector(*idValue, 4));
        f->setUrl(o.GetString("url"));
        tag->addFrame(f);
    } else if (id.compare("COMM") == 0) {
        auto *f = new TagLib::ID3v2::CommentsFrame();
        f->setText(o.GetString("text"));
        tag->addFrame(f);
    } else if (id.compare("APIC") == 0) {
        uint32_t type = o.GetUint32("type");
        auto *f = new TagLib::ID3v2::AttachedPictureFrame();
        f->setMimeType(o.GetString("mimeType"));
        if (APIC.count(type)) f->setType(APIC[type]);
        else f->setType(TagLib::ID3v2::AttachedPictureFrame::Other);
        f->setDescription(o.GetString("description"));
        f->setPicture(o.GetBytes("picture"));
        tag->addFrame(f);
    } else if (id.compare("GEOB") == 0) {
        auto *f = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
        f->setMimeType(o.GetString("mimeType"));
        f->setFileName(o.GetString("fileName"));
        f->setObject(o.GetBytes("object"));
        tag->addFrame(f);
    } else if (id.compare("PRIV") == 0) {
        auto *f = new TagLib::ID3v2::PrivateFrame();
        f->setOwner(o.GetString("owner"));
    } else if (id.compare("RVA2") == 0) {
        //TODO: Test RVA2 usability
        Local<Array> channelArray = Local<Array>::Cast(object->Get(String::NewFromUtf8(isolate, "text")));
        auto *f = new TagLib::ID3v2::RelativeVolumeFrame();
        for (unsigned int i = 0; i < channelArray->Length(); i++) {
            Local<Object> channelObject = channelArray->Get(i)->ToObject();
            Wrapper co(isolate, *channelObject);
            TagLib::ID3v2::RelativeVolumeFrame::ChannelType channelType = RVA2[co.GetInt32("channelType")];
            float volumeAdjustment = (float) co.GetNumber("volumeAdjustment");
            TagLib::ID3v2::RelativeVolumeFrame::PeakVolume peakVolume;
            peakVolume.bitsRepresentingPeak = (unsigned char) co.GetString("bitsRepresentingPeak")[0];
            peakVolume.peakVolume = TagLib::ByteVector(co.GetString("peakVolume").toCString());
            f->setVolumeAdjustment(volumeAdjustment, channelType);
            f->setPeakVolume(peakVolume, channelType);
        }
        tag->addFrame(f);
    } else if (id.compare("UFID") == 0) {
        auto *f = new TagLib::ID3v2::UniqueFileIdentifierFrame(o.GetString("owner"), TagLib::ByteVector(*idValue, 4));
        f->setIdentifier(o.GetBytes("identifier"));
    } else if (id.compare("USLT") == 0) {
        auto *f = new TagLib::ID3v2::UrlLinkFrame(TagLib::ByteVector(*idValue, 4));
        f->setUrl(o.GetString("url"));
        f->setText(o.GetString("text"));
        tag->addFrame(f);
    } else { // fallback
        auto *f = new TagLib::ID3v2::TextIdentificationFrame(*idValue, TagLib::String::UTF8);
        f->setText(o.GetString("text"));
        tag->addFrame(f);
    }
}
