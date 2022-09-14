/*
 * rc522.c
 *
 *  Created on: 30-Aug-2022
 *      Author: Mukarram Saeed
 */



#include "rc522.h"


/**
 * Constructor.
 * Prepares the output pins.
 */

MIFARE_Key key;



/**
 * Transmits a REQuest command, Type A. Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522_Status_T PICC_RequestA( MFRC522_Data_T *bufferATQA,MFRC522_Data_T *bufferSize)  {
  return PICC_REQA_or_WUPA(PICC_CMD_REQA, bufferATQA, bufferSize);
} // End PICC_RequestA()



/**
 * Transmits REQA or WUPA commands.
 * Beware: When two PICCs are in the field at the same time I often get STATUS_TIMEOUT - probably due do bad antenna design.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522_Status_T PICC_REQA_or_WUPA(MFRC522_Data_T command,MFRC522_Data_T *bufferATQA,MFRC522_Data_T *bufferSize)  {
  MFRC522_Data_T validBits;
  MFRC522_Status_T status;

  if (bufferATQA == NULL || *bufferSize < 2) {  // The ATQA response is 2 chars long.
      return STATUS_NO_ROOM;
  }
  MFRC522_ClearBitMark(CollReg, 0x80);    // ValuesAfterColl=1 => Bits received after collision are cleared.
  validBits = 7;                  // For REQA and WUPA we need the short frame format - transmit only 7 bits of the last (and only) char. TxLastBits = BitFramingReg[2..0]
  status = PCD_TransceiveData(&command, 1, bufferATQA, bufferSize, &validBits, 0, false);
  if (status != STATUS_OK) {
      return status;
  }
  if (*bufferSize != 2 || validBits != 0) {   // ATQA must be exactly 16 bits.
      return STATUS_ERROR;
  }
  return STATUS_OK;
} // End PICC_REQA_or_WUPA()




/**
 * Executes the Transceive command.
 * CRC validation can only be done if backData and backLen are specified.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522_Status_T PCD_TransceiveData(MFRC522_Data_T *sendData, ///< Pointer to the data to transfer to the FIFO.
                                    MFRC522_Data_T sendLen,   ///< Number of bytes to transfer to the FIFO.
                                    MFRC522_Data_T *backData,   ///< nullptr or pointer to buffer if data should be read back after executing the command.
                                    MFRC522_Data_T *backLen,    ///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
                                    MFRC522_Data_T *validBits,            ///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits.
                                    MFRC522_Data_T rxAlign,               ///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
                                    bool checkCRC                         ///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
                                      )
{
  MFRC522_Data_T waitIRq = 0x30;    // RxIRq and IdleIRq
  return PCD_CommunicateWithPICC(PCD_Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
} // End PCD_TransceiveData()



/**
 * Transfers data to the MFRC522 FIFO, executes a command, waits for completion and transfers data back from the FIFO.
 * CRC validation can only be done if backData and backLen are specified.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522_Status_T PCD_CommunicateWithPICC( MFRC522_Data_T command, ///< The command to execute. One of the PCD_Command enums.
                                         MFRC522_Data_T waitIRq,  ///< The bits in the ComIrqReg register that signals successful completion of the command.
                                         MFRC522_Data_T *sendData,  ///< Pointer to the data to transfer to the FIFO.
                                         MFRC522_Data_T sendLen,  ///< Number of bytes to transfer to the FIFO.
                                         MFRC522_Data_T *backData,  ///< nullptr or pointer to buffer if data should be read back after executing the command.
                                         MFRC522_Data_T *backLen, ///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
                                         MFRC522_Data_T *validBits,    ///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits.
                                         MFRC522_Data_T rxAlign,       ///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
                                         bool checkCRC                 ///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
                                           )
{
  // Prepare values for BitFramingReg
  MFRC522_Data_T txLastBits = validBits ? *validBits : 0;
  MFRC522_Data_T bitFraming = (rxAlign << 4) + txLastBits;    // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

  MFRC522_Write_Reg(CommandReg, PCD_Idle);      // Stop any active command.
  MFRC522_Write_Reg(ComIrqReg, 0x7F);         // Clear all seven interrupt request bits
  MFRC522_SetBitMark(FIFOLevelReg, 0x80);     // FlushBuffer = 1, FIFO initialization
  for(uint8_t i =0;i<sendLen;i++)
  {
      MFRC522_Write_Reg(FIFODataReg, sendData[i]);       // Write sendData to the FIFO
  }
  MFRC522_Write_Reg(BitFramingReg, bitFraming);   // Bit adjustments
  MFRC522_Write_Reg(CommandReg, command);       // Execute the command
  if (command == PCD_Transceive) {
      MFRC522_SetBitMark(BitFramingReg, 0x80);  // StartSend=1, transmission of data starts
  }

  // Wait for the command to complete.
  // In PCD_Init() we set the TAuto flag in TModeReg. This means the timer automatically starts when the PCD stops transmitting.
  // Each iteration of the do-while-loop takes 17.86ï¿½s.
  uint32_t i;
  for (i = 2000; i > 0; i--)
  {
   MFRC522_Data_T n = MFRC522_Read_Reg(ComIrqReg); // ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
    if (n & waitIRq) // One of the interrupts that signal success has been set.
    {
      break;
    }
    if (n & 0x01) // Timer interrupt - nothing received in 25ms
    {
      return STATUS_TIMEOUT;
    }
  }

  // 35.7ms and nothing happend. Communication with the MFRC522 might be down.
  if (i == 0)
  {
    return STATUS_TIMEOUT;
  }

  // Stop now if any errors except collisions were detected.
  MFRC522_Data_T errorRegValue = MFRC522_Read_Reg(ErrorReg); // ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr
   if (errorRegValue & 0x13) // BufferOvfl ParityErr ProtocolErr
   {
     return STATUS_ERROR;
   }

   MFRC522_Data_T _validBits=0;
  // If the caller wants data back, get it from the MFRC522.
  if (backData && backLen) {
      MFRC522_Data_T n = MFRC522_Read_Reg(FIFOLevelReg);     // Number of chars in the FIFO
      if (n > *backLen) {
          return STATUS_NO_ROOM;
      }
      *backLen = n;                     // Number of chars returned
      // Number of bytes returned
      for (uint8_t j=0; j<n; j++)
      {
        backData[j] = MFRC522_Read_Reg(FIFODataReg);
      }

      if(rxAlign)
      {
          MFRC522_Data_T mask = (0xFF << rxAlign) & 0xFF;
          MFRC522_Data_T pbackData = backData[0];
        backData[0]  = (backData[0]  & ~mask) | (pbackData & mask);
      }

      _validBits = MFRC522_Read_Reg(ControlReg) & 0x07;   // RxLastBits[2:0] indicates the number of valid bits in the last received char. If this value is 000b, the whole char is valid.
      if (validBits) {
          *validBits = _validBits;
      }
  }

  // Tell about collisions
  if (errorRegValue & 0x08) {   // CollErr
      return STATUS_COLLISION;
  }

  // Perform CRC_A validation if requested.
  if (backData && backLen && checkCRC) {
      // In this case a MIFARE Classic NAK is not OK.
      if (*backLen == 1 && _validBits == 4) {
          return STATUS_MIFARE_NACK;
      }
      // We need at least the CRC_A value and all 8 bits of the last char must be received.
      if (*backLen < 2 || _validBits != 0) {
          return STATUS_CRC_WRONG;
      }
      // Verify CRC_A - do our own calculation and store the control in controlBuffer.
      byte controlBuffer[2];
      MFRC522_Status_T status = PCD_CalculateCRC(&backData[0], *backLen - 2, &controlBuffer[0]);
      if (status != STATUS_OK) {
          return status;
      }
      if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
          return STATUS_CRC_WRONG;
      }
  }

  return STATUS_OK;
} // End PCD_CommunicateWithPICC()



/**
 * Transmits SELECT/ANTICOLLISION commands to select a single PICC.
 * Before calling this function the PICCs must be placed in the READY(*) state by calling PICC_RequestA() or PICC_WakeupA().
 * On success:
 *    - The chosen PICC is in state ACTIVE(*) and all other PICCs have returned to state IDLE/HALT. (Figure 7 of the ISO/IEC 14443-3 draft.)
 *    - The UID size and value of the chosen PICC is returned in *uid along with the SAK.
 *
 * A PICC UID consists of 4, 7 or 10 chars.
 * Only 4 chars can be specified in a SELECT command, so for the longer UIDs two or three iterations are used:
 *    UID size  Number of UID chars   Cascade levels    Example of PICC
 *    ========  ===================   ==============    ===============
 *    single         4            1       MIFARE Classic
 *    double         7            2       MIFARE Ultralight
 *    triple        10            3       Not currently in use?
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522_Status_T PICC_Select(MFRC522_Uid_T *uid,MFRC522_Data_T validBits)
 {
  MFRC522_Data_T uidComplete;
  MFRC522_Data_T selectDone;
  MFRC522_Data_T useCascadeTag;
  MFRC522_Data_T cascadeLevel = 1;
  MFRC522_Status_T result;
  MFRC522_Data_T count;
  MFRC522_Data_T index;
  MFRC522_Data_T uidIndex;          // The first index in uid->uidchar[] that is used in the current Cascade Level.
  int8_t currentLevelKnownBits;   // The number of known UID bits in the current Cascade Level.
  MFRC522_Data_T buffer[9];         // The SELECT/ANTICOLLISION commands uses a 7 char standard frame + 2 chars CRC_A
  MFRC522_Data_T bufferUsed;        // The number of chars used in the buffer, ie the number of chars to transfer to the FIFO.
  MFRC522_Data_T rxAlign;         // Used in BitFramingReg. Defines the bit position for the first bit received.
  MFRC522_Data_T txLastBits;        // Used in BitFramingReg. The number of valid bits in the last transmitted char.
  MFRC522_Data_T *responseBuffer;
  MFRC522_Data_T responseLength;

  // Description of buffer structure:
  //    char 0: SEL         Indicates the Cascade Level: PICC_CMD_SEL_CL1, PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
  //    char 1: NVB         Number of Valid Bits (in complete command, not just the UID): High nibble: complete chars, Low nibble: Extra bits.
  //    char 2: UID-data or CT    See explanation below. CT means Cascade Tag.
  //    char 3: UID-data
  //    char 4: UID-data
  //    char 5: UID-data
  //    char 6: BCC         Block Check Character - XOR of chars 2-5
  //    char 7: CRC_A
  //    char 8: CRC_A
  // The BCC and CRC_A is only transmitted if we know all the UID bits of the current Cascade Level.
  //
  // Description of chars 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft: UID contents and cascade levels)
  //    UID size  Cascade level char2 char3 char4 char5
  //    ========  ============= ===== ===== ===== =====
  //     4 chars    1     uid0  uid1  uid2  uid3
  //     7 chars    1     CT    uid0  uid1  uid2
  //            2     uid3  uid4  uid5  uid6
  //    10 chars    1     CT    uid0  uid1  uid2
  //            2     CT    uid3  uid4  uid5
  //            3     uid6  uid7  uid8  uid9

  // Sanity checks
  if (validBits > 80) {
      return STATUS_INVALID;
  }

  // Prepare MFRC522
  MFRC522_ClearBitMark(CollReg, 0x80);    // ValuesAfterColl=1 => Bits received after collision are cleared.

  // Repeat Cascade Level loop until we have a complete UID.
  uidComplete = false;
  while (!uidComplete) {
      // Set the Cascade Level in the SEL char, find out if we need to use the Cascade Tag in char 2.
      switch (cascadeLevel) {
        case 1:
          buffer[0] = PICC_CMD_SEL_CL1;
          uidIndex = 0;
          useCascadeTag = validBits && uid->size > 4; // When we know that the UID has more than 4 chars
          break;

        case 2:
          buffer[0] = PICC_CMD_SEL_CL2;
          uidIndex = 3;
          useCascadeTag = validBits && uid->size > 7; // When we know that the UID has more than 7 chars
          break;

        case 3:
          buffer[0] = PICC_CMD_SEL_CL3;
          uidIndex = 6;
          useCascadeTag = false;            // Never used in CL3.
          break;

        default:
          return STATUS_INTERNAL_ERROR;
          break;
      }

      // How many UID bits are known in this Cascade Level?
      currentLevelKnownBits = validBits - (8 * uidIndex);
      if (currentLevelKnownBits < 0) {
          currentLevelKnownBits = 0;
      }
      // Copy the known bits from uid->uidchar[] to buffer[]
      index = 2; // destination index in buffer[]
      if (useCascadeTag) {
          buffer[index++] = PICC_CMD_CT;
      }
      MFRC522_Data_T bytesToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of chars needed to represent the known bits for this level.
      if (bytesToCopy) {
          MFRC522_Data_T maxBytes = useCascadeTag ? 3 : 4; // Max 4 chars in each Cascade Level. Only 3 left if we use the Cascade Tag
          if (bytesToCopy > maxBytes) {
              bytesToCopy = maxBytes;
          }
          for (count = 0; count < bytesToCopy; count++) {
              buffer[index++] = uid->uidchar[uidIndex + count];
          }
      }
      // Now that the data has been copied we need to include the 8 bits in CT in currentLevelKnownBits
      if (useCascadeTag) {
          currentLevelKnownBits += 8;
      }

      // Repeat anti collision loop until we can transmit all UID bits + BCC and receive a SAK - max 32 iterations.
      selectDone = false;
      while (!selectDone) {
          // Find out how many bits and chars to send and receive.
          if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade Level are known. This is a SELECT.
              //Serial.print(F("SELECT: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
              buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole chars
              // Calculate BCC - Block Check Character
              buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
              // Calculate CRC_A
              result = PCD_CalculateCRC(buffer, 7, &buffer[7]);
              if (result != STATUS_OK) {
                  return result;
              }
              txLastBits    = 0; // 0 => All 8 bits are valid.
              bufferUsed    = 9;
              // Store response in the last 3 chars of buffer (BCC and CRC_A - not needed after tx)
              responseBuffer  = &buffer[6];
              responseLength  = 3;
          }
          else { // This is an ANTICOLLISION.
              //Serial.print(F("ANTICOLLISION: currentLevelKnownBits=")); Serial.println(currentLevelKnownBits, DEC);
              txLastBits    = currentLevelKnownBits % 8;
              count     = currentLevelKnownBits / 8;  // Number of whole chars in the UID part.
              index     = 2 + count;          // Number of whole chars: SEL + NVB + UIDs
              buffer[1]   = (index << 4) + txLastBits;  // NVB - Number of Valid Bits
              bufferUsed    = index + (txLastBits ? 1 : 0);
              // Store response in the unused part of buffer
              responseBuffer  = &buffer[index];
              responseLength  = sizeof(buffer) - index;
          }

          // Set bit adjustments
          rxAlign = txLastBits;                     // Having a seperate variable is overkill. But it makes the next line easier to read.
          MFRC522_Write_Reg(BitFramingReg, (rxAlign << 4) + txLastBits);  // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

          // Transmit the buffer and receive the response.
          result = PCD_TransceiveData(buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, false);
          if (result == STATUS_COLLISION) { // More than one PICC in the field => collision.
              MFRC522_Data_T valueOfCollReg = MFRC522_Read_Reg(CollReg); // CollReg[7..0] bits are: ValuesAfterColl reserved CollPosNotValid CollPos[4:0]
              if (valueOfCollReg & 0x20) { // CollPosNotValid
                  return STATUS_COLLISION; // Without a valid collision position we cannot continue
              }
              byte collisionPos = result & 0x1F; // Values 0-31, 0 means bit 32.
              if (collisionPos == 0) {
                  collisionPos = 32;
              }
              if (collisionPos <= currentLevelKnownBits) { // No progress - should not happen
                  return STATUS_INTERNAL_ERROR;
              }
              // Choose the PICC with the bit set.
              currentLevelKnownBits = collisionPos;
              count     = (currentLevelKnownBits - 1) % 8; // The bit to modify
              index     = 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First char is index 0.
              buffer[index] |= (1 << count);
          }
          else
            if (result != STATUS_OK) {
              return result;
          }
          else { // STATUS_OK
              if (currentLevelKnownBits >= 32) { // This was a SELECT.
                  selectDone = true; // No more anticollision
                  // We continue below outside the while.
              }
              else { // This was an ANTICOLLISION.
                  // We now have all 32 bits of the UID in this Cascade Level
                  currentLevelKnownBits = 32;
                  // Run loop again to do the SELECT.
              }
          }
      } // End of while (!selectDone)

      // We do not check the CBB - it was constructed by us above.

      // Copy the found UID chars from buffer[] to uid->uidchar[]
      index     = (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
      bytesToCopy   = (buffer[2] == PICC_CMD_CT) ? 3 : 4;
      for (count = 0; count < bytesToCopy; count++) {
          uid->uidchar[uidIndex + count] = buffer[index++];
      }

      // Check response SAK (Select Acknowledge)
      if (responseLength != 3 || txLastBits != 0) { // SAK must be exactly 24 bits (1 char + CRC_A).
          return STATUS_ERROR;
      }
      // Verify CRC_A - do our own calculation and store the control in buffer[2..3] - those chars are not needed anymore.
      result = PCD_CalculateCRC(responseBuffer, 1, &buffer[2]);
      if (result != STATUS_OK) {
          return result;
      }
      if ((buffer[2] != responseBuffer[1]) || (buffer[3] != responseBuffer[2])) {
          return STATUS_CRC_WRONG;
      }
      if (responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
          cascadeLevel++;
      }
      else {
          uidComplete = true;
          uid->sak = responseBuffer[0];
      }
  } // End of while (!uidComplete)

  // Set correct uid->size
  uid->size = 3 * cascadeLevel + 1;

  return STATUS_OK;
} // End PICC_Select()



/**
 * Use the CRC coprocessor in the MFRC522 to calculate a CRC_A.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522_Status_T  PCD_CalculateCRC(MFRC522_Data_T *data,MFRC522_Data_T length,MFRC522_Data_T *result)
{
  MFRC522_Write_Reg(CommandReg, PCD_Idle);    // Stop any active command.
  MFRC522_Write_Reg(DivIrqReg, 0x04);       // Clear the CRCIRq interrupt request bit
  MFRC522_SetBitMark(FIFOLevelReg, 0x80);   // FlushBuffer = 1, FIFO initialization
  for(uint8_t i =0;i<length;i++)
  {
    MFRC522_Write_Reg(FIFODataReg, data[i]);  // Write data to the FIFO
  }
  MFRC522_Write_Reg(CommandReg, PCD_CalcCRC);   // Start the calculation

  // Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73ï¿½s.
  // TODO check/modify for other architectures than Arduino Uno 16bit

  // Wait for the CRC calculation to complete. Each iteration of the while-loop takes 17.73us.
  for (uint16_t i = 5000; i > 0; i--)
  {
    // DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
      MFRC522_Data_T n = MFRC522_Read_Reg(DivIrqReg);
    if (n & 0x04)
    {                 // CRCIRq bit set - calculation done
      MFRC522_Write_Reg(CommandReg, PCD_Idle);  // Stop calculating CRC for new content in the FIFO.
      // Transfer the result from the registers to the result buffer
      result[0] = MFRC522_Read_Reg(CRCResultRegL);
      result[1] = MFRC522_Read_Reg(CRCResultRegH);
      return STATUS_OK;
    }
  }
  return STATUS_TIMEOUT;
} // End PCD_CalculateCRC()




/**
 * Writes a char to the specified register in the MFRC522 chip.
 * The interface is described in the datasheet section 8.1.2.
 */
