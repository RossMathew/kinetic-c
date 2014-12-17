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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/

#include "kinetic_admin_client.h"
#include "kinetic_types_internal.h"
#include "kinetic_controller.h"
#include "kinetic_operation.h"
#include "kinetic_auth.h"

void KineticAdminClient_Init(const char* log_file, int log_level)
{
    KineticClient_Init(log_file, log_level);
}

void KineticAdminClient_Shutdown(void)
{
    KineticClient_Shutdown();
}

KineticStatus KineticAdminClient_CreateConnection(KineticSession* const session)
{
    return KineticClient_CreateConnection(session);
}

KineticStatus KineticAdminClient_DestroyConnection(KineticSession* const session)
{
    return KineticClient_DestroyConnection(session);
}


KineticStatus KineticAdminClient_GetLog(KineticSession const * const session,
                                   KineticDeviceInfo_Type type,
                                   KineticDeviceInfo** info,
                                   KineticCompletionClosure* closure)
{
    assert(session != NULL);
    assert(session->connection != NULL);
    assert(info != NULL);

    KineticOperation* operation = KineticController_CreateOperation(session);
    if (operation == NULL) {return KINETIC_STATUS_MEMORY_ERROR;}

    // Initialize request
    KineticOperation_BuildGetLog(operation, type, info);

    // Execute the operation
    return KineticController_ExecuteOperation(operation, closure);
}

KineticStatus KineticAdminClient_InstantSecureErase(KineticSession const * const session)
{
    assert(session != NULL);
    assert(session->connection != NULL);

    KineticStatus status;
    status = KineticAuth_EnsurePinSupplied(session);
    if (status != KINETIC_STATUS_SUCCESS) {return status;}
    status = KineticAuth_EnsureSslEnabled(session);
    if (status != KINETIC_STATUS_SUCCESS) {return status;}

    KineticOperation* operation = KineticController_CreateOperation(session);
    if (operation == NULL) {return KINETIC_STATUS_MEMORY_ERROR;}

    KineticOperation_BuildInstantSecureErase(operation);
    return KineticController_ExecuteOperation(operation, NULL);
}