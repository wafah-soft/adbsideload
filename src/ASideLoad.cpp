#include "ASideLoad.h"
#include <QtDebug>
#include <QThread>

static usb_handle *usb;
static int *stop_flag_;

ASideload::ASideload()
{
    usb = nullptr;
    stop_flag_ = 0;
}

int ASideload::sideload_connect_dev(int *stop_flag)
{
    stop_flag_ = stop_flag;
    return sideload_scan_device();
}

usb_handle *ASideload::sideload_open_device() {
    int announce = 1;
    usb = usb_open(search_sideload);
    if(usb != nullptr) return usb;
    if(announce) {
        announce = 0;
    }
    return nullptr;
}

int ASideload::sideload_scan_device()
{
    usb_handle *usb = 0;
    int announce = 1;
    if(usb) return 0;
    usb = usb_open(search_sideload);
    if(usb != nullptr) {
        usb_close(usb);
        return 0;
    }
    if(announce) {
        announce = 0;
        qInfo() << "Waiting for device...";
    }
    QThread::sleep(1);

    return -1;
}

int ASideload::sideload_exec(QByteArray cmd, QByteArray &output, QByteArray payload)
{
    output.clear();

    QByteArray rBuff;

    qint32 status = 0;

    QByteArray init_cmd = "host::";
    init_cmd.append(QChar(0x00));

    adb_packet pack;
    pack.cmd = A_CNXN;
    pack.arg0 = A_VERSION;
    pack.arg1 = 0x00001000;
    pack.data_length = init_cmd.size();
    status = send_packet(&pack, init_cmd);
    if(status < 0)
        return -1;

    memset(&pack, 0, sizeof(adb_packet));
    status = adb_read(&pack, A_CNXN, output);
    if(status < 0)
        return -1;


    memset(&pack, 0, sizeof(adb_packet));
    cmd.append(':');
    if(!payload.isEmpty()) {
        cmd.append(payload);
    }
    cmd.append(QChar(0x00));
    pack.cmd = A_OPEN;
    pack.arg0 = 0x01;
    pack.arg1 = 0;
    pack.data_length = cmd.size();
    status = send_packet(&pack, cmd);
    if(status < 0)
        return -1;

    memset(&pack, 0, sizeof(adb_packet));
    status = adb_read(&pack, A_OKAY, output);
    if(status < 0)
        return -1;

    status = adb_read(&pack, A_WRTE, output, 70);
    if(status < 0)
        return -1;

    memset(&pack, 0, sizeof(adb_packet));
    quint32 remote_id = pack.arg0;
    pack.cmd = A_OKAY;
    pack.arg0 = 0x01;
    pack.arg1 = remote_id;
    pack.crc = 0;
    status = send_packet(&pack);
    if(status < 0)
        return -1;

    memset(&pack, 0, sizeof(adb_packet));
    status = adb_read(&pack, A_CLSE, rBuff);
    if(status < 0)
        return -1;

    status = sideload_disconnect(pack.arg0);

    usb_close(usb);

    return 0;
}

int ASideload::sideload_connect(QByteArray cmd)
{
    QByteArray output;
    qint32 status = 0;
    QByteArray init_cmd = "host::";
    init_cmd.append(QChar(0x00));

    adb_packet pack;
    pack.cmd = A_CNXN;
    pack.arg0 = A_VERSION;
    pack.arg1 = 0x00001000;
    pack.data_length = init_cmd.size();
    status = send_packet(&pack, init_cmd);
    if(status < 0)
        return -1;

    pack = adb_packet();
    status = adb_read(&pack, A_CNXN, output);
    if(status < 0)
        return -1;

    return 0;
}

int ASideload::sideload_send_cmd(QByteArray cmd, QByteArray &output)
{
    cmd.append(':');
    cmd.append(QChar(0x00));

    adb_packet pack;

    pack.cmd = A_OPEN;
    pack.arg0 = 0x01;
    pack.arg1 = 0;
    pack.data_length = cmd.size();
    auto status = send_packet(&pack, cmd);
    if(status < 0)
        return -1;

    pack = adb_packet();
    status = adb_read(&pack, A_CLSE, output);
    if(status < 0)
        return -1;

    return 0;
}

