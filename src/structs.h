#ifndef B_STRUCTS
#define B_STRUCTS

#include "types.h"
#include "defines.h"

struct newsaveblock{
    #if NEWFLAGS > 0
        u8 new_flags[NEWFLAGS / 8];
    #endif // NEWFLAGS
    #if NEWVARS > 0
        u16 new_vars[NEWVARS];
    #endif // NEWVARS
};

extern struct newsaveblock new_saveblock;

struct tile_obj{
    void* tile_ptr;
    u16 size;
    u16 tileID;
};

struct move_info{
    u8 scriptID;
    u8 power;
    u8 type;
    u8 acc;
    u8 pp;
    u8 effect_chance;
    u8 target;
    u8 priority;
    u8 flags;
    u8 padd[3];
};

extern const struct move_info (*move_table)[355];

struct fame_hall_poke_pos{
    s16 x_jump;
    s16 y_jump;
    u16 field4;
    u16 field6;
};

extern struct fame_hall_poke_pos fame_hall_pokes_pos1[6];
extern struct fame_hall_poke_pos fame_hall_pokes_pos2[3];

struct fame_hall_pokes_new{
    u32 TiD;
    u32 PiD;
    u16 species;
    u8 lvl;
    u8 nick[10];
    //u8 pad1;
    //u8 pad2;
};

struct fame_hall_team_new{
    struct fame_hall_pokes_new pokes[6];
    u8 trainerID; //for allowing to have custom entries
};

extern struct fame_hall_team_new* fame_hall_pokes;
extern struct fame_hall_team_new (*fame_hall_teams)[FAMEHALL_MAX];

struct famehall_data{
    u8 unkown;
};

extern struct famehall_data* fame_hall_data;

union decomp_buffer{
    u8 bytes[0x2000];
    struct fame_hall_team_new saved_HOF_pokes[FAMEHALL_MAX];
};

extern union decomp_buffer decompression_buffer;

enum menu_options{
    MENU_DEX,
    MENU_POKE,
    MENU_BAG,
    MENU_POKENAV,
    MENU_TRAINERCARD,
    MENU_SAVE,
    MENU_OPTION,
    MENU_EXIT,
    MENU_SAFARI_RETIRE,
    MENU_FRONTIER_CARD,
    MENU_REST,
    MENU_FRONTIER_RETIRE,
    MENU_FRONTIER_BAG,
};

struct blockset{
    u8 compressed;
    u8 pal_mode_flag;
    u8 field2;
    u8 field3;
    void* tileset;
    void* pals;
    void* blockset_data;
    void* bg_bytes;
    void* anim_routine;
};

struct palette{
  u16 c[16];
};

extern struct palette OBJ_PaletteMem[16];
extern struct palette OBJ_PaletteMem2[16];
extern struct palette BG_PaletteMem[16];
extern struct palette BG_PaletteMem2[16];

struct footer{
    u32 width_blocks;
    u32 height_blocks;
    const void* border_data;
    const void* blocksID_movement;
    const struct blockset* primary_blockset;
    const struct blockset* secondary_blockset;
};


struct mapheader{
    struct footer* map_footer;
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

extern const struct evolution_sub (*evo_table)[POKES_NO][EVO_PER_POKE];

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

extern const struct battle_participant battle_participants[4];

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

struct rbox_ram{
    u8 bg_id;
    u8 x_coord;
    u8 y_coord;
    u8 width;
    u8 height;
    u8 field5;
    u16 vram_tileset_offset;
    void* pixels;
};

extern struct rbox_ram rboxes[16];

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
    u8 double_battle;
    u8 padd1[3];
    u32 ai_scripts;
    u8 poke_number;
    u8 padd2[3];
    struct poke_trainer_data* poke_data;
};

extern const struct trainer_data (*trainer_table)[];

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
    u16 padding;
};

extern const struct poke_basestats (*basestat_table)[POKES_NO];

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
  u32 field1;
  u32 field2;
};

