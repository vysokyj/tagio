#include "mpeg.h"
#include "configuration.h"
#include "audioproperties.h"
#include "tag.h"
#include "id3v2tag.h"

using namespace TagIO;
using namespace v8;
using namespace node;
using namespace std;

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
        Configuration::Set(isolate, *object);
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
    Configuration &cfg = Configuration::Get();
    Isolate *isolate = Isolate::GetCurrent();
    int NoTags  = 0x0000;
    int ID3v1   = 0x0001;
    int ID3v2   = 0x0002;
    int APE     = 0x0004;
    int AllTags = 0xffff;
    int tags = NoTags;
    MPEG *mpeg = ObjectWrap::Unwrap<MPEG>(args.Holder());
    if (cfg.GetSaveID3v1Tag())  tags = tags | ID3v1;
    if (cfg.GetSaveID3v2Tag())  tags = tags | ID3v2;
    if (cfg.GetSaveApeTag())    tags = tags | APE;
    if (cfg.GetSaveID3v1Tag() && cfg.GetSaveID3v2Tag() && cfg.GetSaveApeTag()) tags = AllTags;
    bool result = mpeg->file->save(tags);
    args.GetReturnValue().Set(Boolean::New(isolate, result));
}

void MPEG::GetPath(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<MPEG>(args.Holder());
    string path = ref->GetFilePath();
    if (Configuration::Get().GetBinaryDataMethod() == Configuration::BinaryDataMethod::ABSOLUTE_URL)
        path = "file://" + path;
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
    Local<Object> object = Tag::New(isolate, ref->file->tag());
    args.GetReturnValue().Set(object);
}

void MPEG::SetTag(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<MPEG>(args.Holder());
    Local<Object> object = Local<Object>::Cast(args[0]);
    Tag::Set(isolate, *object, ref->file->tag());
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
    Local<Array> array = ID3v2Tag::New(isolate, tag);
    args.GetReturnValue().Set(array);
}

void MPEG::SetID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    if (args.Length() != 1 || !args[0]->IsArray()) return;
    Isolate *isolate = Isolate::GetCurrent();
    MPEG *mpeg = ObjectWrap::Unwrap<MPEG>(args.Holder());
    TagLib::ID3v2::Tag *tag = mpeg->file->ID3v2Tag(true);
    Local<Array> frames = Local<Array>::Cast(args[0]);
    ID3v2Tag::Set(isolate, *frames, tag);
    // return frames
    Local<Array> array = ID3v2Tag::New(isolate, tag);
    args.GetReturnValue().Set(array);
}
