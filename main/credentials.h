/*

Credentials file

*/

#pragma once

// Only one of these settings must be defined
//#define USE_ABP
#define USE_OTAA

#ifdef USE_ABP

    // LoRaWAN NwkSKey, network session key
    static const u1_t PROGMEM NWKSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // LoRaWAN AppSKey, application session key
    static const u1_t PROGMEM APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // LoRaWAN end-device address (DevAddr)
    // This has to be unique for every node
    static const u4_t DEVADDR = 0x00000000;

#endif

#ifdef USE_OTAA

    // This EUI must be in little-endian format, so least-significant-byte (lsb)
    // first. When copying an EUI from ttnctl output, this means to reverse
    // the bytes. For TTN issued EUIs the last bytes should be 0x00, 0x00,
    // 0x00.
    //static const u1_t PROGMEM APPEUI[8]  = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x04, 0xD4, 0x99 };
    static const u1_t PROGMEM APPEUI[8]  = { 0xDF, 0xF5, 0xF6, 0x5F, 0xFF, 0xD6, 0x64, 0x5D };
    // This should also be in little endian format (lsb), see above.
    // Note: You do not need to set this field, if unset it will be generated automatically based on the device macaddr
    //static u1_t DEVEUI[8]  = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x07, 0x0F, 0xAA };
    static u1_t DEVEUI[8]  = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x07, 0x0F, 0xC1 };
    // This key should be in big endian format (msb) (or, since it is not really a
    // number but a block of memory, endianness does not really apply). In
    // practice, a key taken from ttnctl can be copied as-is.
    // The key shown here is the semtech default key.
    //static const u1_t PROGMEM APPKEY[16] = { 0x0B, 0xDD, 0x20, 0xCA, 0x86, 0x7C, 0x9C, 0x8A, 0x7A, 0x04, 0xE5, 0x77, 0xAA, 0xF8, 0xC1, 0x2C };
    static const u1_t PROGMEM APPKEY[16] = { 0x90, 0xA9, 0xFE, 0xF8, 0xC1, 0x8E, 0x64, 0x0A, 0x8D, 0xCB, 0x06, 0xC6, 0xAD, 0x76, 0xB2, 0x66 };
#endif
