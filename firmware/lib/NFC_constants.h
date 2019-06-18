/*
 * \file NFC.h
 *
 *  Created on: Apr 27, 2016
 *  \author Remko Welling, 541858
 *          remko@rfsee.nl
 *
 *
 *	\version 0.8	Initial release for the HACOX project team for code review
 *
 *
 */

#ifndef NFC_H_
#define NFC_H_


#define PN532_PACKBUFFSIZ 64

#define true 1
#define false 0

typedef unsigned char uint8_t;
typedef int uint32_t;
typedef char boolean;
typedef short uint16_t;

const uint8_t pn532ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
const uint8_t pn532response_firmwarevers[] = {0x00, 0xFF, 0x06, 0xFA, 0xD5, 0x03};

const uint8_t PN532_PREAMBLE                      = 0x00;
const uint8_t PN532_STARTCODE1                    = 0x00;
const uint8_t PN532_STARTCODE2                    = 0xFF;
const uint8_t PN532_POSTAMBLE                     = 0x00;

const uint8_t PN532_HOSTTOPN532                   = 0xD4;
const uint8_t PN532_PN532TOHOST                   = 0xD5;

// PN532 Commands
const uint8_t PN532_COMMAND_DIAGNOSE              = 0x00;
const uint8_t PN532_COMMAND_GETFIRMWAREVERSION    = 0x02;
const uint8_t PN532_COMMAND_GETGENERALSTATUS      = 0x04;
const uint8_t PN532_COMMAND_READREGISTER          = 0x06;
const uint8_t PN532_COMMAND_WRITEREGISTER         = 0x08;
const uint8_t PN532_COMMAND_READGPIO              = 0x0C;
const uint8_t PN532_COMMAND_WRITEGPIO             = 0x0E;
const uint8_t PN532_COMMAND_SETSERIALBAUDRATE     = 0x10;
const uint8_t PN532_COMMAND_SETPARAMETERS         = 0x12;
const uint8_t PN532_COMMAND_SAMCONFIGURATION      = 0x14;
const uint8_t PN532_COMMAND_POWERDOWN             = 0x16;
const uint8_t PN532_COMMAND_RFCONFIGURATION       = 0x32;
const uint8_t PN532_COMMAND_RFREGULATIONTEST      = 0x58;
const uint8_t PN532_COMMAND_INJUMPFORDEP          = 0x56;
const uint8_t PN532_COMMAND_INJUMPFORPSL          = 0x46;
const uint8_t PN532_COMMAND_INLISTPASSIVETARGET   = 0x4A;
const uint8_t PN532_COMMAND_INATR                 = 0x50;
const uint8_t PN532_COMMAND_INPSL                 = 0x4E;
const uint8_t PN532_COMMAND_INDATAEXCHANGE        = 0x40;
const uint8_t PN532_COMMAND_INCOMMUNICATETHRU     = 0x42;
const uint8_t PN532_COMMAND_INDESELECT            = 0x44;
const uint8_t PN532_COMMAND_INRELEASE             = 0x52;
const uint8_t PN532_COMMAND_INSELECT              = 0x54;
const uint8_t PN532_COMMAND_INAUTOPOLL            = 0x60;
const uint8_t PN532_COMMAND_TGINITASTARGET        = 0x8C;
const uint8_t PN532_COMMAND_TGSETGENERALBYTES     = 0x92;
const uint8_t PN532_COMMAND_TGGETDATA             = 0x86;
const uint8_t PN532_COMMAND_TGSETDATA             = 0x8E;
const uint8_t PN532_COMMAND_TGSETMETADATA         = 0x94;
const uint8_t PN532_COMMAND_TGGETINITIATORCOMMAND = 0x88;
const uint8_t PN532_COMMAND_TGRESPONSETOINITIATOR = 0x90;
const uint8_t PN532_COMMAND_TGGETTARGETSTATUS     = 0x8A;

const uint8_t PN532_RESPONSE_INDATAEXCHANGE       = 0x41;
const uint8_t PN532_RESPONSE_INLISTPASSIVETARGET  = 0x4B;

const uint8_t PN532_WAKEUP                        = 0x55;

const uint8_t PN532_SPI_STATREAD                  = 0x02;
const uint8_t PN532_SPI_DATAWRITE                 = 0x01;
const uint8_t PN532_SPI_DATAREAD                  = 0x03;
const uint8_t PN532_SPI_READY                     = 0x01;

