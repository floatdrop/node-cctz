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

	const get = ct.cs.year + 1;
	t.is(get, 2016);
	t.is(ct.cs.year, 2015);
	t.is(get === ct.cs, false);

	ct.cs.year += 1;
	t.is(ct.cs.year, 2016);
});

test('convert shortcut is working', t => {
	const now = Date.now() / 1000;
	const tz = cctz.load_time_zone('America/New_York');
	const cs = cctz.convert(now, tz);
	t.is(cs.year, new Date(now * 1000).getFullYear());

	const tp = cctz.convert(cs, tz);
	// Since CivilTime does not contains milliseconds
	t.is(tp, Math.floor(now));
});

test('format shortcut is working', t => {
	const now = Date.now() / 1000;
	const tz = cctz.load_time_zone('America/New_York');
	const cs = cctz.convert(now, tz);

	const a = cctz.format('%Y-%m-%d %H:%M:%S', now, tz);
	const b = cctz.format('%Y-%m-%d %H:%M:%S', cs, tz);

	t.is(a, b);
});

test('example1.cc works', t => {
	const lax = cctz.load_time_zone('America/Los_Angeles');
	const tp = cctz.convert(new cctz.CivilTime(2015, 9, 22, 9), lax);

	const nyc = cctz.load_time_zone('America/New_York');
	const str = cctz.format('Talk starts at %T %z (%Z)', tp, nyc);

	t.is(str, 'Talk starts at 12:00:00 -0400 (EDT)');
});

test('normalization works', t => {
	const now = new cctz.CivilTime(2016, 5);
	const endOfMonth = new cctz.CivilTime(now.year, now.month + 1, 0);

	t.is(now.year, endOfMonth.year);
	t.is(now.month, endOfMonth.month);
	t.is(endOfMonth.day, 31);
});

test('CivilTime clone works', t => {
	const now = new cctz.CivilTime(2016, 5);
	const tomorrow = now.clone();
	tomorrow.day += 1;

	t.is(now.day, 1);
	t.is(now.day + 1, tomorrow.day);
});

test('utc_time_zone works', t => {
	const tz = cctz.utc_time_zone();
	t.is(tz.name, 'UTC');
});
