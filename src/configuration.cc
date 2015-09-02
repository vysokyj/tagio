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
    o.SetString("binaryDataDirectory", conf.binaryDataDirectory);
    o.SetString("binaryDataUrlPrefix", conf.binaryDataUrlPrefix);

    switch(conf.binaryDataMethod) {
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

    return handleScope.Escape(object);
}

void Configuration::Set(Isolate *isolate, Object *object) {
    Configuration &conf = Configuration::Get();

    Wrapper o(isolate, object);

    string s = o.GetString("binaryDataMethod").to8Bit(true);
    if (s.compare("FILENAME") == 0)      conf.binaryDataMethod = Configuration::BinaryDataMethod::FILENAME;
    if (s.compare("ABSOLUTE_URL") == 0)  conf.binaryDataMethod = Configuration::BinaryDataMethod::ABSOLUTE_URL;
    if (s.compare("PREFIXED_URL") == 0)  conf.binaryDataMethod = Configuration::BinaryDataMethod::PREFIXED_URL;

    conf.binaryDataDirectory = o.GetString("binaryDataDirectory").toCString(true);
    conf.binaryDataUrlPrefix = o.GetString("binaryDataUrlPrefix").toCString(true);
    conf.saveID3v1Tag = o.GetBoolean("saveID3v1Tag");
    conf.saveID3v2Tag = o.GetBoolean("saveID3v2Tag");
    conf.saveApeTag = o.GetBoolean("saveApeTag");


//    cout << "binaryDataMethod: " << conf.binaryDataMethod << endl;
//    cout << "binaryDataDirectory: " << conf.binaryDataDirectory << endl;
//    cout << "binaryDataUrlPrefix: " << conf.binaryDataUrlPrefix << endl;
//    cout << "saveID3v1Tag: " << conf.saveID3v1Tag << endl;
//    cout << "saveID3v2Tag: " << conf.saveID3v2Tag << endl;
//    cout << "saveApeTag: " << conf.saveApeTag << endl;

}

