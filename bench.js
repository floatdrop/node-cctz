const Benchmark = require('benchmark');
const moment = require('moment-timezone');
const cctz = require('./');

const suite = new Benchmark.Suite();

suite
	.add('Parse-Format (moment)', () => {
		const date = moment.tz('2015-09-22 09:35:12', 'America/New_York');
		return date.format('YYYY-MM-DD hh-mm-ss');
	})
	.add('Parse-Format   (cctz)', () => {
		const tz = cctz.load_time_zone('America/New_York');
		const time = cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
		return cctz.format('%Y-%m-%d %H:%M:%S', time, tz);
	})
	.on('cycle', event => {
		console.log(String(event.target));
	})
	.run({
		async: true
	});
