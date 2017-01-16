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

#define REPEL_X 0
#define REPEL_Y 0

#define POKES_NO        442
#define EVO_PER_POKE    5
#define ALL_ITEMS       386
#define MAX_LEVEL		250
#define MORELEVELS      true
#define COLORED_STATS	true

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
