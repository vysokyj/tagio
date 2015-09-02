#include "wrapper.h"
#include "configuration.h"

using namespace TagIO;
using namespace v8;
using namespace std;

Wrapper::Wrapper(Isolate *isolate, Object *object) : isolate(isolate), object(object) {}

Wrapper::~Wrapper() {}

bool Wrapper::GetBoolean(const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        return object->Get(String::NewFromUtf8(isolate, key))->BooleanValue();
    } else {
        return false;
    }
}

void Wrapper::SetBoolean(const char *key, bool value) {
    object->Set(String::NewFromUtf8(isolate, key), Boolean::New(isolate, value));
}

int Wrapper::GetInt32(const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        return (int) (object->Get(String::NewFromUtf8(isolate, key)))->Int32Value();
    } else {
        return 0;
    }
}

void Wrapper::SetInt32(const char *key, int value) {
    object->Set(String::NewFromUtf8(isolate, key), Integer::New(isolate, value));
}

TagLib::uint Wrapper::GetUint32(const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        return (uint) (object->Get(String::NewFromUtf8(isolate, key)))->Uint32Value();
    } else {
        return 0;
    }
}

void Wrapper::SetUint32(const char *key, const TagLib::uint value) {
    object->Set(String::NewFromUtf8(isolate, key), Integer::New(isolate, value));
}

TagLib::String Wrapper::GetString(const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        String::Utf8Value value(object->Get(keyString));
        return TagLib::String(*value, TagLib::String::UTF8);
    } else {
        return TagLib::String("", TagLib::String::UTF8);
    }
}

void Wrapper::SetString(const char *key, TagLib::String value) {
    object->Set(String::NewFromUtf8(isolate, key), String::NewFromUtf8(isolate, value.toCString(true)));
}

TagLib::ByteVector Wrapper::GetBinary(const char *key) {
    Configuration &conf = Configuration::Get();
    //TODO: Implement
    return nullptr;
}
void Wrapper::SetBinary(const char *key, const TagLib::ByteVector value, TagLib::String mimeType) {
    //TODO: Implement
}