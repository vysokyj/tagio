#include "mpeg.h"

using namespace TagIO;
using namespace v8;
using namespace std;
using namespace TagLib::MPEG;
using namespace TagLib::ID3v2;

Persistent<Function> MPEG::constructor;

MPEG::MPEG(string path) : Base(path) {
    file = new File(this->GetFilePath().c_str());
    apicMap[0x00] = TagLib::ID3v2::AttachedPictureFrame::Other;
    apicMap[0x01] = TagLib::ID3v2::AttachedPictureFrame::FileIcon;
    apicMap[0x02] = TagLib::ID3v2::AttachedPictureFrame::OtherFileIcon;
    apicMap[0x03] = TagLib::ID3v2::AttachedPictureFrame::FrontCover;
    apicMap[0x04] = TagLib::ID3v2::AttachedPictureFrame::BackCover;
    apicMap[0x05] = TagLib::ID3v2::AttachedPictureFrame::LeafletPage;
    apicMap[0x06] = TagLib::ID3v2::AttachedPictureFrame::Media;
    apicMap[0x07] = TagLib::ID3v2::AttachedPictureFrame::LeadArtist;
    apicMap[0x08] = TagLib::ID3v2::AttachedPictureFrame::Artist;
    apicMap[0x09] = TagLib::ID3v2::AttachedPictureFrame::Conductor;
    apicMap[0x0A] = TagLib::ID3v2::AttachedPictureFrame::Band;
    apicMap[0x0B] = TagLib::ID3v2::AttachedPictureFrame::Composer;
    apicMap[0x0C] = TagLib::ID3v2::AttachedPictureFrame::Lyricist;
    apicMap[0x0D] = TagLib::ID3v2::AttachedPictureFrame::RecordingLocation;
    apicMap[0x0E] = TagLib::ID3v2::AttachedPictureFrame::DuringRecording;
    apicMap[0x0F] = TagLib::ID3v2::AttachedPictureFrame::DuringPerformance;
    apicMap[0x10] = TagLib::ID3v2::AttachedPictureFrame::MovieScreenCapture;
    apicMap[0x11] = TagLib::ID3v2::AttachedPictureFrame::ColouredFish;
    apicMap[0x12] = TagLib::ID3v2::AttachedPictureFrame::Illustration;
    apicMap[0x13] = TagLib::ID3v2::AttachedPictureFrame::BandLogo;
    apicMap[0x14] = TagLib::ID3v2::AttachedPictureFrame::PublisherLogo;
}

MPEG::~MPEG() {
    delete file;
    apicMap.empty();
}

void MPEG::Init(Handle<Object> exports) {
    Isolate *isolate = Isolate::GetCurrent();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "MPEG"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    // Generic API
    NODE_SET_PROTOTYPE_METHOD(tpl, "save", Save);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getPath", GetPath);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getTitle", GetTitle);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTitle", SetTitle);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getAlbum", GetAlbum);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setAlbum", SetAlbum);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getArtist", GetArtist);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setArtist", SetArtist);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getTrack", GetTrack);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTrack", SetTrack);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getYear", GetYear);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setYear", SetYear);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getGenre", GetGenre);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setGenre", SetGenre);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getComment", GetComment);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setComment", SetComment);
    // MPEG API
    NODE_SET_PROTOTYPE_METHOD(tpl, "hasAPETag", HasAPETag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "hasID3v1Tag", HasID3v1Tag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "hasID3v2Tag", HasID3v2Tag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getID3v2Tag", GetID3v2Tag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setID3v2Tag", SetID3v2Tag);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "MPEG"), tpl->GetFunction());
}

