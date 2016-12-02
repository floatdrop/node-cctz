#include <string>
#include <chrono>
#include <nan.h>
#include <unordered_map>

#include "civil_time.h"
#include "time_zone.h"
#include "timezone.h"
#include "civiltime.h"
#include "util.h"

static std::unordered_map<std::string, std::unique_ptr<Nan::Persistent<v8::Object>>> time_zone_map;

NAN_METHOD(utc_time_zone) {
	// TODO: use cctz::utc_time_zone
	v8::Local<v8::Object> result = TimeZone::NewInstance(Nan::New("UTC").ToLocalChecked());
	info.GetReturnValue().Set(result);
}

NAN_METHOD(load_time_zone) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError("Expected timezone name as first argument");
		return;
	}

	if (!info[0]->IsString()) {
		Nan::ThrowTypeError("timezone must be a String");
		return;
	}

	std::string name = *Nan::Utf8String(info[0]);

	auto itr = time_zone_map.find(name);
	if (itr != time_zone_map.end()) {
		info.GetReturnValue().Set(Nan::New(*(itr->second)));
		return;
	}

	std::unique_ptr<Nan::Persistent<v8::Object>> persist(new Nan::Persistent<v8::Object>(TimeZone::NewInstance(info[0])));
	info.GetReturnValue().Set(Nan::New(*persist));
	time_zone_map[name] = std::move(persist);
}

NAN_METHOD(parse) {
	if (info.Length() < 3) {
		Nan::ThrowTypeError("Expected 3 arguments: format, input, timezone");
		return;
	}

	if (!info[0]->IsString()) {
		Nan::ThrowTypeError("format must be a string");
		return;
	}

	if (!info[1]->IsString()) {
		Nan::ThrowTypeError("input must be a string");
		return;
	}

	auto arg2 = info[2]->ToObject();
	if (!Nan::New(TimeZone::prototype)->HasInstance(arg2)) {
		Nan::ThrowTypeError("timezone must be an instance of TimeZone");
		return;
	}

	std::string format = *Nan::Utf8String(info[0]);
	std::string input = *Nan::Utf8String(info[1]);

	TimeZone* tz = Nan::ObjectWrap::Unwrap<TimeZone>(arg2);
	std::chrono::system_clock::time_point tp;
	bool parsed = cctz::parse(format, input, tz->value, &tp);

	if (!parsed) {
		return;
	}

	info.GetReturnValue().Set(Nan::New(toUnixTimestamp(tp)));
}

NAN_METHOD(format) {
	if (info.Length() < 3) {
		Nan::ThrowTypeError("Expected 3 arguments: format, unix timestamp or CilivTime, timezone");
		return;
	}

	if (!info[0]->IsString()) {
		Nan::ThrowTypeError("format must be a string");
		return;
	}
	std::string format = *Nan::Utf8String(info[0]);

	auto arg2 = info[2]->ToObject();
	if (!Nan::New(TimeZone::prototype)->HasInstance(arg2)) {
		Nan::ThrowTypeError("timezone must be an instance of TimeZone");
		return;
	}
	TimeZone* tz = Nan::ObjectWrap::Unwrap<TimeZone>(arg2);

	if (info[1]->IsNumber()) {
		auto arg1 = info[1]->NumberValue();
		auto tp = toTimePoint(arg1);
		std::string str = cctz::format(format, tp, tz->value);
		info.GetReturnValue().Set(Nan::New(str).ToLocalChecked());
		return;
	}

	if (info[1]->IsObject() && Nan::New(CivilTime::prototype)->HasInstance(info[1]->ToObject())) {
		auto arg1 = info[1]->ToObject();
		CivilTime* cs = Nan::ObjectWrap::Unwrap<CivilTime>(arg1);

		std::string str = cctz::format(format, cctz::convert(cs->value, tz->value), tz->value);
		info.GetReturnValue().Set(Nan::New(str).ToLocalChecked());
		return;
	}

	Nan::ThrowTypeError("second argument must be unix timestamp or CivilTime");
}

NAN_METHOD(convert) {
	if (info.Length() < 2) {
		Nan::ThrowTypeError("Expected 2 arguments: unix timestamp or civilseconds, timezone");
		return;
	}

	auto arg1 = info[1]->ToObject();
	if (!Nan::New(TimeZone::prototype)->HasInstance(arg1)) {
		Nan::ThrowTypeError("timezone must be an instance of TimeZone");
		return;
	}

	TimeZone* tz = Nan::ObjectWrap::Unwrap<TimeZone>(arg1);

	if (info[0]->IsNumber()) {
		auto tp = toTimePoint(info[0]->NumberValue());
		auto al = tz->value.lookup(tp);

		v8::Local<v8::Object> csObj = CivilTime::NewInstance();
		CivilTime* cs = Nan::ObjectWrap::Unwrap<CivilTime>(csObj);
		cs->value = al.cs;

		info.GetReturnValue().Set(csObj);
		return;
	}

	auto arg0 = info[0]->ToObject();
	if (Nan::New(CivilTime::prototype)->HasInstance(arg0)) {
		CivilTime* cs = Nan::ObjectWrap::Unwrap<CivilTime>(arg0);
		const cctz::time_zone::civil_lookup cl = tz->value.lookup(cs->value);

		if (cl.kind == cctz::time_zone::civil_lookup::SKIPPED) {
			info.GetReturnValue().Set(toUnixTimestamp(cl.trans));
		} else {
			info.GetReturnValue().Set(toUnixTimestamp(cl.pre));
		}

		return;
	}

	Nan::ThrowTypeError("first argument should be unix timestamp or CivilTime");
}

NAN_MODULE_INIT(Init) {
	TimeZone::Init(target);
	CivilTime::Init(target);

	NAN_EXPORT(target, load_time_zone);
	NAN_EXPORT(target, utc_time_zone);
	NAN_EXPORT(target, parse);
	NAN_EXPORT(target, format);
	NAN_EXPORT(target, convert);
}

NODE_MODULE(node_cctz, Init)