const uint8_t PN532_I2C_ADDRESS                   = 0x48 >> 1;
const uint8_t PN532_I2C_READBIT                   = 0x01;
const uint8_t PN532_I2C_BUSY                      = 0x00;
const uint8_t PN532_I2C_READY                     = 0x01;
const uint8_t PN532_I2C_READYTIMEOUT              = 20;

const uint8_t PN532_MIFARE_ISO14443A              = 0x00;

const uint8_t PN532_GPIO_VALIDATIONBIT            = 0x80;
const uint8_t PN532_GPIO_P30                      = 0;
const uint8_t PN532_GPIO_P31                      = 1;
const uint8_t PN532_GPIO_P32                      = 2;
const uint8_t PN532_GPIO_P33                      = 3;
const uint8_t PN532_GPIO_P34                      = 4;
const uint8_t PN532_GPIO_P35                      = 5;

// Mifare Commands
const uint8_t MIFARE_CMD_AUTH_A                   = 0x60;
const uint8_t MIFARE_CMD_AUTH_B                   = 0x61;
const uint8_t MIFARE_CMD_READ                     = 0x30;
const uint8_t MIFARE_CMD_WRITE                    = 0xA0;
const uint8_t MIFARE_CMD_TRANSFER                 = 0xB0;
const uint8_t MIFARE_CMD_DECREMENT                = 0xC0;
const uint8_t MIFARE_CMD_INCREMENT                = 0xC1;
const uint8_t MIFARE_CMD_STORE                    = 0xC2;
const uint8_t MIFARE_ULTRALIGHT_CMD_WRITE         = 0xA2;

// Prefixes for NDEF Records = to identify record type;
const uint8_t NDEF_URIPREFIX_NONE                 = 0x00;
const uint8_t NDEF_URIPREFIX_HTTP_WWWDOT          = 0x01;
const uint8_t NDEF_URIPREFIX_HTTPS_WWWDOT         = 0x02;
const uint8_t NDEF_URIPREFIX_HTTP                 = 0x03;
const uint8_t NDEF_URIPREFIX_HTTPS                = 0x04;
const uint8_t NDEF_URIPREFIX_TEL                  = 0x05;
const uint8_t NDEF_URIPREFIX_MAILTO               = 0x06;
const uint8_t NDEF_URIPREFIX_FTP_ANONAT           = 0x07;
const uint8_t NDEF_URIPREFIX_FTP_FTPDOT           = 0x08;
const uint8_t NDEF_URIPREFIX_FTPS                 = 0x09;
const uint8_t NDEF_URIPREFIX_SFTP                 = 0x0A;
const uint8_t NDEF_URIPREFIX_SMB                  = 0x0B;
const uint8_t NDEF_URIPREFIX_NFS                  = 0x0C;
const uint8_t NDEF_URIPREFIX_FTP                  = 0x0D;
const uint8_t NDEF_URIPREFIX_DAV                  = 0x0E;
const uint8_t NDEF_URIPREFIX_NEWS                 = 0x0F;
const uint8_t NDEF_URIPREFIX_TELNET               = 0x10;
const uint8_t NDEF_URIPREFIX_IMAP                 = 0x11;
const uint8_t NDEF_URIPREFIX_RTSP                 = 0x12;
const uint8_t NDEF_URIPREFIX_URN                  = 0x13;
const uint8_t NDEF_URIPREFIX_POP                  = 0x14;
const uint8_t NDEF_URIPREFIX_SIP                  = 0x15;
const uint8_t NDEF_URIPREFIX_SIPS                 = 0x16;
const uint8_t NDEF_URIPREFIX_TFTP                 = 0x17;
const uint8_t NDEF_URIPREFIX_BTSPP                = 0x18;
const uint8_t NDEF_URIPREFIX_BTL2CAP              = 0x19;
const uint8_t NDEF_URIPREFIX_BTGOEP               = 0x1A;
const uint8_t NDEF_URIPREFIX_TCPOBEX              = 0x1B;
const uint8_t NDEF_URIPREFIX_IRDAOBEX             = 0x1C;
const uint8_t NDEF_URIPREFIX_FILE                 = 0x1D;
const uint8_t NDEF_URIPREFIX_URN_EPC_ID           = 0x1E;
const uint8_t NDEF_URIPREFIX_URN_EPC_TAG          = 0x1F;
const uint8_t NDEF_URIPREFIX_URN_EPC_PAT          = 0x20;
const uint8_t NDEF_URIPREFIX_URN_EPC_RAW          = 0x21;
const uint8_t NDEF_URIPREFIX_URN_EPC              = 0x22;
const uint8_t NDEF_URIPREFIX_URN_NFC              = 0x23;

#endif /* NFC_H_ */
