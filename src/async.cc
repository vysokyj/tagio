#include "async.h"

#include <iostream>

#include <uv.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include <taglib/fileref.h>

#include "audioproperties.h"
#include "tag.h"



using namespace v8;
using namespace TagIO;


struct Work {
    uv_work_t request;
    std::string path;
    Persistent<Function> callback;
    TagLib::FileRef *filreRef;
    TagLib::AudioProperties *audioProperties; // not owned
    TagLib::Tag *tag; // not owned
};

static void WorkAsync(uv_work_t *req) {
    Work *work = static_cast<Work *>(req->data);
    work->filreRef = new TagLib::FileRef(work->path.c_str());
    work->audioProperties = work->filreRef->audioProperties();
    work->tag = work->filreRef->tag();
}

static void WorkAsyncComplete(uv_work_t *req, int status) {
    Work *work = static_cast<Work *>(req->data);
    Isolate * isolate = Isolate::GetCurrent();
    HandleScope handleScope(isolate);
    Local<Object> result = Object::New(isolate);

    // wrap objects
    result->Set(String::NewFromUtf8(isolate, "path"), String::NewFromUtf8(isolate, work->path.c_str()));
    result->Set(String::NewFromUtf8(isolate, "audioProperties"), AudioProperties::New(isolate, work->audioProperties));
     //result->Set(String::NewFromUtf8(isolate, "tag"), String::NewFromUtf8(isolate, "POKUS"));

    // set up return arguments and execute callback
    Handle<Value> argv[] = { Null(isolate) , result };
    Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 2, argv);

    // free up the persistent function callback
    work->callback.Reset();

    delete work->filreRef; // delete only ref!!
    delete work;
}

void TagIO::TagReadAsync(const FunctionCallbackInfo<Value> &args) {
    Isolate* isolate = args.GetIsolate();
    String::Utf8Value path(args[0]->ToString());
    Local<Function> callback = Local<Function>::Cast(args[1]);

    Work * work = new Work();
    work->request.data = work;
    work->path = std::string(*path);
    work->callback.Reset(isolate, callback);

    std::cout << work->path << std::endl;

    // kick of the worker thread
    uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);

    // return
    args.GetReturnValue().Set(Undefined(isolate));
}

void TagIO::TagInit(Handle<Object> exports) {
    NODE_SET_METHOD(exports, "tagReadAsync", TagReadAsync);
}