void MFRC522_Write_Reg(  MFRC522_Reg_T iReg,MFRC522_Data_T iValue) {

  //int b[6];
  I2C_TransferSeq_TypeDef i2cTransfer;
  I2C_TransferReturn_TypeDef result;

  MFRC522_Data_T txBuffer[2];
  txBuffer[0]=iReg;
  txBuffer[1]=iValue;

  // Initializing I2C transfer
  i2cTransfer.addr          = RC522_ADDRESS<<1;
  i2cTransfer.flags         = I2C_FLAG_WRITE;
  i2cTransfer.buf[0].data   = txBuffer;
  i2cTransfer.buf[0].len    = 2;
  result=I2CSPM_Transfer(I2C0, &i2cTransfer);
  if (result != i2cTransferDone)    {
   //   UARTDRV_Transmit (handle,"transfer failed\n\r",sizeof("transfer failed\n\r"),uart_callback);
    }
  //sprintf(b,"%d\n\r",result);
 // UARTDRV_Transmit (handle,b,sizeof(b),uart_callback);
}



/**
 * Reads a char from the specified register in the MFRC522 chip.
 * The interface is described in the datasheet section 8.1.2.
 */
MFRC522_Data_T MFRC522_Read_Reg( MFRC522_Reg_T iReg)  ///< The register to read from. One of the PCD_Register enums.
  {
  // Transfer structure
  I2C_TransferSeq_TypeDef i2cTransfer;
  I2C_TransferReturn_TypeDef result;

  uint8_t rxBuffer;

  // Initializing I2C transfer
  i2cTransfer.addr          = RC522_ADDRESS<<1;
  i2cTransfer.flags         = I2C_FLAG_WRITE_READ;
  i2cTransfer.buf[0].data   = &iReg;
  i2cTransfer.buf[0].len    = 1;
  i2cTransfer.buf[1].data   = &rxBuffer;
  i2cTransfer.buf[1].len    = 1;
  result=I2CSPM_Transfer(I2C0, &i2cTransfer);
  if (result != i2cTransferDone)    {
   //   UARTDRV_Transmit (handle,"transfer Read failed\n\r",sizeof("transfer Read failed\n\r"),uart_callback);
    }
  return rxBuffer;
} // End MFRC522_Read_Reg()



