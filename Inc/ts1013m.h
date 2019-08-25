/**
 * @section LICENSE
 *
 * Copyright (c) 2013 @tosihisa, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * Fingerprint reader module "GT-511C3" class.
 *
 * http://www.adh-tech.com.tw/?22,gt-511c3-gt-511c31
 * http://www.adh-tech.com.tw/files/GT-511C3_datasheet_V1%201_20131127[1].pdf
 * https://www.sparkfun.com/products/11792
 * https://github.com/sparkfun/Fingerprint_Scanner-TTL/ 
 * http://blog.digit-parts.com/archives/51894096.html
 */
#ifndef __TS1013M_H
#define __TS1013M_H

#include "stm32f0xx_hal.h"
#include "commondef.h"

#define TS1013_PWR_GPIO_PIN                   GPIO_PIN_1
#define TS1013_PWR_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()

#define TS1013_PWR_0 HAL_GPIO_WritePin(GPIOA,TS1013_PWR_GPIO_PIN,GPIO_PIN_RESET)
#define TS1013_PWR_1 HAL_GPIO_WritePin(GPIOA,TS1013_PWR_GPIO_PIN,GPIO_PIN_SET)

    enum Command {
        CMD_Enroll1 = 0x01, // 01 Open Initialization 
				CMD_Enroll2 = 0x02, // 01 Open Initialization
				CMD_Enroll3 = 0x03, // 01 Open Initialization 
        CMD_DeleteID = 0x04, // 40 DeleteID Delete the fingerprint with the specified ID 
        CMD_DeleteAll = 0x05, // 41 DeleteAll Delete all fingerprints from the database  
        CMD_GetEnrollCount = 0x09, // 20 GetEnrollCount Get enrolled fingerprint count 
        CMD_CheckEnrolled = 0x0a, // 21 CheckEnrolled Check whether the specified ID is already enrolled 
        CMD_Verify = 0x0b, // 50 Verify 1:1 Verification of the capture fingerprint image with the specified ID 
        CMD_Identify = 0x0c, // 51 Identify 1:N Identification of the capture fingerprint image with the database 
				CMD_GetEmptyId = 0x0d,
				CMD_SetMode = 0x2d,
				CMD_Cancel = 0xfe,
    };
    enum Error {
		ACK_SUCCESS = 0x00, //执行成功
		ACK_FAIL = 0x01, //执行失败
		ACK_FULL = 0x04, //数据库满
		ACK_NOUSER = 0x05, //没有这个用户
		ACK_USER_EXIST = 0x07, //用户已存在
		ACK_TIMEOUT = 0x08, //图像采集超时
		ACK_BREAK = 0x18, //终止当前指令
		ACK_IMAGEFAIL = 0x84, //图像不合格
		
    };
		bool TSM1013M_shakehand(uint16_t timeov);
		void TS1013M_Poweron();
		void TS1013M_Poweroff();
    int TS1013M_Init(void);
		void TS1013M_DeInit(void);
    int TS1013M_SendCommand(uint32_t Parameter,uint8_t Command);
    int TS1013M_RecvResponse(uint32_t *Parameter,uint8_t *Response);
    int TS1013M_SendRecv(uint8_t Command,uint32_t *Parameter,uint8_t *Response);
    int TS1013M_ClearLine(void);
    int TS1013M_Open(void);
    int TS1013M_Identify(uint16_t* id);
    int TS1013M_Enroll(int ID);
    int TS1013M_CheckEnrolled(int ID);
    int TS1013M_DeleteID(int ID);
		int TS1013M_DeleteALL();
		int TS1013M_GetEnrollCount();
		int TS1013M_GetEmptyId();
		int TS1013M_SetMode();
		bool TS1013M_Exit();
 
#endif  //__TS1013M_HPP