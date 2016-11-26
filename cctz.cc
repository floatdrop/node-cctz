#include <string>
#include <chrono>

#include "civil_time.h"
#include "time_zone.h"

#include <nan.h>

// void CivilTime(const Nan::FunctionCallbackInfo<v8::Value>& info) {
// }

// void TimeZone(const Nan::FunctionCallbackInfo<v8::Value>& info) {
// }

void Init(v8::Local<v8::Object> exports) {
    // exports->Set(Nan::New("civil_time").ToLocalChecked(),
    //              Nan::New<v8::FunctionTemplate>(CivilTime)->GetFunction());
    // exports->Set(Nan::New("time_zone").ToLocalChecked(),
    //              Nan::New<v8::FunctionTemplate>(TimeZone)->GetFunction());
}

NODE_MODULE(cctz, Init)
