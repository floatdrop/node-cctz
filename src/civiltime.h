#ifndef CIVILTIME_H
#define CIVILTIME_H

#include <nan.h>
#include <chrono>
#include "civil_time.h"

class CivilTime : public Nan::ObjectWrap {
public:
	static void Init(v8::Local<v8::Object> target);
	static v8::Local<v8::Object> NewInstance();
	static Nan::Persistent<v8::FunctionTemplate> prototype;

	cctz::civil_second value;

private:
	explicit CivilTime();
	~CivilTime();

	static Nan::Persistent<v8::Function> constructor;
	static NAN_METHOD(New);
	static NAN_METHOD(Clone);

	static NAN_GETTER(GetYear);
	static NAN_SETTER(SetYear);

	static NAN_GETTER(GetMonth);
	static NAN_SETTER(SetMonth);

	static NAN_GETTER(GetDay);
	static NAN_SETTER(SetDay);

	static NAN_GETTER(GetHour);
	static NAN_SETTER(SetHour);

	static NAN_GETTER(GetMinute);
	static NAN_SETTER(SetMinute);

	static NAN_GETTER(GetSecond);
	static NAN_SETTER(SetSecond);

	static NAN_GETTER(GetWeekday);
	// TODO: prev_weekday
	// TODO: next_weekday

	static NAN_GETTER(GetYearday);

	// These methods 2x faster, that reassigning fields
	// TODO: Other StartOf* and EndOf* methods
	static NAN_METHOD(StartOfYear);
	static NAN_METHOD(StartOfMonth);
	static NAN_METHOD(StartOfDay);
	static NAN_METHOD(StartOfHour);
};

#endif
