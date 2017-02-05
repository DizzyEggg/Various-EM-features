#include "defines.h"

struct poke_config config_table[255] = {
    {{252, 252, 252, 252, 252, 252}, {31, 31, 31, 31, 31, 31}, 1, 1, 1},
};

void create_trainer_pokemon(struct pokemon* poke, u16 trainerID, u8 purge)
{
    if (trainerID != 0x400 && battle_flags.trainer && !(BATTLE_FRONTIER_BATTLE || battle_flags.flagx800 || battle_flags.flag_x4000000))
    {
        struct trainer_data* trainer = &trainer_table[trainerID];
        if (purge)
            pokemon_purge_opponent();
        u8 no_of_pokes = trainer->poke_number;
        if (battle_flags.multibattle && no_of_pokes > 3)
            no_of_pokes = 3;
        u32 TiD = calculate_checksum(trainer, sizeof(struct trainer_data));
        struct poke_trainer_data* poke_info = trainer->poke_data;
        u8 struct_size = 8;
        if (trainer->custom_moves)
            struct_size += 8;
        for (u8 i = 0; i < no_of_pokes; i++)
        {
            struct poke_trainer_data* curr_poke = (struct poke_trainer_data*) ADD_POINTER(poke_info, i * struct_size);
            struct poke_config* config = &config_table[curr_poke->evs_id];
            u32 custom_pid = 0;
            u32 PiD = 0;
            if (trainer->custom_item && trainer->custom_moves)
            {
                u8 nature = config->nature;
                if (nature < 25)
                {
                    u32 PiD = get_attributes(&poke[i], ATTR_PID, 0);
                    while(__umodsi3(PiD, 25) != nature)
                    {
                        PiD = rng() | (rng() << 0x10);
                    }
                    custom_pid = 1;
                }
            }
            pokemon_make_wrapper(&poke[i], curr_poke->poke_id, curr_poke->level, 0, custom_pid, PiD, 1, TiD);
            if (trainer->custom_item)
            {
                set_attributes(&poke[i], ATTR_HELD_ITEM, &curr_poke->item_id);
            }
            if (trainer->custom_moves)
            {
                for (u8 j = 0; j < 4; j++)
                {
                    set_attributes(&poke[i], ATTR_ATTACK_1 + j, &curr_poke->move[j]);
                }
            }
            if (trainer->custom_item && trainer->custom_moves)
            {
                for (u8 j = 0; j < 6; j++)
                {
                    set_attributes(&poke[i], ATTR_HP_EV + j, &config->evs[j]);
                    set_attributes(&poke[i], ATTR_HP_IV + j, &config->ivs[j]);
                }
                set_attributes(&poke[i], ATTR_POKEBALL, &config->poke_ball);
                set_attributes(&poke[i], ATTR_ABILITY_BIT, &config->abilityID);
            }
        }
        if (trainer->double_battle)
            battle_flags.double_battle = 1;
    }
}
