# node-cctz [![Build Status](https://travis-ci.org/floatdrop/node-cctz.svg?branch=master)](https://travis-ci.org/floatdrop/node-cctz)

> Because sometimes Moment.JS is too slow

Google [CCTZ](https://github.com/google/cctz) binding for Node.JS.

## Usage

```js
const cctz = require('cctz');

const tz = cctz.load_time_zone('Asia/Yekaterinburg');
const tp = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
const time = tz.lookup(tp);

console.log(time);
/*
{ offset: 18000,
  is_dst: false,
  abbr: '+05',
  cs:
   CivilSecond {
     yearday: 265,
     weekday: 6,
     second: 12,
     minute: 35,
     hour: 9,
     day: 22,
     month: 9,
     year: 2015 } }
*/
```

## API

To be discussed.

## Benchmarks

```
Parse-Format      (Date) x 497,848 ops/sec ±11.73% (79 runs sampled)
Parse-Format      (cctz) x 476,584 ops/sec ±3.59% (78 runs sampled)
Parse-Format    (moment) x 16,757 ops/sec ±5.38% (72 runs sampled)
Increment hour    (cctz) x 4,077 ops/sec ±12.77% (66 runs sampled)
Increment hour  (moment) x 338 ops/sec ±26.23% (55 runs sampled)
```

Run `npm i` and then `npm run bench`.

## License

MIT © [Vsevolod Strukchinsky](mailto://floatdrop@gmail.com)
