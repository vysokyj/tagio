#include "tagio.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
    TagIO::MPEG::Init(exports);
}

NODE_MODULE(tagio, InitAll)