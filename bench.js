const Benchmark = require('benchmark');
const moment = require('moment-timezone');
const cctz = require('./');

const suite = new Benchmark.Suite();
const tz = cctz.load_time_zone('America/New_York');

suite
	.add('Parse-Format      (Date)', () => {
		const date = new Date('2015-09-22 09:35:12 GMT+0300');
		return date.toString();
	})
	.add('Parse-Format      (cctz)', () => {
		const time = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
		return cctz.format('%Y-%m-%d %H:%M:%S', time, tz);
	})
	.add('Parse-Format    (moment)', () => {
		const date = moment.tz('2015-09-22 09:35:12', 'America/New_York');
		return date.format('YYYY-MM-DD hh-mm-ss');
	})
	.add('Increment hour    (Date)', () => {
		const date = new Date('2015-09-22 09:35:12 GMT+0300');
		for (let i = 0; i < 1000; i++) {
			date.setTime(date.getTime() + (i * 60 * 60 * 1000));
		}
	})
	.add('Increment hour    (cctz)', () => {
		const tp = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
		const time = tz.lookup(tp).cs;
		for (let i = 0; i < 1000; i++) {
			time.hour += i;
		}
	})
	.add('Increment hour  (moment)', () => {
		const date = moment.tz('2015-09-22 09:35:12', 'America/New_York');
		for (let i = 0; i < 1000; i++) {
			date.add(i, 'h');
		}
	})
	.on('cycle', event => {
		console.log(String(event.target));
	})
	.run({
		async: true
	});