/**
 * Initializes the MFRC522 chip.
 */
void MFRC522_Init(void) {

  MFRC522_Reset();


  //MFRC522_Write_Reg(TModeReg, 0x80);      // TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
  MFRC522_Write_Reg(TPrescalerReg, 0xA9);   // TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25ï¿½s.
  MFRC522_Write_Reg(TReloadRegH, 0x03);   // Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
  MFRC522_Write_Reg(TReloadRegL, 0xE8);

  MFRC522_Write_Reg(TxASKReg, 0x40);    // Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
  MFRC522_Write_Reg(ModeReg, 0x3D);   // Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
  MFRC522_AntennaOn();            // Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
  // Reduce the MFRC522 Receiver Gain to maximum value of some 48 dB
  MFRC522_SetAntennaGain(0x05 << 4);
  // Prepare the key (used both as key A and as key B)
  // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
  for(byte i = 0;i < 6;i++)
  {
    key.keychar[i] = 0xFF;
  }
} // End PCD_Init()



/**
 * Sets the bits given in mask in register reg.
 */
void MFRC522_SetBitMark( MFRC522_Reg_T iReg,MFRC522_Data_T iMask) {
  MFRC522_Data_T pData;
  pData = MFRC522_Read_Reg(iReg);
  MFRC522_Write_Reg(iReg, pData | iMask);     // set bit mask
} // End MFRC522_SetBitMark()



