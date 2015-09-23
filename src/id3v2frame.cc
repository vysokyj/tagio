#include "id3v2frame.h"

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

//TODO: chapterframe.h missing?

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
    if      (id.compare("TXXX") == 0) GetTXXX(o, frame);
    else if (id.at(0) == 'T')         GetTYYY(o, frame);
    else if (id.compare("WXXX") == 0) GetWXXX(o, frame);
    else if (id.at(0) == 'W')         GetWYYY(o, frame);
    else if (id.compare("COMM") == 0) GetCOMM(o, frame);
    else if (id.compare("APIC") == 0) GetAPIC(o, frame);
    else if (id.compare("GEOB") == 0) GetGEOB(o, frame);
    else if (id.compare("POPM") == 0) GetPOPM(o, frame);
    else if (id.compare("PRIV") == 0) GetPRIV(o, frame);
    else if (id.compare("RVA2") == 0) GetRVA2(o, frame, isolate, *object);
    else if (id.compare("UFID") == 0) GetUFID(o, frame);
    else if (id.compare("USLT") == 0) GetUSLT(o, frame);
    else                              GetNONE(o, frame);
    return handleScope.Escape(object);
}

void ID3v2Frame::Set(Isolate *isolate, Object *object, TagLib::ID3v2::Tag *tag ) {
    Wrapper o(isolate, object);
    const TagLib::String idString = o.GetString("id");
    const TagLib::ByteVector idVector(idString.toCString(), idString.length());
    string id = idString.to8Bit(true);
    if      (id.compare("TXXX") == 0) SetTXXX(o, tag);
    else if (id.at(0) == 'T')         SetTYYY(o, tag, idVector);
    else if (id.compare("WXXX") == 0) SetWXXX(o, tag);
    else if (id.at(0) == 'W')         SetWYYY(o, tag, idVector);
    else if (id.compare("COMM") == 0) SetCOMM(o, tag);
    else if (id.compare("APIC") == 0) SetAPIC(o, tag);
    else if (id.compare("GEOB") == 0) SetGEOB(o, tag);
    else if (id.compare("POPM") == 0) SetPOPM(o, tag);
    else if (id.compare("PRIV") == 0) SetPRIV(o, tag);
    else if (id.compare("RVA2") == 0) SetRVA2(o, tag, isolate, object);
    else if (id.compare("UFID") == 0) SetUFID(o, tag, idVector);
    else if (id.compare("USLT") == 0) SetUSLT(o, tag, idVector);
    else                              SetNONE(o, tag, idVector);
}

inline void ID3v2Frame::GetTXXX(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::UserTextIdentificationFrame *>(frame);
    o.SetString("description", f->description());
    o.SetString("text", f->toString());
}

