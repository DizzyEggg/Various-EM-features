#ifndef DEFINES_H
#define DEFINES_H

enum overworld_weather{
    HOUSE_WEATHER,
    SUNNY_CLOUDS,
    NORMAL_WEATHER,
    RAINY,
    SNOWFLAKES,
    THUNDERSTORM,
    STEADY_MIST,
    STEADY_SNOW,
    SANDSTORM,
};

enum trainer_class{
    CLASS_PKMN_TRAINER0, //0
    CLASS_PKMN_TRAINER1, //1
    CLASS_HIKER, //2
    CLASS_TEAM_AQUA, //3
    CLASS_PKMN_BREEDER, //4
    CLASS_COOLTRAINER, //5
    CLASS_BIRDKEEPER, //6
    CLASS_COLLECTOR, //7
    CLASS_SWIMMER_MALE, //8
    CLASS_TEAM_MAGMA, //9
    CLASS_EXPERT, //xA
    CLASS_AQUA_ADMIN, //xB
    CLASS_BLACK_BELT, //xC
    CLASS_AQUA_LEADER, //xD
    CLASS_HEX_MANIAC, //xE
    CLASS_AROMA_LADY, //xF
    CLASS_RUIN_MANIAC, //x10
    CLASS_INTERVIEWER, //x11
    CLASS_TUBER_FEMALE, //x12
    CLASS_TUBER_MALE, //x13
    CLASS_LADY, //x14
    CLASS_BEAUTY, //x15
    CLASS_RICH_BOY, //x16
    CLASS_POKEMANIAC, //x17
    CLASS_GUITARIST, //x18
    CLASS_KINDLER, //x19
    CLASS_CAMPER, //x1A
    CLASS_PICKNICKER, //x1B
    CLASS_BUG_MANIAC, //x1C
    CLASS_PSYCHIC, //x1D
    CLASS_GENTLEMAN, //x1E
    CLASS_ELITE_FOUR, //x1F
    CLASS_LEADER, //x20
    CLASS_CHAMPION = 0x26,
    CLASS_MAGMA_ADMIN = 0x31,
    CLASS_PKMN_TRAINER_RIVAL = 0x32,
    CLASS_MAGMA_LEADER = 0x35,
    CLASS_DOME_ACE = 0x3B,
    CLASS_PALACE_MAVEN, //0x3C
    CLASS_ARENA_TYCOON, //0x3D
    CLASS_FACTORY_HEAD, //0x3E
    CLASS_PIKE_QUEEN, //0x3F
    CLASS_PYRAMID_KING, //0x40
    CLASS_PKMN_TRAINER2, //0x41
};

#define REPEL_X 0
#define REPEL_Y 0

#define POKES_NO        442
#define EVO_PER_POKE		5
#define EEVEE_TABLE		true
#define ALL_ITEMS       386
#define MAX_LEVEL		250
#define MORELEVELS      true
#define CUSTOM_WILD_POKE_MUSIC		0x0
#define TRAINER_BATTLE_MUSIC_VAR		0x0
#define COLORED_STATS		false
#define BALL_EXPANSION  true
#define FIRST_NEW_BALL_ID		226
#define LAST_OLD_BALL_ID    0xC
#define BALLS_AMOUNT    0x1A
#define NEWBALLS_TILE   0xF100
#define NEWBALLS_STARS  0xF120

#define NEWFLAGS		0x0
#define NEWVARS 0x6
#define USEOLD_DEX_FLAGS true

#define NO_OF_TUTORS		32
#define NO_OF_TMS_HMS		58
#define EXPANDED_LEARNSETS		false

#define FAMEHALL_MAX 40

#define custom_flagcheck(flagID)(flagID == 0 ? 1 : getflag(flagID))
#define custom_flagset(flagID)(flagID == 0 ? 0 : setflag(flagID))
#define ATLEAST_ONE(value)(value ? value : 1)
#define BIC(value, bit)(value & (~(bit)))
#define NEG_AND(value, to_neg)(value & (to_neg * (-1)))
#define get_nature(PiD)((__umodsi3(PiD, 25)))
#define get_gender(pid, ratio)(((ratio) > (0xFF & pid) ? 0xFE : 0))

#define DEX_MENU_FLAG		0x861
#define POKE_MENU_FLAG		0x860
#define POKENAV_MENU_FLAG		0x862
#define BAG_MENU_FLAG		0x0
#define SAVE_MENU_FLAG		0x0
#define EXIT_MENU_FLAG		0x0
#define TRAINERCARD_MENU_FLAG		0x0
#define OPTIONS_MENU_FLAG		0x0

