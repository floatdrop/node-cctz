#include <node.h>
#include "timezone.h"
#include "timepoint.h"
#include "civiltime.h"

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
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("name").ToLocalChecked(), GetName);

	prototype.Reset(tpl);
	constructor.Reset(tpl->GetFunction());

	target->Set(name, tpl->GetFunction());
}

NAN_METHOD(TimeZone::Lookup) {
	TimeZone* tz = ObjectWrap::Unwrap<TimeZone>(info.Holder());

	if (info.Length() < 1 || !info[0]->IsObject()) {
		Nan::ThrowTypeError("One argument required: TimePoint or CivilTime");
		return;
	}

	auto arg0 = info[0]->ToObject();

	if (Nan::New(CivilSecond::prototype)->HasInstance(arg0)) {
		CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(arg0);
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

		v8::Local<v8::Object> preObj = TimePoint::NewInstance();
		TimePoint* pre = Nan::ObjectWrap::Unwrap<TimePoint>(preObj);
		pre->value = lookup.pre;
		obj->Set(Nan::New("pre").ToLocalChecked(), preObj);

		v8::Local<v8::Object> transObj = TimePoint::NewInstance();
		TimePoint* trans = Nan::ObjectWrap::Unwrap<TimePoint>(transObj);
		trans->value = lookup.trans;
		obj->Set(Nan::New("trans").ToLocalChecked(), transObj);

		v8::Local<v8::Object> postObj = TimePoint::NewInstance();
		TimePoint* post = Nan::ObjectWrap::Unwrap<TimePoint>(postObj);
		post->value = lookup.post;
		obj->Set(Nan::New("post").ToLocalChecked(), postObj);

		info.GetReturnValue().Set(obj);
		return;
	}

	if (Nan::New(TimePoint::prototype)->HasInstance(arg0)) {
		TimePoint* tp = Nan::ObjectWrap::Unwrap<TimePoint>(arg0);
		auto lookup = tz->value.lookup(tp->value);

		// TODO: Move this to AbsoluteLookup class
		v8::Local<v8::Object> obj = Nan::New<v8::Object>();
		obj->Set(Nan::New("offset").ToLocalChecked(), Nan::New<v8::Number>(lookup.offset));
		obj->Set(Nan::New("is_dst").ToLocalChecked(), Nan::New<v8::Boolean>(lookup.is_dst));
		obj->Set(Nan::New("abbr").ToLocalChecked(), Nan::New<v8::String>(lookup.abbr).ToLocalChecked());

		v8::Local<v8::Object> csObj = CivilSecond::NewInstance();
		CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(csObj);
		cs->value = lookup.cs;

		obj->Set(Nan::New("cs").ToLocalChecked(), csObj);

		info.GetReturnValue().Set(obj);
		return;
	}

	Nan::ThrowTypeError("Expected TimePoint or CivilTime as argument");
}

NAN_GETTER(TimeZone::GetName) {
	TimeZone* cs = Nan::ObjectWrap::Unwrap<TimeZone>(info.This());
	info.GetReturnValue().Set(Nan::New<v8::String>(cs->name.c_str()).ToLocalChecked());
}

NAN_METHOD(TimeZone::New) {
	if (!info.IsConstructCall()) {
		return Nan::ThrowTypeError("TimeZone constructor cannot be invoked without 'new'");
	}

	TimeZone* obj = new TimeZone();
	obj->name = *Nan::Utf8String(info[0]);

	if (!cctz::load_time_zone(obj->name, &(obj->value))) {
		Nan::ThrowError("Failed to load time zone");
		return;
	}

	obj->Wrap(info.This());
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
