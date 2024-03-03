/**
 * @file adbms.cpp
 * @author Pranit Arya
 * @brief Communication functions for ADBMScheck IC (LTC6813)
 * @version 0.1
 * @date 2023-10-29
 *
 * Created by Pranit Arya 10/29/23
 *
 */

#include <cstdint>
#include "adbms.h"
#include "ltc6813.h"
#include "spi.h"


// Interrupt callbacks
static LTC6813_Error_t bms_error = LTC6813_ERROR;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  bms_error = LTC6813_OK;
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi) {
  bms_error = LTC6813_SPI_ERROR;
}

LTC6813_Error_t ltc6813_cmd_write(LTC6813_Command_t command, uint8_t *data, uint8_t len) {
//  LTC6813_Command_t cmd_buf = command; // PEC15 requires pointer to command
//  uint16_t cmd_crc = 0;//pec15(cmd_buf, 2);
//  uint16_t data_crc = 0;//pec15(data, len);
//  // TODO fix this ^
//
//  // Send command
//  if (HAL_SPI_Transmit(&hspi2, (uint8_t *) &cmd_buf, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
//    return LTC6813_SPI_ERROR;
//  }
//  if (HAL_SPI_Transmit(&hspi2, (uint8_t *) &cmd_crc, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
//    return LTC6813_SPI_ERROR;
//  }
//
//  // TODO: Handle shifting (comm with multiple BMS ICs)
//  if (HAL_SPI_Transmit(&hspi2, data, len, LTC_SPI_TIMEOUT) != HAL_OK) {
//    return LTC6813_SPI_ERROR;
//  }
//  if (HAL_SPI_Transmit(&hspi2, (uint8_t *) &data_crc, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
//    return LTC6813_SPI_ERROR;
//  }
//
  return LTC6813_OK;
}

LTC6813_Error_t ltc6813_cmd_read(LTC6813_Command_t command, uint8_t *data_buf) {
  uint16_t command_int = command;
  LTC6813_Error_t output_status = LTC6813_OK;
  uint16_t received_data_crc;

  uint8_t cmd_buf[2];
  cmd_buf[0] = command_int >> 8;
  cmd_buf[1] = command_int & 0xFF;
  uint16_t cmd_crc = pec15(cmd_buf, 2);
  uint8_t crc_buf[2];
  crc_buf[0] = cmd_crc >> 8;
  crc_buf[1] = cmd_crc & 0xFF;

  HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_RESET);

  // Send command
  if (HAL_SPI_Transmit(&hspi2, cmd_buf, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
    HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_SET);
    return LTC6813_SPI_ERROR;
  }
  if (HAL_SPI_Transmit(&hspi2, crc_buf, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
    HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_SET);
    return LTC6813_SPI_ERROR;
  }

  // Get result from daisy chain, check CRC
  for (int i = 0; i < NUM_BMS_ICS; i++) {
    if (HAL_SPI_Receive(&hspi2, data_buf + (i * 6), 6, LTC_SPI_TIMEOUT) != HAL_OK) {
      HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_SET);
      return LTC6813_SPI_ERROR;
    }
    if (HAL_SPI_Receive(&hspi2, crc_buf, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
      HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_SET);
      return LTC6813_SPI_ERROR;
    }
    uint16_t data_crc = pec15(data_buf, 6);
    received_data_crc = (crc_buf[0] << 8) | crc_buf[1];
    if (data_crc != received_data_crc) {
      output_status = LTC6813_INVALID_DATA;
    }
  }

  HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_SET);

  return output_status;

}

LTC6813_Error_t ltc6813_start_adc_conv(uint8_t cells) {
    ltc6813_adcv(MD_7KHZ_3KHZ, DCP_ENABLED, cells);

    return LTC6813_OK;
}

// data_buf must be >= data size (6 bytes per ADC IC?)
LTC6813_Error_t ltc6813_cmd_read(LTC6813_Command_t command, uint8_t *data_buf) {
    LTC6813_Command_t cmd_buf = command; // PEC15 requires pointer to command
    LTC6813_Error_t output_status = LTC6813_OK;
    uint16_t received_data_crc;
    uint16_t cmd_crc = pec15((char *) cmd_buf, 2);

    // Send command
    if(HAL_SPI_Transmit(&hspi1, (uint8_t *) &cmd_buf, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
        return LTC6813_SPI_ERROR;
    }
    if(HAL_SPI_Transmit(&hspi1, (uint8_t *) &cmd_crc, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
        return LTC6813_SPI_ERROR;
    }

LTC6813_Error_t ltc6813_cmd(LTC6813_Command_t command) {
  uint16_t command_int = command;

  uint8_t cmd_buf[2];
  cmd_buf[0] = command_int >> 8;
  cmd_buf[1] = command_int & 0xFF;
  uint16_t cmd_crc = pec15(cmd_buf, 2);
  uint8_t crc_buf[2];
  crc_buf[0] = cmd_crc >> 8;
  crc_buf[1] = cmd_crc & 0xFF;

  HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_RESET);

  // Send command
  if (HAL_SPI_Transmit(&hspi2, cmd_buf, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
    HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_SET);
    return LTC6813_SPI_ERROR;
  }
  if (HAL_SPI_Transmit(&hspi2, crc_buf, 2, LTC_SPI_TIMEOUT) != HAL_OK) {
    HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_SET);
    return LTC6813_SPI_ERROR;
  }
  HAL_GPIO_WritePin(SPI_CS_BMS_GPIO_Port, SPI_CS_BMS_Pin, GPIO_PIN_SET);

  return LTC6813_OK;
}

LTC6813_Error_t ltc6813_adcv(uint8_t channel) {
  return ltc6813_cmd(static_cast<LTC6813_Command_t>(CMD_ADCV + channel));
}

LTC6813_Error_t ltc6813_adax() {
  return ltc6813_cmd(CMD_ADAX_ALL);
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

/**
 * @brief Generate pec15 CRC for LTC6813
 *
 * @param data
 * @param len
 * @return uint16_t
 */
static uint16_t pec15(const uint8_t *data, int len) {
  uint16_t remainder, address;
  remainder = 16; // PEC seed
  for (int i = 0; i < len; i++) {
    address = ((remainder >> 7) ^ data[i]) & 0xff; //calculate PEC table address
    remainder = (remainder << 8) ^ pec15Table[address];
  }
  return (remainder * 2);//The CRC15 has a 0 in the LSB so the final value must be multiplied by 2
}

// *******************************************************************************************************************