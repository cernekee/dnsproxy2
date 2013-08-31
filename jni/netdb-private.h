#ifndef _NETDB_PRIVATE_H
#define _NETDB_PRIVATE_H

#include <netdb.h>

#ifdef __cplusplus
extern "C" {
#endif

/* from bionic/libc/include/netdb.h in AOSP 4.3 */

struct hostent	*android_gethostbyaddrforiface(const void *, socklen_t, int, const char*);
struct hostent	*android_gethostbynameforiface(const char *, int, const char *);
int android_getaddrinfoforiface(const char *, const char *, const struct addrinfo *, const char *, struct addrinfo **);
int android_getnameinfoforiface(const struct sockaddr *, socklen_t, char *, size_t, char *, size_t, int, const char *);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* _NETDB_PRIVATE_H */
