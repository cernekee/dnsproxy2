/* vi:set sw=4 sts=4 expandtab: */
/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <grp.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include <cutils/log.h>

#include "DnsProxyListener.h"
#include "resolv_iface.h"

#define SOCKPATH        "/dev/socket/dnsproxyd"

static int restore_oldsock;

static void handle_signal(int sig)
{
    if (restore_oldsock) {
        /* re-enable the old proxy, then exit */
        unlink(SOCKPATH);
        rename(SOCKPATH ".bak", SOCKPATH);
    }
    exit(0);
}

static void setup_resolver(const char *server)
{
    _resolv_set_default_iface("eth0");
    _resolv_set_nameservers_for_iface("eth0", (char **)&server, 1, "");
}

static void setup_listener(int do_wait)
{
    int sockfd;
    struct sockaddr_un sock;
    struct stat statbuf;
    char buf[16];
    DnsProxyListener *dpl;

    /* wait for netd to start up, then take over its socket */
    while (do_wait && stat(SOCKPATH, &statbuf) < 0) {
        sleep(1);
    }

    if (stat(SOCKPATH, &statbuf) == 0) {
        unlink(SOCKPATH ".bak");
        rename(SOCKPATH, SOCKPATH ".bak");
        restore_oldsock = 1;
    }

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
        ALOGE("socket() failed (%s)\n", strerror(errno));

    memset(&sock, 0, sizeof(sock));
    sock.sun_family = AF_UNIX;
    strcpy(sock.sun_path, SOCKPATH);

    if (bind(sockfd, (struct sockaddr *)&sock, sizeof(sock)) < 0)
        ALOGE("can't bind (%s)\n", strerror(errno));

    sprintf(buf, "%d", sockfd);
    setenv("ANDROID_SOCKET_dnsproxyd", buf, 1);

    dpl = new DnsProxyListener();
    if (dpl->startListener()) {
        ALOGE("Unable to start DnsProxyListener (%s)", strerror(errno));
        exit(1);
    }
}

static void usage(void)
{
    puts("usage: dnsproxy2 [ -w ] [ -v ]");
    exit(1);
}

int main(int argc, char **argv)
{
    int do_wait = 0, val;
    struct group *grp;

    while ((val = getopt(argc, argv, "hwv")) != -1) {
        switch (val) {
        case 'w':
            do_wait = 1;
            break;
        case 'v':
            setenv("RES_OPTIONS", "debug", 1);
            break;
        case 'h':
        default:
            usage();
        }
    }

    grp = getgrnam("inet");
    if (!grp || setgid(grp->gr_gid)) {
        ALOGW("Unable to change to 'inet' group (%s)", strerror(errno));
    }
    umask(0002);

    setenv("ANDROID_DNS_MODE", "local", 1);

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    signal(SIGHUP, handle_signal);

    if (optind < argc)
        setup_resolver(argv[optind]);
    else
        setup_resolver("8.8.8.8");

    setup_listener(do_wait);

    while(1) {
        sleep(1000);
    }

    return 0;
}
