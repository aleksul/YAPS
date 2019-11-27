/**************************************************************************
 *  @file     pn532.c
 *  @author   Yehui from Waveshare
 *  @license  BSD
 *  
 *  This is a library for the Waveshare PN532 NFC modules
 *  
 *  Check out the links above for our tutorials and wiring diagrams 
 *  These chips use SPI communicate.
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **************************************************************************/

#include <stdio.h>
#include "pn532.h"
#include "main.h"

const uint8_t PN532_ACK[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
const uint8_t PN532_FRAME_START[] = {0x00, 0x00, 0xFF};

#define PN532_FRAME_MAX_LENGTH              255
#define PN532_DEFAULT_TIMEOUT               1000

/**
  * @brief: Write a frame to the PN532 of at most length bytes in size.
  *     Note that less than length bytes might be returned!
  * @retval: Returns -1 if there is an error parsing the frame.  
  */
int PN532_WriteFrame(PN532* pn532, uint8_t* data, uint16_t length) {
    if (length > PN532_FRAME_MAX_LENGTH || length < 1) {
        return PN532_STATUS_ERROR; // Data must be array of 1 to 255 bytes.
    }
    // Build frame to send as:
    // - Preamble (0x00)
    // - Start code  (0x00, 0xFF)
    // - Command length (1 byte)
    // - Command length checksum
    // - Command bytes
    // - Checksum
    // - Postamble (0x00)

    uint8_t frame[PN532_FRAME_MAX_LENGTH + 7];
    uint8_t checksum = 0;
    frame[0] = PN532_PREAMBLE;
    frame[1] = PN532_STARTCODE1;
    frame[2] = PN532_STARTCODE2;
    for (uint8_t i = 0; i < 3; i++) {
        checksum += frame[i];
    }
    frame[3] = length & 0xFF;
    frame[4] = (~length + 1) & 0xFF;
    for (uint8_t i = 0; i < length; i++) {
        frame[5 + i] = data[i];
        checksum += data[i];
    }
    frame[length + 5] = ~checksum & 0xFF;
    frame[length + 6] = PN532_POSTAMBLE;
    if (pn532->write_data(frame, length + 7) != PN532_STATUS_OK) {
        return PN532_STATUS_ERROR;
    }
    return PN532_STATUS_OK;
}

/**
  * @brief: Read a response frame from the PN532 of at most length bytes in size.
  *     Note that less than length bytes might be returned!
  * @retval: Returns frame length or -1 if there is an error parsing the frame.  
  */
int PN532_ReadFrame(PN532* pn532, uint8_t* response, uint16_t length) {
    uint8_t buff[PN532_FRAME_MAX_LENGTH + 7];
    uint8_t checksum = 0;
    // Read frame with expected length of data.
    pn532->read_data(buff, length + 7);
    // Swallow all the 0x00 values that preceed 0xFF.
    uint8_t offset = 0;
    while (buff[offset] == 0x00) {
        offset += 1;
        if (offset >= length + 8){
            pn532->log("Response frame preamble does not contain 0x00FF!");
            return PN532_STATUS_ERROR;
        }
    }
    if (buff[offset] != 0xFF) {
        pn532->log("Response frame preamble does not contain 0x00FF!");
        return PN532_STATUS_ERROR;
    }
    offset += 1;
    if (offset >= length + 8) {
        pn532->log("Response contains no data!");
        return PN532_STATUS_ERROR;
    }
    // Check length & length checksum match.
    uint8_t frame_len = buff[offset];
    if (((frame_len + buff[offset+1]) & 0xFF) != 0) {
        pn532->log("Response length checksum did not match length!");
        return PN532_STATUS_ERROR;
    }
    // Check frame checksum value matches bytes.
    for (uint8_t i = 0; i < frame_len + 1; i++) {
        checksum += buff[offset + 2 + i];
    }
    checksum &= 0xFF;
    if (checksum != 0) {
        pn532->log("Response checksum did not match expected checksum");
        return PN532_STATUS_ERROR;
    }
    // Return frame data.
    for (uint8_t i = 0; i < frame_len; i++) {
        response[i] = buff[offset + 2 + i];
    }
    return frame_len;
}

/**
  * @brief: Send specified command to the PN532 and expect up to response_length.
  *     Will wait up to timeout seconds for a response and read a bytearray into
  *     response buffer.
  * @param pn532: PN532 handler
  * @param command: command to send
  * @param response: buffer returned
  * @param response_length: expected response length
  * @param params: can optionally specify an array of bytes to send as parameters
  *     to the function call, or NULL if there is no need to send parameters.
  * @param params_length: length of the argument params
  * @param timeout: timout of systick
  * @retval: Returns the length of response or -1 if error.
  */
int PN532_CallFunction(
    PN532* pn532,
    uint8_t command,
    uint8_t* response,
    uint16_t response_length,
    uint8_t* params,
    uint16_t params_length,
    uint32_t timeout
) {
    // Build frame data with command and parameters.
    uint8_t buff[PN532_FRAME_MAX_LENGTH];
    buff[0] = PN532_HOSTTOPN532;
    buff[1] = command & 0xFF;
    for (uint8_t i = 0; i < params_length; i++) {
        buff[2 + i] = params[i];
    }
    // Send frame and wait for response.
    if (PN532_WriteFrame(pn532, buff, params_length + 2) != PN532_STATUS_OK) {
        pn532->wakeup();
        pn532->log("Trying to wakeup");
        return PN532_STATUS_ERROR;
    }
    if (!pn532->wait_ready(timeout)) {
        return PN532_STATUS_ERROR;
    }
    // Verify ACK response and wait to be ready for function response.
    pn532->read_data(buff, sizeof(PN532_ACK));
    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++) {
        if (PN532_ACK[i] != buff[i]) {
            pn532->log("Did not receive expected ACK from PN532!");
            return PN532_STATUS_ERROR;
        }
    }
    if (!pn532->wait_ready(timeout)) {
        return PN532_STATUS_ERROR;
    }
    // Read response bytes.
    int frame_len = PN532_ReadFrame(pn532, buff, response_length + 2);

    // Check that response is for the called function.
    if (! ((buff[0] == PN532_PN532TOHOST) && (buff[1] == (command+1)))) {
        pn532->log("Received unexpected command response!");
        return PN532_STATUS_ERROR;
    }
    // Return response data.
    for (uint8_t i = 0; i < response_length; i++) {
        response[i] = buff[i + 2];
    }
    // The the number of bytes read
    return frame_len - 2;
}

