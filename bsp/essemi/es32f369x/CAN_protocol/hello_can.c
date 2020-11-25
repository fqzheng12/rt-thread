typedef struct _tagCAN_RX_FRAME_MAP
{
u32 StdId;
union
{
u32 ExtId;
struct
{
u32 nFn:8;
u32 nAfn:8;
u32 nCmd:3;
u32 nDestId:5;
u32 nSrcId:5;
u32 nHeard:3;
}sDet;
}uExtId;
u8 IDE;
u8 RTR;
u8 DLC;
u8 Data[8];
u8 FMI;
}CAN_RX_FRAME_MAP ,*pCAN_RX_FRAME_MAP;
