#include "converter.h"

using namespace TagIO;
using namespace v8;

Converter::Converter(Isolate *isolate, Object *object) : isolate(isolate), object(object) {
}

Converter::~Converter() {}

bool Converter::GetBoolean(const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        return object->Get(String::NewFromUtf8(isolate, key))->BooleanValue();
    } else {
        return false;
    }
}

void Converter::SetBoolean(const char *key, bool value) {
    object->Set(String::NewFromUtf8(isolate, key), Boolean::New(isolate, value));
}

int Converter::GetInt32(const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        return (int) (object->Get(String::NewFromUtf8(isolate, key)))->Int32Value();
    } else {
        return 0;
    }
}

void Converter::SetInt32(const char *key, int value) {
    object->Set(String::NewFromUtf8(isolate, key), Integer::New(isolate, value));
}

TagLib::uint Converter::GetUint32(const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        return (uint) (object->Get(String::NewFromUtf8(isolate, key)))->Uint32Value();
    } else {
        return 0;
    }
}

void Converter::SetUint32(const char *key, const TagLib::uint value) {
    object->Set(String::NewFromUtf8(isolate, key), Integer::New(isolate, value));
}

TagLib::String Converter::GetString(const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        String::Utf8Value value(object->Get(keyString));
        return TagLib::String(*value, TagLib::String::UTF8);
    } else {
        return TagLib::String("", TagLib::String::UTF8);
    }
}

void Converter::SetString(const char *key, TagLib::String value) {
    object->Set(String::NewFromUtf8(isolate, key), String::NewFromUtf8(isolate, value.toCString(true)));
}