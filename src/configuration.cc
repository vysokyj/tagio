#include "configuration.h"
#include "wrapper.h"

using namespace TagIO;
using namespace v8;
using namespace std;

Local<Object> Configuration::New(Isolate *isolate) {
    EscapableHandleScope handleScope(isolate);
    Local<Object> object = Object::New(isolate);
    Configuration &conf = Configuration::Get();
    Wrapper o(isolate, *object);
    o.SetString("binaryDataMethod", BinaryDataMethodToString(conf.binaryDataMethod));
    o.SetString("binaryDataDirectory", conf.binaryDataDirectory);
    o.SetString("binaryDataUrlPrefix", conf.binaryDataUrlPrefix);
    o.SetBoolean("apeSave", conf.apeSave);
    o.SetBoolean("id3v1Save", conf.id3v1Save);
    o.SetEncoding("id3v1Encoding", conf.id3v1Encoding);
    o.SetBoolean("id3v2Save", conf.id3v2Save);
    o.SetUint32("id3v2Version", conf.id3v2Version);
    o.SetEncoding("id3v2Encoding", conf.id3v2Encoding);
    o.SetBoolean("id3v2UseFrameEncoding", conf.id3v2UseFrameEncoding);
    return handleScope.Escape(object);
}

void Configuration::Set(Isolate *isolate, Object *object) {
    Configuration &conf = Configuration::Get();
    Wrapper o(isolate, object);
    conf.binaryDataMethod = StringToBinaryDataMethod(o.GetString("binaryDataMethod"));
    conf.binaryDataDirectory = o.GetString("binaryDataDirectory").toCString(true);
    conf.binaryDataUrlPrefix = o.GetString("binaryDataUrlPrefix").toCString(true);
    conf.apeSave       = o.GetBoolean("apeSave");
    conf.id3v1Save     = o.GetBoolean("id3v1Save");
    conf.id3v1Encoding = o.GetEncoding("id3v1Encoding");
    conf.id3v2Save     = o.GetBoolean("id3v2Save");
    conf.id3v2Version  = o.GetUint32("id3v2Version");
    conf.id3v2Encoding = o.GetEncoding("id3v2Encoding");
    conf.id3v2UseFrameEncoding = o.GetBoolean("id3v2UseFrameEncoding");
}

inline int Configuration::StringToBinaryDataMethod(TagLib::String string) {
    std::string s = string.to8Bit(true);
    if (s.compare("IGNORE") == 0)
        return Configuration::BinaryDataMethodTypes::IGNORE;
    else if (s.compare("FILENAME") == 0)
        return Configuration::BinaryDataMethodTypes::FILENAME;
    else if (s.compare("ABSOLUTE_URL") == 0)
        return Configuration::BinaryDataMethodTypes::ABSOLUTE_URL;
    else if (s.compare("PREFIXED_URL") == 0)
        return Configuration::BinaryDataMethodTypes::PREFIXED_URL;
    else
        return Configuration::BinaryDataMethodTypes::IGNORE;
}

inline TagLib::String Configuration::BinaryDataMethodToString(int method) {
    switch(method) {
        case Configuration::BinaryDataMethodTypes::IGNORE:
            return "IGNORE";
        case Configuration::BinaryDataMethodTypes::FILENAME:
            return "FILENAME";
        case Configuration::BinaryDataMethodTypes::ABSOLUTE_URL:
            return "ABSOLUTE_URL";
        case Configuration::BinaryDataMethodTypes::PREFIXED_URL:
            return "PREFIXED_URL";
        default:
            return "IGNORE";
    }
}
