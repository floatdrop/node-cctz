# node-cctz [![Build Status](https://travis-ci.org/floatdrop/node-cctz.svg?branch=master)](https://travis-ci.org/floatdrop/node-cctz)

> Because sometimes Moment.JS is too slow

Google [CCTZ](https://github.com/google/cctz) binding for Node.JS.

## Usage

```js
const cctz = require('cctz');

const tz = cctz.load_time_zone('Asia/Yekaterinburg');
const time = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);

console.log(time);
```

## API

To be discussed.

## License

MIT © [Vsevolod Strukchinsky](mailto://floatdrop@gmail.com)
