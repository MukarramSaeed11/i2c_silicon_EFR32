/*
 * rc522.h
 *
 *  Created on: 30-Aug-2022
 *      Author: Mukarram Saeed
 */

#ifndef RC522_H_
#define RC522_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include "i2c_init.h"



#define RC522_ADDRESS                   0x28
#define RC522_ADDRESS_WRITE             (RC522_ADDRESS << 1)
#define RC522_ADDRESS_READ              (RC522_ADDRESS_WRITE + 1)





// MFRC522 registers. Described in chapter 9 of the data-sheet.
enum PCD_Register {
  // Page 0: Command and status
  //                    0x00      // reserved for future use
  CommandReg          = 0x01 ,  // starts and stops command execution
  ComIEnReg           = 0x02 ,  // enable and disable interrupt request control bits
  DivIEnReg           = 0x03 ,  // enable and disable interrupt request control bits
  ComIrqReg           = 0x04 ,  // interrupt request bits
  DivIrqReg           = 0x05 ,  // interrupt request bits
  ErrorReg            = 0x06 ,  // error bits showing the error status of the last command executed
  Status1Reg          = 0x07 ,  // communication status bits
  Status2Reg          = 0x08 ,  // receiver and transmitter status bits
  FIFODataReg         = 0x09 ,  // input and output of 64 char FIFO buffer
  FIFOLevelReg        = 0x0A ,  // number of chars stored in the FIFO buffer
  WaterLevelReg       = 0x0B ,  // level for FIFO underflow and overflow warning
  ControlReg          = 0x0C ,  // miscellaneous control registers
  BitFramingReg       = 0x0D ,  // adjustments for bit-oriented frames
  CollReg             = 0x0E ,  // bit position of the first bit-collision detected on the RF interface
  //                    0x0F      // reserved for future use

  // Page 1: Command
  //                    0x10      // reserved for future use
  ModeReg             = 0x11 ,  // defines general modes for transmitting and receiving
  TxModeReg           = 0x12 ,  // defines transmission data rate and framing
  RxModeReg           = 0x13 ,  // defines reception data rate and framing
  TxControlReg        = 0x14 ,  // controls the logical behaviour of the antenna driver pins TX1 and TX2
  TxASKReg            = 0x15 ,  // controls the setting of the transmission modulation
  TxSelReg            = 0x16 ,  // selects the internal sources for the antenna driver
  RxSelReg            = 0x17 ,  // selects internal receiver settings
  RxThresholdReg      = 0x18 ,  // selects thresholds for the bit decoder
  DemodReg            = 0x19 ,  // defines demodulator settings
  //                    0x1A      // reserved for future use
  //                    0x1B      // reserved for future use
  MfTxReg             = 0x1C ,  // controls some MIFARE communication transmit parameters
  MfRxReg             = 0x1D ,  // controls some MIFARE communication receive parameters
  //                    0x1E      // reserved for future use
  SerialSpeedReg      = 0x1F ,  // selects the speed of the serial UART interface

  // Page 2: Configuration
  //                    0x20      // reserved for future use
  CRCResultRegH       = 0x21 ,  // shows the MSB and LSB values of the CRC calculation
  CRCResultRegL       = 0x22 ,
  //                    0x23      // reserved for future use
  ModWidthReg         = 0x24 ,  // controls the ModWidth setting?
  //                    0x25      // reserved for future use
  RFCfgReg            = 0x26 ,  // configures the receiver gain
  GsNReg              = 0x27 ,  // selects the conductance of the antenna driver pins TX1 and TX2 for modulation
  CWGsPReg            = 0x28 ,  // defines the conductance of the p-driver output during periods of no modulation
  ModGsPReg           = 0x29 ,  // defines the conductance of the p-driver output during periods of modulation
  TModeReg            = 0x2A ,  // defines settings for the internal timer
  TPrescalerReg       = 0x2B ,  // the lower 8 bits of the TPrescaler value. The 4 high bits are in TModeReg.
  TReloadRegH         = 0x2C ,  // defines the 16-bit timer reload value
  TReloadRegL         = 0x2D ,
  TCounterValueRegH   = 0x2E ,  // shows the 16-bit timer value
  TCounterValueRegL   = 0x2F ,

};

