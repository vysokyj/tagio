#include "tagio.h"

using namespace v8;
using namespace TagIO;

void InitAll(Handle<Object> exports) {
    GENERIC::Init(exports);
    MPEG::Init(exports);
    FLAC::Init(exports);
    ASYNC::Init(exports);
}

NODE_MODULE(tagio, InitAll)
