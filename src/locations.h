#ifndef B_LOCATIONS
#define B_LOCATIONS

#include "types.h"

u8 script_text_buff1[0x100];
u8 battle_text_buff1[0x10];
u8 displayed_string[0x3E8];
u32 bits_table[32];
u16 var_8004;
u16 var_8005;
u16 var_8006;
u16 var_8007;
u16 var_800D_lastresult;
u16 var_8015_trainer_opponent_A;
u16 tutor_moves_table[NO_OF_TUTORS];
u16 tm_moves_table[NO_OF_TMS_HMS];
u16 egg_move_buffer[10];
void* old_special_table[527];
u8 safari_balls;
u16 safari_step_counter;
u32 custom_timer;
u32 timer_helper;
void* script_function_to_return;
struct multichoice_options* multi_options;
u8 text_box_flags;
u8 textbox_text_loading;
u16 last_fished_poke;
bool scripts_disabled;
u8 fishing_succesfull_tries;
bool has_fame_hall_records;
u8 text_Saving_dont_turn_off[34];
u8 text_No_dot[5];
u8 text_Lv_dot[5];
u8 text_IDNo_dot[6];
u8 text_Not_even_a_nibble[21];
u8 text_it_got_away[15];
u8 text_WALLY[6];
u8 (*poke_name_table)[POKES_NO][11];
u32 fame_hall_fading_related;
void* sav_function_sth;
u32 sav_ret_state;
u8 text_HALL_OF_FAME_NO[20];
u8 text_PICK_CANCEL[18];
u8 text_PICK_NEXT_CANCEL[24];
u32 damage_loc;
u32 oe_arguments[8];
struct template bag_item_template;

#endif /* B_LOCATIONS */
