#include "xiphcomment.h"
#include "wrapper.h"

using namespace TagIO;
using namespace v8;
using namespace std;

Local<Array> XiphComment::New(Isolate *isolate, TagLib::Ogg::XiphComment *tag) {
    uint32_t i = 0;
    uint32_t c = 0;
    EscapableHandleScope handleScope(isolate);
    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    // count items
    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::StringList values = it->second;
        c += values.size();
    }

    Local<Array> array = Array::New(isolate, c);

    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::String key = it->first;
        TagLib::StringList values = it->second;
        for (auto const& value: values) {
            Local<Object> object = Object::New(isolate);
            Wrapper o(isolate, *object);
            o.SetString("id", key);
            o.SetString("text", value);
            array->Set(i++, object);
        }
    }
    return handleScope.Escape(array);
}

void XiphComment::Set(Isolate *isolate, Array *array, TagLib::Ogg::XiphComment *tag) {
    //tag->removeAllFields(); // in new version
    Clear(tag);
    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    map.clear();
    for (unsigned int i = 0; i < array->Length(); i++) {
        Local<Object> object = array->Get(i)->ToObject();
        Wrapper o(isolate, *object);
        tag->addField(o.GetString("id"), o.GetString("text"));
    }
}

void XiphComment::Clear(TagLib::Ogg::XiphComment *tag) {
    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::String key = it->first;
        TagLib::StringList values = it->second;
        for (auto const& value: values)
            tag->removeField(key, value);
    }
}
