#include "generic.h"

using namespace TagIO;
using namespace v8;
using namespace node;
using namespace std;

Persistent<Function> GENERIC::constructor;

GENERIC::GENERIC(string path) : Base(path) {
    file = new TagLib::FileRef(this->GetFilePath().c_str());
}

GENERIC::~GENERIC() {
    delete file;
}

void GENERIC::Init(Handle<Object> exports) {
    Isolate *isolate = Isolate::GetCurrent();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "GENERIC"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "save", Save);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getPath", GetPath);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getTag", GetTag);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTag", SetTag);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "GENERIC"), tpl->GetFunction());
}

void GENERIC::New(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    //HandleScope scope(isolate);

    if (args.IsConstructCall()) {
        // Invoked as constructor
        if (args.Length() >= 1) {
            String::Utf8Value path(args[0]->ToString());
            auto *ref = new GENERIC(string(*path));
            ref->Wrap(args.This());

            if (args.Length() >= 2) {

                Local<String> attachmentsDirKey = String::NewFromUtf8(isolate, "attachmentsDir");
                Local<String> attachmentsCtxKey = String::NewFromUtf8(isolate, "attachmentsCtx");

                Local<Object> config = args[1]->ToObject();
                if (config->Has(attachmentsDirKey)) {
                    String::Utf8Value val(config->Get(attachmentsDirKey)->ToString());
                    ref->SetAttachmentsDir(string(*val));
                }
                if (config->Has(attachmentsCtxKey)) {
                    String::Utf8Value val(config->Get(attachmentsCtxKey)->ToString());
                    ref->SetAttachmentsCtx(string(*val));
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

void GENERIC::Save(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<GENERIC>(args.Holder());
    bool result = ref->file->save();
    args.GetReturnValue().Set(Boolean::New(isolate, result));
}

void GENERIC::GetPath(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<GENERIC>(args.Holder());
    string path = ref->GetFilePath();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, path.c_str()));
}

void GENERIC::GetTag(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<GENERIC>(args.Holder());
    Local<Object> object = Object::New(isolate);
    SetObjectByTag(isolate, *object, ref->file->tag());
    args.GetReturnValue().Set(object);
}

void GENERIC::SetTag(const v8::FunctionCallbackInfo<v8::Value>& args) {
    Isolate *isolate = Isolate::GetCurrent();
    auto *ref = ObjectWrap::Unwrap<GENERIC>(args.Holder());
    Local<Object> object = Local<Object>::Cast(args[0]);
    SetTagByObject(isolate, *object, ref->file->tag());
}
