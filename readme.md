# node-cctz [![Build Status](https://travis-ci.org/floatdrop/node-cctz.svg?branch=master)](https://travis-ci.org/floatdrop/node-cctz)

> Because sometimes Moment.JS is slow

[CCTZ](https://github.com/google/cctz) is a C++ library for translating between absolute and civil times using the rules of a time zone.

## Usage

```js
const cctz = require('cctz');

const lax = cctz.load_time_zone('America/Los_Angeles');
const tp = cctz.convert(new cctz.CivilTime(2015, 9, 22, 9), lax);

const nyc = cctz.load_time_zone('America/New_York');
console.log(cctz.format('Talk starts at %T %z (%Z)', tp, nyc));

// => Talk starts at 12:00:00 -0400 (EDT)
```

## API

#### load_time_zone(name)

Returns TimeZone object for time zone with `name` from `/usr/share/zoneinfo`.
It is recomended to use this method, because it maintain internal cache of V8 objects.

#### parse(format, input, timezone)

Parses `input` string according to `format` string (assuming `input` in `timezone`).

Returns unix timestamp.

#### format(format, unix, timezone)

Formats unix timestamp `unix` object according to `format` in `timezone`.

Returns string.

#### convert(unix, timezone)

Returns CivilTime object from unix timestamp in `timezone`.

#### convert(civilsecond, timezone)

Returns unix timestamp from `civilsecond` in `timezone`.


### CivilTime

Holder for [`cctz::civil_second`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/civil_time.h#L22) with getters and setters for properties.

#### CivilTime(year = 1970, month = 1, day = 1, hour = 0, minute = 0, second = 0)

Creates CivilTime object with next properties:

- `year` – getter and setter
- `month` – getter and setter [1:12]
- `day` – getter and setter [1:31]
- `hour` – getter and setter [0:23]
- `minute` – getter and setter [0:59]
- `second` – getter and setter [0:59]
- `yearday` – only getter [1:356]
- `weekday` – only getter [0:6]

##### CivilTime.startOfYear()

Returns new CivilTime object with start of year.

##### CivilTime.startOfMonth()

Returns new CivilTime object with start of month.

##### CivilTime.startOfDay()

Returns new CivilTime object with start of day.

##### CivilTime.clone()

Returns cloned CivilTime object.


### TimeZone

Holder for [`cctz::time_zone`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L37).

#### TimeZone(name)

Creates __new__ object with TimeZone.

##### TimeZone.lookup(unix)

Returns [`cctz::absolute_lookup`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L60) object.

##### TimeZone.lookup(civilsecond)

Returns [`cctz::civil_lookup`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L85) object.

##### TimeZone.name

Name of TimeZone.


## Benchmarks

```
Format Now      (baseline) x 3,099,085 ops/sec ±2.73% (83 runs sampled)
Format Now   (cctz-format) x 1,326,945 ops/sec ±2.73% (80 runs sampled)
Format Now   (cctz-concat) x 679,269 ops/sec ±1.27% (80 runs sampled)
Format Now (moment-format) x 88,098 ops/sec ±2.08% (84 runs sampled)
Format Now (moment-concat) x 118,912 ops/sec ±4.52% (76 runs sampled)

Parse-Format    (baseline) x 593,916 ops/sec ±1.72% (83 runs sampled)
Parse-Format        (cctz) x 743,739 ops/sec ±1.79% (81 runs sampled)
Parse-Format      (moment) x 20,709 ops/sec ±1.36% (85 runs sampled)

Increment hour  (baseline) x 18,841 ops/sec ±1.15% (89 runs sampled)
Increment hour      (cctz) x 5,449 ops/sec ±2.95% (81 runs sampled)
Increment hour    (moment) x 609 ops/sec ±1.33% (83 runs sampled)
```

Run `npm i` and then `npm run bench`.

## License

MIT © [Vsevolod Strukchinsky](mailto://floatdrop@gmail.com)
