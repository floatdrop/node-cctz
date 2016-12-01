#include <node.h>
#include "civiltime.h"

CivilSecond::CivilSecond() {};
CivilSecond::~CivilSecond() {};

Nan::Persistent<v8::Function> CivilSecond::constructor;
Nan::Persistent<v8::FunctionTemplate> CivilSecond::prototype;

void CivilSecond::Init(v8::Local<v8::Object> target) {
	Nan::HandleScope scope;
	auto name = Nan::New("CivilSecond").ToLocalChecked();

	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(name);
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("year").ToLocalChecked(), GetYear, SetYear);
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("month").ToLocalChecked(), GetMonth, SetMonth);
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("day").ToLocalChecked(), GetDay, SetDay);
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("hour").ToLocalChecked(), GetHour, SetHour);
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("minute").ToLocalChecked(), GetMinute, SetMinute);
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("second").ToLocalChecked(), GetSecond, SetSecond);
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("weekday").ToLocalChecked(), GetWeekday);
	Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("yearday").ToLocalChecked(), GetYearday);

	prototype.Reset(tpl);
	constructor.Reset(tpl->GetFunction());

	target->Set(name, tpl->GetFunction());
}

NAN_GETTER(CivilSecond::GetYear) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	info.GetReturnValue().Set(Nan::New<v8::Number>(cs->value.year()));
}

NAN_SETTER(CivilSecond::SetYear) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	auto& d = cs->value;
	cs->value = cctz::civil_second(value->ToInteger()->Value(), d.month(), d.day(), d.hour(), d.minute(), d.second());
}

NAN_GETTER(CivilSecond::GetMonth) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	info.GetReturnValue().Set(Nan::New<v8::Number>(cs->value.month()));
}

NAN_SETTER(CivilSecond::SetMonth) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	auto& d = cs->value;
	cs->value = cctz::civil_second(d.year(), value->ToInteger()->Value(), d.day(), d.hour(), d.minute(), d.second());
}

NAN_GETTER(CivilSecond::GetDay) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	info.GetReturnValue().Set(Nan::New<v8::Number>(cs->value.day()));
}

NAN_SETTER(CivilSecond::SetDay) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	auto& d = cs->value;
	cs->value = cctz::civil_second(d.year(), d.month(), value->ToInteger()->Value(), d.hour(), d.minute(), d.second());
}

NAN_GETTER(CivilSecond::GetHour) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	info.GetReturnValue().Set(Nan::New<v8::Number>(cs->value.hour()));
}

NAN_SETTER(CivilSecond::SetHour) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	auto& d = cs->value;
	cs->value = cctz::civil_second(d.year(), d.month(), d.day(), value->ToInteger()->Value(), d.minute(), d.second());
}

NAN_GETTER(CivilSecond::GetMinute) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	info.GetReturnValue().Set(Nan::New<v8::Number>(cs->value.minute()));
}

NAN_SETTER(CivilSecond::SetMinute) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	auto& d = cs->value;
	cs->value = cctz::civil_second(d.year(), d.month(), d.day(), d.hour(), value->ToInteger()->Value(), d.second());
}

NAN_GETTER(CivilSecond::GetSecond) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	info.GetReturnValue().Set(Nan::New<v8::Number>(cs->value.second()));
}

NAN_SETTER(CivilSecond::SetSecond) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	auto& d = cs->value;
	cs->value = cctz::civil_second(d.year(), d.month(), d.day(), d.hour(), d.minute(), value->ToInteger()->Value());
}

NAN_GETTER(CivilSecond::GetWeekday) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	auto wd = cctz::get_weekday(cctz::civil_day(cs->value));
	switch (wd) {
		case cctz::weekday::monday:
			info.GetReturnValue().Set(Nan::New<v8::Number>(0));
		case cctz::weekday::tuesday:
			info.GetReturnValue().Set(Nan::New<v8::Number>(1));
		case cctz::weekday::wednesday:
			info.GetReturnValue().Set(Nan::New<v8::Number>(2));
		case cctz::weekday::thursday:
			info.GetReturnValue().Set(Nan::New<v8::Number>(3));
		case cctz::weekday::friday:
			info.GetReturnValue().Set(Nan::New<v8::Number>(4));
		case cctz::weekday::saturday:
			info.GetReturnValue().Set(Nan::New<v8::Number>(5));
		case cctz::weekday::sunday:
			info.GetReturnValue().Set(Nan::New<v8::Number>(6));
	}
}

NAN_GETTER(CivilSecond::GetYearday) {
	CivilSecond* cs = Nan::ObjectWrap::Unwrap<CivilSecond>(info.This());
	info.GetReturnValue().Set(Nan::New<v8::Number>(cctz::get_yearday(cctz::civil_day(cs->value))));
}

NAN_METHOD(CivilSecond::New) {
	if (!info.IsConstructCall()) {
		return Nan::ThrowTypeError("CivilSecond constructor cannot be invoked without 'new'");
	}

	auto year   = (info[0]->IsUndefined() || !info[0]->IsNumber()) ? 0 : info[0]->NumberValue();
	auto month  = (info[1]->IsUndefined() || !info[1]->IsNumber()) ? 0 : info[1]->NumberValue();
	auto day    = (info[2]->IsUndefined() || !info[2]->IsNumber()) ? 0 : info[2]->NumberValue();
	auto hour   = (info[3]->IsUndefined() || !info[3]->IsNumber()) ? 0 : info[3]->NumberValue();
	auto minute = (info[4]->IsUndefined() || !info[4]->IsNumber()) ? 0 : info[4]->NumberValue();
	auto second = (info[5]->IsUndefined() || !info[5]->IsNumber()) ? 0 : info[5]->NumberValue();

	CivilSecond* obj = new CivilSecond();
	obj->value = cctz::civil_second(year, month, day, hour, minute, second);

	obj->Wrap(info.This());
	info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> CivilSecond::NewInstance() {
	v8::Local<v8::Function> cons = Nan::New(constructor);
	return Nan::NewInstance(cons).ToLocalChecked();
}
