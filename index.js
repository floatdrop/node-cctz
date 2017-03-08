const cctz = require('bindings')('node_cctz');

cctz.tz = cctz.load_time_zone;

cctz.now = () => Math.floor(Date.now() / 1000);

module.exports = cctz;
