#include <string>
#include <chrono>
#include <nan.h>
#include <unordered_map>
#include <memory>

#include "civil_time.h"
#include "time_zone.h"
#include "timezone.h"
#include "civiltime.h"
#include "util.h"

static std::unordered_map<std::string, std::unique_ptr<Nan::Persistent<v8::Object>>> time_zone_map;

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
	if (info.Length() < 2) {
		Nan::ThrowTypeError("Expected 2 arguments: format, input");
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

	cctz::time_zone tz;

	if (info.Length() > 2 && !info[2]->IsUndefined()) {
		if (info[2]->IsObject() && Nan::New(TimeZone::prototype)->HasInstance(info[2]->ToObject())) {
			TimeZone* tzObj = Nan::ObjectWrap::Unwrap<TimeZone>(info[2]->ToObject());
			tz = tzObj->value;
		} else if (info[2]->IsString()) {
			cctz::load_time_zone(*Nan::Utf8String(info[2]), &tz);
		} else {
			Nan::ThrowTypeError("timezone must be a string or an instance of TimeZone");
			return;
		}
	} else {
		tz = cctz::utc_time_zone();
	}

	std::string format = *Nan::Utf8String(info[0]);
	std::string input = *Nan::Utf8String(info[1]);

	std::chrono::system_clock::time_point tp;
	bool parsed = cctz::parse(format, input, tz, &tp);

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

	cctz::time_zone tz;

	if (info[2]->IsObject() && Nan::New(TimeZone::prototype)->HasInstance(info[2]->ToObject())) {
		TimeZone* tzObj = Nan::ObjectWrap::Unwrap<TimeZone>(info[2]->ToObject());
		tz = tzObj->value;
	} else if (info[2]->IsString()) {
		cctz::load_time_zone(*Nan::Utf8String(info[2]), &tz);
	} else {
		Nan::ThrowTypeError("timezone must be a string or an instance of TimeZone");
		return;
	}

	if (info[1]->IsNumber()) {
		auto arg1 = info[1]->NumberValue();
		auto tp = toTimePoint(arg1);
		std::string str = cctz::format(format, tp, tz);
		info.GetReturnValue().Set(Nan::New(str).ToLocalChecked());
		return;
	}

	if (info[1]->IsObject() && Nan::New(CivilTime::prototype)->HasInstance(info[1]->ToObject())) {
		auto arg1 = info[1]->ToObject();
		CivilTime* cs = Nan::ObjectWrap::Unwrap<CivilTime>(arg1);

		std::string str = cctz::format(format, cctz::convert(cs->value, tz), tz);
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

	cctz::time_zone tz;

	if (info[1]->IsObject() && Nan::New(TimeZone::prototype)->HasInstance(info[1]->ToObject())) {
		TimeZone* tzObj = Nan::ObjectWrap::Unwrap<TimeZone>(info[1]->ToObject());
		tz = tzObj->value;
	} else if (info[1]->IsString()) {
		cctz::load_time_zone(*Nan::Utf8String(info[1]), &tz);
	} else {
		Nan::ThrowTypeError("timezone must be a string or an instance of TimeZone");
		return;
	}

	if (info[0]->IsNumber()) {
		auto tp = toTimePoint(info[0]->NumberValue());
		auto al = tz.lookup(tp);

		v8::Local<v8::Object> csObj = CivilTime::NewInstance();
		CivilTime* cs = Nan::ObjectWrap::Unwrap<CivilTime>(csObj);
		cs->value = al.cs;

		info.GetReturnValue().Set(csObj);
		return;
	}

	if (info[0]->IsObject() && Nan::New(CivilTime::prototype)->HasInstance(info[0]->ToObject())) {
		CivilTime* cs = Nan::ObjectWrap::Unwrap<CivilTime>(info[0]->ToObject());
		const cctz::time_zone::civil_lookup cl = tz.lookup(cs->value);

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
	NAN_EXPORT(target, parse);
	NAN_EXPORT(target, format);
	NAN_EXPORT(target, convert);
}

NODE_MODULE(node_cctz, Init)
