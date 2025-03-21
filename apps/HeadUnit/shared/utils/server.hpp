// Copyright (C) 2014-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef SERVER_HPP
#define SERVER_HPP

//Vehicle value. speed & battery using same eventgroup id
#define VEHICLE_SERVICE_ID      0x1234
#define VEHICLE_EVENTGROUP_ID   0x4465

//speed value
#define SPEED_INSTANCE_ID       0x2001
#define SPEED_EVENT_ID          0x8779

//battery value
#define BATTERY_INSTANCE_ID     0x3001
#define BATTERY_EVENT_ID        0x8780

// gear value
#define GEAR_INSTANCE_ID        0x4001
#define GEAR_EVENT_ID           0x8781

// Ambient value
#define AL_INSTANCE_ID          0x5001
#define AL_EVENT_ID             0x8782

#define SAMPLE_INSTANCE_ID      0x5678

// 공통 메서드 ID
#define GET_METHOD_ID           0x0001
#define SET_METHOD_ID           0x0002

#define GEAR_SET_METHOD_ID      0x0421
#define JOY_GEAR_SET_MID        0x0423
#define JOY_GEAR_RESPONSE_MID   0x0425

#define AL_SET_METHOD_ID        0x0427

#endif // SERVER_HPP

//previous

// #define SAMPLE_SERVICE_ID       0x1234
// #define SAMPLE_INSTANCE_ID      0x5678
// #define SAMPLE_METHOD_ID        0x0421
// #define SAMPLE_EVENTGROUP_ID    0x4465


// #define SPEED_INSTANCE_ID      0x2001
// #define BATTERY_INSTANCE_ID      0x3001


// #define SAMPLE_EVENT_ID         0x8778

// #define SAMPLE_GET_METHOD_ID    0x0001
// #define SAMPLE_SET_METHOD_ID    0x0002

// #endif // SERVER_HPP
