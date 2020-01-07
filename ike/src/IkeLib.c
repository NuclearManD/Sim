#include "IkeLib.h"

#include <sys/time.h>

/* ===================================
 * long millis()
 * returns current millisecond time.
 */
/*
long millis(){
	struct timeval spec;
	gettimeofday(&spec, NULL);
	return (unsigned long long)(spec.tv_sec) * 1000 + (unsigned long long)(spec.tv_usec) / 1000;
}
*/
