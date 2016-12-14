/* eslint-disable camelcase */

import test from 'ava';
import {CivilTime, TimeZone, tz, convert, parse, format} from './';

process.env.TZ = 'UTC';

test('example1.cc', t => {
	const lax = tz('America/Los_Angeles');
	const tp = convert(new CivilTime(2015, 9, 22, 9), lax);
	const nyc = tz('America/New_York');
	const str = format('Talk starts at %T %z (%Z)', tp, nyc);

	t.is(str, 'Talk starts at 12:00:00 -0400 (EDT)');
});

test('example2.cc', t => {
	const lax = tz('America/Los_Angeles');
	const tp = parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:00', lax);

	const now = Date.now() / 1000;

	t.true(now > tp);
});

test('example3.cc', t => {
	const lax = tz('America/Los_Angeles');
	const now = convert(1442914512, lax);
	const then = now.clone();
	then.month += 6;
	t.is(format('%F %T %z', now, lax), '2015-09-22 02:35:12 -0700');
	t.is(format('%F %T %z', then, lax), '2016-03-22 02:35:12 -0700');
});

test('example4.cc', t => {
	const lax = tz('America/Los_Angeles');
	const now = convert(1442914512, lax);
	const day = now.startOfDay();

	t.is(format('%F %T %z', now, lax), '2015-09-22 02:35:12 -0700');
	t.is(format('%F %T %z', day, lax), '2015-09-22 00:00:00 -0700');
});

test('startOfYear', t => {
	const time = new CivilTime(2015, 2, 2, 2, 2, 2);
	const year = time.startOfYear();
	t.is(year.year, 2015);
	t.is(year.month, 1);
	t.is(year.day, 1);
	t.is(year.hour, 0);
	t.is(year.minute, 0);
	t.is(year.second, 0);
});

test('startOfMonth', t => {
	const time = new CivilTime(2015, 2, 2, 2, 2, 2);
	const month = time.startOfMonth();
	t.is(month.year, 2015);
	t.is(month.month, 2);
	t.is(month.day, 1);
	t.is(month.hour, 0);
	t.is(month.minute, 0);
	t.is(month.second, 0);
});

test('startOfDay', t => {
	const time = new CivilTime(2015, 2, 2, 2, 2, 2);
	const day = time.startOfDay();
	t.is(day.year, 2015);
	t.is(day.month, 2);
	t.is(day.day, 2);
	t.is(day.hour, 0);
	t.is(day.minute, 0);
	t.is(day.second, 0);
});

test('startOfHour', t => {
	const time = new CivilTime(2015, 2, 2, 2, 2, 2);
	const hour = time.startOfHour();
	t.is(hour.year, 2015);
	t.is(hour.month, 2);
	t.is(hour.day, 2);
	t.is(hour.hour, 2);
	t.is(hour.minute, 0);
	t.is(hour.second, 0);
});

test('undefined in arguments', t => {
	t.throws(() => tz(undefined));
	t.throws(() => parse(undefined));
	t.throws(() => parse('', undefined));
	t.throws(() => format(undefined));
	t.throws(() => format('', undefined));
	t.throws(() => convert(undefined));
	t.throws(() => convert('', undefined));
	t.throws(() => convert(new CivilTime(), undefined));
	t.throws(() => new TimeZone(undefined));
	t.throws(() => (new TimeZone('UTC')).lookup(undefined));
});

test('parse', t => {
	const nyc = tz('America/New_York');
	t.is(parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12'), 1442914512);
	t.is(parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', nyc), 1442928912);
	t.is(parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', 'America/New_York'), 1442928912);
	t.is(parse('%Y-%m-%d %H:%M:%S %Ez', '2015-09-22 09:35:12-0400'), 1442928912);
});

test('format', t => {
	const nyc = tz('America/New_York');
	t.is(format('%Y-%m-%d %H:%M:%S', 1442928912, nyc), '2015-09-22 09:35:12');
	t.is(format('%Y-%m-%d %H:%M:%S', 1442928912, 'America/New_York'), '2015-09-22 09:35:12');
	t.is(format('%Y-%m-%d %H:%M:%S', new CivilTime(2015, 9, 22, 9, 35, 12), nyc), '2015-09-22 09:35:12');
	t.is(format('%Y-%m-%d %H:%M:%S', new CivilTime(2015, 9, 22, 9, 35, 12), 'America/New_York'), '2015-09-22 09:35:12');
});

test('convert', t => {
	const now = Date.now() / 1000;
	const nyc = tz('America/New_York');
	const cs = convert(now, nyc);
	const tp = convert(cs, nyc);
	const tp2 = convert(cs, 'America/New_York');
	// Since CivilTime does not contains milliseconds
	t.is(tp, Math.floor(now));
	t.is(tp2, Math.floor(now));
	t.is(cs.year, new Date(now * 1000).getFullYear());
});

test('CivilTime has getters', t => {
	const nyc = tz('America/New_York');
	const tp = parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', nyc);
	const ct = nyc.lookup(tp);

	t.is(ct.cs.year, 2015);
	t.is(ct.cs.month, 9);
	t.is(ct.cs.day, 22);
	t.is(ct.cs.hour, 9);
	t.is(ct.cs.minute, 35);
	t.is(ct.cs.second, 12);
	t.is(ct.cs.weekday, 1);
	t.is(ct.cs.yearday, 265);

	t.is(ct.abbr, 'EDT');
	t.is(ct.is_dst, true);
	t.is(ct.offset, -14400);
});

test('CivilTime has setters', t => {
	const nyc = tz('America/New_York');
	const tp = parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', nyc);
	const ct = nyc.lookup(tp);
	const get = ct.cs.year + 1;

	t.is(get, 2016);
	t.is(ct.cs.year, 2015);
	t.is(get === ct.cs, false);

	ct.cs.year += 1;
	t.is(ct.cs.year, 2016);
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
