/*
 * PS3Net.c — the socket calls PSL1GHT lacks, for MMNet.c and MMOnline.c (see PS3Net.h).
 */
#define PS3_PLATFORM_IMPLEMENTATION
#include "PS3Platform.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <net/net.h>
#include <net/netctl.h>
#include <net/netdb.h>
#include <net/poll.h>
#include <net/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sysmodule/sysmodule.h>

#define PS3NET_IMPLEMENTATION     /* the real calls here, not the macros */
#include "PS3Net.h"

static int gNetStarted;         // 1 started, -1 failed

/// The network library and its connection state, once.
static int StartNetwork(void)
{
    if (gNetStarted == 0) {
        gNetStarted = -1;
        if (netInitialize() < 0) {
            printf("PS3Net: the network library didn't start\n");
        } else {
            if (sysModuleLoad(SYSMODULE_NETCTL) < 0 || netCtlInit() < 0)
                printf("PS3Net: netctl didn't start (no local addresses)\n");
            gNetStarted = 1;
            printf("PS3Net: network started\n");
        }
    }
    return gNetStarted > 0;
}

int PS3Net_socket(int domain, int type, int protocol)
{
    if (!StartNetwork()) {
        errno = ENETDOWN;
        return -1;
    }
    return socket(domain, type, protocol);
}

/// Only what the game does: F_GETFL, and F_SETFL with O_NONBLOCK (SO_NBIO here).
int PS3Net_fcntl(int sock, int command, ...)
{
    va_list args;
    int flags, on;

    if (command == F_GETFL)
        return 0;
    if (command != F_SETFL) {
        errno = EINVAL;
        return -1;
    }
    va_start(args, command);
    flags = va_arg(args, int);
    va_end(args);
    on = (flags & O_NONBLOCK) != 0;
    return setsockopt(sock, SOL_SOCKET, SO_NBIO, &on, sizeof(on));
}

/// select() through poll(): the PS3's socket numbers don't fit an fd_set's bits.
int PS3Net_select(int count, PS3FdSet *readable, PS3FdSet *writable, PS3FdSet *errors, struct timeval *timeout)
{
    struct pollfd fds[48];
    int n = 0, i, ready, ms;
    PS3FdSet *sets[3] = { readable, writable, errors };
    static const short kEvents[3] = { POLLIN, POLLOUT, 0 };
    int s;

    (void)count;
    for (s = 0; s < 3; s++) {
        if (sets[s] == NULL)
            continue;
        for (i = 0; i < sets[s]->count && n < 48; i++) {
            fds[n].fd = sets[s]->fds[i];
            fds[n].events = kEvents[s];
            fds[n].revents = 0;
            n++;
        }
    }
    ms = timeout == NULL ? -1 : (int)(timeout->tv_sec * 1000 + timeout->tv_usec / 1000);
    ready = poll(fds, (nfds_t)n, ms);
    if (ready < 0)
        return -1;
        // keep in each set only what is ready (an error counts as ready, as select() does)
    n = 0;
    ready = 0;
    for (s = 0; s < 3; s++) {
        int kept = 0;
        if (sets[s] == NULL)
            continue;
        for (i = 0; i < sets[s]->count && n < 48; i++, n++) {
            short want = s == 2 ? (POLLERR | POLLHUP) : (kEvents[s] | POLLERR | POLLHUP);
            if (fds[n].revents & want) {
                sets[s]->fds[kept++] = sets[s]->fds[i];
                ready++;
            }
        }
        sets[s]->count = kept;
    }
    return ready;
}

/// IPv4 only: a dotted address, or a name looked up with the system's resolver.
int PS3Net_getaddrinfo(const char *host, const char *port, const struct addrinfo *hints, struct addrinfo **result)
{
    struct addrinfo *info;
    struct sockaddr_in *address;
    struct in_addr in;

    *result = NULL;
    if (!StartNetwork())
        return -1;
    if (host == NULL || !inet_aton(host, &in)) {
        struct hostent *entry = host != NULL ? gethostbyname(host) : NULL;
        if (entry == NULL || entry->h_addr_list == NULL || entry->h_addr_list[0] == NULL)
            return -1;
        memcpy(&in, entry->h_addr_list[0], sizeof(in));
    }
    info = calloc(1, sizeof(*info) + sizeof(*address));
    if (info == NULL)
        return -1;
    address = (struct sockaddr_in *)(info + 1);
    address->sin_len = sizeof(*address);
    address->sin_family = AF_INET;
    address->sin_port = htons((unsigned short)(port != NULL ? atoi(port) : 0));
    address->sin_addr = in;
    info->ai_family = AF_INET;
    info->ai_socktype = hints != NULL && hints->ai_socktype != 0 ? hints->ai_socktype : SOCK_STREAM;
    info->ai_protocol = 0;
    info->ai_addrlen = sizeof(*address);
    info->ai_addr = (struct sockaddr *)address;
    *result = info;
    return 0;
}

void PS3Net_freeaddrinfo(struct addrinfo *list)
{
    free(list);
}

/// The PS3's one network connection (wired or Wi-Fi), when it has an address.
int PS3Net_getifaddrs(struct ifaddrs **list)
{
    struct {
        struct ifaddrs     entry;
        struct sockaddr_in address, netmask, broadcast;
    } *one;
    union net_ctl_info info;
    struct in_addr ip, mask;
    s32 state = 0;

    *list = NULL;
    if (!StartNetwork() || netCtlGetState(&state) < 0 || state != NET_CTL_STATE_IPObtained)
        return -1;
    if (netCtlGetInfo(NET_CTL_INFO_IP_ADDRESS, &info) < 0 || !inet_aton(info.ip_address, &ip))
        return -1;
    if (netCtlGetInfo(NET_CTL_INFO_NETMASK, &info) < 0 || !inet_aton(info.netmask, &mask))
        mask.s_addr = htonl(0xffffff00);
    one = calloc(1, sizeof(*one));
    if (one == NULL)
        return -1;
    one->address.sin_len = one->netmask.sin_len = one->broadcast.sin_len = sizeof(struct sockaddr_in);
    one->address.sin_family = one->netmask.sin_family = one->broadcast.sin_family = AF_INET;
    one->address.sin_addr = ip;
    one->netmask.sin_addr = mask;
    one->broadcast.sin_addr.s_addr = ip.s_addr | ~mask.s_addr;
    one->entry.ifa_name = "eth0";
    one->entry.ifa_flags = IFF_BROADCAST;
    one->entry.ifa_addr = (struct sockaddr *)&one->address;
    one->entry.ifa_netmask = (struct sockaddr *)&one->netmask;
    one->entry.ifa_broadaddr = (struct sockaddr *)&one->broadcast;
    *list = &one->entry;
    return 0;
}

void PS3Net_freeifaddrs(struct ifaddrs *list)
{
    free(list);                 // the entry is the start of the block
}

const char *PS3Net_inet_ntop(int family, const void *address, char *text, socklen_t size)
{
    const unsigned char *b = address;

    if (family != AF_INET || size < INET_ADDRSTRLEN) {
        errno = EAFNOSUPPORT;
        return NULL;
    }
    snprintf(text, size, "%u.%u.%u.%u", b[0], b[1], b[2], b[3]);
    return text;
}
