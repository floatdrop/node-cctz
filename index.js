const cctz = require('bindings')('node_cctz');

cctz.tz = cctz.load_time_zone;

cctz.now = () => Date.now() / 1000;

module.exports = cctz;
