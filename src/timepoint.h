#ifndef TIMEPOINT_H
#define TIMEPOINT_H

#include <nan.h>
#include <chrono>

class TimePoint : public Nan::ObjectWrap {
public:
	static void Init();
	static v8::Local<v8::Object> NewInstance();
	std::chrono::system_clock::time_point value;

private:
	explicit TimePoint();
	~TimePoint();

	static Nan::Persistent<v8::Function> constructor;
	static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
};

#endif
