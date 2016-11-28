#include <node.h>
#include "timepoint.h"

TimePoint::TimePoint() {};
TimePoint::~TimePoint() {};

Nan::Persistent<v8::Function> TimePoint::constructor;

void TimePoint::Init() {
	Nan::HandleScope scope;

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("TimePoint").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	constructor.Reset(tpl->GetFunction());
}

void TimePoint::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	TimePoint* obj = new TimePoint();
	obj->Wrap(info.This());
	info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> TimePoint::NewInstance() {
	v8::Local<v8::Function> cons = Nan::New(constructor);
	return Nan::NewInstance(cons).ToLocalChecked();
}
