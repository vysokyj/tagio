
#include "base.h"

using namespace TagIO;
using namespace std;
using namespace v8;

Base::Base(const char *path) : path(FixPath(path)) { }

Base::~Base() {}

void Base::GetBaseConfiguration(Isolate *isolate, Object *object, Base *base) {
    // prepare keys
    Local<String> binaryDataDirectoryKey = (String::NewFromUtf8(isolate, "binaryDataDirectory"))->ToString();
    Local<String> binaryDataUrlPrefixKey = (String::NewFromUtf8(isolate, "binaryDataUrlPrefix"))->ToString();
    Local<String> binaryDataMethodKey = (String::NewFromUtf8(isolate, "binaryDataMethod"))->ToString();

    if (object->Has(binaryDataDirectoryKey)) {
        String::Utf8Value val(object->Get(binaryDataDirectoryKey)->ToString());
        base->binaryDataDirectory = *val;
    }

    if (object->Has(binaryDataUrlPrefixKey)) {
        String::Utf8Value val(object->Get(binaryDataUrlPrefixKey)->ToString());
        base->binaryDataUrlPrefix = *val;
    }

    if (object->Has(binaryDataMethodKey)) {
        String::Utf8Value val(object->Get(binaryDataMethodKey)->ToString());
        string s(*val);
        if (s.compare("FILENAME") == 0)      base->binaryDataMethod = Base::BinaryDataMethod::FILENAME;
        if (s.compare("ABSOLUTE_URL") == 0)  base->binaryDataMethod = Base::BinaryDataMethod::ABSOLUTE_URL;
        if (s.compare("PREFIXED_URL") == 0)  base->binaryDataMethod = Base::BinaryDataMethod::PREFIXED_URL;
    }
}

void Base::SetBaseConfiguration(Isolate *isolate, Object *object, Base *base) {
    SetString(isolate, object, "binaryDataDirectory", base->binaryDataDirectory);
    SetString(isolate, object, "binaryDataUrlPrefix", base->binaryDataUrlPrefix);

    switch(base->binaryDataMethod) {
        case BinaryDataMethod::FILENAME:
            SetString(isolate, object, "binaryDataMethod", "FILENAME");
            break;
        case BinaryDataMethod::ABSOLUTE_URL:
            SetString(isolate, object, "binaryDataMethod", "ABSOLUTE_URL");
            break;
        case BinaryDataMethod::PREFIXED_URL:
            SetString(isolate, object, "binaryDataMethod", "PREFIXED_URL");
            break;
    }

}

TagLib::String Base::ExportFile(TagLib::ByteVector byteVector, TagLib::String mimeType) {
    string fileName = NewFileName(byteVector, mimeType.to8Bit(true));
    string filePath = NewPath(binaryDataDirectory, fileName);
    ofstream ofs;
    ofs.open(filePath, ios::out | ios::binary);
    ofs.write(byteVector.data(), byteVector.size());
    ofs.close();
    string retval = NewRelativeUrl(binaryDataUrlPrefix, fileName);
    return TagLib::String(retval, TagLib::String::UTF8);
}

TagLib::ByteVector Base::ImportFile(TagLib::String path) {
    string fileName = path.to8Bit(true).substr(binaryDataUrlPrefix.length());
    string filePath = NewPath(binaryDataUrlPrefix, fileName);
    ifstream ifs;
    ifs.open(filePath, ios::in | ios::binary);
    ifs.seekg(0, ios::end);
    long length = ifs.tellg();
    char *data = new char[length];
    ifs.seekg(0, ios::beg);
    ifs.read(data, length);
    ifs.close();
    return TagLib::ByteVector(data, (uint) length);
}

bool Base::GetBoolean(Isolate *isolate, Object *object, const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        return object->Get(String::NewFromUtf8(isolate, key))->BooleanValue();
    } else {
        return false;
    }
}

void Base::SetBoolean(Isolate *isolate, Object *object, const char *key, bool value) {
    object->Set(String::NewFromUtf8(isolate, key), Boolean::New(isolate, value));
}

