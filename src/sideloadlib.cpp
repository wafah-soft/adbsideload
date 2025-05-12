#include "sideloadlib.h"

int wait_for_sideload_device(int *stop_flag)
{
    return ASideload::sideload_connect_dev(stop_flag);
}

int sideload_send_command(QByteArray cmd, QByteArray &output, QByteArray data)
{
    ASideload::sideload_open_device();
    return ASideload::adb_sideload_send(cmd, output, data);
}

int sideload_connect(QByteArray init_cmd)
{
    return ASideload::sideload_connect(init_cmd);
}

int sideload_send_cmd(QByteArray cmd, QByteArray &output)
{
    return ASideload::sideload_send_cmd(cmd, output);
}

