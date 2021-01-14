#define __BSP_CAN_H
#include "can.h"
#include "sys.h"
#define CAN_FilterFIFO0             CAN_FILTER_FIFO0
#if defined ( __CC_ARM)
#pragma anon_unions
#endif
u8 CAN1_Send_Msg(u8* msg,u8 len);
u16 CAN_Send_Msg(u16* msg,u16 len);
//void can_processInit(void);
typedef enum {

	notUpdated = 0,
	updated = 1

} msgUpdatedEnum;

typedef struct userCommand_t{

	msgUpdatedEnum msgUpdated;
	uint8_t pathType;			//1 = judge power, 2 = cap power
//	uint8_t robotType;		//1 = 80W limit, 2 = 120W limit 
	uint8_t chassis_max_power; 
	float ChassisPower;
} userCommand_t;
typedef struct {
  /*lint -save -e46 [6.1] Standard types are fine too.*/
  uint8_t                   FMI;            /**< @brief Filter id.          */
  uint16_t                  TIME;           /**< @brief Time stamp.         */
  uint8_t                   DLC:4;          /**< @brief Data length.        */
  uint8_t                   RTR:1;          /**< @brief Frame type.         */
  uint8_t                   IDE:1;          /**< @brief Identifier type.    */
union {  
  uint32_t                SID:11;         /**< @brief Standard identifier.*/
  uint32_t                EID:11;         /**< @brief Extended identifier.*/
  uint32_t                _align1;
};
  /*lint -restore*/
union { 
    uint8_t                 data4[8];       /**< @brief Frame data.         */
    uint16_t                data2[16];      /**< @brief Frame data.         */
    uint32_t                data1[32];      /**< @brief Frame data.         */
};
} CANRxFrame;
typedef struct canFeedback_t{

	uint8_t Vin;					//V
	uint8_t pathType;			//0 = diode, 1 = judge power, 2 = cap power
	int16_t capEnergy;		//J, 0.5 * C * (Vcap^2 - Vmin^2)
//	uint16_t Pjudge;			//W
//	uint16_t Pchassis;		//W

}canFeedback_t;

typedef struct canFeedback_t1{

//	uint8_t Vin;					//V
//	uint8_t pathType;			//0 = diode, 1 = judge power, 2 = cap power
//	int16_t capEnergy;		//J, 0.5 * C * (Vcap^2 - Vmin^2)
  uint8_t mode_type;

}canFeedback_t1;
typedef struct {
  /*lint -save -e46 [6.1] Standard types are fine too.*/
  uint8_t                   DLC:4;          /**< @brief Data length.        */
  uint8_t                   RTR:1;          /**< @brief Frame type.         */
  uint8_t                   IDE:1;          /**< @brief Identifier type.    */
union { 
    uint32_t                SID:11;         /**< @brief Standard identifier.*/
    uint32_t                EID:11;         /**< @brief Extended identifier.*/
    uint32_t                _align1;
};  
  /*lint -restore*/
union {
    uint8_t                 data4[8];       /**< @brief Frame data.         */
    uint16_t                data2[16];      /**< @brief Frame data.         */
    uint32_t                data1[32];      /**< @brief Frame data.         */
};
} CANTxFrame;
