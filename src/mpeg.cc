#include "mpeg.h"

#include "audioproperties.h"

using namespace TagIO;
using namespace v8;
using namespace node;
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

Persistent<Function> MPEG::constructor;

MPEG::MPEG(const char *path) : Base(path) {
    file = new TagLib::MPEG::File(path);
}

MPEG::~MPEG() {
    delete file;
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
    NODE_SET_PROTOTYPE_METHOD(tpl, "getAudioProperties", GetAudioProperties);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getTag", GetTag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTag", SetTag);
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
    HandleScope scope(isolate);

    if (args.Length() < 2) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }

    if (!args[0]->IsString() || !args[1]->IsObject()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
    }

    if (args.IsConstructCall()) {
        // Invoked as constructor
        String::Utf8Value path(args[0]->ToString());
        auto *ref = new MPEG(*path);
        ref->Wrap(args.This());
        Local<Object> object = args[1]->ToObject();
        GetBaseConfiguration(isolate, *object, ref);

        Local<String> saveID3v1TagKey = String::NewFromUtf8(isolate, "saveID3v1Tag");
        Local<String> saveID3v2TagKey = String::NewFromUtf8(isolate, "saveID3v2Tag");
        Local<String> saveApeTagKey = String::NewFromUtf8(isolate, "saveApeTag");

        if (object->Has(saveID3v1TagKey)) {
            Local<Boolean> val(object->Get(saveID3v1TagKey)->ToBoolean());
            ref->SetID3v1TagEnabled(val->BooleanValue());
        }
        if (object->Has(saveID3v2TagKey)) {
            Local<Boolean> val(object->Get(saveID3v2TagKey)->ToBoolean());
            ref->SetID3v2TagEnabled(val->BooleanValue());
        }
        if (object->Has(saveApeTagKey)) {
            Local<Boolean> val(object->Get(saveApeTagKey)->ToBoolean());
            ref->SetApeTagEnabled(val->BooleanValue());
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
    MPEG *mpeg = ObjectWrap::Unwrap<MPEG>(args.Holder());
    if (mpeg->saveID3v1Tag)  tags = tags | ID3v1;
    if (mpeg->saveID3v2Tag)  tags = tags | ID3v2;
    if (mpeg->saveApeTag)    tags = tags | APE;
    if (mpeg->saveID3v1Tag && mpeg->saveID3v2Tag && mpeg->saveApeTag) tags = AllTags;
    bool result = mpeg->file->save(tags);
    args.GetReturnValue().Set(Boolean::New(isolate, result));
}

void MPEG::GetPath(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<MPEG>(args.Holder());
    string path = ref->GetFilePath();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, path.c_str()));
}

void MPEG::GetAudioProperties(const FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<MPEG>(args.Holder());
    Local<Object> object = AudioProperties::New(isolate, ref->file->audioProperties());
    args.GetReturnValue().Set(object);
}

void MPEG::GetTag(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<MPEG>(args.Holder());
    Local<Object> object = Object::New(isolate);
    SetObjectByTag(isolate, *object, ref->file->tag());
    args.GetReturnValue().Set(object);
}

void MPEG::SetTag(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<MPEG>(args.Holder());
    Local<Object> object = Local<Object>::Cast(args[0]);
    SetTagByObject(isolate, *object, ref->file->tag());
}

//----------------------------------------------------------------------------------------------------------------------
// MPEG API


void MPEG::HasAPETag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = ObjectWrap::Unwrap<MPEG>(args.Holder());
    args.GetReturnValue().Set(Boolean::New(isolate, mpeg->file->hasAPETag()));
}

void MPEG::HasID3v1Tag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = ObjectWrap::Unwrap<MPEG>(args.Holder());
    args.GetReturnValue().Set(Boolean::New(isolate, mpeg->file->hasID3v1Tag()));
}

void MPEG::HasID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = ObjectWrap::Unwrap<MPEG>(args.Holder());
    args.GetReturnValue().Set(Boolean::New(isolate, mpeg->file->hasID3v2Tag()));
}

void MPEG::GetID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = ObjectWrap::Unwrap<MPEG>(args.Holder());
    if (!mpeg->file->hasID3v2Tag()) return;
    TagLib::ID3v2::Tag *tag = mpeg->file->ID3v2Tag(false);
    TagLib::ID3v2::FrameList frameList = tag->frameList();
    Local<Array> frameArray = Array::New(isolate, frameList.size());

    for (unsigned int i = 0; i < frameList.size(); i++) {
        TagLib::ID3v2::Frame *frame = frameList[i];
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
            o.SetString("file", mpeg->ExportFile(f->picture(), f->mimeType()));
        } else if (id.compare("GEOB") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::GeneralEncapsulatedObjectFrame *>(frame);
            o.SetString("mimeType", f->mimeType());
            o.SetString("fileName", f->fileName());
            o.SetString("file", mpeg->ExportFile(f->object(), f->mimeType()));
        } else if (id.compare("PRIV") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::PrivateFrame *>(frame);
            o.SetString("owner", f->owner());
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
            o.SetString("owner", f->owner());
            o.SetString("file", mpeg->ExportFile(f->identifier(), mimeType));
        } else if (id.compare("USLT") == 0) {
            auto *f = dynamic_cast<TagLib::ID3v2::UnsynchronizedLyricsFrame *>(frame);
            o.SetString("description", f->description());
            o.SetString("text", f->toString());
        } else {
            // fallback
            o.SetString("text", frame->toString());
        }
        frameArray->Set(i, object);
    }
    args.GetReturnValue().Set(frameArray);
}

void MPEG::SetID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    if (args.Length() != 1 || !args[0]->IsArray()) return;
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = ObjectWrap::Unwrap<MPEG>(args.Holder());
    TagLib::ID3v2::Tag *tag = mpeg->file->ID3v2Tag(true);
    Local<Array> frames = Local<Array>::Cast(args[0]);
    mpeg->ClearID3v2Frames();
    for (unsigned int i = 0; i < frames->Length(); i++) {
        Local<Object> object = frames->Get(i)->ToObject();
        Wrapper o(isolate, *object);
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
            frame->setPicture(mpeg->ImportFile(o.GetString("file")));
            tag->addFrame(frame);
        } else if (id.compare("GEOB") == 0) {
            auto *frame = new TagLib::ID3v2::GeneralEncapsulatedObjectFrame();
            frame->setMimeType(o.GetString("mimeType"));
            frame->setFileName(o.GetString("fileName"));
            frame->setObject(mpeg->ImportFile(o.GetString("file")));
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
            frame->setIdentifier(mpeg->ImportFile(o.GetString("identifier")));
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
}

void MPEG::ClearID3v2Frames() {
    if (!file->hasID3v2Tag()) return;
    typedef list<TagLib::ByteVector> ByteVectorList;
    TagLib::ID3v2::Tag *tag = file->ID3v2Tag();
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