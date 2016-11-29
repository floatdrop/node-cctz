#ifndef TIMEPOINT_H
#define TIMEPOINT_H

#include <nan.h>
#include <chrono>

class TimePoint : public Nan::ObjectWrap {
public:
	static void Init(v8::Local<v8::Object> target);
	static v8::Local<v8::Object> NewInstance();
	static Nan::Persistent<v8::FunctionTemplate> prototype;

	std::chrono::system_clock::time_point value;

private:
	explicit TimePoint();
	~TimePoint();

	static Nan::Persistent<v8::Function> constructor;
	static NAN_METHOD(New);

	static NAN_GETTER(GetUnixTimestamp);
};

#endif
