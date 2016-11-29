import test from 'ava';
import cctz from './';

test('simple parsing -> format cycle works', t => {
	const tz = cctz.load_time_zone('America/New_York');
	const time = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
	const str = cctz.format('%Y-%m-%d %H:%M:%S', time, tz);
	t.is(str, '2015-09-22 09:35:12');
});

test('CivilTime has getters', t => {
	const tz = cctz.load_time_zone('America/New_York');
	const tp = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
	const ct = tz.lookup(tp);
	t.is(ct.cs.year, 2015);
	t.is(ct.cs.month, 9);
	t.is(ct.cs.day, 22);
	t.is(ct.cs.hour, 9);
	t.is(ct.cs.minute, 35);
	t.is(ct.cs.second, 12);
	t.is(ct.cs.weekday, 6);
	t.is(ct.cs.yearday, 265);

	t.is(ct.abbr, 'EDT');
	t.is(ct.is_dst, true);
	t.is(ct.offset, -14400);
});

test('CivilTime has setters', t => {
	const tz = cctz.load_time_zone('America/New_York');
	const tp = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
	const ct = tz.lookup(tp);

	ct.cs.year += 1;
	t.is(ct.cs.year, 2016);
});
// test('has time_zone.h methods', t => {
// 	t.is(typeof cctz.time_zone, 'function');
// 	t.is(typeof cctz.utc_time_zone, 'function');
// 	t.is(typeof cctz.local_time_zone, 'function');
// 	t.is(typeof cctz.convert, 'function');
// 	t.is(typeof cctz.format, 'function');
// 	t.is(typeof cctz.parse, 'function');
// });

