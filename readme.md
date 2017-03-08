# node-cctz [![Build Status](https://travis-ci.org/floatdrop/node-cctz.svg?branch=master)](https://travis-ci.org/floatdrop/node-cctz)

[CCTZ](https://github.com/google/cctz) is a C++ library for translating between absolute and civil times using the rules of a time zone.


## Install

You will need C++11 compatible compiler to build this binding. For most systems this will work:

```
$ npm install --save cctz
```

If you have Ubuntu 12.04, then install `clang-3.4` and set-up environment:

```
$ sudo apt-get install clang-3.4
$ export CXX=clang++
$ export npm_config_clang=1
```


## Usage

```js
const {convert, format, CivilTime} = require('cctz');

const unix = convert(new CivilTime(2015, 9, 22, 9), 'America/Los_Angeles');
console.log(format('Talk starts at %T %z (%Z)', unix, 'America/New_York'));

// => Talk starts at 12:00:00 -0400 (EDT)
```


## API

### convert(time, timezone)

Converts `CivilTime` to unix timestamp and vice versa.

##### time

Type: `CivilTime` or `number`

If `time` is `CivilTime`, then method returns Unix timestamp (without fractional part).
Otherwise returns `CivilTime`.

##### timezone

Type: `TimeZone` or `string`

TimeZone objcet, that represents target timezone for converting.

### format(format, unix, [timezone])

Returns formatted unix timestamp according to timezone.

##### format

Type: `string`

Format of output. See [strftime](http://www.cplusplus.com/reference/ctime/strftime/) documentation and [Google CCTZ](https://github.com/google/cctz/blob/6e09ceb/include/time_zone.h#L197) sources for syntax.

##### unix

Type: `number`

Unix timestamp in seconds (can have fractional part).

##### timezone

Type: `TimeZone` or `string`
Default: local timezone

TimeZone objcet, that represents target timezone for formatting.

### parse(format, input, [timezone])

Parses `input` string according to `format` string (assuming `input` in `timezone`).

Returns unix timestamp or `undefined` if parsing failed.

##### format

Type: `string`

Format of `input` argument. See [strftime](http://www.cplusplus.com/reference/ctime/strftime/) documentation and [Google CCTZ](https://github.com/google/cctz/blob/6e09ceb/include/time_zone.h#232) sources for syntax.

##### input

Type: `string`

Input string to parse.

##### timezone

Type: `TimeZone` or `string`<br>
Default: Timezone from `input` or local timezone

Timezone, that should be used in parse. Timezone can be part of input:

```js
cctz.parse('%Y-%m-%d %H:%M:%S %Ez', '2015-09-22 09:35:12+03:00');
```

### tz(name)

> Alias for `cctz.load_time_zone`

Use this method instead `new TimeZone` – because it caches `TimeZone` objects inside.

Returns `TimeZone` object.

##### name
Type: `string`

Timezone name, that should be loaded from `/usr/share/zoneinfo`.

### now()

> Alias for `Date.now() / 1000`

Returns unix timestamp in seconds (with fractional milliseconds)


### CivilTime

Holder for [`cctz::civil_second`](https://github.com/google/cctz/blob/6e09ceb/include/civil_time.h#L22) with getters and setters for properties.

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

> __Tip:__ Fastest way to add amount of time is to add number of seconds to unix timestamp.

##### CivilTime.startOfYear()

Returns new CivilTime object with start of year.

##### CivilTime.startOfMonth()

Returns new CivilTime object with start of month.

##### CivilTime.startOfDay()

Returns new CivilTime object with start of day.

##### CivilTime.startOfHour()

Returns new CivilTime object with start of hour.

##### CivilTime.clone()

Returns cloned CivilTime object.


### TimeZone

Holder for [`cctz::time_zone`](https://github.com/google/cctz/blob/6e09ceb/include/time_zone.h#L37).

#### TimeZone(name)

Creates __new__ object with TimeZone.

##### TimeZone.lookup(unix)

Returns [`cctz::absolute_lookup`](https://github.com/google/cctz/blob/6e09ceb/include/time_zone.h#L60) object.

##### TimeZone.lookup(civiltime)

Returns [`cctz::civil_lookup`](https://github.com/google/cctz/blob/6e09ceb/include/time_zone.h#L85) object.

##### TimeZone.name

Name of TimeZone.


## Benchmarks

```
Format              (cctz) x 532,683 ops/sec ±2.80% (78 runs sampled)
                    (Date) x 191,400 ops/sec ±2.59% (81 runs sampled)
                (date-fns) x  91,772 ops/sec ±1.58% (85 runs sampled)
                  (moment) x  56,508 ops/sec ±6.35% (76 runs sampled)

Parse               (cctz) x 1,105,353 ops/sec ±0.59% (86 runs sampled)
                    (Date) x 1,200,218 ops/sec ±2.00% (82 runs sampled)
                (date-fns) x   249,225 ops/sec ±5.00% (77 runs sampled)
                  (moment) x    23,905 ops/sec ±1.91% (80 runs sampled)

Add hour            (cctz) x 6,123,083 ops/sec ±5.39% (82 runs sampled)
                    (Date) x 3,604,915 ops/sec ±1.13% (69 runs sampled)
                (date-fns) x 3,211,900 ops/sec ±2.28% (81 runs sampled)
                  (moment) x   590,854 ops/sec ±8.48% (72 runs sampled)

Convert Ut->Time    (cctz) x 653,305 ops/sec ±9.33% (68 runs sampled)
                  (moment) x 144,580 ops/sec ±2.18% (83 runs sampled)

Convert Time->Ut    (cctz) x 987,437 ops/sec ±11.68% (70 runs sampled)
                  (moment) x 154,492 ops/sec ±3.54% (82 runs sampled)
```

Run `npm i` and then `npm run bench`.

## License

MIT © [Vsevolod Strukchinsky](mailto://floatdrop@gmail.com)
