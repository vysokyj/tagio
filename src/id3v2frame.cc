#include "ID3v2frame.h"
#include <taglib/ID3v2tag.h>
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

using namespace v8;
using namespace std;

static map<TagLib::uint, TagLib::ID3v2::AttachedPictureFrame::Type> APIC = {
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

static map<TagLib::uint, TagLib::ID3v2::RelativeVolumeFrame::ChannelType> RVA2 = {
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


static inline void GetTXXX(TagLibWrapper &o, TagLib::ID3v2::Frame *frame, Configuration *conf) {
    auto *f = dynamic_cast<TagLib::ID3v2::UserTextIdentificationFrame *>(frame);
    string s1 = f->description().to8Bit(true);
    string s2 = "[" + s1 + "] " + s1 + " "; // yes - so stupid
    string s3 = f->toString().to8Bit(true);
    string s4 = s3.substr(s2.length());
    TagLib::String text(s4);
    if (conf->ID3v2UseFrameEncoding()) o.SetEncoding("textEncoding", f->textEncoding());
    o.SetString("description", f->description());
    o.SetString("text", text);
}

static inline void SetTXXX(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, Configuration *conf) {
    auto *f= new TagLib::ID3v2::UserTextIdentificationFrame(TagLib::String::UTF8);
    if (conf->ID3v2UseFrameEncoding()) f->setTextEncoding(o.GetEncoding("textEncoding"));
    else f->setTextEncoding(conf->ID3v2Encoding());
    f->setDescription(o.GetString("description"));
    f->setText(o.GetString("text"));
    tag->addFrame(f);
}

static inline void GetTYYY(TagLibWrapper &o, TagLib::ID3v2::Frame *frame, Configuration *conf) {
    auto *f = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(frame);
    if (conf->ID3v2UseFrameEncoding()) o.SetEncoding("textEncoding", f->textEncoding());
    o.SetString("text", f->toString());
}

static inline void SetTYYY(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id, Configuration *conf) {
    auto *f= new TagLib::ID3v2::TextIdentificationFrame(id, TagLib::String::UTF8);
    if (conf->ID3v2UseFrameEncoding()) f->setTextEncoding(o.GetEncoding("textEncoding"));
    else f->setTextEncoding(conf->ID3v2Encoding());
    f->setText(o.GetString("text"));
    tag->addFrame(f);
}

static inline void GetWXXX(TagLibWrapper &o, TagLib::ID3v2::Frame *frame, Configuration *conf) {
    auto *f = dynamic_cast<TagLib::ID3v2::UserUrlLinkFrame *>(frame);
    if (conf->ID3v2UseFrameEncoding()) o.SetEncoding("textEncoding", f->textEncoding());
    o.SetString("description", f->description());
    o.SetString("url", f->url());
}

static inline void SetWXXX(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, Configuration *conf) {
    auto *f = new TagLib::ID3v2::UserUrlLinkFrame();
    if (conf->ID3v2UseFrameEncoding()) f->setTextEncoding(o.GetEncoding("textEncoding"));
    else f->setTextEncoding(conf->ID3v2Encoding());
    f->setTextEncoding(conf->ID3v2Encoding());
    f->setDescription(o.GetString("description"));
    f->setUrl(o.GetString("url"));
    tag->addFrame(f);
}

static inline void GetWYYY(TagLibWrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::UrlLinkFrame *>(frame);
    o.SetString("url", f->url());
}

static inline void SetWYYY(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id) {
    auto *f = new TagLib::ID3v2::UrlLinkFrame(id);
    f->setUrl(o.GetString("url"));
    tag->addFrame(f);
}

static inline void GetCOMM(TagLibWrapper &o, TagLib::ID3v2::Frame *frame, Configuration *conf) {
    auto *f = dynamic_cast<TagLib::ID3v2::CommentsFrame *>(frame);
    if (conf->ID3v2UseFrameEncoding()) o.SetEncoding("textEncoding", f->textEncoding());
    o.SetString("text", f->toString());
}

static inline void SetCOMM(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, Configuration *conf) {
    auto *f = new TagLib::ID3v2::CommentsFrame();
    if (conf->ID3v2UseFrameEncoding()) f->setTextEncoding(o.GetEncoding("textEncoding"));
    else f->setTextEncoding(conf->ID3v2Encoding());
    f->setTextEncoding(conf->ID3v2Encoding());
    f->setText(o.GetString("text"));
    tag->addFrame(f);
}

static inline void GetAPIC(TagLibWrapper &o, TagLib::ID3v2::Frame *frame, Configuration *conf) {
    auto *f = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame);
    if (conf->ID3v2UseFrameEncoding()) o.SetEncoding("textEncoding", f->textEncoding());
    o.SetString("mimeType", f->mimeType());
    o.SetString("description", f->description());
    o.SetUint32("type", f->type());
    //TODO: o.SetBytes("picture", f->picture(), f->mimeType());
}

static inline void SetAPIC(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, Configuration *conf) {
    uint32_t type = o.GetUint32("type");
    auto *f = new TagLib::ID3v2::AttachedPictureFrame();
    if (conf->ID3v2UseFrameEncoding()) f->setTextEncoding(o.GetEncoding("textEncoding"));
    else f->setTextEncoding(conf->ID3v2Encoding());
    f->setTextEncoding(conf->ID3v2Encoding());
    f->setMimeType(o.GetString("mimeType"));
    if (APIC.count(type)) f->setType(APIC[type]);
    else f->setType(TagLib::ID3v2::AttachedPictureFrame::Other);
    f->setDescription(o.GetString("description"));
    //TODO: f->setPicture(o.GetBytes("picture"));
    tag->addFrame(f);
}

static inline void GetGEOB(TagLibWrapper &o, TagLib::ID3v2::Frame *frame, Configuration *conf) {
    auto *f = dynamic_cast<TagLib::ID3v2::GeneralEncapsulatedObjectFrame *>(frame);
    if (conf->ID3v2UseFrameEncoding()) o.SetEncoding("textEncoding", f->textEncoding());
    o.SetString("mimeType", f->mimeType());
    o.SetString("fileName", f->fileName());
    o.SetString("description", f->description());
    //TODO: o.SetBytes("object", f->object(), f->mimeType());
}

static inline void SetGEOB(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, Configuration *conf) {
    auto *f = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
    if (conf->ID3v2UseFrameEncoding()) f->setTextEncoding(o.GetEncoding("textEncoding"));
    else f->setTextEncoding(conf->ID3v2Encoding());
    f->setTextEncoding(conf->ID3v2Encoding());
    f->setMimeType(o.GetString("mimeType"));
    f->setFileName(o.GetString("fileName"));
    f->setDescription(o.GetString("description"));
    //TODO: f->setObject(o.GetBytes("object"));
    tag->addFrame(f);
}

static inline void GetPOPM(TagLibWrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::PopularimeterFrame *>(frame);
    o.SetString("email", f->email());
    o.SetInt32("rating", f->rating()); // 0 - 255
    o.SetUint32("counter", f->counter());
}

static inline void SetPOPM(TagLibWrapper &o, TagLib::ID3v2::Tag *tag) {
    auto *f = new TagLib::ID3v2::PopularimeterFrame();
    f->setEmail(o.GetString("email"));
    f->setRating(o.GetInt32("rating")); // 0 - 255
    f->setCounter(o.GetUint32("counter"));
    tag->addFrame(f);
}

static inline void GetPRIV(TagLibWrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::PrivateFrame *>(frame);
    o.SetString("owner", f->owner());
}

static inline void SetPRIV(TagLibWrapper &o, TagLib::ID3v2::Tag *tag) {
    auto *f = new TagLib::ID3v2::PrivateFrame();
    f->setOwner(o.GetString("owner"));
    tag->addFrame(f);
}

//static inline void GetRVA2(TagLibWrapper &o, TagLib::ID3v2::Frame *frame, Isolate *isolate, Object *object) {
//    //TODO: Test RVA2 usability
//    auto *f = dynamic_cast<TagLib::ID3v2::RelativeVolumeFrame *>(frame);
//    TagLib::List<TagLib::ID3v2::RelativeVolumeFrame::ChannelType> channels = f->channels();
//    Local<Array> channelArray = Array::New(isolate, channels.size());
//    for (unsigned int i = 0; i < channels.size(); i++) {
//        TagLib::ID3v2::RelativeVolumeFrame::ChannelType channelType = channels[i];
//        Local<Object> channelObject = Object::New(isolate);
//        TagLibWrapper co(isolate, *channelObject);
//        TagLib::ID3v2::RelativeVolumeFrame::PeakVolume peakVolume = f->peakVolume(channelType);
//        co.SetUint32("channelType", channelType); //numeric
//        co.SetNumber("volumeAdjustment", (double) f->volumeAdjustment(channelType));
//        co.SetString("bitsRepresentingPeak", (char) peakVolume.bitsRepresentingPeak);
//        co.SetString("peakVolume", TagLib::String(peakVolume.peakVolume));
//        channelArray->Set(i, channelObject);
//    }
//    object->Set(String::NewFromUtf8(isolate, "channels"), channelArray);
//}
//
//static inline void SetRVA2(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, Isolate *isolate, Object *object) {
//    //TODO: Test RVA2 usability
//    Local<Array> channelArray = Local<Array>::Cast(object->Get(String::NewFromUtf8(isolate, "channels")));
//    auto *f = new TagLib::ID3v2::RelativeVolumeFrame();
//    for (unsigned int i = 0; i < channelArray->Length(); i++) {
//        Local<Object> channelObject = channelArray->Get(i)->ToObject();
//        TagLibWrapper co(isolate, *channelObject);
//        TagLib::ID3v2::RelativeVolumeFrame::ChannelType channelType = RVA2[co.GetInt32("channelType")];
//        float volumeAdjustment = (float) co.GetNumber("volumeAdjustment");
//        TagLib::ID3v2::RelativeVolumeFrame::PeakVolume peakVolume;
//        peakVolume.bitsRepresentingPeak = (unsigned char) co.GetString("bitsRepresentingPeak")[0];
//        peakVolume.peakVolume = TagLib::ByteVector(co.GetString("peakVolume").toCString());
//        f->setVolumeAdjustment(volumeAdjustment, channelType);
//        f->setPeakVolume(peakVolume, channelType);
//    }
//    tag->addFrame(f);
//}

static inline void GetUFID(TagLibWrapper &o, TagLib::ID3v2::Frame *frame) {
    auto *f = dynamic_cast<TagLib::ID3v2::UniqueFileIdentifierFrame *>(frame);
    TagLib::String mimeType("data/bin", TagLib::String::UTF8); //TODO: Mime type
    o.SetString("owner", f->owner());
    //TODO: o.SetBytes("file", f->identifier(), mimeType);
}

static inline void SetUFID(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id) {
    TagLib::String owner = o.GetString("owner");
    auto *f = new TagLib::ID3v2::UniqueFileIdentifierFrame(owner, id);
    //TODO: f->setIdentifier(o.GetBytes("identifier"));
    tag->addFrame(f);
}

static inline void GetUSLT(TagLibWrapper &o, TagLib::ID3v2::Frame *frame, Configuration *conf) {
    auto *f = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frame);
    if (conf->ID3v2UseFrameEncoding()) o.SetEncoding("textEncoding", f->textEncoding());
    o.SetString("description", f->description());
    o.SetString("language", TagLib::String(f->language()));
    o.SetString("text", f->toString());
}