struct rotscale_frame {
  u16 scale_delta_x;
  u16 scale_delta_y;
  u8 rot_delta;
  u8 duration;
  u16 field_6;
};

struct sprite{
    u8 x;
    u8 flags1;
    u8 y;
    u8 flags2;
    u8 attr1;
    u8 attr2;
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

struct tile_ball{
    void* ptr;
    u16 field_4;
    u16 tile_ID;
};

struct tile_pal{
    const struct palette* ptr;
    u16 tile_ID;
};

extern const struct tile_pal pal_npc_tile_table[];

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
    struct multichoice_option *options_ptr;
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

extern const struct item_data* item_table;

struct pokeblock{
    u8 index;
    u8 spicy_lvl;
    u8 dry_lvl;
    u8 sweet_lvl;
    u8 bitter_lvl;
    u8 sour_lvl;
    u8 feel;
    u8 unkown;
};

struct warp{
    u8 map_bank;
    u8 map_no;
    s16 x_coords;
    s16 y_coords;
    u16 field6;
};

struct owned_items{
    u16 itemID;
    u16 amount;
};

struct rom_npc{
    u8 localID; //0
    u8 spriteID; //1
    u16 field2; //2
    s16 x_pos; //4
    s16 y_pos; //6
    u8 height; //8
    u8 behaviour;//9
    u16 behaviour_property;// xA
    u8 is_trainer; //xC
    u8 fieldD; //xD
    u16 radius_plantID; //xE
    void* script; //x10
    u16 flag; //x14
    u16 field16; //x16
};

struct planted_berry{
    u8 berryID;
    u8 state;
    u16 minutes_stage;
    u8 yield;
    u8 watered;
    u8 field6;
    u8 field7;
};

struct secret_base{
    u8 baseID; //0
    u8 owner_state; //1
    u8 owner_name[7]; //2
    u8 trainerID[4]; //9
    u8 font; //xD
    u16 fieldE; //xE
    u8 enters; //x10
    u8 field11; //x11
    u8 used_decorationsID[16]; //x12
    u8 decoration_positions[16];
    u8 field32;
    u8 field33;
    u32 PiD[6];
    u16 moves[6][4];
    u16 species[6];
    u16 held_items[6];
    u8 levels[6];
    u8 evs[6];
};

struct npc_state{
    u8 field0; //0
    u8 field1; //1
    u8 field2; //2
    u8 field3; //3
    u8 oam_id; //4
    u8 spriteID; //5
    u8 movement_type;// 6
    u8 walking_type; //7
    u8 localID; //8
    u8 map_no; //9
    u8 map_bank; //xA
    u8 height; //xB
    u32 field_C; //xC
    u16 pos_x; //x10
    u16 pox_y;
    u16 pos_x_plus7;
    u16 pos_y_plus7;
    u8 walking_direction;
    u8 field_19;
    u8 field_1A;
    u8 field_1B;
    u8 field_1C;
    u8 plantID;
    u8 field_1E;
    u8 field_1F;
    u8 field_20;
    u8 field_21;
    u8 field_22;
    u8 field_23;
};

extern struct npc_state npc_states[16];

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
    u16 coins;
    u16 item_on_select;
    struct owned_items PC_items[50];
    struct owned_items items_pocket[30];
    struct owned_items key_items_pocket[30];
    struct owned_items balls_pocket[16];
    struct owned_items tms_pocket[64];
    struct owned_items berries_pocket[46];
    struct pokeblock pokeblocks[40];
    u8 seenflags_1[52];
    u16 berry_blending_results[3];
    u16 field_9C2[3];
    u16 step_counter_to_xFF;
    u8 pokenav_matchcall[64];
    u8 field_A0A[38];
    struct npc_state npc_states_rom[16];
    struct rom_npc map_npcs[64];
    u8 flags[300];
    u16 vars[256];
    u32 encrypted_counters[64];
    struct planted_berry planted_berries[128];
    struct secret_base secret_bases[20];
    u8 field_271C[5128];
    u8 seenflags_2[52];
};

