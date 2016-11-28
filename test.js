import test from 'ava';
import cctz from './';

test('simple parsing -> format cycle works', t => {
	const tz = cctz.load_time_zone('America/New_York');
	const time = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
	const str = cctz.format('%Y-%m-%d %H:%M:%S', time, tz);
	t.is(str, '2015-09-22 09:35:12');
});

// test('has time_zone.h methods', t => {
// 	t.is(typeof cctz.time_zone, 'function');
// 	t.is(typeof cctz.utc_time_zone, 'function');
// 	t.is(typeof cctz.local_time_zone, 'function');
// 	t.is(typeof cctz.convert, 'function');
// 	t.is(typeof cctz.format, 'function');
// 	t.is(typeof cctz.parse, 'function');
// });

// test('has civil_time.h methods', t => {
// 	t.is(typeof cctz.civil_second, 'function');
// 	t.is(typeof cctz.civil_minute, 'function');
// 	t.is(typeof cctz.civil_hour, 'function');
// 	t.is(typeof cctz.civil_day, 'function');
// 	t.is(typeof cctz.civil_month, 'function');
// 	t.is(typeof cctz.civil_year, 'function');
// 	t.is(typeof cctz.get_weekday, 'function');
// 	t.is(typeof cctz.next_weekday, 'function');
// 	t.is(typeof cctz.prev_weekday, 'function');
// 	t.is(typeof cctz.get_yearday, 'function');
// });

// test('has civil_time.h constants', t => {
// 	t.is(typeof cctz.weekday.monday, 'number');
// 	t.is(typeof cctz.weekday.tuesday, 'number');
// 	t.is(typeof cctz.weekday.wednesday, 'number');
// 	t.is(typeof cctz.weekday.thursday, 'number');
// 	t.is(typeof cctz.weekday.friday, 'number');
// 	t.is(typeof cctz.weekday.saturday, 'number');
// 	t.is(typeof cctz.weekday.sunday, 'number');
// });
