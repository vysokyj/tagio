#include "apetag.h"
#include "wrapper.h"

using namespace TagIO;
using namespace v8;
using namespace std;

Local<Object> APETag::New(Isolate *isolate, TagLib::APE::Tag *tag) {
    EscapableHandleScope handleScope(isolate);
    Local<Object> object = Object::New(isolate);
    Wrapper o(isolate, *object);
    o.SetString("title", tag->title());
    o.SetString("album", tag->album());
    o.SetString("artist", tag->artist());
    o.SetUint32("track", tag->track());
    o.SetUint32("year", tag->year());
    o.SetString("genre", tag->genre());
    o.SetString("comment", tag->comment());
    return handleScope.Escape(object);
}

void APETag::Set(Isolate *isolate, Object *object, TagLib::APE::Tag *tag) {
    Wrapper o(isolate, object);
    tag->setTitle(o.GetString("title"));
    tag->setAlbum(o.GetString("album"));
    tag->setArtist(o.GetString("artist"));
    tag->setTrack(o.GetUint32("track"));
    tag->setYear(o.GetUint32("year"));
    tag->setGenre(o.GetString("genre"));
    tag->setComment(o.GetString("comment"));
}