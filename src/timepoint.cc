#include <node.h>
#include "timepoint.h"

TimePoint::TimePoint() {};
TimePoint::~TimePoint() {};

Nan::Persistent<v8::Function> TimePoint::constructor;
Nan::Persistent<v8::FunctionTemplate> TimePoint::prototype;

void TimePoint::Init(v8::Local<v8::Object> target) {
	Nan::HandleScope scope;
	auto name = Nan::New("TimePoint").ToLocalChecked();

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(name);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("unix").ToLocalChecked(), GetUnixTimestamp);

	prototype.Reset(tpl);
	constructor.Reset(tpl->GetFunction());

	target->Set(name, tpl->GetFunction());
}

NAN_GETTER(TimePoint::GetUnixTimestamp) {
	TimePoint* tp = Nan::ObjectWrap::Unwrap<TimePoint>(info.This());
	auto duration = tp->value.time_since_epoch();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	info.GetReturnValue().Set(Nan::New<v8::Number>(millis));
}

NAN_METHOD(TimePoint::New) {
	TimePoint* obj = new TimePoint();

	if (info.Length() > 0 && info[0]->IsNumber()) {
		auto unix_timestamp = info[0]->NumberValue();
		obj->value = std::chrono::system_clock::time_point{std::chrono::milliseconds{(long long) unix_timestamp}};
	} else {
		obj->value = std::chrono::system_clock::now();
	}

	obj->Wrap(info.This());
	info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> TimePoint::NewInstance() {
	v8::Local<v8::Function> cons = Nan::New(constructor);
	return Nan::NewInstance(cons).ToLocalChecked();
}
