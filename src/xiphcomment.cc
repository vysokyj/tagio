#include "xiphcomment.h"
#include "wrapper.h"
#include "configuration.h"

using namespace TagIO;
using namespace v8;
using namespace std;

Local<Array> XiphComment::New(Isolate *isolate, TagLib::Ogg::XiphComment *tag) {
    EscapableHandleScope handleScope(isolate);
    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    Local<Array> array = Array::New(isolate, map.size());
    int i = 0;
    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::String key = it->first;
        TagLib::StringList values = it->second;
        Local<Object> object = Object::New(isolate);
        Wrapper o(isolate, *object);
        o.SetString("key", key);
        o.SetStringList("values", values);
        array->Set(i++, object);
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
        TagLib::StringList values = o.GetStringList("values");
        for (uint32_t i = 0; i < values.size(); i++)
            tag->addField(o.GetString("key"), values[i]);
    }
}

void XiphComment::Clear(TagLib::Ogg::XiphComment *tag) {
    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::String key = it->first;
        TagLib::StringList values = it->second;
        for( auto const& value: values) {
            tag->removeField(key, value);
        }
    }
}