/**
 * Clears the bits given in mask from register reg.
 */
void MFRC522_ClearBitMark( MFRC522_Reg_T iReg,MFRC522_Data_T iMask) {
  MFRC522_Data_T pData;
  pData = MFRC522_Read_Reg(iReg);
  MFRC522_Write_Reg(iReg, pData & (~iMask));    // clear bit mask
} // End MFRC522_ClearBitMark()



/**
 * Performs a soft reset on the MFRC522 chip and waits for it to be ready again.
 */
void MFRC522_Reset(void) {
  MFRC522_Write_Reg(CommandReg, PCD_SoftReset); // Issue the SoftReset command.
} // End MFRC522_Reset()



/**
 * Turns the antenna on by enabling pins TX1 and TX2.
 * After a reset these pins are disabled.
 */
void MFRC522_AntennaOn(void) {
  MFRC522_Data_T value = MFRC522_Read_Reg(TxControlReg);
  if ((value & 0x03) != 0x03) {
      MFRC522_Write_Reg(TxControlReg, value | 0x03);
  }
} // End MFRC522_AntennaOn()



/**
 * Turns the antenna off by disabling pins TX1 and TX2.
 */
void MFRC522_AntennaOff(void) {
  MFRC522_ClearBitMark(TxControlReg, 0x03);
} // End MFRC522_AntennaOff()



