const Benchmark = require('benchmark');
const moment = require('moment-timezone');
const cctz = require('./');

const suite = new Benchmark.Suite();

suite
	.add('Format Now      (baseline)', () => {
		const date = new Date();
		return date.getFullYear() + '-' + date.getMonth() + '-' + date.getDay();
	})
	.add('Format Now   (cctz-format)', () => {
		const tz = cctz.load_time_zone('America/New_York');
		return cctz.format('%Y-%m-%d', Date.now() / 1000, tz);
	})
	.add('Format Now   (cctz-concat)', () => {
		const tz = cctz.load_time_zone('America/New_York');
		const cs = cctz.convert(Date.now() / 1000, tz);
		return cs.year + '-' + cs.month + '-' + cs.day;
	})
	.add('Format Now (moment-format)', () => {
		return moment.tz('America/New_York').format('YYYY-MM-DD');
	})
	.add('Format Now (moment-concat)', () => {
		const date = moment.tz('America/New_York');
		return date.year() + '-' + date.month() + '-' + date.day();
	})
	.add('Parse-Format    (baseline)', () => {
		const date = new Date('2015-09-22 09:35:12 GMT+0300');
		return date.toString();
	})
	.add('Parse-Format        (cctz)', () => {
		const tz = cctz.load_time_zone('America/New_York');
		const time = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
		return cctz.format('%Y-%m-%d %H:%M:%S', time, tz);
	})
	.add('Parse-Format      (moment)', () => {
		const date = moment.tz('2015-09-22 09:35:12', 'America/New_York');
		return date.format('YYYY-MM-DD hh-mm-ss');
	})
	.add('Increment hour  (baseline)', () => {
		const date = new Date('2015-09-22 09:35:12 GMT+0300');
		for (let i = 0; i < 1000; i++) {
			date.setTime(date.getTime() + (i * 60 * 60 * 1000));
		}
	})
	.add('Increment hour      (cctz)', () => {
		const tz = cctz.load_time_zone('America/New_York');
		const tp = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
		const time = tz.lookup(tp).cs;
		for (let i = 0; i < 1000; i++) {
			time.hour += i;
		}
	})
	.add('Increment hour    (moment)', () => {
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
