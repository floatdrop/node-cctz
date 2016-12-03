const cctz = require('bindings')('node_cctz');

cctz.tz = cctz.load_time_zone;

module.exports = cctz;
