 /* eslint-disable no-unused-vars, no-unused-expressions, no-new */

const Benchmark = require('benchmark');
const moment = require('moment-timezone');
const cctz = require('./');

const suite = new Benchmark.Suite();

let iunix = new Date() / 1000;
let idate = new Date();
let icctz = cctz.convert(new Date() / 1000, cctz.tz('UTC'));
let immtz = moment().tz('America/New_York');

suite
	.add('Format          (baseline)', () => {
		const date = new Date();
		date.getFullYear() + '-' + date.getMonth() + '-' + date.getDay();
	})
	.add('Format              (cctz)', () => {
		const tz = cctz.tz('America/New_York');
		cctz.format('%Y-%m-%d', Date.now() / 1000, tz);
	})
	.add('Format            (moment)', () => {
		moment.tz('America/New_York').format('YYYY-MM-DD');
	})
	.add('Parse           (baseline)', () => {
		new Date('2015-09-22 09:35:12 GMT+0300');
	})
	.add('Parse               (cctz)', () => {
		const tz = cctz.tz('America/New_York');
		cctz.parse('%Y-%m-%d %H:%M:%S', '2015-09-22 09:35:12', tz);
	})
	.add('Parse             (moment)', () => {
		const date = moment.tz('2015-09-22 09:35:12', 'America/New_York');
		date.format('YYYY-MM-DD hh-mm-ss');
	})
	.add('Add hour        (baseline)', () => {
		idate.setTime(idate.getTime() + (60 * 60 * 1000));
	})
	.add('Add hour       (cctz-unix)', () => {
		iunix += 60 * 60;
	})
	.add('Add hour      (cctz-civil)', () => {
		icctz.hour += 1;
	})
	.add('Add hour          (moment)', () => {
		immtz.add(1, 'h');
	})
	.on('cycle', event => {
		console.log(String(event.target));
	})
	.run({
		async: true
	});
