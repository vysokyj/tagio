#ifndef TAGIO_ASYNC_H
#define TAGIO_ASYNC_H

#include <node.h>


void TagReadAsync(const v8::FunctionCallbackInfo<v8::Value> &args);
void TagInit(v8::Handle <v8::Object> exports);

#endif //TAGIO_ASYNC_H
