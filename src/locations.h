#ifndef B_LOCATIONS
#define B_LOCATIONS

#include "types.h"

extern u8 script_text_buff1[0x100];
extern u8 battle_text_buff1[0x10];
extern u8 displayed_string[0x3E8];
extern const u32 bits_table[32];
extern u16 var_8004;
extern u16 var_8005;
extern u16 var_8006;
extern u16 var_8007;
extern u16 var_800D_lastresult;
extern u16 var_8015_trainer_opponent_A;
extern u16 tutor_moves_table[NO_OF_TUTORS];
extern u16 tm_moves_table[NO_OF_TMS_HMS];
extern u16 egg_move_buffer[10];
extern const void* old_special_table[527];
extern u8 safari_balls;
extern u16 safari_step_counter;
extern u32 custom_timer;
extern u32 timer_helper;
extern void* script_function_to_return;
extern struct multichoice_options* multi_options;
extern u8 text_box_flags;
extern u8 textbox_text_loading;
extern u16 last_fished_poke;
extern bool scripts_disabled;
extern u8 fishing_succesfull_tries;
extern bool has_fame_hall_records;
extern const u8 text_Saving_dont_turn_off[34];
extern const u8 text_No_dot[5];
extern const u8 text_Lv_dot[5];
extern const u8 text_IDNo_dot[6];
extern const u8 text_Not_even_a_nibble[21];
extern const u8 text_it_got_away[15];
extern const u8 text_WALLY[6];
extern const u8 (*poke_name_table)[POKES_NO][11];
extern u32 fame_hall_fading_related;
extern void* sav_function_sth;
extern u32 sav_ret_state;
extern const u8 text_HALL_OF_FAME_NO[20];
extern const u8 text_PICK_CANCEL[18];
extern const u8 text_PICK_NEXT_CANCEL[24];
extern u32 damage_loc;
extern u32 oe_arguments[8];
extern u16 gpu_pal_tags[0x10];
extern struct template bag_item_template;
extern u8 flags_x4000[0x10];
extern u16* vars_8000[22];
extern struct palette decompressed_pal[64];

#endif /* B_LOCATIONS */
