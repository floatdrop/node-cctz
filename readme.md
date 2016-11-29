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
