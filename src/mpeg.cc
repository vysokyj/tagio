#include "mpeg.h"
#include "configuration.h"
#include "audioproperties.h"
#include "tag.h"
#include "apetag.h"
#include "id3v1tag.h"
#include "id3v2tag.h"

#include "taglib/mpegfile.h"
#include "taglib/id3v1tag.h"
#include "taglib/id3v2tag.h"

using std::string;
using v8::Function;
using v8::Local;
using v8::Value;
using v8::String;
using v8::Object;
using v8::Array;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::New;
using Nan::Null;
using Nan::To;

class MPEGWorker : public AsyncWorker {
public:
    MPEGWorker(Callback *callback, string *path, Configuration *conf)
            : AsyncWorker(callback), path(path), conf(conf) {}

    MPEGWorker(Callback *callback, string *path, Configuration *conf,
               TagLib::ID3v1::Tag *id3v1Tag, TagLib::ID3v2::Tag *id3v2Tag)
            : AsyncWorker(callback), path(path), conf(conf), id3v1Tag(id3v1Tag), id3v2Tag(id3v2Tag) { save = true; }

    ~MPEGWorker() {
        delete path;
        delete conf;
        delete file;
    }

    void Execute () {
        file = new TagLib::MPEG::File(path->c_str());
        if (save) {
            TagLib::ID3v1::Tag *t1 = file->ID3v1Tag(true);
            t1->setArtist(id3v1Tag->artist());
            t1->setAlbum(id3v1Tag->album());
            t1->setTrack(id3v1Tag->track());
            t1->setTitle(id3v1Tag->title());
            t1->setGenre(id3v1Tag->genre());
            t1->setGenreNumber(id3v1Tag->genreNumber());
            t1->setYear(id3v1Tag->year());
            t1->setComment(id3v1Tag->comment());

            TagLib::ID3v2::Tag *t2 = file->ID3v2Tag(true);
            TagLib::ID3v2::FrameList l2 = id3v2Tag->frameList();

            //TODO: Clear t2;
            for (unsigned int i = 0; i < l2.size(); i++) {
                TagLib::ID3v2::Frame *frame = l2[i];
                t2->addFrame(frame);
            }

            // save operation
            int NoTags  = 0x0000;
            int ID3v1   = 0x0001;
            int ID3v2   = 0x0002;
            int APE     = 0x0004;
            int AllTags = 0xffff;
            int tags = NoTags;
            if (conf->ID3v1Writable())  tags = tags | ID3v1;
            if (conf->ID3v1Writable())  tags = tags | ID3v2;
            if (conf->APEWritable())    tags = tags | APE;
            if (conf->ID3v1Writable() && conf->ID3v1Writable() && conf->APEWritable()) tags = AllTags;
            //cout << "SAVE TAGS: " << tags << endl;
            bool stripOthers = true;
            uint32_t id3v2Version = conf->ID3v2Version();
            bool duplicateTags = true;
            bool result = file->save(tags, stripOthers, id3v2Version, duplicateTags);

            delete file;
            delete id3v1Tag;
            //delete id3v2Tag; //TODO: Memory leak...
        }

        file = new TagLib::MPEG::File(path->c_str());
        audioProperties = file->audioProperties();
        tag = file->tag();
        id3v1Tag = file->ID3v1Tag(false);
        id3v2Tag = file->ID3v2Tag(false);
    }

    void HandleOKCallback () {
        HandleScope scope;
        Local<Object> result= New<Object>();

        Local<String> pathKey = New<String>("path").ToLocalChecked();
        Local<String> pathVal = New<String>(path->c_str()).ToLocalChecked();
        result->Set(pathKey, pathVal);

        Local<String> confKey = New<String>("configuration").ToLocalChecked();
        Local<Object> confVal = New<Object>();
        ExportConfiguration(conf, *confVal);
        result->Set(confKey, confVal);

        Local<String> audioPropertiesKey = New<String>("audioProperties").ToLocalChecked();
        Local<Object> audioPropertiesVal = New<Object>();
        ExportAudioProperties(audioProperties, *audioPropertiesVal);
        result->Set(audioPropertiesKey, audioPropertiesVal);

        Local<String> tagKey = New<String>("tag").ToLocalChecked();
        Local<Object> tagVal = New<Object>();
        ExportTag(tag, *tagVal);
        result->Set(tagKey, tagVal);

        if (id3v1Tag != nullptr) {
            Local<String> id3v1Key = New<String>("id3v1").ToLocalChecked();
            Local<Object> id3v1Val = New<Object>();
            ExportID3v1Tag(id3v1Tag, *id3v1Val);
            result->Set(id3v1Key, id3v1Val);
        }

        if (id3v2Tag != nullptr) {
            Local<String> id3v2Key = New<String>("id3v2").ToLocalChecked();
            Local<Array> id3v2Val = New<Array>(id3v2Tag->frameList().size());
            ExportID3v2Tag(id3v2Tag, *id3v2Val, conf);
            result->Set(id3v2Key, id3v2Val);
        }

        Local<Value> argv[] = { Null(), result };
        callback->Call(2, argv);
    }

private:
    bool save = false;
    string *path;
    Configuration *conf;
    TagLib::MPEG::File *file;

    // extracted
    TagLib::MPEG::Properties *audioProperties;
    TagLib::Tag *tag;
    TagLib::ID3v1::Tag *id3v1Tag;
    TagLib::ID3v2::Tag *id3v2Tag;
};

NAN_METHOD(ReadMPEG) {
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

    AsyncQueueWorker(new MPEGWorker(callback, path, conf));
}

NAN_METHOD(WriteMPEG) {
    Local<Object> reqObj = info[0].As<Object>();
    Callback *callback = new Callback(info[1].As<Function>());

    Local<String> pathKey = New<String>("path").ToLocalChecked();
    Local<String> pathObj = reqObj->Get(pathKey).As<String>();
    String::Utf8Value pathVal(pathObj);
    std::string *path = new std::string(*pathVal);

    Local<String> confKey = New<String>("configuration").ToLocalChecked();
    Local<Object> confVal = reqObj->Get(confKey).As<Object>();
    Configuration *conf = new Configuration();
    ExportConfiguration(conf, *confVal);

    Local<String> id3v1Key = New<String>("id3v1").ToLocalChecked();
    Local<Object> id3v1Val = reqObj->Get(id3v1Key).As<Object>();
    TagLib::ID3v1::Tag *id3v1Tag = new TagLib::ID3v1::Tag();
    ImportID3v1Tag(*id3v1Val, id3v1Tag, conf);

    Local<String> id3v2Key = New<String>("id3v2").ToLocalChecked();
    Local<Array> id3v2Val = reqObj->Get(id3v2Key).As<Array>();
    TagLib::ID3v2::Tag *id3v2Tag = new TagLib::ID3v2::Tag();
    ImportID3v2Tag(*id3v2Val, id3v2Tag, conf);
//
//    //TODO: Add ape tag.
//
    AsyncQueueWorker(new MPEGWorker(callback, path, conf, id3v1Tag, id3v2Tag));
}
