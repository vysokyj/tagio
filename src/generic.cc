#include "generic.h"
#include "configuration.h"
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
    ReadWorker(Callback *callback, string *path, Configuration *conf)
            : AsyncWorker(callback), path(path), conf(conf) {}
    ~ReadWorker() {
        delete path;
        delete conf;
        delete file;
    }

    void Execute () {
        file = new TagLib::FileRef(path->c_str());
        file->tag();
        file->audioProperties();
    }

    void HandleOKCallback () {
        HandleScope scope;
        Local<Object> resultObj = New<Object>();
        Local<String> pathKey = New<String>("path").ToLocalChecked();
        Local<String> pathStr = New<String>(path->c_str()).ToLocalChecked();
        Local<String> confKey = New<String>("configuration").ToLocalChecked();
        Local<Object> confObj = New<Object>();
        Local<String> audioPropertiesKey = New<String>("audioProperties").ToLocalChecked();
        Local<Object> audioPropertiesObj = New<Object>();
        Local<String> tagKey = New<String>("tag").ToLocalChecked();
        Local<Object> tagObj = New<Object>();
        ExportConfiguration(conf, *confObj);
        ExportAudioProperties(file->audioProperties(), *audioPropertiesObj);
        ExportTag(file->tag(), *tagObj);
        resultObj->Set(pathKey, pathStr);
        resultObj->Set(confKey, confObj);
        resultObj->Set(audioPropertiesKey, audioPropertiesObj);
        resultObj->Set(tagKey, tagObj);
        Local<Value> argv[] = { Null(), resultObj };
        callback->Call(2, argv);
    }

private:
    string *path;
    Configuration *conf;
    TagLib::FileRef *file;
};

NAN_METHOD(ReadGeneric) {
    Local<Object> reqObj = info[0].As<Object>();
    Callback *callback = new Callback(info[1].As<Function>());

    Local<String> pathKey = New<String>("path").ToLocalChecked();
    Local<String> pathObj = reqObj->Get(pathKey).As<String>();
    Local<String> confKey = New<String>("configuration").ToLocalChecked();
    Local<Object> confObj = reqObj->Get(confKey).As<Object>();

    String::Utf8Value pathVal(pathObj);
    std::string *path = new std::string(*pathVal);
    Configuration *conf = new Configuration();
    ExportConfiguration(conf, *confObj);

    AsyncQueueWorker(new ReadWorker(callback, path, conf));
}
