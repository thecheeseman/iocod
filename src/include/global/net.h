/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

/**
 * @file net.h
 * @date 2022-02-13
*/

#ifndef __NET_H__
#define __NET_H__

struct msg {
    bool allow_overflow;     // if false, do a Com_Error
    bool overflowed;        // set to true if the buffer size failed (with allowoverflow set)
    bool oob;               // set to true if the buffer size failed (with allowoverflow set)
    byte *data;
    int maxsize;
    int cursize;
    int uncompsize;             // NERVE - SMF - net debugging
    int readcount;
    int bit;                    // for bitwise reads and writes
};

enum netadr_type {
    NA_BOT,
    NA_BAD,                 // an address lookup failed
    NA_LOOPBACK,
    NA_BROADCAST,
    NA_IP,
    NA_IPX,
    NA_BROADCAST_IPX
};

enum netsrc_type {
    NS_CLIENT,
    NS_SERVER
};

struct netadr {
    enum netadr_type type;

    byte ip[4];
    byte ipx[10];

    unsigned short port;
};

// net profiling
struct netprofile {
    char data[1504];
};

struct netchan {
    int sock;
    int dropped;

    struct netadr remote_address;

    int qport;
    int incoming_sequence;
    int outgoing_sequence;

    int fragment_sequence;
    int fragment_length;
    char fragment_buffer[MAX_MSGLEN];

    int unsent_fragments;
    int unsent_fragment_start;
    int unsent_length;
    char unsent_buffer[MAX_MSGLEN];

    struct netprofile *netprofile;
};

//
//
// network/msg.c
//
//
void huff_decompress(struct msg *msg, int a);
void msg_begin_reading_oob(struct msg *msg);
char *msg_read_string_line(struct msg *msg);
int msg_read_long(struct msg *msg);

//
//
// network/net.c
//
//
bool net_compare_base_address(struct netadr a, struct netadr b);
bool net_compare_address(struct netadr a, struct netadr b);
const char *net_address_to_string(struct netadr a);

#endif // __NET_H__

