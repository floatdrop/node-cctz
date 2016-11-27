const Benchmark = require('benchmark');
const moment = require('moment-timezone');
const bindings = require('bindings');
const cctz = bindings('cctz.node');

const suite = new Benchmark.Suite();

suite
	.add('moment-timezone', () => {
		const date = moment.tz('2015-09-22 09:35:12', 'America/New_York');
		return date.add(1, 'h').format('YYYY-MM-DD hh-mm-ss');
	})
	.add('cctz', () => {
		const time = cctz.parse('2015-09-22 09:35:12', 'America/New_York');
		return time.format('%Y-%M-%D %h-%m-%s');
	})
	.on('cycle', event => {
		console.log(String(event.target));
	})
	.on('complete', () => {
		console.log('Fastest is ' + this.filter('fastest').map('name'));
	})
	.run({
		async: true
	});
