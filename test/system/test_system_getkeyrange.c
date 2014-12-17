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
#include "byte_array.h"
#include "unity.h"
#include "unity_helper.h"
#include "system_test_fixture.h"
#include "protobuf-c/protobuf-c.h"
#include "socket99.h"
#include <string.h>
#include <stdlib.h>

#include "kinetic_client.h"
#include "kinetic_types.h"
#include "kinetic_types_internal.h"
#include "kinetic_controller.h"
#include "kinetic_device_info.h"
#include "kinetic_serial_allocator.h"
#include "kinetic_proto.h"
#include "kinetic_allocator.h"
#include "kinetic_message.h"
#include "kinetic_pdu.h"
#include "kinetic_logger.h"
#include "kinetic_operation.h"
#include "kinetic_hmac.h"
#include "kinetic_connection.h"
#include "kinetic_socket.h"
#include "kinetic_nbo.h"

static bool suiteInitialized = false;
static SystemTestFixture Fixture;

static bool add_keys(int count)
{
    static const ssize_t sz = 10;
    char key_buf[sz];
    char value_buf[sz];

    for (int i = 0; i < count; i++) {

        ByteBuffer KeyBuffer = ByteBuffer_CreateAndAppendFormattedCString(key_buf, sz, "mykey_%02d", i);
        ByteBuffer ValueBuffer = ByteBuffer_CreateAndAppendFormattedCString(value_buf, sz, "val_%02d", i);

        KineticEntry entry = {
            .key = KeyBuffer,
            .value = ValueBuffer,
            .algorithm = KINETIC_ALGORITHM_SHA1,
            .force = true,
        };

        KineticStatus status = KineticClient_Put(&Fixture.session, &entry, NULL);
        if (KINETIC_STATUS_SUCCESS != status) { return false; }
    }
    return true;
}

void setUp(void)
{
    if (!suiteInitialized) {
        SystemTestSetup(&Fixture, 2);
        add_keys(3);
    }
}

void tearDown(void)
{
    LOG_LOCATION;
    SystemTestTearDown(&Fixture);
}

void test_GetKeyRange_should_retrieve_a_range_of_keys_from_device(void)
{
    const size_t numKeys = 3;
    const size_t keyLen = 64;
    uint8_t startKeyData[keyLen], endKeyData[keyLen];
    KineticKeyRange range = {
        .startKey = ByteBuffer_CreateAndAppendCString(startKeyData, sizeof(startKeyData), "mykey_00"),
        .endKey = ByteBuffer_CreateAndAppendCString(endKeyData, sizeof(endKeyData), "mykey_01"),
        .startKeyInclusive = true,
        .endKeyInclusive = true,
        .maxReturned = 2,
    };
    uint8_t keysData[numKeys][keyLen];
    for (size_t i = 0; i < numKeys; i++) {
        memset(&keysData[i], 0, keyLen);
    }
    ByteBuffer keyBuff[] = {
        ByteBuffer_Create(&keysData[0], keyLen, 0),
        ByteBuffer_Create(&keysData[1], keyLen, 0),
        ByteBuffer_Create(&keysData[2], keyLen, 0),
    };
    ByteBufferArray keys = {.buffers = &keyBuff[0], .count = numKeys};

    KineticStatus status = KineticClient_GetKeyRange(&Fixture.session, &range, &keys, NULL);

    TEST_ASSERT_EQUAL_KineticStatus(KINETIC_STATUS_SUCCESS, status);
    TEST_ASSERT_EQUAL(2, keys.used);
    TEST_ASSERT_EQUAL_STRING("mykey_00", keyBuff[0].array.data);
    TEST_ASSERT_EQUAL(strlen("mykey_00"), keyBuff[0].bytesUsed);
    TEST_ASSERT_EQUAL_STRING("mykey_01", keyBuff[1].array.data);
    TEST_ASSERT_EQUAL(strlen("mykey_01"), keyBuff[1].bytesUsed);
    TEST_ASSERT_ByteBuffer_EMPTY(keyBuff[2]);
}

