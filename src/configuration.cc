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
        return BDM_IGNORE;
    else if (s.compare("FILENAME") == 0)
        return BDM_FILENAME;
    else if (s.compare("ABSOLUTE_URL") == 0)
        return BDM_ABSOLUTE_URL;
    else if (s.compare("RELATIVE_URL") == 0)
        return BDM_RELATIVE_URL;
    else
        return BDM_IGNORE;
}

inline TagLib::String Configuration::BinaryDataMethodToString(int method) {
    switch(method) {
        case BDM_IGNORE:
            return "IGNORE";
        case BDM_FILENAME:
            return "FILENAME";
        case BDM_ABSOLUTE_URL:
            return "ABSOLUTE_URL";
        case BDM_RELATIVE_URL:
            return "RELATIVE_URL";
        default:
            return "IGNORE";
    }
}
