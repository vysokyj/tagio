#include "flac.h"
#include "configuration.h"
#include "audioproperties.h"
#include "tag.h"

using namespace TagIO;
using namespace v8;
using namespace node;
using namespace std;

Persistent<Function> FLAC::constructor;

FLAC::FLAC(const char *path) : Base(path) {
    file = new TagLib::FLAC::File(path);
}

FLAC::~FLAC() {
    delete file;
}

void FLAC::Init(Handle<Object> exports) {
    Isolate *isolate = Isolate::GetCurrent();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "FLAC"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    // Generic API
    NODE_SET_PROTOTYPE_METHOD(tpl, "save", Save);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getPath", GetPath);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getAudioProperties", GetAudioProperties);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getTag", GetTag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTag", SetTag);
    // FLAC API

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "FLAC"), tpl->GetFunction());
}

void FLAC::New(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    //HandleScope scope(isolate);

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
        auto *ref = new FLAC(*path);
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

void FLAC::Save(const FunctionCallbackInfo<Value>& args) {
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    Isolate *isolate = Isolate::GetCurrent();

    if (flac->saved) return; //TODO: Throw exception.
    else flac->saved = true;

    bool result = flac->file->save();
    args.GetReturnValue().Set(Boolean::New(isolate, result));
}

void FLAC::GetPath(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<FLAC>(args.Holder());
    string path = ref->GetFilePath();
    if (Configuration::Get().BinaryDataMethod() == BinaryDataMethod::ABSOLUTE_URL)
        path = "file://" + path;
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, path.c_str()));
}

void FLAC::GetAudioProperties(const FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    Local<Object> object = AudioProperties::New(isolate, flac->file->audioProperties());
    args.GetReturnValue().Set(object);
}

void FLAC::GetTag(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<FLAC>(args.Holder());
    Local<Object> object = Tag::New(isolate, ref->file->tag());
    args.GetReturnValue().Set(object);
}

void FLAC::SetTag(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<FLAC>(args.Holder());
    Local<Object> object = Local<Object>::Cast(args[0]);
    Tag::Set(isolate, *object, ref->file->tag());
}

//----------------------------------------------------------------------------------------------------------------------
// FLAC API