/**
 * Returns true if a PICC responds to PICC_CMD_REQA.
 * Only "new" cards in state IDLE are invited. Sleeping cards in state HALT are ignored.
 *
 * @return bool
 */
MFRC522_Status_T MFRC522_IsNewCardPresent(void) {
  MFRC522_Data_T bufferATQA[2];
  MFRC522_Data_T bufferSize = sizeof(bufferATQA);
  // Reset baud rates
  MFRC522_Write_Reg(TxModeReg, 0x00);
  MFRC522_Write_Reg(RxModeReg, 0x00);
  // Reset ModWidthReg
  MFRC522_Write_Reg(ModWidthReg, 0x26);
  return PICC_RequestA(bufferATQA, &bufferSize);
} // End MFRC522_IsNewCardPresent()



MFRC522_Status_T MFRC522_ReadCard(MFRC522_Uid_T *oUID)
{
  MFRC522_Uid_T uid;
  uid.size = 0;
  uid.sak = 0;
  memset(uid.uidchar,0x00,sizeof(uid.uidchar));
  MFRC522_Status_T Status = PICC_Select(&uid,0);

  if(Status==STATUS_OK)
  {
    oUID->size = uid.size;
    oUID->sak = uid.sak;
    memcpy(&oUID->uidchar, uid.uidchar, sizeof(uid.uidchar));
  }
  return Status;
}



