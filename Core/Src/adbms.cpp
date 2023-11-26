/**
 * @file adbms.cpp
 * @author Pranit Arya
 * @brief Communication functions for ADBMS IC (LTC6813)
 * @version 0.1
 * @date 2023-10-29
 *
 * Created by Pranit Arya 10/29/23
 *
 */

#include <iostream>
#include <cstdint>
#include "ADBMS.h"
#include "spi.h"

// Interrupt callbacks
static LTC6813_Error_t bms_error = LTC6813_ERROR;
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    bms_error = LTC6813_OK;
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
    bms_error = LTC6813_SPI_ERROR;
}

extern SPI_HandleTypeDef hspi1; // BMS SPI

LTC6813_Error_t ltc6813_cmd_write(LTC6813_Command_t command, uint8_t *data, uint8_t len) {
    LTC6813_Command_t cmd_buf = command; // PEC15 requires pointer to command
    uint16_t cmd_crc = pec15((char *) cmd_buf, 2);
    uint16_t data_crc = pec15((char *) data, len);
    HAL_SPI_Transmit_IT(&hspi1, (uint8_t *) &cmd_buf, 2);
    if(bms_error != LTC6813_OK) {
        return bms_error;
    }
    HAL_SPI_Transmit_IT(&hspi1, (uint8_t *) &cmd_crc, 2);
    if(bms_error != LTC6813_OK) {
        return bms_error;
    }
    // TODO: Handle shifting (comm with multiple BMS ICs)
    HAL_SPI_Transmit_IT(&hspi1, data, len);
    if(bms_error != LTC6813_OK) {
        return bms_error;
    }
    HAL_SPI_Transmit_IT(&hspi1, (uint8_t *) &data_crc, 2);
    if(bms_error != LTC6813_OK) {
        return bms_error;
    }

    return bms_error;
}

/************************************************
 * ADI-given PEC15 Code
*************************************************/

/************************************
Copyright 2012 Analog Devices, Inc. (ADI)
Permission to freely use, copy, modify, and distribute this software for any purpose with or
without fee is hereby granted, provided that the above copyright notice and this permission
notice appear in all copies: THIS SOFTWARE IS PROVIDED “AS IS” AND ADI DISCLAIMS ALL WARRANTIES
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL ADI BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM ANY
USE OF SAME, INCLUDING ANY LOSS OF USE OR DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
***************************************/

uint16_t pec15 (char *data , int len) {
    uint16_t remainder, address;
    remainder = 16; // PEC seed
    for (int i = 0; i < len; i++) {
        address = ((remainder >> 7) ^ data[i]) & 0xff; //calculate PEC table address
        remainder = (remainder << 8 ) ^ pec15Table[address];
    }
    return (remainder*2);//The CRC15 has a 0 in the LSB so the final value must be multiplied by 2
}

// *******************************************************************************************************************