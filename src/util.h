#ifndef UTIL_H
#define UTIL_H

#include <chrono>

typedef std::chrono::duration<double, std::ratio<1>> d_seconds;

static std::chrono::system_clock::time_point toTimePoint(const double unix) {
	return std::chrono::system_clock::time_point(
		std::chrono::duration_cast<std::chrono::system_clock::duration>(
			d_seconds{unix}
		)
	);
}

static int toUnixTimestamp(const std::chrono::system_clock::time_point& timepoint) {
	auto duration = timepoint.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

#endif
