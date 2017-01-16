#ifndef B_STRUCTS
#define B_STRUCTS

#include "types.h"
#include "defines.h"

struct mapheader{
    void* footer;
    void* events;
    void* scripts;
    void* connections;
    u16 music_id;
    u16 footer_id;
    u8 name;
    u8 light;
    u8 weather;
    u8 type;
    u8 field_18;
    u8 can_dig;
    u8 show_name;
    u8 battletype;
};

extern struct mapheader curr_mapheader;

struct real_time_clock_hex{
    u16 field0;
    u8 hour;
    u8 minute;
    u8 second;
};

extern struct real_time_clock_hex rtc_hex;

struct evolution_sub{
    u8 method;
    u16 paramter;
    u16 poke;
    u8 pad1;
    u8 pad2;
};

extern struct evolution_sub (*evo_table)[POKES_NO][EVO_PER_POKE];

struct iv_set
{
    u32 iv_hp: 5;
    u32 iv_atk: 5;
    u32 iv_def: 5;
    u32 iv_spd: 5;
    u32 iv_sp_atk: 5;
    u32 iv_sp_def: 5;
};

struct battle_participant {
    u16 poke_species;
    u16 atk;
    u16 def;
    u16 spd;
    u16 sp_atk;
    u16 sp_def;
    u16 moves[4];
    struct iv_set ivs;
    u8 hp_buff;
    u8 atk_buff;
    u8 def_buff;
    u8 spd_buff;
    u8 sp_atk_buff;
    u8 sp_def_buff;
    u8 acc_buff;
    u8 evasion_buff;
    u8 ability_id;
    u8 type1;
    u8 type2;
    u8 padd;
    u8 current_pp[4];
    u16 current_hp;
    u8 level;
    u8 happiness;
    u16 max_hp;
    u16 held_item;
    u8 poke_name[12];
    u8 trainer_name[8];
    u32 padd2;
    u32 pid;
    u32 status;
    u32 status2;
    u32 otid;
};

extern struct battle_participant battle_participants[4];

struct pokemenu{
    void* field_0;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 field_8;
};

struct rbox_rom{
    u8 bg_id;
    u8 x_coord;
    u8 y_coord;
    u8 width;
    u8 height;
    u8 field5;
    u16 vram_tileset_offset;
};

struct nature_stat{
    u8 stat[5];
};

extern struct nature_stat nature_stat_table[25];

struct pokemon_summary{
    u8 field0[112];
    u16 species;
    u8 field2;
    u8 field3;
    u8 field4;
    u8 lvl;
    u8 field6[10];
    u32 owned_exp;
    u8 field14[12];
    u16 curr_hp;
    u16 max_hp;
    u16 atk;
    u16 def;
    u16 sp_atk;
    u16 sp_def;
    u16 spd;
    u8 field_2E[5];
    u8 nature;
};

extern struct pokemon_summary* poke_summary;

struct exp_group{
    u32 required[256];
};

struct battle_flags{
    u32 double_battle : 1; //1
    u32 link : 1; //2
    u32 battle : 1; //4
    u32 trainer : 1; //8
    u32 save_birch : 1; //0x10
    u32 flagx20 : 1; //0x20
    u32 player_partner : 1; //0x40
    u32 safari : 1; //0x80
    u32 frontier_general : 1; //0x100
    u32 wally : 1; //0x200
    u32 roaming : 1; //0x400
    u32 flagx800 : 1; //0x800
    u32 flagx1000 : 1; //0x1000
    u32 legendary : 1; //0x2000
    u32 regis : 1; //0x4000
    u32 multibattle : 1; //0x8000
    u32 battle_dome : 1; //0x10 000
    u32 battle_palace : 1; //0x20 000
    u32 battle_arena : 1; //0x40 000
    u32 battle_factory : 1; //0x80 000
    u32 flag_x100000 : 1; //0x100 000
    u32 battle_pyramid : 1; //0x200 000
    u32 player_ingame_partner : 1; //0x400 000
    u32 flag_x800000 : 1; //0x800 000
    u32 recorded_battle : 1;// 0x1 000 000
    u32 flag_x2000000 : 1; //0x2 000 000
    u32 flag_x4000000 : 1; //0x4 000 000
    u32 secret_base : 1; //0x8 000 000
    u32 groudon : 1; //0x10 000 000
    u32 kyorge : 1; //0x20 000 000
    u32 rayquaza : 1; //0x40 000 000
    u32 flag_x80000000 : 1; //0x80 000 000
};

extern struct battle_flags battle_flags;

struct poke_config{
    u8 evs[6];
    u8 ivs[6];
    u8 abilityID;
    u8 nature;
    u8 poke_ball;
    u8 filler;
};

struct poke_trainer_data{
    u8 evs_id;
    u8 filler;
    u8 level;
    u8 filler2;
    u16 poke_id;
    u16 item_id;
    u16 move[4];
};

struct trainer_data{
    u8 custom_moves : 1;
    u8 custom_item : 1;
    u8 class;
    u8 music : 7;
    u8 gender : 1;
    u8 sprite;
    u8 name[10];
    u8 field_E;
    u16 items[4];
    u8 flags[4];
    u32 ai_scripts;
    u8 poke_number;
    u8 padd[3];
    struct poke_trainer_data* poke_data;
};

