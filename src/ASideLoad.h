#ifndef ASideload_H
#define ASideload_H

#include <QString>
#include "usb.h"
#include "adb.h"


class ASideload
{
public:
    ASideload();

    static int sideload_connect_dev(int *stop_flag);

    static usb_handle *sideload_open_device();

    static int sideload_scan_device();

    static int sideload_exec(QByteArray cmd, QByteArray &output, QByteArray payload);

    static int sideload_connect(QByteArray cmd);

    static int sideload_send_cmd(QByteArray cmd, QByteArray &output);

    static int sideload_disconnect(quint32 remote_id);

    static int adb_sideload_send(QByteArray cmd, QByteArray &output, QByteArray data);

    static int send_packet(adb_packet *data, QByteArray cmd = nullptr);

    static int adb_read(adb_packet *data, quint32 chk, QByteArray &output, int retry = 10);

    static int adb_read_data(adb_packet *data, QByteArray &output);

private:
    static int list_devices_callback(usb_ifc_info *info);
    static int search_sideload(usb_ifc_info *info);


private:
};

#endif // ASideload_H