void test_GetKeyRange_should_retrieve_a_range_of_keys_from_device_in_reverse_order(void)
{
    const size_t numKeys = 3;
    const size_t keyLen = 64;
    uint8_t startKeyData[keyLen], endKeyData[keyLen];
    KineticKeyRange range = {
        .startKey = ByteBuffer_CreateAndAppendCString(startKeyData, sizeof(startKeyData), "mykey_00"),
        .endKey = ByteBuffer_CreateAndAppendCString(endKeyData, sizeof(endKeyData), "mykey_02"),
        .startKeyInclusive = true,
        .endKeyInclusive = true,
        .maxReturned = 3,
        .reverse = true,
    };
    uint8_t keysData[numKeys][keyLen];
    for (size_t i = 0; i < numKeys; i++) {
        memset(&keysData[i], 0, keyLen);
    }
    ByteBuffer keyBuff[] = {
        ByteBuffer_Create(&keysData[0], keyLen, 0),
        ByteBuffer_Create(&keysData[1], keyLen, 0),
        ByteBuffer_Create(&keysData[2], keyLen, 0),
    };
    ByteBufferArray keys = {.buffers = &keyBuff[0], .count = numKeys};

    KineticStatus status = KineticClient_GetKeyRange(&Fixture.session, &range, &keys, NULL);

    TEST_ASSERT_EQUAL_KineticStatus(KINETIC_STATUS_SUCCESS, status);
    TEST_ASSERT_EQUAL(3, keys.used);
    TEST_ASSERT_EQUAL_STRING("mykey_02", keyBuff[0].array.data);
    TEST_ASSERT_EQUAL(strlen("mykey_02"), keyBuff[0].bytesUsed);
    TEST_ASSERT_EQUAL_STRING("mykey_01", keyBuff[1].array.data);
    TEST_ASSERT_EQUAL(strlen("mykey_01"), keyBuff[1].bytesUsed);
    TEST_ASSERT_EQUAL_STRING("mykey_00", keyBuff[2].array.data);
    TEST_ASSERT_EQUAL(strlen("mykey_00"), keyBuff[2].bytesUsed);
}

void test_GetKeyRange_should_retrieve_a_range_of_keys_from_device_with_start_and_end_keys_excluded(void)
{
    const size_t numKeys = 3;
    const size_t keyLen = 64;
    uint8_t startKeyData[keyLen], endKeyData[keyLen];
    KineticKeyRange range = {
        .startKey = ByteBuffer_CreateAndAppendCString(startKeyData, sizeof(startKeyData), "mykey_00"),
        .endKey = ByteBuffer_CreateAndAppendCString(endKeyData, sizeof(endKeyData), "mykey_02"),
        .startKeyInclusive = false,
        .endKeyInclusive = false,
        .maxReturned = 2,
    };
    uint8_t keysData[numKeys][keyLen];
    for (size_t i = 0; i < numKeys; i++) {
        memset(&keysData[i], 0, keyLen);
    }
    ByteBuffer keyBuff[] = {
        ByteBuffer_Create(&keysData[0], keyLen, 0),
        ByteBuffer_Create(&keysData[1], keyLen, 0),
        ByteBuffer_Create(&keysData[2], keyLen, 0),
    };
    ByteBufferArray keys = {.buffers = &keyBuff[0], .count = numKeys};

    KineticStatus status = KineticClient_GetKeyRange(&Fixture.session, &range, &keys, NULL);

    TEST_ASSERT_EQUAL_KineticStatus(KINETIC_STATUS_SUCCESS, status);
    TEST_ASSERT_EQUAL(1, keys.used);
    TEST_ASSERT_EQUAL_STRING("mykey_01", keyBuff[0].array.data);
    TEST_ASSERT_EQUAL(strlen("mykey_01"), keyBuff[0].bytesUsed);
    TEST_ASSERT_ByteBuffer_EMPTY(keyBuff[1]);
    TEST_ASSERT_ByteBuffer_EMPTY(keyBuff[2]);
}

/*******************************************************************************
* ENSURE THIS IS AFTER ALL TESTS IN THE TEST SUITE
*******************************************************************************/
SYSTEM_TEST_SUITE_TEARDOWN(&Fixture)