//
// Created by jirka on 21.7.15.
//

#include <taglib/tstring.h>
#include "base.h"

using namespace std;
using namespace TagIO;
using namespace v8;


Base::Base(string path) : path(FixPath(path)) { }

Base::~Base() {}


TagLib::String Base::ExportFile(TagLib::ByteVector byteVector, TagLib::String mimeType) {
    string fileName = NewFileName(byteVector, mimeType.to8Bit(true));
    string filePath = NewPath(attachmentsDir, fileName);
    ofstream ofs;
    ofs.open(filePath, ios::out | ios::binary);
    ofs.write(byteVector.data(), byteVector.size());
    ofs.close();
    string retval = attachmentsCtx + fileName;
    return TagLib::String(retval, TagLib::String::UTF8);
}

TagLib::ByteVector Base::ImportFile(TagLib::String path) {
    string fileName = path.to8Bit(true).substr(attachmentsCtx.length());
    string filePath = NewPath(attachmentsDir, fileName);
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

TagLib::String Base::GetStringProperty(Isolate *isolate, Object *object, const char *key) {
    String::Utf8Value value(object->Get(String::NewFromUtf8(isolate, key))->ToString());
    return TagLib::String(*value, TagLib::String::UTF8);
}

void Base::SetStringProperty(Isolate *isolate, Object *object, const char *key, TagLib::String value) {
    object->Set(String::NewFromUtf8(isolate, key), String::NewFromUtf8(isolate, value.toCString(true)));
}

//----------------------------------------------------------------------------------------------------------------------
// Private methods

string Base::FixPath(string path) {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    replace(path.begin(), path.end(), '/', '\\');
    #else
    replace(path.begin(), path.end(), '\\', '/');
    #endif
    return path;
}

string Base::FixContext(string context) {
    if (context.length() == 0) return context;
    else if (context.back() == '/') return context;
    else return context + '/';
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
