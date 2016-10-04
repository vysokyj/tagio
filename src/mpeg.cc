#include <taglib/generalencapsulatedobjectframe.h>
#include "mpeg.h"
#include "configuration.h"
#include "audioproperties.h"
#include "bytevector.h"
#include "tag.h"
#include "apetag.h"
#include "id3v1tag.h"
#include "id3v2tag.h"

#include "taglib/mpegfile.h"
#include "taglib/id3v1tag.h"
#include "taglib/id3v2tag.h"
#include "taglib/attachedpictureframe.h"
#include "taglib/generalencapsulatedobjectframe.h"
#include "taglib/uniquefileidentifierframe.h"


using std::string;
using std::map;
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
               TagLib::ID3v1::Tag *id3v1Tag, TagLib::ID3v2::Tag *id3v2Tag, std::map<uintptr_t, std::string> *fmap)
            : AsyncWorker(callback),
              path(path),
              conf(conf),
              id3v1Tag(id3v1Tag),
              id3v2Tag(id3v2Tag),
              fmap(fmap),
              save(true) {}

    ~MPEGWorker() {
        delete path;
        delete conf;
        delete file;
        if (fmap != nullptr) {
            //TODO: memory leak?
            //fmap->clear();
            //delete fmap;
        }
    }

    void Execute () {
        file = new TagLib::MPEG::File(path->c_str());
        if (save) {
            if (conf->ID3v1Writable()) WriteID3v1();
            if (conf->ID3v2Writable()) WriteID3v2();
            if (conf->APEWritable()) WriteAPE();
            SaveFile();
            delete file;
            delete id3v1Tag;
            delete id3v2Tag;
        }
        file = new TagLib::MPEG::File(path->c_str());
        audioProperties = file->audioProperties();
        tag = file->tag();
        id3v1Tag = file->ID3v1Tag(false);
        id3v2Tag = file->ID3v2Tag(false);
    }

    void HandleOKCallback () {
        //std::cout << conf->fileDirectory() << std::endl;
        HandleScope scope;
        Local<Object> result= New<Object>();

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

        if (conf->ID3v1Readable() && id3v1Tag != nullptr) {
            Local<String> id3v1Key = New<String>("id3v1").ToLocalChecked();
            Local<Object> id3v1Val = New<Object>();
            ExportID3v1Tag(id3v1Tag, *id3v1Val);
            result->Set(id3v1Key, id3v1Val);
        }

        if (conf->ID3v2Readable() && id3v2Tag != nullptr) {
            Local<String> id3v2Key = New<String>("id3v2").ToLocalChecked();
            Local<Array> id3v2Val = New<Array>(id3v2Tag->frameList().size());
            ExportID3v2Tag(id3v2Tag, *id3v2Val, conf);
            result->Set(id3v2Key, id3v2Val);
        }

        //TODO: Read ape tag...

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
    std::map<uintptr_t, std::string> *fmap;

    void WriteID3v1();
    void WriteID3v2();
    void WriteAPE();
    void SaveFile();
};

inline void MPEGWorker::WriteID3v1() {
    TagLib::ID3v1::Tag *t1 = file->ID3v1Tag(true);
    t1->setArtist(id3v1Tag->artist());
    t1->setAlbum(id3v1Tag->album());
    t1->setTrack(id3v1Tag->track());
    t1->setTitle(id3v1Tag->title());
    t1->setGenre(id3v1Tag->genre());
    t1->setGenreNumber(id3v1Tag->genreNumber());
    t1->setYear(id3v1Tag->year());
    t1->setComment(id3v1Tag->comment());
}