/**
 * Get the current MFRC522 Receiver Gain (RxGain[2:0]) value.
 * See 9.3.3.6 / table 98 in http://www.nxp.com/documents/data_sheet/MFRC522.pdf
 * NOTE: Return value scrubbed with (0x07<<4)=01110000b as RCFfgReg may use reserved bits.
 *
 * @return Value of the RxGain, scrubbed to the 3 bits used.
 */
MFRC522_Data_T MFRC522_GetAntennaGain(void)  {
  return MFRC522_Read_Reg(RFCfgReg) & (0x07<<4);
} // End MFRC522_GetAntennaGain()



/**
 * Set the MFRC522 Receiver Gain (RxGain) to value specified by given mask.
 * See 9.3.3.6 / table 98 in http://www.nxp.com/documents/data_sheet/MFRC522.pdf
 * NOTE: Given mask is scrubbed with (0x07<<4)=01110000b as RCFfgReg may use reserved bits.
 */
void MFRC522_SetAntennaGain(MFRC522_Data_T mask) {
  if (MFRC522_GetAntennaGain() != mask) {           // only bother if there is a change
      MFRC522_ClearBitMark(RFCfgReg, (0x07<<4));    // clear needed to allow 000 pattern
      MFRC522_SetBitMark(RFCfgReg, mask & (0x07<<4)); // only set RxGain[2:0] bits
  }
} // End MFRC522_SetAntennaGain()



