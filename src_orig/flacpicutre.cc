#include "flacpicture.h"

using namespace TagIO;
using namespace v8;
using namespace std;

Local<Object> FLACPictures::New(Isolate *isolate, TagLib::List<TagLib::FLAC::Picture *> list) {
    //TODO: Implement...
    Local<Object> object = Object::New(isolate);
    return object;
}

void FLACPictures::Set(Isolate *isolate, Object *object, TagLib::List<TagLib::FLAC::Picture *> list) {

}