inline void MPEGWorker::WriteID3v2() {
    TagLib::ID3v2::Tag *t2 = file->ID3v2Tag(true);
    TagLib::ID3v2::FrameList l2 = id3v2Tag->frameList();
    TagLib::ID3v2::FrameFactory *factory = TagLib::ID3v2::FrameFactory::instance();

    for (unsigned int i = 0; i < l2.size(); i++) {
        TagLib::ID3v2::Frame *frame = l2[i];
        if (auto f = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame)) {
            uintptr_t addr = (uintptr_t) frame;
            std::string path = (*fmap)[addr];
            f->setPicture(ImportByteVector(path, conf));
        }
        if (auto f = dynamic_cast<TagLib::ID3v2::GeneralEncapsulatedObjectFrame *>(frame)) {
            uintptr_t addr = (uintptr_t) frame;
            std::string path = (*fmap)[addr];
            f->setObject(ImportByteVector(path, conf));
        }
        if (auto f = dynamic_cast<TagLib::ID3v2::UniqueFileIdentifierFrame *>(frame)) {
            uintptr_t addr = (uintptr_t) frame;
            std::string path = (*fmap)[addr];
            f->setIdentifier(ImportByteVector(path, conf));
        }
        //TODO: Slow but safe?
        t2->addFrame(factory->createFrame(frame->render(), conf->ID3v2Version()));
    }
}

inline void MPEGWorker::WriteAPE() {
    //TODO: Implement
}

inline void MPEGWorker::SaveFile() {
    int NoTags  = 0x0000;
    int ID3v1   = 0x0001;
    int ID3v2   = 0x0002;
    int APE     = 0x0004;
    int AllTags = 0xffff;
    int tags = NoTags;
    if (conf->ID3v1Writable())  tags = tags | ID3v1;
    if (conf->ID3v2Writable())  tags = tags | ID3v2;
    if (conf->APEWritable())    tags = tags | APE;
    if (conf->ID3v1Writable() && conf->ID3v2Writable() && conf->APEWritable()) tags = AllTags;
    //cout << "SAVE TAGS: " << tags << endl;
    bool stripOthers = true;
    uint32_t id3v2Version = conf->ID3v2Version();
    bool duplicateTags = true;
    bool result = file->save(tags, stripOthers, id3v2Version, duplicateTags);
}



NAN_METHOD(ReadMPEG) {
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

    AsyncQueueWorker(new MPEGWorker(callback, path, conf));
}

NAN_METHOD(WriteMPEG) {
    Configuration *conf = new Configuration();
    TagLib::ID3v1::Tag *id3v1Tag = new TagLib::ID3v1::Tag();
    TagLib::ID3v2::Tag *id3v2Tag = new TagLib::ID3v2::Tag();
    std::map<uintptr_t, std::string> *fmap = new std::map<uintptr_t, std::string>();

    Local<Object> reqObj = info[0].As<Object>();
    Callback *callback = new Callback(info[1].As<Function>());

    Local<String> pathKey = New<String>("path").ToLocalChecked();
    Local<String> confKey = New<String>("configuration").ToLocalChecked();
    Local<String> id3v1Key = New<String>("id3v1").ToLocalChecked();
    Local<String> id3v2Key = New<String>("id3v2").ToLocalChecked();
    Local<String> apeKey = New<String>("ape").ToLocalChecked();

    Local<String> pathObj = reqObj->Get(pathKey).As<String>();
    String::Utf8Value pathVal(pathObj);
    std::string *path = new std::string(*pathVal);

    if (reqObj->Has(confKey)) {
        Local<Object> confVal = reqObj->Get(confKey).As<Object>();
        ImportConfiguration(*confVal, conf);
    }

    if (conf->ID3v1Writable() && reqObj->Has(id3v1Key)) {
        Local<Object> id3v1Val = reqObj->Get(id3v1Key).As<Object>();
        ImportID3v1Tag(*id3v1Val, id3v1Tag, conf);
    }

    if (conf->ID3v2Writable() && reqObj->Has(id3v2Key)) {
        Local<Array> id3v2Val = reqObj->Get(id3v2Key).As<Array>();
        ImportID3v2Tag(*id3v2Val, id3v2Tag, fmap, conf);
    }

//
//    //TODO: Add ape tag.
//
    AsyncQueueWorker(new MPEGWorker(callback, path, conf, id3v1Tag, id3v2Tag, fmap));
}
