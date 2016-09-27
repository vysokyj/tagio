#include "xiphcomment.h"
#include "wrapper.h"


using namespace v8;
using namespace std;

using v8::Local;
using v8::Object;
using v8::Array;
using Nan::HandleScope;
using Nan::New;


static void Clear(TagLib::Ogg::XiphComment *tag) {
    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::String key = it->first;
        TagLib::StringList values = it->second;
        for (auto const& value: values)
            tag->removeField(key, value);
    }
}

void ExportXiphComment(TagLib::Ogg::XiphComment *tag, v8::Array *array) {
    uint32_t i = 0;
    uint32_t c = 0;

    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    // count items
    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::StringList values = it->second;
        c += values.size();
    }

    for (auto it = map.begin(); it != map.end(); it++) {
        TagLib::String key = it->first;
        TagLib::StringList values = it->second;
        for (auto const& value: values) {
            Local<Object> object = New<Object>();
            TagLibWrapper o(*object);
            o.SetString("id", key);
            o.SetString("text", value);
            array->Set(i++, object);
        }
    }
}

void ImportXiphComment(v8::Array *array, TagLib::Ogg::XiphComment *tag) {
    //tag->removeAllFields(); // in new version
    Clear(tag);
    TagLib::Ogg::FieldListMap map = tag->fieldListMap();
    map.clear();
    for (unsigned int i = 0; i < array->Length(); i++) {
        Local<Object> object = array->Get(i)->ToObject();
        TagLibWrapper o(*object);
        tag->addField(o.GetString("id"), o.GetString("text"));
    }
}