// MFRC522 commands. Described in chapter 10 of the data-sheet.
enum PCD_Command {
  PCD_Idle              = 0x00,   // no action, cancels current command execution
  PCD_Mem               = 0x01,   // stores 25 chars into the internal buffer
  PCD_GenerateRandomID  = 0x02,   // generates a 10-char random ID number
  PCD_CalcCRC           = 0x03,   // activates the CRC co-processor or performs a self test
  PCD_Transmit          = 0x04,   // transmits data from the FIFO buffer
  PCD_NoCmdChange       = 0x07,   // no command change, can be used to modify the CommandReg register bits without affecting the command, for example, the PowerDown bit
  PCD_Receive           = 0x08,   // activates the receiver circuits
  PCD_Transceive        = 0x0C,   // transmits data from FIFO buffer to antenna and automatically activates the receiver after transmission
  PCD_MFAuthent         = 0x0E,   // performs the MIFARE standard authentication as a reader
  PCD_SoftReset         = 0x0F    // resets the MFRC522
};

// MFRC522 RxGain[2:0] masks, defines the receiver's signal voltage gain factor (on the PCD).
// Described in 9.3.3.6 / table 98 of the data-sheet at http://www.nxp.com/documents/data_sheet/MFRC522.pdf
enum PCD_RxGain {
  RxGain_18dB       = 0x00 << 4,  // 000b - 18 dB, minimum
  RxGain_23dB       = 0x01 << 4,  // 001b - 23 dB
  RxGain_18dB_2     = 0x02 << 4,  // 010b - 18 dB, it seems 010b is a duplicate for 000b
  RxGain_23dB_2     = 0x03 << 4,  // 011b - 23 dB, it seems 011b is a duplicate for 001b
  RxGain_33dB       = 0x04 << 4,  // 100b - 33 dB, average, and typical default
  RxGain_38dB       = 0x05 << 4,  // 101b - 38 dB
  RxGain_43dB       = 0x06 << 4,  // 110b - 43 dB
  RxGain_48dB       = 0x07 << 4,  // 111b - 48 dB, maximum
  RxGain_min        = 0x00 << 4,  // 000b - 18 dB, minimum, convenience for RxGain_18dB
  RxGain_avg        = 0x04 << 4,  // 100b - 33 dB, average, convenience for RxGain_33dB
  RxGain_max        = 0x07 << 4   // 111b - 48 dB, maximum, convenience for RxGain_48dB
};

// Commands sent to the PICC.
enum PICC_Command {
  // The commands used by the PCD to manage communication with several PICCs (ISO 14443-3, Type A, section 6.4)
  PICC_CMD_REQA     = 0x26,   // REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anti-collision or selection. 7 bit frame.
  PICC_CMD_WUPA     = 0x52,   // Wake-UP command, Type A. Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anti-collision or selection. 7 bit frame.
  PICC_CMD_CT       = 0x88,   // Cascade Tag. Not really a command, but used during anti collision.
  PICC_CMD_SEL_CL1  = 0x93,   // Anti collision/Select, Cascade Level 1
  PICC_CMD_SEL_CL2  = 0x95,   // Anti collision/Select, Cascade Level 2
  PICC_CMD_SEL_CL3  = 0x97,   // Anti collision/Select, Cascade Level 3
  PICC_CMD_HLTA     = 0x50,   // HaLT command, Type A. Instructs an ACTIVE PICC to go to state HALT.
  // The commands used for MIFARE Classic (from http://www.nxp.com/documents/data_sheet/MF1S503x.pdf, Section 9)
  // Use PCD_MFAuthent to authenticate access to a sector, then use these commands to read/write/modify the blocks on the sector.
  // The read/write commands can also be used for MIFARE Ultra-light.
  PICC_CMD_MF_AUTH_KEY_A  = 0x60,   // Perform authentication with Key A
  PICC_CMD_MF_AUTH_KEY_B  = 0x61,   // Perform authentication with Key B
  PICC_CMD_MF_READ        = 0x30,   // Reads one 16 char block from the authenticated sector of the PICC. Also used for MIFARE Ultralight.
  PICC_CMD_MF_WRITE       = 0xA0,   // Writes one 16 char block to the authenticated sector of the PICC. Called "COMPATIBILITY WRITE" for MIFARE Ultralight.
  PICC_CMD_MF_DECREMENT   = 0xC0,   // Decrements the contents of a block and stores the result in the internal data register.
  PICC_CMD_MF_INCREMENT   = 0xC1,   // Increments the contents of a block and stores the result in the internal data register.
  PICC_CMD_MF_RESTORE     = 0xC2,   // Reads the contents of a block into the internal data register.
  PICC_CMD_MF_TRANSFER    = 0xB0,   // Writes the contents of the internal data register to a block.
  // The commands used for MIFARE Ultra-light (from http://www.nxp.com/documents/data_sheet/MF0ICU1.pdf, Section 8.6)
  // The PICC_CMD_MF_READ and PICC_CMD_MF_WRITE can also be used for MIFARE Ultra-light.
  PICC_CMD_UL_WRITE       = 0xA2    // Writes one 4 char page to the PICC.
};

