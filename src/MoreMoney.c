#include "defines.h"

void prepare_money_box(u8 rboxID, s32 value, u8 unkown)
{
    u8 width = 0x26;
    if (value > 999999)
        width = 0x20;
    put_money_on_rbox(rboxID, width, 1, value, unkown);
}
