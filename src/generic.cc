#include "generic.h"
#include "tag.h"
#include "audioproperties.h"

#include <taglib/fileref.h>


using std::string;
using v8::Function;
using v8::Local;
using v8::Value;
using v8::String;
using v8::Object;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;

class ReadWorker : public AsyncWorker {
public:
    ReadWorker(Callback *callback, string *path)
            : AsyncWorker(callback), path(path) {}
    ~ReadWorker() {
        delete path;
        delete file;
    }

    void Execute () {
        file = new TagLib::FileRef(path->c_str());
        file->tag();
        file->audioProperties();
    }

    void HandleOKCallback () {
        HandleScope scope;
        Local<Object> result = New<Object>();
        Local<Object> audioProperties = ExportAudioProperties(file->audioProperties());
        Local<Object> tag = ExportTag(file->tag());
        result->Set(New<String>("audioProperties").ToLocalChecked(), audioProperties);
        result->Set(New<String>("tag").ToLocalChecked(), tag);
        Local<Value> argv[] = { Null(), result };
        callback->Call(2, argv);
    }

private:
    string *path;
    TagLib::FileRef *file;
};

NAN_METHOD(ReadGeneric) {
    String::Utf8Value arg(info[0]->ToString());
    std::string *path = new std::string(*arg);
    Callback *callback = new Callback(info[1].As<Function>());

    AsyncQueueWorker(new ReadWorker(callback, path));
}