void MPEG::New(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    //HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor
        if (args.Length() >= 1) {
            String::Utf8Value path(args[0]->ToString());
            MPEG* mpeg = new MPEG(string(*path));
            mpeg->Wrap(args.This());

            if (args.Length() >= 2) {

                Local<String> attachmentsDirKey = String::NewFromUtf8(isolate, "attachmentsDir");
                Local<String> attachmentsCtxKey = String::NewFromUtf8(isolate, "attachmentsCtx");
                Local<String> saveID3v1TagKey = String::NewFromUtf8(isolate, "saveID3v1Tag");
                Local<String> saveID3v2TagKey = String::NewFromUtf8(isolate, "saveID3v2Tag");
                Local<String> saveApeTagKey = String::NewFromUtf8(isolate, "saveApeTag");

                Local<Object> config = args[1]->ToObject();
                if (config->Has(attachmentsDirKey)) {
                    String::Utf8Value val(config->Get(attachmentsDirKey)->ToString());
                    mpeg->SetAttachmentsDir(string(*val));
                }
                if (config->Has(attachmentsCtxKey)) {
                    String::Utf8Value val(config->Get(attachmentsCtxKey)->ToString());
                    mpeg->SetAttachmentsCtx(string(*val));
                }
                if (config->Has(saveID3v1TagKey)) {
                    Local<Boolean> val(config->Get(saveID3v1TagKey)->ToBoolean());
                    mpeg->SetID3v1TagEnabled(val->BooleanValue());
                }
                if (config->Has(saveID3v2TagKey)) {
                    Local<Boolean> val(config->Get(saveID3v2TagKey)->ToBoolean());
                    mpeg->SetID3v2TagEnabled(val->BooleanValue());
                }
                if (config->Has(saveApeTagKey)) {
                    Local<Boolean> val(config->Get(saveApeTagKey)->ToBoolean());
                    mpeg->SetApeTagEnabled(val->BooleanValue());
                }
            }
        }

        args.GetReturnValue().Set(args.This());
    } else {
        // Invoked as plain function, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

//----------------------------------------------------------------------------------------------------------------------
// Generic API

void MPEG::Save(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    int NoTags  = 0x0000;
    int ID3v1   = 0x0001;
    int ID3v2   = 0x0002;
    int APE     = 0x0004;
    int AllTags = 0xffff;
    int tags = NoTags;
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    if (mpeg->saveID3v1Tag)  tags = tags | ID3v1;
    if (mpeg->saveID3v2Tag)  tags = tags | ID3v2;
    if (mpeg->saveApeTag)    tags = tags | APE;
    if (mpeg->saveID3v1Tag && mpeg->saveID3v2Tag && mpeg->saveApeTag) tags = AllTags;
    bool result = mpeg->file->save(tags);
    args.GetReturnValue().Set(Boolean::New(isolate, result));
}

void MPEG::GetPath(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    string path = mpeg->GetFilePath();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, path.c_str()));
}

void MPEG::GetTitle(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    const char *value = mpeg->file->tag()->title().toCString(true);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, value));
}

void MPEG::SetTitle(const FunctionCallbackInfo<Value>& args) {
    if (args[0]->IsUndefined() || !args[0]->IsString()) return;
    String::Utf8Value value(args[0]->ToString());
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    mpeg->file->tag()->setTitle(std::string(*value));
}

void MPEG::GetAlbum(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    const char *value = mpeg->file->tag()->album().toCString(true);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, value));
}

void MPEG::SetAlbum(const FunctionCallbackInfo<Value>& args) {
    if (args[0]->IsUndefined() || !args[0]->IsString()) return;
    String::Utf8Value value(args[0]->ToString());
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    mpeg->file->tag()->setAlbum(std::string(*value));
}

void MPEG::GetArtist(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    const char *value = mpeg->file->tag()->artist().toCString(true);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, value));
}

void MPEG::SetArtist(const FunctionCallbackInfo<Value>& args) {
    if (args[0]->IsUndefined() || !args[0]->IsString()) return;
    String::Utf8Value value(args[0]->ToString());
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    mpeg->file->tag()->setArtist(std::string(*value));
}

void MPEG::GetTrack(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    uint value = mpeg->file->tag()->track();
    args.GetReturnValue().Set(Integer::NewFromUnsigned(isolate, value));
}

void MPEG::SetTrack(const FunctionCallbackInfo<Value>& args) {
    if (args[0]->IsUndefined() || !args[0]->IsInt32()) return;
    uint value = (uint) args[0]->Int32Value();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    mpeg->file->tag()->setTrack(value);
}

void MPEG::GetYear(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    uint value = mpeg->file->tag()->year();
    args.GetReturnValue().Set(Integer::NewFromUnsigned(isolate, value));
}

void MPEG::SetYear(const FunctionCallbackInfo<Value>& args) {
    if (args[0]->IsUndefined() || !args[0]->IsInt32()) return;
    uint value = (uint) args[0]->Int32Value();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    mpeg->file->tag()->setYear(value);
}

void MPEG::GetGenre(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    const char *value = mpeg->file->tag()->genre().toCString(true);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, value));
}

void MPEG::SetGenre(const FunctionCallbackInfo<Value>& args) {
    if (args[0]->IsUndefined() || !args[0]->IsString()) return;
    String::Utf8Value value(args[0]->ToString());
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    mpeg->file->tag()->setGenre(std::string(*value));
}

