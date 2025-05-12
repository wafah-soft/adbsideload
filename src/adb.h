#ifndef ADB_H
#define ADB_H


#include <QtCore>

struct adb_device_t {
    quint32     local_id;
    quint32     remote_id;
    void        *buffer;
    quint32     max_len;

    quint8      target_epin;
    quint8      target_epout;
    quint8      host_epin;
    quint8      host_epout;
};

#if 1
#define A_SYNC 0x434e5953
#define A_CNXN 0x4e584e43
#define A_OPEN 0x4e45504f
#define A_OKAY 0x59414b4f
#define A_CLSE 0x45534c43
#define A_WRTE 0x45545257
#define A_AUTH 0x48545541
//#define S_ID_LOCAL  0x00003456
/* AUTH packets first argument */
/* Request */
#define ADB_AUTH_TOKEN 1
/* Response */
#define ADB_AUTH_SIGNATURE 2
#define ADB_AUTH_RSAPUBLICKEY 3

#define A_VERSION 0x01000000 // ADB protocol version

#define ADB_VERSION_MAJOR 1 // Used for help/version information
#define ADB_VERSION_MINOR 0 // Used for help/version information

#else
#define A_SYNC 0x53594e43
#define A_CNXN 0x434e584e
#define A_OPEN 0x4f50454e
#define A_OKAY 0x4f4b4159
#define A_CLSE 0x434c5345
#define A_WRTE 0x57525445

#define A_VERSION 0x00000001 // ADB protocol version


#define ADB_VERSION_MAJOR 1 // Used for help/version information
#define ADB_VERSION_MINOR 0 // Used for help/version information

#endif

#define A_CLOSE 0x00000006 // ADB protocol version


typedef struct adb_packet {
    quint32     cmd;
    quint32     arg0;
    quint32     arg1;
    quint32     data_length;
    quint32     crc;
    quint32     magic;
} adb_packet;



#endif // ADB_H
