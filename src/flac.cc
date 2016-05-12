#include "flac.h"
#include "configuration.h"
#include "audioproperties.h"
#include "tag.h"
#include "xiphcomment.h"
#include "id3v1tag.h"
#include "id3v2tag.h"

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
    NODE_SET_PROTOTYPE_METHOD(tpl, "getIncludedTags", GetIncludedTags);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getXiphComment", GetXiphComment);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setXiphComment", SetXiphComment);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getID3v1Tag", GetID3v1Tag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setID3v1Tag", SetID3v1Tag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getID3v2Tag", GetID3v2Tag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setID3v2Tag", SetID3v2Tag);

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

void FLAC::GetIncludedTags(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    int c = 0;
    uint32_t i = 0;

    if (flac->file->hasXiphComment()) c++;
    if (flac->file->hasID3v1Tag()) c++;
    if (flac->file->hasID3v2Tag()) c++;

    Local<Array> array = Array::New(isolate, c);
    if (flac->file->hasXiphComment()) {
        array->Set(i++, String::NewFromUtf8(isolate, "xiphComment"));
    }
    if (flac->file->hasID3v1Tag()) {
        array->Set(i++, String::NewFromUtf8(isolate, "ID3v1"));
    }
    if (flac->file->hasID3v2Tag()) {
        TagLib::ID3v2::Tag *tag = flac->file->ID3v2Tag(false);
        string str = "ID3v2";
        str = str + "." + to_string(tag->header()->majorVersion());
        str = str + "." + to_string(tag->header()->revisionNumber());
        array->Set(i++, String::NewFromUtf8(isolate, str.c_str()));
    }
    args.GetReturnValue().Set(array);
}

void FLAC::GetXiphComment(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = Isolate::GetCurrent();
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    if (!flac->file->hasXiphComment()) return;
    TagLib::Ogg::XiphComment *tag = flac->file->xiphComment(false);
    Local<Object> object = XiphComment::New(isolate, tag);
    args.GetReturnValue().Set(object);
}

void FLAC::SetXiphComment(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = Isolate::GetCurrent();
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    TagLib::Ogg::XiphComment *tag = flac->file->xiphComment(true);
    Local<Array> object1 = Local<Array>::Cast(args[0]);
    XiphComment::Set(isolate, *object1, tag);
    // return tag
    Local<Object> object2 = XiphComment::New(isolate, tag);
    args.GetReturnValue().Set(object2);
}

void FLAC::GetID3v1Tag(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = Isolate::GetCurrent();
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    if (!flac->file->hasID3v1Tag()) return;
    TagLib::ID3v1::Tag *tag = flac->file->ID3v1Tag(false);
    Local<Object> object = ID3v1Tag::New(isolate, tag);
    args.GetReturnValue().Set(object);
}

//TODO: SetID3v1Tag not working
void FLAC::SetID3v1Tag(const FunctionCallbackInfo<Value> &args) {
    Isolate *isolate = Isolate::GetCurrent();
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    TagLib::ID3v1::Tag *tag = flac->file->ID3v1Tag(true);
    Local<Object> object1 = Local<Object>::Cast(args[0]);
    ID3v1Tag::Set(isolate, *object1, tag);
    // return tag
    Local<Object> object2 = ID3v1Tag::New(isolate, tag);
    args.GetReturnValue().Set(object2);
}

void FLAC::GetID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    if (!flac->file->hasID3v2Tag()) return;
    TagLib::ID3v2::Tag *tag = flac->file->ID3v2Tag(false);
    Local<Array> array = ID3v2Tag::New(isolate, tag);
    args.GetReturnValue().Set(array);
}

//TODO: SetID3v2Tag not working
void FLAC::SetID3v2Tag(const FunctionCallbackInfo<Value>& args) {
    if (args.Length() != 1 || !args[0]->IsArray()) return;
    Isolate *isolate = Isolate::GetCurrent();
    FLAC *flac = ObjectWrap::Unwrap<FLAC>(args.Holder());
    TagLib::ID3v2::Tag *tag = flac->file->ID3v2Tag(true);
    Local<Array> frames = Local<Array>::Cast(args[0]);
    ID3v2Tag::Set(isolate, *frames, tag);
    // return frames
    Local<Array> array = ID3v2Tag::New(isolate, tag);
    args.GetReturnValue().Set(array);
}