#define BW_REPEL		false
#define FIRERED_FISHING		false
#define CHAIN_FISHING		false
#define SHINY_CHARM		0x0
#define FISHING_FLAG		0x0

#define txt_pokemon P_, o_, k_, Poke_e, m_, o_, n_
#define txt_A_ball A_, Space, B_, a_, l_, l_
#define txt_for_catching f_, o_, r_, Space, c_, a_, t_, c_, h_, i_, n_, g_

#define percent_chance(percent)(__umodsi3(rng(), 101) <= percent)

enum ball_index{
    BALL_POKE, //0x0
    BALL_GREAT, //0x1
    BALL_SAFARI, //0x2
    BALL_ULTRA, //0x3
    BALL_MASTER, //0x4
    BALL_NET, //0x5
    BALL_DIVE, //0x6
    BALL_NEST, //0x7
    BALL_REPEAT, //0x8
    BALL_TIMER, //0x9
    BALL_LUXURY, //0xA
    BALL_PREMIER, //0xB
    //new balls
    #if BALL_EXPANSION == true
    BALL_LEVEL, //0xC
    BALL_LURE, //0xD
    BALL_MOON, //0xE
    BALL_FRIEND, //0xF
    BALL_LOVE, //0x10
    BALL_HEAVY, //0x11
    BALL_FAST, //0x12
    BALL_SPORT, //0x13
    BALL_DUSK, //0x14
    BALL_QUICK, //0x15
    BALL_HEAL, //0x16
    BALL_CHERISH, //0x17
    BALL_PARK, //0x18
    BALL_DREAM, //0x19
    BALL_BEAST, //0x1A
    #endif // BALL_EXPANSION
};

enum ball_stars{
    BALL_STICKS, //0
    BALL_STARS, //1
    BALL_BUBBLES, //2
    BALL_HEARTS, //3
    BALL_BIG_CROSSES, //4
    BALL_SMALL_CROSSES, //5
};


#define DAY_FIRST_HOUR      8
#define DAY_LAST_HOUR       20
#define NIGHT_FIRST_HOUR    21
#define NIGHT_LAST_HOUR     7

#include "types.h"
#include "structs.h"
#include "functions.h"
#include "locations.h"

#include "./defines/poke_types.h"
#include "./defines/abilities.h"
#include "./defines/moves.h"
#include "./defines/get_attr.h"
#include "./defines/items.h"
#include "./defines/item_effects.h"
#include "./defines/pokes.h"
#include "./defines/flavours.h"

#define BATTLE_FRONTIER_BATTLE (battle_flags.frontier_general || battle_flags.battle_dome || battle_flags.battle_palace || battle_flags.battle_arena || battle_flags.battle_factory || battle_flags.flag_x100000 || battle_flags.battle_pyramid)
#define ADD_POINTER(ptr, to_add)((void*)((void*)(ptr) + (u32)(to_add)))

#define Space 0x00
#define JumpLine 0xFE
#define BuffCharac 0xFD
#define Apos 0xB4
#define Comma 0xB8
#define Dot 0xAD
#define Exclam 0xAB
#define QuestionMark 0xAC
#define Termin 0xFF
#define Dash 0xAE
#define Slash 0xBA

#define Poke_e 0x1B

#define CharTwo 0xA3

#define A_ 0xBB
#define B_ 0xBC
#define C_ 0xBD
#define D_ 0xBE
#define E_ 0xBF
#define F_ 0xC0
#define G_ 0xC1
#define H_ 0xC2
#define I_ 0xC3
#define J_ 0xC4
#define K_ 0xC5
#define L_ 0xC6
#define M_ 0xC7
#define N_ 0xC8
#define O_ 0xC9
#define P_ 0xCA
#define Q_ 0xCB
#define R_ 0xCC
#define S_ 0xCD
#define T_ 0xCE
#define U_ 0xCF
#define V_ 0xD0
#define W_ 0xD1
#define X_ 0xD2
#define Y_ 0xD3
#define Z_ 0xD4

#define a_ 0xD5
#define b_ 0xD6
#define c_ 0xD7
#define d_ 0xD8
#define e_ 0xD9
#define f_ 0xDA
#define g_ 0xDB
#define h_ 0xDC
#define i_ 0xDD
#define j_ 0xDE
#define k_ 0xDF
#define l_ 0xE0
#define m_ 0xE1
#define n_ 0xE2
#define o_ 0xE3
#define p_ 0xE4
#define q_ 0xE5
#define r_ 0xE6
#define s_ 0xE7
#define t_ 0xE8
#define u_ 0xE9
#define v_ 0xEA
#define w_ 0xEB
#define x_ 0xEC
#define y_ 0xED
#define z_ 0xEE

#endif /* DEFINES_H */




