inline void ID3v2Frame::SetTXXX(Wrapper &o, TagLib::ID3v2::Tag *tag) {
    auto *f= new TagLib::ID3v2::UserTextIdentificationFrame(TagLib::String::UTF8);
    f->setDescription(o.GetString("description"));
    f->setText(o.GetString("text"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetTYYY(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(frame);
    o.SetString("text", f->toString());
}

inline void ID3v2Frame::SetTYYY(Wrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id) {
    auto *f= new TagLib::ID3v2::TextIdentificationFrame(id, TagLib::String::UTF8);
    f->setText(o.GetString("text"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetWXXX(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::UserUrlLinkFrame *>(frame);
    o.SetString("description", f->description());
    o.SetString("url", f->url());
}

inline void ID3v2Frame::SetWXXX(Wrapper &o, TagLib::ID3v2::Tag *tag) {
    auto *f = new TagLib::ID3v2::UserUrlLinkFrame();
    f->setDescription(o.GetString("description"));
    f->setUrl(o.GetString("url"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetWYYY(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::UrlLinkFrame *>(frame);
    o.SetString("url", f->url());
}

inline void ID3v2Frame::SetWYYY(Wrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id) {
    auto *f = new TagLib::ID3v2::UrlLinkFrame(id);
    f->setUrl(o.GetString("url"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetCOMM(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::CommentsFrame *>(frame);
    o.SetString("text", f->toString());
}

inline void ID3v2Frame::SetCOMM(Wrapper &o, TagLib::ID3v2::Tag *tag) {
    auto *f = new TagLib::ID3v2::CommentsFrame();
    f->setText(o.GetString("text"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetAPIC(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame);
    o.SetString("mimeType", f->mimeType());
    o.SetString("description", f->description());
    o.SetUint32("type", f->type());
    o.SetBytes("picture", f->picture(), f->mimeType());
}

inline void ID3v2Frame::SetAPIC(Wrapper &o, TagLib::ID3v2::Tag *tag) {
    uint32_t type = o.GetUint32("type");
    auto *f = new TagLib::ID3v2::AttachedPictureFrame();
    f->setMimeType(o.GetString("mimeType"));
    if (APIC.count(type)) f->setType(APIC[type]);
    else f->setType(TagLib::ID3v2::AttachedPictureFrame::Other);
    f->setDescription(o.GetString("description"));
    f->setPicture(o.GetBytes("picture"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetGEOB(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::GeneralEncapsulatedObjectFrame *>(frame);
    o.SetString("mimeType", f->mimeType());
    o.SetString("fileName", f->fileName());
    o.SetBytes("object", f->object(), f->mimeType());
}

inline void ID3v2Frame::SetGEOB(Wrapper &o, TagLib::ID3v2::Tag *tag) {
    auto *f = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
    f->setMimeType(o.GetString("mimeType"));
    f->setFileName(o.GetString("fileName"));
    f->setObject(o.GetBytes("object"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetPOPM(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::PopularimeterFrame *>(frame);
    o.SetString("email", f->email());
    o.SetInt32("rating", f->rating()); // 0 - 255
    o.SetUint32("counter", f->counter());
}

inline void ID3v2Frame::SetPOPM(Wrapper &o, TagLib::ID3v2::Tag *tag) {
    auto *f = new TagLib::ID3v2::PopularimeterFrame();
    f->setEmail(o.GetString("email"));
    f->setRating(o.GetInt32("rating")); // 0 - 255
    f->setCounter(o.GetUint32("counter"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetPRIV(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::PrivateFrame *>(frame);
    o.SetString("owner", f->owner());
}

inline void ID3v2Frame::SetPRIV(Wrapper &o, TagLib::ID3v2::Tag *tag) {
    auto *f = new TagLib::ID3v2::PrivateFrame();
    f->setOwner(o.GetString("owner"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetRVA2(Wrapper &o, TagLib::ID3v2::Frame *frame, Isolate *isolate, Object *object) {
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
}

inline void ID3v2Frame::SetRVA2(Wrapper &o, TagLib::ID3v2::Tag *tag, Isolate *isolate, Object *object) {
    //TODO: Test RVA2 usability
    Local<Array> channelArray = Local<Array>::Cast(object->Get(String::NewFromUtf8(isolate, "channels")));
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
}

inline void ID3v2Frame::GetUFID(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::UniqueFileIdentifierFrame *>(frame);
    TagLib::String mimeType("data/bin", TagLib::String::UTF8); //TODO: Mime type
    o.SetString("owner", f->owner());
    o.SetBytes("file", f->identifier(), mimeType);
}

inline void ID3v2Frame::SetUFID(Wrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id) {
    TagLib::String owner = o.GetString("owner");
    auto *f = new TagLib::ID3v2::UniqueFileIdentifierFrame(owner, id);
    f->setIdentifier(o.GetBytes("identifier"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetUSLT(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frame);
    o.SetString("description", f->description());
    o.SetString("text", f->toString());
}

inline void ID3v2Frame::SetUSLT(Wrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id) {
    auto *f = new TagLib::ID3v2::UrlLinkFrame(id);
    f->setUrl(o.GetString("url"));
    f->setText(o.GetString("text"));
    tag->addFrame(f);
}

inline void ID3v2Frame::GetNONE(Wrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::UnknownFrame *>(frame);
    o.SetBytes("data", f->data(), "application/octet-stream");
}

inline void ID3v2Frame::SetNONE(Wrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id) {
    auto *f = new TagLib::ID3v2::UnknownFrame(o.GetBytes("data"));
    tag->addFrame(f);
}



