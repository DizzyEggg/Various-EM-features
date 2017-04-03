#include "defines.h"

u16 choose_song_depending_on_class(enum trainer_class class, u16 trainerID)
{
    u16 songID;
    switch (class)
    {
    //evil team
    case CLASS_TEAM_AQUA:
    case CLASS_TEAM_MAGMA:
    case CLASS_AQUA_ADMIN:
    case CLASS_MAGMA_ADMIN:
        songID = 0x1DB;
        break;
    //evil team leader
    case CLASS_AQUA_LEADER:
    case CLASS_MAGMA_LEADER:
        songID = 0x1E3;
        break;
    //battle frontier bosses
    case CLASS_DOME_ACE:
    case CLASS_PALACE_MAVEN:
    case CLASS_ARENA_TYCOON:
    case CLASS_FACTORY_HEAD:
    case CLASS_PIKE_QUEEN:
    case CLASS_PYRAMID_KING:
        songID = 0x1D7;
        break;
    case CLASS_LEADER:
        songID = 0x1DD;
        break;
    case CLASS_ELITE_FOUR:
        songID = 0x1E2;
        break;
    case CLASS_CHAMPION:
        songID = 0x1DE;
        break;
    case CLASS_PKMN_TRAINER_RIVAL:
        if (BATTLE_FRONTIER_BATTLE || !(compare_two_strings(trainer_table[trainerID].name, text_WALLY)))
        {
            songID = 0x1E1;
            break;
        }
    default:
        songID = 0x1DC;
        break;
    }
    return songID;
}

u16 choose_song_for_battle()
{
    u16 songID;
    if (battle_flags.flagx1000)
        songID = 0x1E0;
    else if (battle_flags.regis)
        songID = 0x1DF;
    else if (battle_flags.link || battle_flags.flag_x2000000)
        songID = 0x1DC;
    else if (battle_flags.trainer)
    {
        if (!(TRAINER_BATTLE_MUSIC_VAR && (songID = get_var_value(TRAINER_BATTLE_MUSIC_VAR))))
        {
            u16 trainerID = var_8015_trainer_opponent_A;
            enum trainer_class class;
            if (BATTLE_FRONTIER_BATTLE)
                class = get_frontier_opponent_class(trainerID);
            else if (battle_flags.flag_x4000000)
                class = 0xA;
            else
                class = trainer_table[trainerID].class;
            songID = choose_song_depending_on_class(class, trainerID);
        }
    }
    else //wild battle
    {
        if (!(CUSTOM_WILD_POKE_MUSIC && (songID = (*basestat_table)[get_attributes(party_opponent, ATTR_SPECIES, 0)].padding)))
            songID = 0x1DA;
    }
    return songID;
}
