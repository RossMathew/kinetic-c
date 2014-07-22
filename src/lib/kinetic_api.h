/*
* kinetic-c
* Copyright (C) 2014 Seagate Technology.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
*/

#ifndef _KINETIC_API_H
#define _KINETIC_API_H

#include "kinetic_types.h"
#include "kinetic_pdu.h"

void KineticApi_Init(const char* log_file);
const KineticConnection KineticApi_Connect(const char* host, int port, bool blocking);
KineticProto_Status_StatusCode KineticApi_SendNoop(
    KineticPDU* const request, KineticPDU* const response,
    uint8_t* const requestBuffer, uint8_t* const responseBuffer);

#endif // _KINETIC_API_H