/**
  * @brief: Call PN532 GetFirmwareVersion function and return a buff with the IC,
  *  Ver, Rev, and Support values.
  */
int PN532_GetFirmwareVersion(PN532* pn532, uint8_t* version) {
    // length of version: 4
    if (PN532_CallFunction(pn532, PN532_COMMAND_GETFIRMWAREVERSION,
                           version, 4, NULL, 0, 500) == PN532_STATUS_ERROR) {
        pn532->log("Failed to detect the PN532");
        return PN532_STATUS_ERROR;
    }
    return PN532_STATUS_OK;
}

/**
  * @brief: Configure the PN532 to read MiFare cards.
  */
int PN532_SamConfiguration(PN532* pn532) {
    // Send SAM configuration command with configuration for:
    // - 0x01, normal mode
    // - 0x14, timeout 50ms * 20 = 1 second
    // - 0x01, use IRQ pin
    // Note that no other verification is necessary as call_function will
    // check the command was executed as expected.
    uint8_t params[] = {0x01, 0x14, 0x01};
    PN532_CallFunction(pn532, PN532_COMMAND_SAMCONFIGURATION,
                       NULL, 0, params, sizeof(params), PN532_DEFAULT_TIMEOUT);
    return PN532_STATUS_OK;
}

/**
  * @brief: Wait for a MiFare card to be available and return its UID when found.
  *     Will wait up to timeout seconds and return None if no card is found,
  *     otherwise a bytearray with the UID of the found card is returned.
  * @retval: Length of UID, or -1 if error.
  */