static inline void SetUSLT(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id, Configuration *conf) {
    auto *f = new TagLib::ID3v2::UnsynchronizedLyricsFrame(id);
    TagLib::String languageString = o.GetString("language");
    if (conf->ID3v2UseFrameEncoding()) f->setTextEncoding(o.GetEncoding("textEncoding"));
    else f->setTextEncoding(conf->ID3v2Encoding());
    f->setTextEncoding(conf->ID3v2Encoding());
    f->setDescription(o.GetString("description"));
    f->setLanguage(TagLib::ByteVector(languageString.toCString(false), languageString.size()));
    f->setText(o.GetString("text"));
    tag->addFrame(f);
}

static inline void GetNONE(TagLibWrapper &o, TagLib::ID3v2::Frame *frame) {
    //auto *f = dynamic_cast<TagLib::ID3v2::UnknownFrame *>(frame);
    //TODO: o.SetBytes("data", f->data(), "application/octet-stream");
}

static inline void SetNONE(TagLibWrapper &o, TagLib::ID3v2::Tag *tag, const TagLib::ByteVector &id) {
    //auto *f = new TagLib::ID3v2::UnknownFrame(o.GetBytes("data"));
    //tag->addFrame(f);
}

void ExportID3v2Frame(TagLib::ID3v2::Frame *frame, v8::Object *object, Configuration *conf) {
    TagLibWrapper o(object);
    TagLib::ByteVector idBytes = frame->frameID();
    string id = string(idBytes.data(), idBytes.size());
    o.SetString("id", TagLib::String(id));
    if      (id.compare("TXXX") == 0) GetTXXX(o, frame, conf);
    else if (id.at(0) == 'T')         GetTYYY(o, frame, conf);
    else if (id.compare("WXXX") == 0) GetWXXX(o, frame, conf);
    else if (id.at(0) == 'W')         GetWYYY(o, frame);
    else if (id.compare("COMM") == 0) GetCOMM(o, frame, conf);
    else if (id.compare("APIC") == 0) GetAPIC(o, frame, conf);
    else if (id.compare("GEOB") == 0) GetGEOB(o, frame, conf);
    else if (id.compare("POPM") == 0) GetPOPM(o, frame);
    else if (id.compare("PRIV") == 0) GetPRIV(o, frame);
    //TODO: Reimplement    
    //else if (id.compare("RVA2") == 0) GetRVA2(o, frame, isolate, *object);
    else if (id.compare("UFID") == 0) GetUFID(o, frame);
    else if (id.compare("USLT") == 0) GetUSLT(o, frame, conf);
    else                              GetNONE(o, frame);
}