void MPEG::GetComment(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    const char *value = mpeg->file->tag()->comment().toCString(true);
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, value));
}

void MPEG::SetComment(const FunctionCallbackInfo<Value>& args) {
    if (args[0]->IsUndefined() || !args[0]->IsString()) return;
    String::Utf8Value value(args[0]->ToString());
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    mpeg->file->tag()->setComment(std::string(*value));
}

//----------------------------------------------------------------------------------------------------------------------
// MPEG API


void MPEG::HasAPETag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    args.GetReturnValue().Set(Boolean::New(isolate, mpeg->file->hasAPETag()));
}

void MPEG::HasID3v1Tag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    args.GetReturnValue().Set(Boolean::New(isolate, mpeg->file->hasID3v1Tag()));
}

void MPEG::HasID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    args.GetReturnValue().Set(Boolean::New(isolate, mpeg->file->hasID3v2Tag()));
}

void MPEG::GetID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    Tag *tag = mpeg->file->ID3v2Tag(false);
    FrameList frameList = tag->frameList();

    Local<Array> frameArray = Array::New(isolate, frameList.size());
    for (unsigned int i = 0; i < frameList.size(); i++) {
        Frame *frame = frameList[i];
        Local<Object> object = Object::New(isolate);
        TagLib::ByteVector idBytes = frame->frameID();
        string id = string(idBytes.data(), idBytes.size());
        SetStringProperty(isolate, *object, "id", TagLib::String(id));
        if (id.at(0) == 'T') {
            auto *f = dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(frame);
            SetStringProperty(isolate, *object, "text", f->toString());
        } else if (id.at(0) == 'W') {
            auto *f = dynamic_cast<TagLib::ID3v2::UrlLinkFrame *>(frame);
            SetStringProperty(isolate, *object, "url", f->url());
            SetStringProperty(isolate, *object, "text", f->toString());
        } else if (id.compare("COMM") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::CommentsFrame *>(frame);
            SetStringProperty(isolate, *object, "text", f->toString());
        } else if (id.compare("APIC") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame);
            SetStringProperty(isolate, *object, "mimeType", f->mimeType());
            SetStringProperty(isolate, *object, "description", f->description());
            object->Set(String::NewFromUtf8(isolate, "type"), Integer::New(isolate, f->type()));
            SetStringProperty(isolate, *object, "text", f->toString());
            SetStringProperty(isolate, *object, "file", mpeg->ExportFile(f->picture(), f->mimeType()));
        } else if (id.compare("GEOB") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::GeneralEncapsulatedObjectFrame *>(frame);
            SetStringProperty(isolate, *object, "mimeType", f->mimeType());
            SetStringProperty(isolate, *object, "fileName", f->fileName());
            SetStringProperty(isolate, *object, "file", mpeg->ExportFile(f->object(), f->mimeType()));
        } else if (id.compare("PRIV") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::PrivateFrame *>(frame);
            SetStringProperty(isolate, *object, "owner", f->owner());
        } else if (id.compare("RVA2") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::RelativeVolumeFrame *>(frame);
            TagLib::List<TagLib::ID3v2::RelativeVolumeFrame::ChannelType> channels = f->channels();
            Local<Array> channelArray = Array::New(isolate, channels.size());
            for (unsigned int j = 0; j < channels.size(); j++)
                channelArray->Set(i, Integer::New(isolate, channels[i]));
            object->Set(String::NewFromUtf8(isolate, "channels"), channelArray);
        } else if (id.compare("UFID") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::UniqueFileIdentifierFrame *>(frame);
            TagLib::String mimeType("data/bin", TagLib::String::UTF8); //TODO: Mime type
            SetStringProperty(isolate, *object, "owner", f->owner());
            SetStringProperty(isolate, *object, "file", mpeg->ExportFile(f->identifier(), mimeType));
        } else if (id.compare("USLT") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frame);
            SetStringProperty(isolate, *object, "description", f->description());
            SetStringProperty(isolate, *object, "text", f->toString());
        } else {
            // fallback
            SetStringProperty(isolate, *object, "text", frame->toString());
        }
        frameArray->Set(i, object);
    }
    args.GetReturnValue().Set(frameArray);
}

