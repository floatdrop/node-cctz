#include <node.h>
#include "timezone.h"

TimeZone::TimeZone() {};
TimeZone::~TimeZone() {};

Nan::Persistent<v8::Function> TimeZone::constructor;

void TimeZone::Init() {
	Nan::HandleScope scope;

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("TimeZone").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	constructor.Reset(tpl->GetFunction());
}

void TimeZone::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	TimeZone* obj = new TimeZone();
	std::string name = *Nan::Utf8String(info[0]);

	if (!cctz::load_time_zone(name, &(obj->value))) {
		Nan::ThrowError("Failed to load time zone");
		return;
	}

	obj->Wrap(info.This());

	info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> TimeZone::NewInstance(v8::Local<v8::Value> arg) {
	const unsigned argc = 1;
	v8::Local<v8::Value> argv[argc] = { arg };
	v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);

	return Nan::NewInstance(cons, argc, argv).ToLocalChecked();
}
