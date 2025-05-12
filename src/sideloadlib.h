#ifndef SIDELOADLIB_H
#define SIDELOADLIB_H

#include "ASideLoad.h"


int wait_for_sideload_device(int *stop_flag);

int sideload_send_command(QByteArray cmd, QByteArray &output, QByteArray data = 0);

int sideload_connect(QByteArray init_cmd);

int sideload_send_cmd(QByteArray cmd, QByteArray &output);

#endif // SIDELOADLIB_H