/**
 * Executes the MFRC522 MFAuthent command.
 * This command manages MIFARE authentication to enable a secure communication to any MIFARE Mini, MIFARE 1K and MIFARE 4K card.
 * The authentication is described in the MFRC522 datasheet section 10.3.1.9 and http://www.nxp.com/documents/data_sheet/MF1S503x.pdf section 10.1.
 * For use with MIFARE Classic PICCs.
 * The PICC must be selected - ie in state ACTIVE(*) - before calling this function.
 * Remember to call PCD_StopCrypto1() after communicating with the authenticated PICC - otherwise no new communications can start.
 *
 * All keys are set to FFFFFFFFFFFFh at chip delivery.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise. Probably STATUS_TIMEOUT if you supply the wrong key.
 */
MFRC522_Status_T MFRC522_Authenticate(byte command,byte blockAddr, MIFARE_Key *key,MFRC522_Uid_T *uid) {
  byte waitIRq = 0x10;    // IdleIRq

  // Build command buffer
  byte sendData[12];
  sendData[0] = command;
  sendData[1] = blockAddr;
  for (byte i = 0; i < MF_KEY_SIZE; i++) {  // 6 key chars
      sendData[2+i] = key->keychar[i];
  }
  for (byte i = 0; i < 4; i++) {        // The first 4 chars of the UID
      sendData[8+i] = uid->uidchar[i];
  }

  // Start the authentication.
  return PCD_CommunicateWithPICC(PCD_MFAuthent, waitIRq, &sendData[0], sizeof(sendData),NULL,NULL,NULL,0,false);
} // End PCD_Authenticate()



