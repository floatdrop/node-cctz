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
const cctz = require('cctz');

const lax = cctz.tz('America/Los_Angeles');
const tp = cctz.convert(new cctz.CivilTime(2015, 9, 22, 9), lax);

const nyc = cctz.tz('America/New_York');
console.log(cctz.format('Talk starts at %T %z (%Z)', tp, nyc));

// => Talk starts at 12:00:00 -0400 (EDT)
```


## API

### tz(name)

> Alias for `cctz.load_time_zone`

Use this method instead `new TimeZone` – because it caches `TimeZone` objects inside.

Returns `TimeZone` object.

##### name
Type: `string`

Timezone name, that should be loaded from `/usr/share/zoneinfo`.

### parse(format, input, [timezone])

Parses `input` string according to `format` string (assuming `input` in `timezone`).

Returns unix timestamp or `undefined` if parsing failed.

##### format

Type: `string`

Format of `input` argument. See [strftime](http://www.cplusplus.com/reference/ctime/strftime/) documentation and [Google CCTZ](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#232) sources for syntax.

##### input

Type: `string`

Input string to parse.

##### timezone

Type: `TimeZone`<br>
Default: Timezone from `input` or UTC

Timezone, that should be used in parse. Timezone can be part of input:

```js
cctz.parse('%Y-%m-%d %H:%M:%S %Ez', '2015-09-22 09:35:12+03:00');
```

### format(format, unix, timezone)

Returns formatted unix timestamp according to timezone.

##### format

Type: `string`

Format of output. See [strftime](http://www.cplusplus.com/reference/ctime/strftime/) documentation and [Google CCTZ](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L197) sources for syntax.

##### unix

Type: `number`

Unix timestamp in seconds (can have fractional part).

##### timezone

Type: `TimeZone`

TimeZone objcet, that represents target timezone for formatting.

### convert(time, timezone)

Converts `CivilTime` to unix timestamp and vice versa.

##### time

Type: `CivilTime` or `number`

If `time` is `CivilTime`, then method returns Unix timestamp (without fractional part).
Otherwise returns `CivilTime`.

##### timezone

Type: `TimeZone`

TimeZone objcet, that represents target timezone for converting.


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

Holder for [`cctz::time_zone`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L37).

#### TimeZone(name)

Creates __new__ object with TimeZone.

##### TimeZone.lookup(unix)

Returns [`cctz::absolute_lookup`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L60) object.

##### TimeZone.lookup(civiltime)

Returns [`cctz::civil_lookup`](https://github.com/google/cctz/blob/6a694a40f3770f6d41e6ab1721c29f4ea1d8352b/include/time_zone.h#L85) object.

##### TimeZone.name

Name of TimeZone.


## Creating Unix timestamp

```js
const timestamp = Date.now() / 1000;
```

All methods expect unix timestamp with fractional seconds, so there is no need for `Math.floor`.


## Benchmarks

```
Format              (cctz) x   527,321 ops/sec ±2.01% (83 runs sampled)
                    (Date) x   172,913 ops/sec ±8.95% (80 runs sampled)
                (date-fns) x    98,439 ops/sec ±1.02% (88 runs sampled)
                  (moment) x    57,317 ops/sec ±3.13% (79 runs sampled)

Parse               (cctz) x 1,366,070 ops/sec ±6.09% (77 runs sampled)
                    (Date) x 1,579,275 ops/sec ±1.44% (87 runs sampled)
                (date-fns) x   307,501 ops/sec ±1.21% (86 runs sampled)
                  (moment) x    32,721 ops/sec ±4.06% (80 runs sampled)

Add hour            (cctz) x 5,764,077 ops/sec ±1.01% (83 runs sampled)
                    (Date) x 3,841,869 ops/sec ±4.57% (78 runs sampled)
                (date-fns) x 2,340,102 ops/sec ±1.37% (85 runs sampled)
                  (moment) x   626,019 ops/sec ±1.28% (86 runs sampled)
```

Run `npm i` and then `npm run bench`.

## License

MIT © [Vsevolod Strukchinsky](mailto://floatdrop@gmail.com)
