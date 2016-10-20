#include "wrapper.h"


using namespace v8;
using namespace std;
using Nan::New;


TagLibWrapper::TagLibWrapper(Object *object) : object(object) {}

TagLibWrapper::~TagLibWrapper() {}

bool TagLibWrapper::GetBoolean(const char *key) {
    Local<String> keyString = New<String>(key).ToLocalChecked();
    if (object->Has(keyString)) {
        return object->Get(New<String>(key).ToLocalChecked())->BooleanValue();
    } else {
        return false;
    }
}

void TagLibWrapper::SetBoolean(const char *key, bool value) {
    object->Set(New<String>(key).ToLocalChecked(), New<Boolean>(value));
}

double TagLibWrapper::GetNumber(const char *key) {
    Local<String> keyString = New<String>(key).ToLocalChecked();
    if (object->Has(keyString)) {
        return object->Get(New<String>(key).ToLocalChecked())->NumberValue();
    } else {
        return 0.0;
    }
}

void TagLibWrapper::SetNumber(const char *key, double value) {
    object->Set(New<String>(key).ToLocalChecked(), New<Number>(value));
}

int TagLibWrapper::GetInt32(const char *key) {
    Local<String> keyString = New<String>(key).ToLocalChecked();
    if (object->Has(keyString)) {
        return (int) (object->Get(New<String>(key).ToLocalChecked()))->Int32Value();
    } else {
        return 0;
    }
}

void TagLibWrapper::SetInt32(const char *key, int value) {
    object->Set(New<String>(key).ToLocalChecked(), New<Integer>(value));
}

TagLib::uint TagLibWrapper::GetUint32(const char *key) {
    Local<String> keyString = New<String>(key).ToLocalChecked();
    if (object->Has(keyString)) {
        return (TagLib::uint) (object->Get(New<String>(key).ToLocalChecked()))->Uint32Value();
    } else {
        return 0;
    }
}

void TagLibWrapper::SetUint32(const char *key, const TagLib::uint value) {
    object->Set(New<String>(key).ToLocalChecked(), New<Integer>(value));
}

TagLib::String TagLibWrapper::GetString(const char *key) {
    Local<String> keyString = New<String>(key).ToLocalChecked();
    if (object->Has(keyString)) {
        String::Utf8Value value(object->Get(keyString));
        return TagLib::String(*value, TagLib::String::UTF8);
    } else {
        return TagLib::String::null;
    }
}

void TagLibWrapper::SetString(const char *key, TagLib::String value) {
    object->Set(New<String>(key).ToLocalChecked(), New<String>(value.toCString(true)).ToLocalChecked());
}

TagLib::StringList TagLibWrapper::GetStringList(const char *key) {
    Local<String> keyString = New<String>(key).ToLocalChecked();
    Local<Array> array = Local<Array>::Cast(object->Get(keyString));
    TagLib::StringList list;
    //cout << "LENGTH" << array->Length() << endl;
    if (object->Has(keyString)) {
       for (uint32_t i = 0; i < array->Length(); i++) {
           String::Utf8Value value(array->Get(i)->ToString());
           TagLib::String string(*value, TagLib::String::UTF8);
           list.append(string);
       }
    }
    return list;
}

void TagLibWrapper::SetStringList(const char *key, TagLib::StringList value) {
    Local<Array> array = New<Array>(value.size());
    for (uint32_t i = 0; i < value.size(); i++) {
        array->Set(i, New<String>(value[i].toCString(true)).ToLocalChecked());
    }
    object->Set(New<String>(key).ToLocalChecked(), array);
}

//TagLib::ByteVector TagLibWrapper::GetBytes(const char *key, std::map<uintptr_t, std::string> *fmap) {
//    Local<String> keyString = (String::NewFromUtf8(key))->ToString();
//    if (object->Has(keyString)) {
//        String::Utf8Value value(object->Get(keyString));
//        return ByteVector::Import(TagLib::String(*value, TagLib::String::UTF8));
//    } else {
//        return TagLib::ByteVector(); // empty byte vector
//    }
//}
//
//void TagLibWrapper::SetBytes(const char *key, const TagLib::ByteVector value, TagLib::String mimeType, std::map<uintptr_t, std::string> *fmap,) {
//    SetString(key, ByteVector::Export(value, mimeType));
//}

TagLib::String::Type TagLibWrapper::GetEncoding(const char *key) {
    Local<String> keyString = New<String>(key).ToLocalChecked();
    if (object->Has(keyString)) {
        String::Utf8Value value(object->Get(keyString));
        TagLib::String encodingString(*value, TagLib::String::UTF8);
        if (encodingString == "Latin1") return TagLib::String::Latin1;
        else if (encodingString == "UTF8") return TagLib::String::UTF8;
        else if (encodingString == "UTF16") return TagLib::String::UTF16;
        else if (encodingString == "UTF16BE") return TagLib::String::UTF16BE;
        else if (encodingString == "UTF16LE") return TagLib::String::UTF16LE;
        else return TagLib::String::UTF16;
    } else {
        return TagLib::String::UTF16;
    }
}

void TagLibWrapper::SetEncoding(const char *key, const TagLib::String::Type value) {
    string enc;
    switch (value) {
        case TagLib::String::Latin1:
            enc = "Latin1";
            break;
        case TagLib::String::UTF8:
            enc = "UTF8";
            break;
        case TagLib::String::UTF16:
            enc = "UTF16";
            break;
        case TagLib::String::UTF16BE:
            enc = "UTF16BE";
            break;
        case TagLib::String::UTF16LE:
            enc = "UTF16LE";
            break;
        default:
            enc = "UTF16";

    }
    object->Set(New<String>(key).ToLocalChecked(), New<String>(enc.c_str()).ToLocalChecked());
}

TagLib::ByteVector TagLibWrapper::GetLanguage(const char *key) {
    //TODO: Check valid ISO format
    //TODO: Find better transoform from ByteVector to char *
    Local<String> keyString = New<String>(key).ToLocalChecked();
    if (object->Has(keyString)) {
        String::Utf8Value value(object->Get(keyString));
        TagLib::String str(*value);
        return TagLib::ByteVector(str.toCString(), str.size());
    } else {
        return TagLib::ByteVector();
    }
}

void TagLibWrapper::SetLanguage(const char *key, const TagLib::ByteVector value) {
    //TODO: Check valid ISO format
    //TODO: Find better transform from ByteVector to char *
    TagLib::String str(value);
    object->Set(New<String>(key).ToLocalChecked(), New<String>(str.toCString(true)).ToLocalChecked());
}


//TagLib::List<TagLib::String> TagLibWrapper::GetStringArray(const char *key) {
//    Local<Array> array = Local<Array>::Cast(object->Get(key));
//    TagLib::List<TagLib::String> list;
//    for (unsigned int i = 0; i < array->Length(); i++) {
//        String::Utf8Value value(array->Get(i)->ToString());
//        TagLib::String string(*value, TagLib::String::UTF8);
//        list.append(string);
//    }
//    return list;
//}
//
//void TagLibWrapper::SetStringArray(const char *key, const TagLib::List<TagLib::String> list) {
//    Local<Array> array = Array::New(isolate, list.size());
//    for (unsigned int i = 0; i < list.size(); i++) {
//        TagLib::String string = list[i];
//        array->Set(i, String::NewFromUtf8(isolate, string.toCString(true)));
//    }
//    object->Set(key, array);
//}