// MIFARE constants that does not fit anywhere else
enum MIFARE_Misc {
  MF_ACK            = 0xA,    // The MIFARE Classic uses a 4 bit ACK/NAK. Any other value than 0xA is NAK.
  MF_KEY_SIZE       = 6     // A Mifare Crypto1 key is 6 chars.
};

// PICC types we can detect. Remember to update PICC_GetTypeName() if you add more.
enum PICC_Type {
  PICC_TYPE_UNKNOWN       = 0,
  PICC_TYPE_ISO_14443_4   = 1,  // PICC compliant with ISO/IEC 14443-4
  PICC_TYPE_ISO_18092     = 2,  // PICC compliant with ISO/IEC 18092 (NFC)
  PICC_TYPE_MIFARE_MINI   = 3,  // MIFARE Classic protocol, 320 chars
  PICC_TYPE_MIFARE_1K     = 4,  // MIFARE Classic protocol, 1KB
  PICC_TYPE_MIFARE_4K     = 5,  // MIFARE Classic protocol, 4KB
  PICC_TYPE_MIFARE_UL     = 6,  // MIFARE Ultra-light or Ultra-light C
  PICC_TYPE_MIFARE_PLUS   = 7,  // MIFARE Plus
  PICC_TYPE_TNP3XXX       = 8,  // Only mentioned in NXP AN 10833 MIFARE Type Identification Procedure
  PICC_TYPE_NOT_COMPLETE  = 255 // SAK indicates UID is not complete.
};

// Return codes from the functions in this class. Remember to update GetStatusCodeName() if you add more.
typedef enum{
  STATUS_OK             = 1,  // Success
  STATUS_ERROR          = 2,  // Error in communication
  STATUS_COLLISION      = 3,  // Collision detected
  STATUS_TIMEOUT        = 4,  // Timeout in communication.
  STATUS_NO_ROOM        = 5,  // A buffer is not big enough.
  STATUS_INTERNAL_ERROR = 6,  // Internal error in the code. Should not happen ;-)
  STATUS_INVALID        = 7,  // Invalid argument.
  STATUS_CRC_WRONG      = 8,  // The CRC_A does not match
  STATUS_MIFARE_NACK    = 9   // A MIFARE PICC responded with NAK.
}MFRC522_Status_T;



typedef unsigned char byte;

/**
* @brief  Type of Length
*/
typedef uint8_t MFRC522_Len_T;
/**
* @brief  Type of Data
*/
typedef uint8_t MFRC522_Data_T;
/**
* @brief Type of Register
*/
typedef uint8_t MFRC522_Reg_T;

// A struct used for passing the UID of a PICC.
typedef struct {
  uint8_t    size;     // Number of chars in the UID. 4, 7 or 10.
  uint8_t    uidchar[10];
  uint8_t    sak;      // The SAK (Select acknowledge) char returned from the PICC after successful selection.
} MFRC522_Uid_T;