extern struct trainer_data trainer_table[];

struct pal_poke{
    void* pal;
    u16 species;
};

struct sprite_poke{
    void* sprite;
    u16 unkown;
    u16 species;
};

struct poke_basestats{
    u8 base_hp;
    u8 base_atk;
    u8 base_def;
    u8 base_spd;
    u8 base_spatk;
    u8 base_spdef;
    u8 type1;
    u8 type2;
    u8 catch_rate;
    u8 exp_yield;
    u16 evs_hp : 2; //0x1, 0x2
    u16 evs_atk : 2; //0x4, 0x8
    u16 evs_def : 2; //0x10, 0x20
    u16 evs_spd : 2; //0x40, 0x80
    u16 evs_spatk : 2;//0x100, 0x200
    u16 evs_spdef : 2; //0x400, 0x800
    u16 unused : 4; //0x1000, 0x2000, 0x4000, 0x8000
    u16 item1;
    u16 item2;
    u8 gender_ratio;
    u8 hatching;
    u8 friendship;
    u8 exp_curve;
    u8 egg_group1;
    u8 egg_group2;
    u8 ability1;
    u8 ability2;
    u8 safari_flee_rate;
    u8 dex_colour;
    u8 padding1;
    u8 padding2;
};

extern struct poke_basestats (*basestat_table)[POKES_NO];

struct pokemon{
	u32 PID;
	u32 OTID;
	char name[10];
	u16 language;
	u8 OT_name[7];
	u8 markings;
	u16 checksum;
	u16 padding_maybe;
	u8 data[48];
	u32 ailment;
	u8 level;
	u8 pokerus;
	u16 current_hp;
	u16 total_hp;
	u16 attack;
	u16 defense;
	u16 speed;
	u16 sp_attack;
	u16 sp_defense;
};

extern struct pokemon party_opponent[6];
extern struct pokemon party_player[6];

struct task{
    void* function;
    u8 id;
    u8 previous;
    u8 next;
    u8 priority;
    u16 private[16];
};

extern struct task tasks[16];

struct frame {
  u16 data;
  u16 duration;
};

struct rotscale_frame {
  u16 scale_delta_x;
  u16 scale_delta_y;
  u8 rot_delta;
  u8 duration;
  u16 field_6;
};

struct sprite{
    u8 y;
    u8 flags1;
    u16 x_and_flag2;
    u16 attr2;
    u16 rotscaleinfo;
};

struct coords16{
    u16 x;
    u16 y;
};

struct coords8{
    u8 x;
    u8 y;
};

struct object;
typedef void (*object_callback)(struct object*);

struct template {
  u16 tiles_tag;
  u16 pal_tag;
  struct sprite *oam;
  struct frame **animation;
  u32 *graphics;
  struct rotscale_frame **rotscale;
  object_callback callback;
};

struct object{
    struct sprite final_oam;
    struct frame **animation_table;
    void* gfx_table;
    void* rotscale_table;
    struct template* template;
    u32 field_18;
    void* callback;
    struct coords16 pos1;
    struct coords16 pos2;
    struct coords8 pos_neg_center;
    u8 anim_number;
    u8 anim_frame;
    u8 anim_delay_countdown;
    u8 anim_unkown_counter;
    u16 private[8];
    u8 in_use : 1;
    u8 flag2 : 1;
    u8 invisible : 1;
    u8 flags : 5;
    u8 bitfield2;
    u16 anim_data_offset;
    u8 field_42;
    u8 y_height_related;
};

struct image_resource{
    void *image;
    u16 size;
    u16 tag;
};

extern struct object objects[64];

struct multichoice_option{
    void* text_ptr;
    u8 no_of_options;
};

struct multichoice_options{
    struct multichoice_option* options_ptr;
    u8 no_of_options;
};

struct item_struct
{
    u8 name[14];
    u16 index;
    u16 price;
    u8 held_effect;
    u8 held_effect_quality;
    void *desc_pointer;
    u16 mystery_value;
    u8 pocket_no;
    u8 type_of_item;
    void *field_usage_code;
    u32 battle_usage;
    void *battle_usage_code;
    u32 extra_param;
};

struct item_data{
    struct item_struct items[ALL_ITEMS];
};

extern struct item_data* item_table;

struct warp{
    u8 map_bank;
    u8 map_no;
    s16 x_coords;
    s16 y_coords;
    u16 field6;
};

struct saveblock1{
    s16 x_coords;
    s16 y_coords;
    u8 map_bank;
    u8 map_no;
    u8 field_6[14];
    struct warp empty_warp;
    struct warp healing_spot;
    struct warp escape_place;
    u16 map_song;
    u8 ov_weather;
    u8 weather_specifier;
    u8 flash_on_map;
    u8 field_31;
    u16 map_footer_no;
    u8 field_34[496];
    u8 field_224[15];
    u8 field_233;
    u8 poke_quantity;
    u8 alignment_padding[3];
    struct pokemon player_party[6];
    u32 money;
    u16 coints;
    u16 item_on_select;
};

extern struct saveblock1* sav1;

#endif /* B_STRUCTS */