TagLib::uint Base::GetUint32(Isolate *isolate, Object *object, const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        return (uint) (object->Get(String::NewFromUtf8(isolate, key)))->Uint32Value();
    } else {
        return 0;
    }
}

void Base::SetUint32(Isolate *isolate, Object *object, const char *key, const TagLib::uint value) {
    object->Set(String::NewFromUtf8(isolate, key), Integer::New(isolate, value));
}

TagLib::String Base::GetString(Isolate *isolate, Object *object, const char *key) {
    Local<String> keyString = (String::NewFromUtf8(isolate, key))->ToString();
    if (object->Has(keyString)) {
        String::Utf8Value value(object->Get(keyString));
        return TagLib::String(*value, TagLib::String::UTF8);
    } else {
        return TagLib::String("", TagLib::String::UTF8);
    }
}

void Base::SetString(Isolate *isolate, Object *object, const char *key, TagLib::String value) {
    object->Set(String::NewFromUtf8(isolate, key), String::NewFromUtf8(isolate, value.toCString(true)));
}

void Base::SetObjectByTag(Isolate *isolate, Object *object, TagLib::Tag *tag) {
    SetString(isolate, object, "title", tag->title());
    SetString(isolate, object, "album", tag->album());
    SetString(isolate, object, "artist", tag->artist());
    SetUint32(isolate, object, "track", tag->track());
    SetUint32(isolate, object, "year", tag->year());
    SetString(isolate, object, "genre", tag->genre());
    SetString(isolate, object, "comment", tag->comment());
}

void Base::SetTagByObject(Isolate *isolate, Object *object, TagLib::Tag *tag) {
    tag->setTitle(GetString(isolate, object, "title"));
    tag->setAlbum(GetString(isolate, object, "album"));
    tag->setArtist(GetString(isolate, object, "artist"));
    tag->setTrack(GetUint32(isolate, object, "track"));
    tag->setYear(GetUint32(isolate, object, "year"));
    tag->setGenre(GetString(isolate, object, "genre"));
    tag->setComment(GetString(isolate, object, "comment"));
}

//----------------------------------------------------------------------------------------------------------------------
// Private methods


const char *Base::FixPath(const char *path) {
    return FixPath(string(path)).c_str();
}

string Base::FixPath(string path) {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    replace(path.begin(), path.end(), '/', '\\');
    #else
    replace(path.begin(), path.end(), '\\', '/');
    #endif
    return path;
}

const char *Base::FixContext(const char *context) {
    return FixContext(string(context)).c_str();
}

string Base::FixContext(string context) {
    if (context.length() == 0) return context;
    if (context.back() == '/') context = context.substr(0, context.length() - 1);
    if (context.front() != '/') context = '/' + context;
    return context;
}

string Base::NewPath(string directoryPath, string fileName) {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    replace(directoryPath.begin(), directoryPath.end(), '/', '\\');
    return directoryPath + '\\' + fileName;
    #else
    replace(directoryPath.begin(), directoryPath.end(), '\\', '/');
    return directoryPath + '/' + fileName;
    #endif
}

string Base::NewRelativeUrl(string relativeUrl, string fileName) {
    return relativeUrl + '/' + fileName;
}


string Base::NewFileName(TagLib::ByteVector byteVector, string mimeType) {
    string hash = CountMD5(byteVector);
    string ext = (mimeType.empty()) ? "bin" : mimeType.substr(mimeType.find("/") + 1);
    //TODO: Implement complete mime type table...
    if (mimeType.compare("image/jpeg")) ext = "jpg";
    else if (mimeType.compare("image/pjpeg")) ext = "jpg";
    else if (mimeType.compare("image/png")) ext = "png";
    else if (mimeType.compare("text/plain")) ext = "txt";
    return hash + '.' + ext;
}

string Base::CountMD5(TagLib::ByteVector byteVector) {
    string s = string(byteVector.data(), byteVector.size());
    string hash = md5(s);
    return hash;
}
