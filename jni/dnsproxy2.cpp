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
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>

#include <cutils/log.h>

#include "DnsProxyListener.h"

#define SOCKPATH        "/dev/socket/dnsproxyd"

static int restore_oldsock;

static void die(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    exit(1);
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_un sock;
    struct stat statbuf;
    char buf[16];
    DnsProxyListener *dpl;

    if (stat(SOCKPATH, &statbuf) == 0) {
        unlink(SOCKPATH ".bak");
        rename(SOCKPATH, SOCKPATH ".bak");
        restore_oldsock = 1;
    }

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
        die("socket() failed: %s\n", strerror(errno));

    memset(&sock, 0, sizeof(sock));
    sock.sun_family = AF_UNIX;
    strcpy(sock.sun_path, SOCKPATH);

    if (bind(sockfd, (struct sockaddr *)&sock, sizeof(sock)) < 0)
        die("can't bind: %s\n", strerror(errno));

    sprintf(buf, "%d", sockfd);
    setenv("ANDROID_SOCKET_dnsproxyd", buf, 1);
    setenv("ANDROID_DNS_MODE", "local", 1);

    dpl = new DnsProxyListener();
    if (dpl->startListener()) {
        ALOGE("Unable to start DnsProxyListener (%s)", strerror(errno));
        exit(1);
    }

    // Eventually we'll become the monitoring thread
    while(1) {
        sleep(1000);
    }

    return 0;
}