MFRC522_Status_T Authenticate(byte TB, MFRC522_Uid_T *uid)
{
  MFRC522_Status_T status = MFRC522_Authenticate(PICC_CMD_MF_AUTH_KEY_A,TB,&key,uid);
  return status;
}



/**
 * Reads 16 chars (+ 2 chars CRC_A) from the active PICC.
 *
 * For MIFARE Classic the sector containing the block must be authenticated before calling this function.
 *
 * For MIFARE Ultralight only addresses 00h to 0Fh are decoded.
 * The MF0ICU1 returns a NAK for higher addresses.
 * The MF0ICU1 responds to the READ command by sending 16 chars starting from the page address defined by the command argument.
 * For example; if blockAddr is 03h then pages 03h, 04h, 05h, 06h are returned.
 * A roll-back is implemented: If blockAddr is 0Eh, then the contents of pages 0Eh, 0Fh, 00h and 01h are returned.
 *
 * The buffer must be at least 18 chars because a CRC_A is also returned.
 * Checks the CRC_A before returning STATUS_OK.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522_Status_T MIFARE_Read( Card_f * card) {
  MFRC522_Status_T result;
 byte buffer[4] = {0};
  // Sanity check
  if (buffer == NULL || card->size < 18) {
      return STATUS_NO_ROOM;
  }

  // Build command buffer
  buffer[0] = PICC_CMD_MF_READ;
  buffer[1] = card->blockAddr;
  // Calculate CRC_A
  result = PCD_CalculateCRC(buffer, 2, &buffer[2]);
  if (result != STATUS_OK) {
      return result;
  }

  // Transmit the buffer and receive the response, validate CRC_A.
  return PCD_TransceiveData(buffer, 4, card->buffer, &card->size, NULL, 0, true);
} // End MIFARE_Read()




/**
 * Instructs a PICC in state ACTIVE(*) to go to state HALT.
 *
 * @return STATUS_OK on success, STATUS_??? otherwise.
 */
MFRC522_Status_T PICC_HaltA() {
  MFRC522_Status_T result;
  byte buffer[4];

  // Build command buffer
  buffer[0] = PICC_CMD_HLTA;
  buffer[1] = 0;
  // Calculate CRC_A
  result = PCD_CalculateCRC(buffer, 2, &buffer[2]);
  if (result != STATUS_OK) {
      return result;
  }

  // Send the command.
  // The standard says:
  //    If the PICC responds with any modulation during a period of 1 ms after the end of the frame containing the
  //    HLTA command, this response shall be interpreted as 'not acknowledge'.
  // We interpret that this way: Only STATUS_TIMEOUT is an success.
  result = PCD_TransceiveData(buffer, sizeof(buffer), NULL, NULL, NULL, 0, false);
  if (result == STATUS_TIMEOUT) {
      return STATUS_OK;
  }
  if (result == STATUS_OK) { // That is ironically NOT ok in this case ;-)
      return STATUS_ERROR;
  }
  return result;
} // End PICC_HaltA()



/**
 * Used to exit the PCD from its authenticated state.
 * Remember to call this function after communicating with an authenticated PICC - otherwise no new communications can start.
 */
void PCD_StopCrypto1(void) {
  MFRC522_ClearBitMark(Status2Reg, 0x08); // Status2Reg[7..0] bits are: TempSensClear I2CForceHS reserved reserved MFCrypto1On ModemState[2:0]
} // End PCD_StopCrypto1()