extern struct saveblock1* sav1;

struct saveblock2{
    u8 name[8];
    u8 gender;
    u8 field_9;
    u8 TiD_1;
    u8 TiD_2;
    u8 TiD_3;
    u8 TiD_4;
    u16 hours_played;
    u8 mintues_played;
    u8 seconds_played;
    u8 frames_played;
    u8 buttons_style;
    u8 txt_speed_frame;
    u8 battle_scene_style_sound;
    u8 field16;
    u8 field17;
    u8 dex_order;
    u8 dex_mode;
    u8 national_dex;
    u8 field1B;
    u32 unown_PiD;
    u32 spinda_PiD;
    u8 field24[4];
    u8 caught_flags[52];
    u8 seen_flags[52];
};

extern struct saveblock2* sav2;

struct old_moveset{
    u16 move : 9;
    u16 level : 7;
};

#pragma pack(push,1)
struct new_moveset{
    u16 move;
    u8 level;
};
#pragma pack(pop)

#if EXPANDED_LEARNSETS == true
    extern struct new_moveset* learnsets_table [POKES_NO];
#else
    extern struct old_moveset* learnsets_table [POKES_NO];
#endif // EXPANDED_LEARNSETS

struct ov_script{
    u8 push_counter;
    u8 mode;
    u8 comp_result;
    u8 padd;
    void* waiting_routine;
    void* curr_instruction;
    void* stack[20];
    void* commands_table_start;
    void* commands_table_end;
    u32 vars [4];
};

struct button{
    u16 A : 1;
    u16 B : 1;
    u16 SELECT : 1;
    u16 START : 1;
    u16 RIGHT : 1;
    u16 LEFT : 1;
    u16 UP : 1;
    u16 DOWN : 1;
    u16 R : 1;
    u16 L : 1;
};

struct superstate{
    void* callback1;
    void* callback2;
    void* callback_backup;
    void* vblank_callback;
    void* hblank_callback;
    void* field14;
    void* field18;
    u32 bits_to_wait_for;
    u32 game_timer;
    u32 field24;
    u16 buttons0;
    u16 buttons1;
    u16 buttons2;
    struct button pressed_buttons;
};

extern struct superstate super;

struct walkrun_state{
    u8 bitfield;
    u8 sprite_change;
    u8 field2;
    u8 moving;
    u8 oamID;
    u8 npcID;
    u8 lock;
    u8 gender;
};

extern struct walkrun_state walkrun;

struct wild_pokemon{
    u8 min_lvl;
    u8 max_lvl;
    u16 species;
};

extern struct wild_pokemon wild_data_feebas;

struct wild_encounter{
    u8 encounter;
    struct wild_pokemon (*poke)[10];
};

struct wild_poke_data{
    u8 map_bank;
    u8 map_no;
    u16 pad;
    struct wild_encounter* grass;
    struct wild_encounter* surfing;
    struct wild_encounter* rocksmash;
    struct wild_encounter* fishing;
};

extern struct wild_poke_data wild_pokemon_data[125];

struct npc_type{
    u16 tiles_tag;
    u16 pal_tag;
    u16 pal_tag2;
    u16 field_6;
    struct coords16 pos_neg_center;
    u8 field_C;
    u8 field_D;
    u8 field_E;
    void* oam;
    void* formation;
    void* image_anims;
    void* gfx_table;
    void* rotscale_anims;
};

struct fadescreen_exec{
    u16 BLDCNT;
    u8 field2;
    u8 field3;
    u16 BLDY;
    u8 field6;
    u8 flags : 7;
    u8 is_fading : 1;
};

extern struct fadescreen_exec fadescreen_info;

#endif /* B_STRUCTS */
