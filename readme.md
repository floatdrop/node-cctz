# node-cctz [![Build Status](https://travis-ci.org/floatdrop/node-cctz.svg?branch=master)](https://travis-ci.org/floatdrop/node-cctz)

> Because sometimes Moment.JS is slow

[CCTZ](https://github.com/google/cctz) is a C++ library for translating between absolute and civil times using the rules of a time zone.

## Usage

```js
const cctz = require('cctz');

const lax = cctz.load_time_zone('America/Los_Angeles');
const tp = cctz.convert(new cctz.CivilSecond(2015, 9, 22, 9), lax);

const nyc = cctz.load_time_zone('America/New_York');
console.log(cctz.format('Talk starts at %T %z (%Z)', tp, nyc));

// => Talk starts at 12:00:00 -0400 (EDT)
```

## API

#### cctz.load_time_zone(name)

Returns TimeZone object for time zone with `name` from `/usr/share/zoneinfo`.

> __Pro-tip__: Cache TimeZone objects.

#### cctz.utc_time_zone()

Returns `UTC` TimeZone object.

#### cctz.local_time_zone()

Returns `localtime` TimeZone object.

#### cctz.parse(format, input, timezone)

Parses `input` string according to `format` string (assuming `input` in `timezone`).

Returns TimePoint object.

#### cctz.format(format, timepoint, timezone)

Formats TimePoint `timepoint` object according to `format` in `timezone`.

Returns string.

#### cctz.convert(timepoint, timezone)

Returns CivilSecond object from `timepoint` in `timezone`.

#### cctz.convert(civilsecond, timezone)

Returns TimePoint object from `civilsecond` in `timezone`.


### TimePoint

Holder for [`std::chrono::time_point`](http://en.cppreference.com/w/cpp/chrono/time_point).

#### TimePoint(unix)

Creates TimePoint from Unix timestamp (in milliseconds).

#### TimePoint.unix

Returns Unix timestamp in milliseconds.


### CivilSecond

Holder for [`cctz::civil_second`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/civil_time.h#L22) with getters and setters for properties.

#### CivilSecond(year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0)

Creates CivilSecond object.

##### CivilSecond.year
##### CivilSecond.month
##### CivilSecond.day
##### CivilSecond.hour
##### CivilSecond.minute
##### CivilSecond.second
##### CivilSecond.yearday (only getter)
##### CivilSecond.weekday (only getter)


### TimeZone

Holder for [`cctz::time_zone`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L37).

#### TimeZone(name)

Same as `load_time_zone`.

##### TimeZone.lookup(timepoint)

Returns [`cctz::absolute_lookup`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L60) object.

##### TimeZone.lookup(civilsecond)

Returns [`cctz::civil_lookup`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L85) object.

##### TimeZone.name

Name of TimeZone.


## Benchmarks

```
Parse-Format      (Date) x 590,555 ops/sec ±1.64% (83 runs sampled)
Parse-Format      (cctz) x 505,249 ops/sec ±3.95% (78 runs sampled)
Parse-Format    (moment) x 19,295 ops/sec ±2.12% (81 runs sampled)
Increment hour    (Date) x 15,962 ops/sec ±6.53% (79 runs sampled)
Increment hour    (cctz) x 5,718 ops/sec ±1.66% (85 runs sampled)
Increment hour  (moment) x 600 ops/sec ±1.58% (84 runs sampled)
```

Run `npm i` and then `npm run bench`.

## License

MIT © [Vsevolod Strukchinsky](mailto://floatdrop@gmail.com)
