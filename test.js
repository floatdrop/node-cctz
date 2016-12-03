/* eslint-disable camelcase */

import test from 'ava';
import cctz, {CivilTime, TimeZone, load_time_zone, convert, parse, format} from './';

test('startOf...', t => {
	const time = new CivilTime(2015, 2, 2, 2, 2, 2);
	const syear = time.startOfYear();
	t.is(syear.year, 2015);
	t.is(syear.month, 1);
	t.is(syear.day, 1);
	t.is(syear.hour, 0);
	t.is(syear.minute, 0);
	t.is(syear.second, 0);

	const smnth = time.startOfMonth();
	t.is(smnth.year, 2015);
	t.is(smnth.month, 2);
	t.is(smnth.day, 1);
	t.is(smnth.hour, 0);
	t.is(smnth.minute, 0);
	t.is(smnth.second, 0);

	const sday = time.startOfDay();
	t.is(sday.year, 2015);
	t.is(sday.month, 2);
	t.is(sday.day, 2);
	t.is(sday.hour, 0);
	t.is(sday.minute, 0);
	t.is(sday.second, 0);
});

test('undefined in arguments', t => {
	t.throws(() => load_time_zone(undefined));
	t.throws(() => parse(undefined));
	t.throws(() => parse('', undefined));
	t.throws(() => parse('', 1, undefined));
	t.throws(() => format(undefined));
	t.throws(() => format('', undefined));
	t.throws(() => format('', 1, undefined));
	t.throws(() => convert(undefined));
	t.throws(() => convert('', undefined));
	t.throws(() => convert(new CivilTime(), undefined));
	t.throws(() => new TimeZone(undefined));

	const tz = new TimeZone('UTC');
	t.throws(() => tz.lookup(undefined));
});

test('simple parsing -> format cycle works', t => {
	const tz = load_time_zone('America/New_York');
	const time = parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
	const str = format('%Y-%m-%d %H:%M:%S', time, tz);
	t.is(str, '2015-09-22 09:35:12');
});

test('CivilTime has getters', t => {
	const tz = load_time_zone('America/New_York');
	const tp = parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
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
	const tz = load_time_zone('America/New_York');
	const tp = parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
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
	const tz = load_time_zone('America/New_York');
	const cs = convert(now, tz);
	t.is(cs.year, new Date(now * 1000).getFullYear());

	const tp = convert(cs, tz);
	// Since CivilTime does not contains milliseconds
	t.is(tp, Math.floor(now));
});

test('format shortcut is working', t => {
	const now = Date.now() / 1000;
	const tz = load_time_zone('America/New_York');
	const cs = convert(now, tz);

	const a = format('%Y-%m-%d %H:%M:%S', now, tz);
	const b = format('%Y-%m-%d %H:%M:%S', cs, tz);

	t.is(a, b);
});

test('example1.cc works', t => {
	const lax = load_time_zone('America/Los_Angeles');
	const tp = convert(new CivilTime(2015, 9, 22, 9), lax);

	const nyc = load_time_zone('America/New_York');
	const str = format('Talk starts at %T %z (%Z)', tp, nyc);

	t.is(str, 'Talk starts at 12:00:00 -0400 (EDT)');
});

test('normalization works', t => {
	const now = new CivilTime(2016, 5);
	const endOfMonth = new CivilTime(now.year, now.month + 1, 0);

	t.is(now.year, endOfMonth.year);
	t.is(now.month, endOfMonth.month);
	t.is(endOfMonth.day, 31);
});

test('CivilTime clone works', t => {
	const now = new CivilTime(2016, 5);
	const tomorrow = now.clone();
	tomorrow.day += 1;

	t.is(now.day, 1);
	t.is(now.day + 1, tomorrow.day);
});

test('cctz.tz shortcut works', t => {
	const tz = cctz.tz('Asia/Yekaterinburg');

	t.truthy(tz instanceof TimeZone);
	t.is(tz.name, 'Asia/Yekaterinburg');
});
