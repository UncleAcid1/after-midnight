/*
 * PS3Net.h — the sockets MMNet.c and MMOnline.c use, on the PS3.
 *
 * Those files are written for BSD sockets. PSL1GHT's network library has the socket calls
 * themselves (once it has been started), but no select() for its socket numbers, fcntl(),
 * getaddrinfo() or getifaddrs(). On the PS3 the two files include this header instead of the
 * system's ones: the missing calls are in PS3Net.c under the usual names (macros), and the
 * network starts the first time a socket is made.
 */
#ifndef PS3NET_H
#define PS3NET_H

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <net/net.h>
#include <net/netdb.h>
#include <net/socket.h>
#include <net/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef TCP_NODELAY
#define TCP_NODELAY         1
#endif
#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN     16
#endif
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK     0x7f000001
#endif
#ifndef IFF_BROADCAST
#define IFF_BROADCAST       0x2
#endif
#ifndef EINPROGRESS
#define EINPROGRESS         119
#endif

struct ifaddrs {
    struct ifaddrs  *ifa_next;
    char            *ifa_name;
    unsigned int     ifa_flags;
    struct sockaddr *ifa_addr, *ifa_netmask, *ifa_broadaddr;
};

/// select() for the few sockets the game waits on (its fd_set is a list, not bits)
typedef struct { int count; int fds[16]; } PS3FdSet;

int     PS3Net_socket(int domain, int type, int protocol);
int     PS3Net_fcntl(int sock, int command, ...);
int     PS3Net_select(int count, PS3FdSet *readable, PS3FdSet *writable, PS3FdSet *errors,
                      struct timeval *timeout);
int     PS3Net_getaddrinfo(const char *host, const char *port, const struct addrinfo *hints,
                           struct addrinfo **result);
void    PS3Net_freeaddrinfo(struct addrinfo *list);
int     PS3Net_getifaddrs(struct ifaddrs **list);
void    PS3Net_freeifaddrs(struct ifaddrs *list);
const char *PS3Net_inet_ntop(int family, const void *address, char *text, socklen_t size);

#ifndef PS3NET_IMPLEMENTATION
#define socket          PS3Net_socket
#define fcntl           PS3Net_fcntl
#define select          PS3Net_select
#define getaddrinfo     PS3Net_getaddrinfo
#define freeaddrinfo    PS3Net_freeaddrinfo
#define getifaddrs      PS3Net_getifaddrs
#define freeifaddrs     PS3Net_freeifaddrs
#define inet_ntop       PS3Net_inet_ntop

#undef fd_set
#undef FD_ZERO
#undef FD_SET
#undef FD_ISSET
#define fd_set              PS3FdSet
#define FD_ZERO(set)        ((set)->count = 0)
#define FD_SET(fd, set)     ((set)->count < 16 ? (void)((set)->fds[(set)->count++] = (fd)) : (void)0)
#define FD_ISSET(fd, set)   PS3Net_FdIsSet((fd), (set))

#endif

static inline int PS3Net_FdIsSet(int fd, const PS3FdSet *set)
{
    int i;
    for (i = 0; i < set->count; i++)
        if (set->fds[i] == fd)
            return 1;
    return 0;
}

#endif