void ImportID3v2Frame(Object *object, TagLib::ID3v2::Tag *tag, Configuration *conf) {
    TagLibWrapper o(object);
    const TagLib::String idString = o.GetString("id");
    const TagLib::ByteVector idVector(idString.toCString(), idString.length());
    string id = idString.to8Bit(true);
    if      (id.compare("TXXX") == 0) SetTXXX(o, tag, conf);
    else if (id.at(0) == 'T')         SetTYYY(o, tag, idVector, conf);
    else if (id.compare("WXXX") == 0) SetWXXX(o, tag, conf);
    else if (id.at(0) == 'W')         SetWYYY(o, tag, idVector);
    else if (id.compare("COMM") == 0) SetCOMM(o, tag, conf);
    else if (id.compare("APIC") == 0) SetAPIC(o, tag, conf);
    else if (id.compare("GEOB") == 0) SetGEOB(o, tag, conf);
    else if (id.compare("POPM") == 0) SetPOPM(o, tag);
    else if (id.compare("PRIV") == 0) SetPRIV(o, tag);
    //else if (id.compare("RVA2") == 0) SetRVA2(o, tag, isolate, object);
    else if (id.compare("UFID") == 0) SetUFID(o, tag, idVector);
    else if (id.compare("USLT") == 0) SetUSLT(o, tag, idVector, conf);
    else                              SetNONE(o, tag, idVector);
}

