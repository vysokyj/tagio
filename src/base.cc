#include "base.h"
#include "audioproperties.h"

using namespace TagIO;
using namespace std;
using namespace v8;

Base::Base(const char *path) : path(path) { }

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
    Wrapper o(isolate, object);
    o.SetString("binaryDataDirectory", base->binaryDataDirectory);
    o.SetString("binaryDataUrlPrefix", base->binaryDataUrlPrefix);

    switch(base->binaryDataMethod) {
        case BinaryDataMethod::FILENAME:
            o.SetString("binaryDataMethod", "FILENAME");
            break;
        case BinaryDataMethod::ABSOLUTE_URL:
            o.SetString("binaryDataMethod", "ABSOLUTE_URL");
            break;
        case BinaryDataMethod::PREFIXED_URL:
            o.SetString("binaryDataMethod", "PREFIXED_URL");
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


//----------------------------------------------------------------------------------------------------------------------
// Private methods


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
