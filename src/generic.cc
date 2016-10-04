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

class GenericWorker : public AsyncWorker {
public:

    GenericWorker(Callback *callback, string *path, Configuration *conf)
            : AsyncWorker(callback), path(path), conf(conf) {
            write = false;
    }

    GenericWorker(Callback *callback, string *path, Configuration *conf, TagLib::FileRef *file, TagLib::Tag *tag)
            : AsyncWorker(callback), path(path), conf(conf), file(file), tag(tag) {
        write = true;
    }

    ~GenericWorker() {
        delete path;
        delete conf;
        delete file;
    }

    void Execute () {
        if (write) file->save();
        file = new TagLib::FileRef(path->c_str());
        tag = file->tag();
        audioProperties = file->audioProperties();
    }

    void HandleOKCallback () {
        HandleScope scope;
        Local<Object> result = New<Object>();

        Local<String> pathKey = New<String>("path").ToLocalChecked();
        Local<String> pathVal = New<String>(path->c_str()).ToLocalChecked();
        result->Set(pathKey, pathVal);

        if (conf->ConfigurationReadable()) {
            Local<String> confKey = New<String>("configuration").ToLocalChecked();
            Local<Object> confVal = New<Object>();
            ExportConfiguration(conf, *confVal);
            result->Set(confKey, confVal);
        }

        if (conf->AudioPropertiesReadable()) {
            Local<String> audioPropertiesKey = New<String>("audioProperties").ToLocalChecked();
            Local<Object> audioPropertiesVal = New<Object>();
            ExportAudioProperties(audioProperties, *audioPropertiesVal);
            result->Set(audioPropertiesKey, audioPropertiesVal);
        }

        if (conf->TagReadable()) {
            Local<String> tagKey = New<String>("tag").ToLocalChecked();
            Local<Object> tagVal = New<Object>();
            ExportTag(tag, *tagVal);
            result->Set(tagKey, tagVal);
        }

        Local<Value> argv[] = { Null(), result };
        callback->Call(2, argv);
    }

private:
    bool write = false;
    string *path;
    Configuration *conf;
    TagLib::FileRef *file;

    TagLib::AudioProperties *audioProperties;
    TagLib::Tag *tag;
};


NAN_METHOD(ReadGeneric) {
    Local<Object> reqObj = info[0].As<Object>();
    Callback *callback = new Callback(info[1].As<Function>());

    Local<String> pathKey = New<String>("path").ToLocalChecked();
    Local<String> pathObj = reqObj->Get(pathKey).As<String>();
    String::Utf8Value pathVal(pathObj);
    std::string *path = new std::string(*pathVal);

    Local<String> confKey = New<String>("configuration").ToLocalChecked();
    Local<Object> confVal = reqObj->Get(confKey).As<Object>();
    Configuration *conf = new Configuration();
    ImportConfiguration(*confVal, conf);

    AsyncQueueWorker(new GenericWorker(callback, path, conf));
}

NAN_METHOD(WriteGeneric) {
    Local<Object> reqObj = info[0].As<Object>();
    Callback *callback = new Callback(info[1].As<Function>());

    Local<String> pathKey = New<String>("path").ToLocalChecked();
    Local<String> pathObj = reqObj->Get(pathKey).As<String>();
    String::Utf8Value pathVal(pathObj);
    std::string *path = new std::string(*pathVal);

    Local<String> confKey = New<String>("configuration").ToLocalChecked();
    Local<Object> confVal = reqObj->Get(confKey).As<Object>();
    Configuration *conf = new Configuration();
    ImportConfiguration(*confVal, conf);

    //TODO: Open and write tag async? Possible?
    TagLib::FileRef *file = new TagLib::FileRef(path->c_str());
    Local<String> tagKey = New<String>("tag").ToLocalChecked();
    Local<Object> tagVal = reqObj->Get(tagKey).As<Object>();
    TagLib::Tag *tag = file->tag();
    ImportTag(*tagVal, tag);

    AsyncQueueWorker(new GenericWorker(callback, path, conf, file, tag));
}
