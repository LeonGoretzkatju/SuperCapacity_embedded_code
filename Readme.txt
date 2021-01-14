Author : LeonGoretzkatju
Team:北洋机甲MaxPro
Time : 2020 Aug 3rd
version: super capacity v1.1.0
toolchain/IDE: keil μvision 5 
RTOS:FreeRTOS
This project is based on the HKUST ENTERPRIZE's idea,we tried to transform the project in the Chibistudio/ChibiRTOS to the Keil IDE and FreeRTOS 
operation system, this is my first experience to program the super capacity , thanks to my teammates.
The content of this folder:
the essential chip for the Super capacity and their driver codes is writen in the USER workgroup ,such as TLI4970.c,MCP4726.c, the tasks and
its completion is writen in the corresponding codes.
The implementation of the corresponding functions are set in the application/user folder,and the driver of the function can be found at bsp folder.
The basic idea of our super capacity is the PID close-loop control of the charging power.When charging at the initial state, because of the 
high power of the judge system, so I set a rampfunction to decrease the charging power at the initial state, about 10 to 15seconds, after that 
process, the thread is switched to the pidcharging mode, at the same time, we use the ADC to monitor the voltage of the supercapacity, 
if the voltage >= 23.5, we must cut off the charging system no matter in which charging mode.
-----Changes 2020 Aug 8th: 
  ---The original CANRxframe and CANTxframe's Extended identifier SID = 29,   However, to satisfy the main controller of the imfantry, I change 
the receiving   and transmiting data frame to SID=11.
-----Changes 2020 Aug 10th
  ---The original CAN process is set a task, however, after our discussions ,I found that this task cannot be executed during operation, so I use the CanRxcallback fuction to redefine the interrupt service function, canceling the can_process_task.Removing the content to the CanRxCCallback fuction defined by the user.
-----Changes 2020 Augg 14th
  ---Using the State Machine to solve the logical problem of the charing source of the chassis, assuming that There are two states called 0 and 1 in our codes, The 0 represents that the judging system is charging to the super capacity, unless the superCap's voltage is higher than the value that we set, it will continue charging to the super capacity.If the voltage of the super capacity is higher than the 1.5*capmin, then the state will be siwtched to the 1,which represents the capacity will provide full power for the chassis, if the voltage of the capacity is lower than the vcapmin, the state will be switched into the state of 0, the default state of the system is state 0 to keep the safety of the robot.
WARNING:来自刘老板的忠告，测试的时候在超级电容的正负极之间接上一个保险丝、保护电阻或者电流表，以防止正负极反接事故或者短路。