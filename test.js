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

test('TimePoint has unix timestamp constructor', t => {
	const unix = new Date().getTime();
	const tp = new cctz.TimePoint(unix);
	t.is(tp.unix, unix);
});

test('convert shortcut is working', t => {
	const now = new Date().getTime();
	const tz = cctz.load_time_zone('America/New_York');
	const cs = cctz.convert(new cctz.TimePoint(now), tz);
	t.is(cs.year, new Date(now).getFullYear());

	const tz2 = cctz.convert(cs, tz);
	// Since CivilSeconds does not contains milliseconds
	t.is(tz2.unix, Math.floor(now / 1000) * 1000);
});

test('example1.cc works', t => {
	const lax = cctz.load_time_zone('America/Los_Angeles');
	const tp = cctz.convert(new cctz.CivilSecond(2015, 9, 22, 9), lax);

	const nyc = cctz.load_time_zone('America/New_York');
	const str = cctz.format('Talk starts at %T %z (%Z)', tp, nyc);

	t.is(str, 'Talk starts at 12:00:00 -0400 (EDT)');
});

test('normalization works', t => {
	const now = new cctz.CivilSecond(2016, 5);
	const endOfMonth = new cctz.CivilSecond(now.year, now.month + 1, -1);

	t.is(now.year, endOfMonth.year);
	t.is(now.month, endOfMonth.month);
	t.is(endOfMonth.day, 29);
});

test('utc_time_zone works', t => {
	const tz = cctz.utc_time_zone();
	t.is(tz.name, 'UTC');
});