// A struct used for passing a MIFARE Crypto1 key
typedef struct {
  byte    keychar[MF_KEY_SIZE];
} MIFARE_Key;

typedef struct{
    byte piccType;
    byte sector;
    byte blockAddr;
    byte trailerBlock;
    byte status;
    byte buffer[18];
    byte size;

}Card_f;

typedef union{
    unsigned int n;     // occupies 4 bytes
    uint8_t c[4];     // occupies 1 byte
} CardData;




// Size of the MFRC522 FIFO
static const char FIFO_SIZE = 64;   // The FIFO is 64 chars.



void MFRC522_Init(void);
MFRC522_Data_T MFRC522_Read_Reg(MFRC522_Reg_T iReg);
void MFRC522_Write_Reg(MFRC522_Reg_T iReg,MFRC522_Data_T iValue);
void MFRC522_SetBitMark(MFRC522_Reg_T iReg,MFRC522_Data_T iMask);
void MFRC522_ClearBitMark(MFRC522_Reg_T iReg,MFRC522_Data_T iMask);
void MFRC522_Reset(void);
void MFRC522_AntennaOn(void);
void MFRC522_AntennaOff(void);
void MFRC522_SetAntennaGain(MFRC522_Data_T mask);
MFRC522_Data_T MFRC522_GetAntennaGain(void);
MFRC522_Status_T MFRC522_IsNewCardPresent(void);
MFRC522_Status_T MFRC522_ReadCard(MFRC522_Uid_T *oUID);



MFRC522_Status_T PICC_RequestA( MFRC522_Data_T *bufferATQA,MFRC522_Data_T *bufferSize);
MFRC522_Status_T PICC_REQA_or_WUPA(MFRC522_Data_T command,MFRC522_Data_T *bufferATQA,MFRC522_Data_T *bufferSize);
MFRC522_Status_T PICC_Select(MFRC522_Uid_T *uid,MFRC522_Data_T validBits);



void PCD_StopCrypto1(void);
MFRC522_Status_T MIFARE_Read(Card_f * card);
MFRC522_Status_T MFRC522_Authenticate(byte command,byte blockAddr, MIFARE_Key *key,MFRC522_Uid_T *uid);
MFRC522_Status_T Authenticate(byte TB, MFRC522_Uid_T *uid);



MFRC522_Status_T PICC_HaltA(void);
MFRC522_Status_T PCD_CalculateCRC(MFRC522_Data_T *data,MFRC522_Data_T length,MFRC522_Data_T *result);
MFRC522_Status_T PCD_TransceiveData(MFRC522_Data_T *sendData, ///< Pointer to the data to transfer to the FIFO.
                                    MFRC522_Data_T sendLen,   ///< Number of bytes to transfer to the FIFO.
                                    MFRC522_Data_T *backData,   ///< nullptr or pointer to buffer if data should be read back after executing the command.
                                    MFRC522_Data_T *backLen,    ///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
                                    MFRC522_Data_T *validBits,            ///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits.
                                    MFRC522_Data_T rxAlign,               ///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
                                    bool checkCRC                         ///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
                                      );
MFRC522_Status_T PCD_CommunicateWithPICC( MFRC522_Data_T command, ///< The command to execute. One of the PCD_Command enums.
                                         MFRC522_Data_T waitIRq,  ///< The bits in the ComIrqReg register that signals successful completion of the command.
                                         MFRC522_Data_T *sendData,  ///< Pointer to the data to transfer to the FIFO.
                                         MFRC522_Data_T sendLen,  ///< Number of bytes to transfer to the FIFO.
                                         MFRC522_Data_T *backData,  ///< nullptr or pointer to buffer if data should be read back after executing the command.
                                         MFRC522_Data_T *backLen, ///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
                                         MFRC522_Data_T *validBits,    ///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits.
                                         MFRC522_Data_T rxAlign,       ///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
                                         bool checkCRC                 ///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
                                           );



#ifdef __cplusplus
}
#endif

#endif /* RC522_H_ */
