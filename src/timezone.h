#ifndef TIMEZONE_H
#define TIMEZONE_H

#include <nan.h>
#include "time_zone.h"

class TimeZone : public Nan::ObjectWrap {
public:
	static void Init();
	static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
	cctz::time_zone value;

private:
	explicit TimeZone();
	~TimeZone();

	static Nan::Persistent<v8::Function> constructor;
	static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
};

#endif
