#include "xiphcomment.h"
#include "wrapper.h"
#include "configuration.h"

using namespace TagIO;
using namespace v8;
using namespace std;

Local<Object> XiphComment::New(Isolate *isolate, TagLib::Ogg::XiphComment *tag) {
    EscapableHandleScope handleScope(isolate);
    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    Local<Array> array = Array::New(isolate, map.size());
    int i = 0;
    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::String key = it->first;
        TagLib::StringList value = it->second;
        Local<Object> object = Object::New(isolate);
        Wrapper o(isolate, *object);
        o.SetString("key", key);
        o.SetStringList("value", value);
        array->Set(i++, object);
    }
    return handleScope.Escape(array);
}

void XiphComment::Set(Isolate *isolate, Object *object, TagLib::Ogg::XiphComment *tag) {
    Wrapper o(isolate, object);
    //TODO
    tag->setTitle(o.GetString("title"));
    tag->setAlbum(o.GetString("album"));
    tag->setArtist(o.GetString("artist"));
    tag->setTrack(o.GetUint32("track"));
    tag->setYear(o.GetUint32("year"));
    tag->setGenre(o.GetString("genre"));
    tag->setComment(o.GetString("comment"));
}
