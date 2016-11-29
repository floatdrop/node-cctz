const Benchmark = require('benchmark');
const moment = require('moment-timezone');
const cctz = require('./');

const suite = new Benchmark.Suite();
const tz = cctz.load_time_zone('America/New_York');

suite
	.add('Parse-Format    (moment)', () => {
		const date = moment.tz('2015-09-22 09:35:12', 'America/New_York');
		return date.format('YYYY-MM-DD hh-mm-ss');
	})
	.add('Parse-Format      (cctz)', () => {
		const time = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
		return cctz.format('%Y-%m-%d %H:%M:%S', time, tz);
	})
	.add('Increment hour  (moment)', () => {
		const date = moment.tz('2015-09-22 09:35:12', 'America/New_York');
		for (let i = 0; i < 1000; i++) {
			date.add(i, 'h');
		}
	})
	.add('Increment hour    (cctz)', () => {
		const tp = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
		const time = tz.lookup(tp).cs;
		for (let i = 0; i < 1000; i++) {
			time.hour += i;
		}
	})
	.on('cycle', event => {
		console.log(String(event.target));
	})
	.run({
		async: true
	});
