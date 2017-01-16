#ifndef VANILLA_FUNCTIONS
#define VANILLA_FUNCTIONS

#include "types.h"
#include "structs.h"

void* get_item_desc(u16 itemID);
u8 get_item_quality(u16 itemID);
u8 get_item_battle_function(u16 itemID);
u8 checkitem(u16 itemID, u16 quantity);
u16 str_font1_get_min_width(void* string_address, u16 min_width);
u16 str_rbox_max_width(u16 width);
u16 str_rbox_X_pos(u8 x_pos, u16 width);
u8 rbox_create_and_sth(u8 x_pos, u8 y_pos, u8 width, u8 options_no);
void rbox_frame_menupalette(u8 rboxID, u8 vram_3);
void rbox_put_multichoice_text(u8 rboxID, u8 height, struct multichoice_option[]);
void rbox_create_multichoice_cursor(u8 rboxID, u8 options_no, u8 starting_option);
void bg_mark_for_sync(u8 bgID);
void prepare_multichoice_task(u8 canhitB, u8 options_no, u8 rboxID, u8 multichoiceID);
u32 get_attributes(struct pokemon* poke_address, u8 request, void* dst);
void set_attributes(struct pokemon* poke_address, u8 request, void* new_value);
u8 check_if_move_learnt(struct pokemon* poke_address, u16 move);
u8 check_walkrun_bitfield_for(u8 bitfield);
void pokemon_purge_opponent();
void pokemon_make_wrapper(struct pokemon* poke, u16 species, u8 lvl, u8 IVs, u8 pid_set, u32 PiD, u8 tid_set, u32 TiD);
u32 calculate_checksum(void* ptr, u32 to_read);
u16 pokemon_calc_checksum(struct pokemon* poke);
u16 rng();
void pokemon_encrypt(struct pokemon* poke);
void pokemon_decrypt(struct pokemon* poke);
void put_money_on_rbox(u8 rboxID, u8 width, u8 unkown, s32 value, u8 unkown2);
void* unsigned_int_to_string(void* destination, u32 num, u8 formatting, u8 max_digits);
u8 pokesummary_create_rbox(struct rbox_rom[], u8 index);
void string_box_build(u8 arg1, void *destination, s8 width_maybe, u8 arg4, u8 arg5, u8 arg6);
u8 bag_add_item(u16 itemID, u16 quantity);
void clear_f7_texts();
void store_f7_text(u8 ID, void* string_ptr);
void display_f7_texts(void* dst, void* src);
void pokemenu_print(u8 arg0, void* string_ptr, u8 arg2, void* ptr);
void fbox_print_sth(u8 arg0, u8 fontID, u8 arg2, u8 arg3, void* text_clr, u8 arg5, void* text_ptr);
u8 battle_print_on_HPmenu(void* text_ptr, u8 width, u8 height, u8 arg3, struct rbox_rom* rbox);
void update_rtc();
u8 poke_get_gender(struct pokemon* poke);

u32 __udivsi3(u32 numerator, u32 denumerator);
u32 __umodsi3(u32 numerator, u32 denumerator);

#endif /* VANILLA_FUNCTIONS */