void MPEG::SetID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    if (args.Length() != 1 || !args[0]->IsArray()) return;
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = node::ObjectWrap::Unwrap<MPEG>(args.Holder());
    TagLib::ID3v2::Tag *tag = mpeg->file->ID3v2Tag(true);
    Local<Array> frames = Local<Array>::Cast(args[0]);
    mpeg->ClearID3v2Frames();
    for (unsigned int i = 0; i < frames->Length(); i++) {
        Local<Object> object = frames->Get(i)->ToObject();
        String::Utf8Value idValue(object->Get(String::NewFromUtf8(isolate, "id"))->ToString());
        string id(*idValue);
        if (id.at(0) == 'T') {
            auto *frame= new TextIdentificationFrame(*idValue, TagLib::String::UTF8);
            frame->setText(GetStringProperty(isolate, *object, "text"));
            tag->addFrame(frame);
        } else if (id.at(0) == 'W') {
            auto *frame = new UrlLinkFrame(TagLib::ByteVector(*idValue, 4));
            frame->setUrl(GetStringProperty(isolate, *object, "url"));
            tag->addFrame(frame);
        } else if (id.compare("COMM") == 0) {
            auto *frame = new CommentsFrame();
            frame->setText(GetStringProperty(isolate, *object, "text"));
            tag->addFrame(frame);
        } else if (id.compare("APIC") == 0) {
            uint32_t type(object->Get(String::NewFromUtf8(isolate, "type"))->Uint32Value());
            auto *frame = new AttachedPictureFrame();
            frame->setMimeType(GetStringProperty(isolate, *object, "mimeType"));
            if (mpeg->apicMap.count(type)) frame->setType(mpeg->apicMap[type]);
            else frame->setType(TagLib::ID3v2::AttachedPictureFrame::Other);
            frame->setDescription(GetStringProperty(isolate, *object, "description"));
            frame->setPicture(mpeg->ImportFile(GetStringProperty(isolate, *object, "file")));
            tag->addFrame(frame);
        } else if (id.compare("GEOB") == 0) {
            auto *frame = new GeneralEncapsulatedObjectFrame();
            frame->setMimeType(GetStringProperty(isolate, *object, "mimeType"));
            frame->setFileName(GetStringProperty(isolate, *object, "fileName"));
            frame->setObject(mpeg->ImportFile(GetStringProperty(isolate, *object, "file")));
            tag->addFrame(frame);
        } else if (id.compare("PRIV") == 0) {
            auto *frame = new PrivateFrame();
            frame->setOwner(GetStringProperty(isolate, *object, "owner"));
        } else if (id.compare("RVA2") == 0) {
//            Local<Array> channelArray = Local<Array>::Cast(object->Get(String::NewFromUtf8(isolate, "text")));
//            auto *frame = new RelativeVolumeFrame();
//            for (unsigned int j = 0; j < channelArray->Length(); j++) {
//                Local<Object> channelObject = channelArray->Get(j)->ToObject();
//            }
//            tag->addFrame(frame);
            cout << "RVA2 not yet supported!" << endl;
        } else if (id.compare("UFID") == 0) {
            auto *frame = new UniqueFileIdentifierFrame(GetStringProperty(isolate, *object, "owner"),
                    TagLib::ByteVector(*idValue, 4));
            frame->setIdentifier(mpeg->ImportFile(GetStringProperty(isolate, *object, "identifier")));
        } else if (id.compare("USLT") == 0) {
            auto *frame = new UrlLinkFrame(TagLib::ByteVector(*idValue, 4));
            frame->setUrl(GetStringProperty(isolate, *object, "url"));
            frame->setText(GetStringProperty(isolate, *object, "text"));
            tag->addFrame(frame);
        } else { // fallback
            auto *frame = new TextIdentificationFrame(*idValue, TagLib::String::UTF8);
            frame->setText(GetStringProperty(isolate, *object, "text"));
            tag->addFrame(frame);
        }
    }
}

void MPEG::ClearID3v2Frames() {
    if (!file->hasID3v2Tag()) return;
    typedef list<TagLib::ByteVector> ByteVectorList;
    TagLib::ID3v2::Tag *tag = file->ID3v2Tag();
    FrameList frameList = tag->frameList();
    ByteVectorList list;
    for (uint32_t i = 0; i < frameList.size(); i++) {
        Frame *frame = frameList[i];
        TagLib::ByteVector vector = frame->frameID();
        string id = string(vector.data(), vector.size());
        if (id.at(0) == 'T' ||
            id.at(0) == 'W' ||
            id.compare("COMM") == 0 ||
            id.compare("APIC") == 0 ||
            id.compare("GEOB") == 0 ||
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