#include "defines.h"

#define EQUAL 0
#define DEF_HIGHER 1
#define ATK_HIGHER 2

enum evo_index{
    LEVEL_UP,
    TRADE,
    STONE_ACTUAL_EVO,
    STONE_CAN_EVO,
};

bool time_check(u8 from, u8 to)
{
    update_rtc();
    u8 hour = rtc_hex.hour;
    if (to > from)
    {
        if (hour >= from && hour <= to)
            return 1;
        return 0;
    }
    else
    {
        if (hour >= from || hour <= to)
            return 1;
    }
    return 0;
}

bool happiness_evo_check(struct pokemon* poke, u8 from, u8 to)
{
    if (time_check(from, to) && get_attributes(poke, ATTR_HAPPINESS, 0) > 219)
        return 1;
    return 0;
}

bool attack_def_evo_check(struct pokemon* poke, u8 condition)
{
    u16 atk = get_attributes(poke, ATTR_ATTACK, 0);
    u16 def = get_attributes(poke, ATTR_DEFENCE, 0);
    if (atk == def)
    {
        if (condition == EQUAL)
            return 1;
        return 0;
    }
    else if (atk > def)
    {
        if (condition == ATK_HIGHER)
            return 1;
        return 0;
    }
    else
    {
        if (condition == DEF_HIGHER)
            return 1;
        return 0;
    }
}

u16 try_evolving_poke(struct pokemon* poke, enum evo_index index, u16 stoneID)
{
    u16 held_item = get_attributes(poke, ATTR_HELD_ITEM, 0);
    if (get_item_battle_function(held_item) == ITEM_EFFECT_EVERSTONE && index != STONE_ACTUAL_EVO && index != STONE_CAN_EVO)
        return 0;
    u16 species = get_attributes(poke, ATTR_SPECIES, 0);
    u16 lvl = get_attributes(poke, ATTR_LEVEL, 0);
    u32 pid_form = __umodsi3(get_attributes(poke, ATTR_PID, 0) >> 0x10, 10);
    struct evolution_sub* evos = (*evo_table)[species];
    for (u8 i = 0; i < EVO_PER_POKE; i++)
    {
        struct evolution_sub* evo = &evos[i];
        u8 evolving = 0;
        u8 item_delete = 0;
        u8 method = evo->method;
        switch (method)
        {
        case 0: //no evolution
            break;
        case 1: //happiness
            evolving = happiness_evo_check(poke, 0, 24);
            break;
        case 2: //happiness daytime
            evolving = happiness_evo_check(poke, DAY_FIRST_HOUR, DAY_LAST_HOUR);
            break;
        case 3: //happiness nighttime
            evolving = happiness_evo_check(poke, NIGHT_FIRST_HOUR, NIGHT_FIRST_HOUR);
            break;
        LEVELUP_EVO:
        case 4: //level up
        case 13: //extra pokemon creation(Nincada to Ninjask)
            if (evo->paramter <= lvl)
                evolving = 1;
            break;
        case 6: //trade with an item
            if (held_item != evo->paramter)
                break;
            item_delete = 1;
        case 5: //trade
            if (index == TRADE)
                evolving = 1;
            break;
        STONE_EVO:
        case 7: //item
            if ((index == STONE_CAN_EVO || index == STONE_ACTUAL_EVO) && stoneID == evo->paramter)
                evolving = 1;
            break;
        case 8: //attack greater than defense
            evolving = attack_def_evo_check(poke, ATK_HIGHER);
            break;
        case 9: //attack equal to defense
            evolving = attack_def_evo_check(poke, EQUAL);
            break;
        case 10: //attack lower than defense
            evolving = attack_def_evo_check(poke, DEF_HIGHER);
            break;
        case 11: //pid <0, 4>
            if (pid_form < 5)
                goto LEVELUP_EVO;
            break;
        case 12: //pid <5, 9>
            if (pid_form >= 5)
                goto LEVELUP_EVO;
            break;
        case 14: //extra pokemon being created(Nincada to Shedinja)
            break;
        case 15: //beauty evolution
            if (get_attributes(poke, ATTR_BEAUTY, 0) >= 170)
                evolving = 1;
            break;
        case 16: //level up when knows a certain move
            for (u8 i = 0; i < 4; i++)
            {
                if (get_attributes(poke, ATTR_ATTACK_1 + i, 0) == evo->paramter)
                {
                    evolving = 1;
                    break;
                }
            }
            break;
        case 17: //level up only in certain arena, parameter is map name
            if (curr_mapheader.name == evo->paramter)
                evolving = 1;
            break;
        case 18: //by level at day time
            if (time_check(DAY_FIRST_HOUR, DAY_LAST_HOUR))
                goto LEVELUP_EVO;
            break;
        case 19: //by level at night time
            if (time_check(NIGHT_FIRST_HOUR, NIGHT_LAST_HOUR))
                goto LEVELUP_EVO;
            break;
        case 20: //level up at day when holding an item
            if (time_check(DAY_FIRST_HOUR, DAY_LAST_HOUR) && held_item == evo->paramter)
            {
                evolving = 1;
                item_delete = 1;
            }
            break;
        case 21: //level up at night when holding an item
            if (time_check(NIGHT_FIRST_HOUR, NIGHT_LAST_HOUR) && held_item == evo->paramter)
            {
                evolving = 1;
                item_delete = 1;
            }
            break;
        case 22: //by level only if male
            if (poke_get_gender(poke) == 0)
                goto LEVELUP_EVO;
            break;
        case 23: //by level only if female
            if (poke_get_gender(poke) == 0xFE)
                goto LEVELUP_EVO;
            break;
        case 24: //by level if it's raining (Goodra evo)
            if (sav1->ov_weather == RAINY || sav1->ov_weather == THUNDERSTORM)
                goto LEVELUP_EVO;
            break;
        case 25: //by level if there's specific pokemon in party (Mantyke evo)
            for (u8 i = 0; i < 6; i++)
            {
                struct pokemon* curr_poke = &party_player[i];
                if (get_attributes(curr_poke, ATTR_SPECIES, 0) == evo->paramter && curr_poke != poke)
                {
                    evolving = 1;
                    break;
                }
            }
            break;
        case 26: //level up if there's a pokemon with specific type in user's paty (Pancham's evo); type in pad1
            for (u8 i = 0; i < 6; i++)
            {
                struct pokemon* curr_poke = &party_player[i];
                u16 curr_species = get_attributes(curr_poke, ATTR_SPECIES, 0);
                if (curr_poke != poke && curr_species)
                {
                    struct poke_basestats* stats = &((*basestat_table)[curr_species]);
                    if (stats->type1 == evo->pad1 || stats->type2 == evo->pad1)
                    {
                        goto LEVELUP_EVO;
                    }
                }
            }
            break;
        case 27: //male by stone
            if (poke_get_gender(poke) == 0)
                goto STONE_EVO;
            break;
        case 28: //female by stone
            if (poke_get_gender(poke) == 0xFE)
                goto STONE_EVO;
            break;
        }
        if (evolving)
        {
            if (index == LEVEL_UP || (method == 5 || method == 6 || method == 7 || method == 27 || method == 28)) //only via LVL up unless evolution types are different
            {
                if (item_delete)
                {
                    u16 zero = 0;
                    set_attributes(poke, ATTR_HELD_ITEM, &zero);
                }
                return evo->poke;
            }
        }
    }
    return 0;
}
