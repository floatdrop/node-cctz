import test from 'ava';
import bindings from 'bindings';
const cctz = bindings('cctz.node');

test('has time_zone.h methods', t => {
	t.ok(cctz.time_zone);
	t.ok(cctz.utc_time_zone);
	t.ok(cctz.local_time_zone);
	t.ok(cctz.convert);
	t.ok(cctz.format);
	t.ok(cctz.parse);
});

test('has civil_time.h methods', t => {
	t.ok(cctz.civil_second);
	t.ok(cctz.civil_minute);
	t.ok(cctz.civil_hour);
	t.ok(cctz.civil_day);
	t.ok(cctz.civil_month);
	t.ok(cctz.civil_year);
	t.ok(cctz.get_weekday);
	t.ok(cctz.next_weekday);
	t.ok(cctz.prev_weekday);
	t.ok(cctz.get_yearday);
});

test('has civil_time.h constants', t => {
	t.ok(cctz.weekday.monday);
	t.ok(cctz.weekday.tuesday);
	t.ok(cctz.weekday.wednesday);
	t.ok(cctz.weekday.thursday);
	t.ok(cctz.weekday.friday);
	t.ok(cctz.weekday.saturday);
	t.ok(cctz.weekday.sunday);
});