int ASideload::sideload_disconnect(quint32 remote_id)
{
    QByteArray output;
    adb_packet pack;
    memset(&pack, 0, sizeof(adb_packet));
    pack.cmd = A_OKAY;
    pack.arg0 = 0x01;
    pack.arg1 = remote_id;
    pack.crc = 0;
    auto status = send_packet(&pack);
    if(status < 0)
        return -1;

    adb_packet response = adb_packet();
    status = adb_read(&response, A_CLSE, output, 0);
    if(status < 0)
        return -1;

    memset(&pack, 0, sizeof(adb_packet));
    pack.cmd = A_CLSE;
    pack.arg0 = 0x01;
    pack.arg1 = response.arg0;
    pack.crc = 0;
    status = send_packet(&pack);
    if(status < 0)
        return -1;

    response = adb_packet();
    status = adb_read(&response, A_CLSE, output, 0);
    if(status < 0)
        return -1;

    return 0;
}

int ASideload::adb_sideload_send(QByteArray cmd, QByteArray &output, QByteArray data)
{
    auto status = sideload_exec(cmd, output, data);
    if(status < 0) {
        return -1;
    }
    return 0;
}

int ASideload::send_packet(adb_packet *a_pack, QByteArray cmd)
{
    a_pack->magic = a_pack->cmd ^ 0xffffffff;
    a_pack->crc = 0;

    if(!cmd.isEmpty()) {
        qint32 count = cmd.size();
        const quint8 *_data_ptr = (quint8*)cmd.data();
        while(count--) {
            a_pack->crc += *_data_ptr++;
        }
    }

    auto status = usb_write(usb, a_pack, sizeof(adb_packet), stop_flag_);
    if(status < 0)
        return -1;

    if(!cmd.isEmpty()) {
        return usb_write(usb, cmd.data(), cmd.size(), stop_flag_);
    }

    return 0;
}

int ASideload::adb_read(adb_packet *data, quint32 chk, QByteArray &output, int retry)
{
    memset(data, 0, sizeof(adb_packet));
    output.clear();

    do {
        auto status = usb_read(usb, data, sizeof(adb_packet), stop_flag_);
        if(status < 0 || retry-- <= 0)
            return -1;
    }
    while(data->cmd != chk);

    if(data->data_length > 0) {
        output.resize(data->data_length);
        return usb_read(usb, output.data(), data->data_length, stop_flag_);
    }
    return 0;
}

int ASideload::adb_read_data(adb_packet *data, QByteArray &output)
{
    output.clear();
    memset(data, 0, sizeof(adb_packet));
    int status = 0;
    int max_tries = 100; // Timeout after 100 tries

    QByteArray tmp_data;
    tmp_data.resize(24);
    tmp_data.fill(0);
    do {

        status = usb_read(usb, tmp_data.data(), 64, stop_flag_);
        if(status < 0 || max_tries-- <= 0)
            return -1;
        memcpy(data, tmp_data.data(), sizeof(adb_packet));
    } while(data->cmd != A_WRTE);

    if(data->data_length > 0) {
        auto minLen = qMin((quint32)1024*1024, data->data_length);
        output.resize(minLen);
        status = usb_read(usb, output.data(), minLen, stop_flag_);
        if(status < 0)
            return -1;
    }
    return 0;
}

int ASideload::list_devices_callback(usb_ifc_info *info)
{
    return search_sideload(info);
}

int ASideload::search_sideload(usb_ifc_info *info)
{
    if(info->dev_vendor == 0x18d1) {}

    if(info->ifc_class != 0xff) return -1;
    if(info->ifc_subclass != 0x42) return -1;
    if(info->ifc_protocol != 0x01) return -1;


    qInfo() << __func__ << __LINE__ << info->serial_number;

    return 0;
}
