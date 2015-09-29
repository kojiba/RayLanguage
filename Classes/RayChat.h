/**
 * RayChat.h
 * Simple chat on RTCPHandler.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 9/11/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __RAY_CHAT__
#define __RAY_CHAT__

#include <RayFoundation/RayFoundation.h>

#ifndef RAY_EMBEDDED
void startServer(RTCPHandler  **server,
                 RTCPDelegate **delegate,
                 u16 serverPort,
                 u16 configuratorPort, RString *password);

#endif

#endif /*__RAY_CHAT__*/