int PN532_ReadPassiveTarget(
    PN532* pn532,
    uint8_t* response,
    uint8_t card_baud,
    uint32_t timeout
) {
    // Send passive read command for 1 card.  Expect at most a 7 byte UUID.
    uint8_t params[] = {0x01, card_baud};
    uint8_t buff[19];
    int length = PN532_CallFunction(pn532, PN532_COMMAND_INLISTPASSIVETARGET,
                        buff, sizeof(buff), params, sizeof(params), timeout);
    if (length < 0) {
        return PN532_STATUS_ERROR; // No card found
    }
    // Check only 1 card with up to a 7 byte UID is present.
    if (buff[0] != 0x01) {
        pn532->log("More than one card detected!");
        return PN532_STATUS_ERROR;
    }
    if (buff[5] > 7) {
        pn532->log("Found card with unexpectedly long UID!");
        return PN532_STATUS_ERROR;
    }
    for (uint8_t i = 0; i < buff[5]; i++) {
        response[i] = buff[6 + i];
    }
    return buff[5];
}

/**
  * @brief: Authenticate specified block number for a MiFare classic card.
  * @param uid: A byte array with the UID of the card.
  * @param uid_length: Length of the UID of the card.
  * @param block_number: The block to authenticate.
  * @param key_number: The key type (like MIFARE_CMD_AUTH_A or MIFARE_CMD_AUTH_B).
  * @param key: A byte array with the key data.
  * @retval: true if the block was authenticated, or false if not authenticated.
  * @retval: PN532 error code.
  */
int PN532_MifareClassicAuthenticateBlock(
    PN532* pn532,
    uint8_t* uid,
    uint8_t uid_length,
    uint16_t block_number,
    uint16_t key_number,
    uint8_t* key
) {
    // Build parameters for InDataExchange command to authenticate MiFare card.
    uint8_t response[1] = {0xFF};
    uint8_t params[3 + MIFARE_UID_MAX_LENGTH + MIFARE_KEY_LENGTH];
    params[0] = 0x01;
    params[1] = key_number & 0xFF;
    params[2] = block_number & 0xFF;
    // params[3:3+keylen] = key
    for (uint8_t i = 0; i < MIFARE_KEY_LENGTH; i++) {
        params[3 + i] = key[i];
    }
    // params[3+keylen:] = uid
    for (uint8_t i = 0; i < uid_length; i++) {
        params[3 + MIFARE_KEY_LENGTH + i] = uid[i];
    }
    // Send InDataExchange request
    PN532_CallFunction(pn532, PN532_COMMAND_INDATAEXCHANGE, response, sizeof(response),
                       params, 3 + MIFARE_KEY_LENGTH + uid_length, PN532_DEFAULT_TIMEOUT);
    return response[0];
}

/**
  * @brief: Read a block of data from the card. Block number should be the block
  *     to read.
  * @param response: buffer of length 16 returned if the block is successfully read.
  * @param block_number: specify a block to read.
  * @retval: PN532 error code.
  */
int PN532_MifareClassicReadBlock(PN532* pn532, uint8_t* response, uint16_t block_number) {
    uint8_t params[] = {0x01, MIFARE_CMD_READ, block_number & 0xFF};
    uint8_t buff[MIFARE_BLOCK_LENGTH + 1];
    // Send InDataExchange request to read block of MiFare data.
    PN532_CallFunction(pn532, PN532_COMMAND_INDATAEXCHANGE, buff, sizeof(buff),
                       params, sizeof(params), PN532_DEFAULT_TIMEOUT);
    // Check first response is 0x00 to show success.
    if (buff[0] != PN532_ERROR_NONE) {
        return buff[0];
    }
    for (uint8_t i = 0; i < MIFARE_BLOCK_LENGTH; i++) {
        response[i] = buff[i + 1];
    }
    return buff[0];
}
