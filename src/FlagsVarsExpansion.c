#include "defines.h"

u8* get_flag_address_new(u16 flagID)
{
    if (flagID <= 0x95F) //use standard location ;0 - 0x95F
    {
        u16 flag_byte = flagID / 8;
        return &sav1->flags[flag_byte];
    }
    else if (flagID <= 0x1FFF) //use new saveblock area; 0x960 - 0x1FFF
    {
        flagID -= 0x960;
        #if NEWFLAGS != 0
            if (flagID < NEWFLAGS)
            {
                u16 flag_byte = flagID / 8;
                return &new_saveblock.new_flags[flag_byte];
            }
        #endif // NEWFLAGS
        return NULL;
    }
    else if (flagID <= 0x3FFF) //use the old seen/caught flags; 0x2000 - 0x3FFF
    {
        if (USEOLD_DEX_FLAGS == true)
        {
            flagID -= 0x2000;
            u16 flag_byte = flagID / 8;
            if (flag_byte <= 51)
                return &sav1->seenflags_1[flag_byte];
            flag_byte -= 52;
            if (flag_byte <= 51)
                return &sav1->seenflags_2[flag_byte];
            flag_byte -= 52;
            if (flag_byte <= 51)
                return &sav2->seen_flags[flag_byte];
            flag_byte -= 52;
            if (flag_byte <= 51)
                return &sav2->caught_flags[flag_byte];
        }
        return NULL;
    }
    else if (flagID <= 0x40FF) //use x4000 flags; 0x4000 - 0x40FF
    {
        flagID -= 0x4000;
        u16 flag_byte = flagID / 8;
        return &flags_x4000[flag_byte];
    }
    return NULL;
}

u16* get_var_address_new(u16 varID)
{
    if (varID >= 0x4000)
    {
        if (varID >= 0x8000)
        {
            varID -= 0x8000;
            if (varID < 22)
                return vars_8000[varID];
        }
        else
        {
            if (varID >= 0x5000) //use new saveblock's space
            {
                varID -= 0x5000;
                if (varID < NEWVARS)
                    return &new_saveblock.new_vars[varID];
            }
            else //use sav1
            {
                return &sav1->vars[varID - 0x4000];
            }
        }
    }
    return NULL;
}
