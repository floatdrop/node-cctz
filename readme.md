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
Parse-Format    (moment) x 19,612 ops/sec ±1.67% (81 runs sampled)
Parse-Format      (cctz) x 501,881 ops/sec ±3.16% (77 runs sampled)
Increment hour  (moment) x 549 ops/sec ±1.89% (82 runs sampled)
Increment hour    (cctz) x 5,318 ops/sec ±2.02% (86 runs sampled)
```

Run `npm i` and then `npm run bench`.

## License

MIT © [Vsevolod Strukchinsky](mailto://floatdrop@gmail.com)
