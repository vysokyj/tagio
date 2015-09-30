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
    o.SetBoolean("id3v2Save", conf.id3v2Save);
    o.SetUint32("id3v2Version", conf.id3v2Version);
    o.SetEncoding("id3v2Encoding", conf.id3v2Encoding);
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
    conf.id3v2Save     = o.GetBoolean("id3v2Save");
    conf.id3v2Version  = o.GetUint32("id3v2Version");
    conf.id3v2Encoding = o.GetEncoding("id3v2Encoding");
}

inline Configuration::BinaryDataMethodEnum Configuration::StringToBinaryDataMethod(TagLib::String string) {
    Configuration &conf = Configuration::Get();
    std::string s = string.to8Bit(true);
    if (s.compare("IGNORE") == 0)
        return Configuration::BinaryDataMethodEnum::IGNORE;
    if (s.compare("FILENAME") == 0)
        return Configuration::BinaryDataMethodEnum::FILENAME;
    if (s.compare("ABSOLUTE_URL") == 0)
        return Configuration::BinaryDataMethodEnum::ABSOLUTE_URL;
    if (s.compare("PREFIXED_URL") == 0)
        return Configuration::BinaryDataMethodEnum::PREFIXED_URL;
}

inline TagLib::String Configuration::BinaryDataMethodToString(Configuration::BinaryDataMethodEnum method) {
    switch(method) {
        case BinaryDataMethodEnum::IGNORE:
            return "IGNORE";
        case BinaryDataMethodEnum::FILENAME:
            return "FILENAME";
        case BinaryDataMethodEnum::ABSOLUTE_URL:
            return "ABSOLUTE_URL";
        case BinaryDataMethodEnum::PREFIXED_URL:
            return "PREFIXED_URL";
    }
}
