#include <node.h>
#include <chrono>
#include "timezone.h"
#include "civiltime.h"
#include "util.h"

TimeZone::TimeZone() {};
TimeZone::~TimeZone() {};

Nan::Persistent<v8::Function> TimeZone::constructor;
Nan::Persistent<v8::FunctionTemplate> TimeZone::prototype;

void TimeZone::Init(v8::Local<v8::Object> target) {
	Nan::HandleScope scope;
	auto name = Nan::New("TimeZone").ToLocalChecked();

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(name);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	Nan::SetPrototypeMethod(tpl, "lookup", Lookup);

	prototype.Reset(tpl);
	constructor.Reset(tpl->GetFunction());

	target->Set(name, tpl->GetFunction());
}

NAN_METHOD(TimeZone::Lookup) {
	TimeZone* tz = ObjectWrap::Unwrap<TimeZone>(info.Holder());

	if (info.Length() < 1) {
		Nan::ThrowTypeError("One argument required: unix timestamp or CivilTime");
		return;
	}

	if (info[0]->IsNumber()) {
		double unix = info[0]->NumberValue();

		auto tp = toTimePoint(unix);
		auto lookup = tz->value.lookup(tp);

		// TODO: Move this to AbsoluteLookup class
		v8::Local<v8::Object> obj = Nan::New<v8::Object>();
		obj->Set(Nan::New("offset").ToLocalChecked(), Nan::New<v8::Number>(lookup.offset));
		obj->Set(Nan::New("is_dst").ToLocalChecked(), Nan::New<v8::Boolean>(lookup.is_dst));
		obj->Set(Nan::New("abbr").ToLocalChecked(), Nan::New<v8::String>(lookup.abbr).ToLocalChecked());

		v8::Local<v8::Object> csObj = CivilTime::NewInstance();
		CivilTime* cs = Nan::ObjectWrap::Unwrap<CivilTime>(csObj);
		cs->value = lookup.cs;

		obj->Set(Nan::New("cs").ToLocalChecked(), csObj);

		info.GetReturnValue().Set(obj);
		return;
	}

	auto arg0 = info[0]->ToObject();

	if (Nan::New(CivilTime::prototype)->HasInstance(arg0)) {
		CivilTime* cs = Nan::ObjectWrap::Unwrap<CivilTime>(arg0);
		auto lookup = tz->value.lookup(cs->value);

		// TODO: Move this to CivilLookup class
		v8::Local<v8::Object> obj = Nan::New<v8::Object>();
		if (lookup.kind == cctz::time_zone::civil_lookup::UNIQUE) {
			obj->Set(Nan::New("kind").ToLocalChecked(), Nan::New("UNIQUE").ToLocalChecked());
		} else if (lookup.kind == cctz::time_zone::civil_lookup::SKIPPED) {
			obj->Set(Nan::New("kind").ToLocalChecked(), Nan::New("SKIPPED").ToLocalChecked());
		} else {
			obj->Set(Nan::New("kind").ToLocalChecked(), Nan::New("REPEATED").ToLocalChecked());
		}

		obj->Set(Nan::New("pre").ToLocalChecked(), Nan::New(toUnixTimestamp(lookup.pre)));
		obj->Set(Nan::New("trans").ToLocalChecked(), Nan::New(toUnixTimestamp(lookup.trans)));
		obj->Set(Nan::New("post").ToLocalChecked(), Nan::New(toUnixTimestamp(lookup.post)));

		info.GetReturnValue().Set(obj);
		return;
	}

	Nan::ThrowTypeError("Expected unix timestamp or CivilTime as argument");
}

NAN_METHOD(TimeZone::New) {
	if (!info.IsConstructCall()) {
		return Nan::ThrowTypeError("TimeZone constructor cannot be invoked without 'new'");
	}

	TimeZone* obj = new TimeZone();

	if (!cctz::load_time_zone(*Nan::Utf8String(info[0]), &(obj->value))) {
		Nan::ThrowError("Failed to load time zone");
		return;
	}

	obj->Wrap(info.This());
	info.This()->Set(Nan::New("name").ToLocalChecked(), info[0]);
	info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> TimeZone::NewInstance() {
	v8::Local<v8::Function> cons = Nan::New(constructor);
	return Nan::NewInstance(cons).ToLocalChecked();
}

v8::Local<v8::Object> TimeZone::NewInstance(v8::Local<v8::Value> arg) {
	const unsigned argc = 1;
	v8::Local<v8::Value> argv[argc] = { arg };
	v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);

	return Nan::NewInstance(cons, argc, argv).ToLocalChecked();
}
