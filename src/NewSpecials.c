#include "defines.h"
#include <stdlib.h>
#include <string.h>

//function is the same as in the repel code, put there to avoid code duplication
#if BW_REPEL == true
    void multichoice_rbox_create(u8 no_of_options, u8 pos_x, u8 pos_y, struct multichoice_option* Options);
#else
    void multichoice_rbox_create(u8 no_of_options, u8 pos_x, u8 pos_y, struct multichoice_option* Options)
    {
        u8 width = 0;
        for (u8 i = 0; i < no_of_options; i++)
            width = str_font1_get_min_width(Options[i].text_ptr, width);
        width = str_rbox_max_width(width);
        pos_x = str_rbox_X_pos(pos_x, width);
        u8 rboxID = rbox_create_and_sth(pos_x, pos_y, width, no_of_options * 2);
        rbox_frame_menupalette(rboxID, 0);
        rbox_put_multichoice_text(rboxID, no_of_options, Options);
        rbox_create_multichoice_cursor(rboxID, no_of_options, 0);
        bg_mark_for_sync(0);
        prepare_multichoice_task(0, no_of_options, rboxID, 0);
    }
#endif // BW_REPEL


u16 sp20F_check_move_comp(void) //poke id, move, checker = 1 - check learnset, 2 - check move tutors, 4 - check tms/hms, 8 - check egg moves, 0x10 - if knows sketch means can learn all moves
{
    u16 can_learn = 0;
    u16 species = get_attributes(&party_player[var_8004], ATTR_SPECIES, 0);
    u16 move = var_8005;
    if (var_8006 & 1)
    {
        #if EXPANDED_LEARNSETS == true
        struct new_moveset* moveset = learnsets_table[species];
        u16 terminator = 0;
        #else
        struct old_moveset* moveset = learnsets_table[species];
        u16 terminator = 0x1FF;
        #endif // EXPANDED_LEARNSETS
        for (u16 i = 0; moveset[i].move != terminator; i++)
        {
            if (moveset[i].move == MOVE_SKETCH && var_8006 & 0x10) {can_learn |= 0x10;}
            if (moveset[i].move == move) {can_learn |= 1; break;}
        }
    }
    if (var_8006 & 2)
    {
        for (u8 i = 0; i < NO_OF_TUTORS; i++)
        {
            if (tutor_moves_table[i] == move)
            {
                if (check_move_tutor_compatibility(species, i)) {can_learn |= 2;}
                break;
            }
        }
    }
    if (var_8006 & 4)
    {
        for (u8 i = 0; i < NO_OF_TMS_HMS; i++)
        {
            if (tm_moves_table[i] == move)
            {
                if (check_TM_compatibility(species, i)) {can_learn |= 4;}
                break;
            }
        }
    }
    if (var_8006 & 8)
    {
        u8 egg_moves_no = get_egg_moves(&party_player[var_8004], egg_move_buffer);
        for (u8 i = 0; i < egg_moves_no; i++)
        {
            if (egg_move_buffer[i] == move) {can_learn |= 0x8; break;}
        }
    }
    return can_learn;
}

u16 sp210_get_attr(void) //poke id, attribute
{
    return get_attributes(&party_player[var_8004], var_8005, 0);
}

void sp211_set_attr(void) //poke id, attribute, wanted value
{
    set_attributes(&party_player[var_8004], var_8005, &var_8006);
}

bool sp212_ability_switcher(void) //poke id; returns 0 if couldnt switch
{
    bool switched = 0;
    struct pokemon* poke = &party_player[var_8004];
    struct poke_basestats* stats = &(*basestat_table)[get_attributes(poke, ATTR_SPECIES, 0)];
    if (stats->ability2)
    {
        u8 ability = get_attributes(poke, ATTR_ABILITY_BIT, 0) ^ 1;
        set_attributes(poke, ATTR_ABILITY_BIT, &ability);
        switched = 1;
    }
    return switched;
}

void sp213_add_safariballs(void) //amount to add or sub if the value is negative
{
    safari_balls += (s8)(var_8004);
}

void sp214_add_safaristeps(void) //amount to add or sub if the value is negative
{
    safari_step_counter += (s16)(var_8004);
}

void playernick_callback2(void)
{
    if (compare_two_strings(battle_text_buff1, &sav2->name) == 0)
        var_800D_lastresult = 0;
    else
        var_800D_lastresult = 1;
    script_function_to_return = script_exec_return_with_music;
    set_callback2(c2_exit_to_overworld_2_switch);
}

void sp215_set_player_nick(void)
{
    strcpy_xFF_terminated_0(battle_text_buff1, &sav2->name);
    prepare_string_inserter(0, &sav2->name, sav2->gender, 0, 0, playernick_callback2);
}

void sp216_set_timer(void)
{
    custom_timer = super.game_timer;
    timer_helper = 0;
}

#define get_passed_frames()(timer_helper == 0 ? super.game_timer - custom_timer : custom_timer)

u16 sp217_get_timer(void) //in seconds
{
    u32 passed_frames = get_passed_frames();
    return passed_frames / 60;
}

void sp218_buffer_timer(void) //minutes, seconds and decimal part
{
    u32 passed_frames = get_passed_frames();
    u8 minute_text[] = {Space, m_, i_, n_, u_, t_, e_, 0xFF};
    u8 seconds_text[] = {Space, s_, e_, c_, o_, n_, d_, s_, 0xFF};
    u8 s_text[] = {s_, 0xFF};
    u8 comma_text[] = {Comma, 0xFF};
    u8 space_text[] = {Space, 0xFF};
    u32 seconds = passed_frames / 60;
    u32 minutes = seconds / 60;
    u8* string_ptr = script_text_buff1;
    if (minutes)
    {
        string_ptr = unsigned_int_to_string(string_ptr, minutes, 0, 3);
        string_ptr = strcpy_xFF_terminated_0(string_ptr, minute_text);
        if (minutes > 1) {string_ptr = strcpy_xFF_terminated_0(string_ptr, s_text);}
        string_ptr = strcpy_xFF_terminated_0(string_ptr, space_text);
    }
    seconds = seconds - minutes * 60;
    string_ptr = unsigned_int_to_string(string_ptr, seconds, 0, 2);
    string_ptr = strcpy_xFF_terminated_0(string_ptr, comma_text);
    string_ptr = unsigned_int_to_string(string_ptr, __umodsi3(passed_frames, 60) * 100 / 60, 2, 2);
    strcpy_xFF_terminated_0(string_ptr, seconds_text);
}

void sp219_stop_timer(void)
{
    custom_timer = super.game_timer - custom_timer;
    timer_helper = custom_timer;
}

void sp21A_continue_timer(void)
{
    custom_timer = super.game_timer - custom_timer;
    timer_helper = 0;
}

void sp21B_new_multichoice(struct ov_script* script) //no_of_options, pos X, pos Y
{
    textbox_load_box_graphics();
    struct multichoice_option Multi[8];
    u8 options_no = var_8004;
    for (u8 i = 0; i < options_no; i++)
    {
        Multi[i].text_ptr = (void*) script_read_word(script);
        Multi[i].no_of_options = 0;
    }
    multichoice_rbox_create(options_no, var_8005, var_8006, Multi);
}

void sp21C_prepare_multichoice(void)
{
    multi_options = (struct multichoice_options*) malloc(sizeof(struct multichoice_options));
    multi_options->options_ptr = malloc(sizeof(struct multichoice_option) * 8);
    multi_options->no_of_options = 0;
}

void sp21D_add_to_multichoice(struct ov_script* script) //no of options to add
{
    struct multichoice_option* options = multi_options->options_ptr;
    for (u8 i = 0; i < var_8004; i++)
    {
        options[multi_options->no_of_options].no_of_options = 0;
        options[multi_options->no_of_options].text_ptr = (void*)(script_read_word(script));
        multi_options->no_of_options++;
    }
}

void sp21E_display_multichoice()//pos x, pos y
{
    textbox_load_box_graphics();
    multichoice_rbox_create(multi_options->no_of_options, var_8004, var_8005, multi_options->options_ptr);
    free(multi_options->options_ptr);
    free(multi_options);
}

void task_text_moving(u8 taskID)
{
    u16 *state_tracker = &tasks[taskID].private[0];
    switch (*state_tracker)
    {
    case 0:
        textbox_load_box_graphics();
        (*state_tracker)++;
        break;
    case 1:
        rbox_prepare_textbox(0, 1);
        (*state_tracker)++;
        break;
    case 2:
        if (textbox_print_ret() != 1)
        {
            textbox_text_loading = 0;
            text_box_flags &= 0xFB;
            task_delete(taskID);
        }
        break;
    }
}

void sp21F_text_moving(struct ov_script* script) //8004 = text speed, 8 - slow, 4 - medium, 1 - fast, 0xFFFF use the one in the menu options
{
    void* text_ptr = (void*) script_read_word(script);
    if (textbox_text_loading == 0)
    {
        fdecode_and_copy(displayed_string, text_ptr);
        text_box_flags = 5;
        u8 text_speed = get_text_speed();
        if (var_8004 != 0xFFFF) {text_speed = var_8004;}
        box_related_one(0, 1, displayed_string, text_speed, 0, 2, 1, 3);
        task_add(task_text_moving, 0x50);
        textbox_text_loading = 2;
    }
}

void sp220_give_pokeblock(struct ov_script* script) //arg pokeblock ptr
{
    var_800D_lastresult = pokeblock_give((void*)(script_read_word(script)));
}

u8 sp221_create_rbox() //X pos, Y pos, width, height
{
    struct rbox_rom rbox;
    rbox.bg_id = 0;
    rbox.x_coord = var_8004;
    rbox.y_coord = var_8005;
    rbox.width = var_8006;
    rbox.height = var_8007;
    rbox.field5 = 0xF;
    u8 freerbox = 0;
    for (u8 i = 0; i < 16; i++)
    {
        if (rboxes[i].bg_id == 0xFF)
        {
            freerbox = i;
            break;
        }
    }
    rbox.vram_tileset_offset = 0x8 + (freerbox * 64);
    u8 rboxID = rbox_create(&rbox);
    if (rboxID != 0xFF)
    {
        rbox_default_unpack_fill(rboxID, 0);
        rbox_fill(rboxID, 0x11);
        rbox_800378C(rboxID);
        rbox_to_vram(rboxID, 3);
        textbox_load_box_graphics();
    }
    return rboxID;
}

void sp222_puttext_on_rbox(struct ov_script* script) //rboxID, right_move, down_move
{
    u8 rboxID = var_8004;
    void* text_ptr = (void*)(script_read_word(script));
    if (rboxID != 0xFF)
    {
        rbox_fill(rboxID, 0x11);
        put_text_on_rbox(rboxID, 1, text_ptr, var_8005, var_8006, 0, 0);
    }
}

void sp223_delete_rbox() //rboxID
{
    u8 rboxID = var_8004;
    if (rboxID != 0xFF)
    {
        rbox_delete2(rboxID, 0);
        rbox_8003574(rboxID);
    }
}

void sp224_delete_pokemon() //pokeID
{
    pokemon_slot_purge(&party_player[var_8004]);
    party_move_up_no_free_slots_in_between();
}

u8 sp225_set_lvl() //pokeID, lvl; returns old lvl
{
    struct pokemon* poke = &party_player[var_8004];
    u8 old_lvl = get_attributes(poke, ATTR_LEVEL, 0);
    set_poke_to_lvl(poke, var_8005);
    return old_lvl;
}

void sp226_setword(struct ov_script* script) //word to write, destination
{
    u32 word = script_read_word(script);
    u32* dst = (u32*) script_read_word(script);
    *dst = word;
}

u16 sp227_getattr2() //pokeID, info
{
    struct pokemon* poke = &party_player[var_8004];
    u16 value;
    switch (var_8005)
    {
    case 0: //nature
        value = get_pokemon_nature(poke);
        break;
    case 1: //gender
        value = poke_get_gender(poke);
        break;
    case 2: //is shiny
        value = is_pokemon_shiny(poke);
        break;
    default:
        value = 0xFFFF;
        break;
    }
    return value;
}

u32 create_shiny_pid(u8 nature, u8 gender, u8 ratio, u32 TiD)
{
    u16 xored_tid = (TiD & 0xFFFF) ^ (TiD >> 0x10);
    //xored_tid ^ xored_pid <= 7, so xored_pid must be the same as xoredtid with the exception of last 3 bits
    xored_tid = BIC(xored_tid, 7);
    xored_tid |= __umodsi3(rng(), 8);

    u32 PiD;
    do
    {
        u16 pid1 = 0, pid2 = 0;
        for (u8 i = 0; i < 16; i++)
        {
            u16 bit = 1 << i;
            bool set = rng() & 1;
            if (xored_tid & bit) //bit is 1; 1 ^ X == 0 iff X is 1; then pid1 is 0 and pid2 is 1 or reversed
            {
                if (set)
                    pid1 |= bit;
                else
                    pid2 |= bit;
            }
            else //bit is 0; 0 ^ X == 0 iff X is 0; then pid1 is 0 and pid2 is 0 or pid is 1 and pid2 is 1
            {
                if (set)
                {
                    pid1 |= bit;
                    pid2 |= bit;
                }
            }
        }
        if (rng() & 1)
            PiD = (pid1) | (pid2 << 0x10);
        else
            PiD = (pid2) | (pid1 << 0x10);
    } while((nature <= 25 && nature != get_nature(PiD)) || (gender != 0xFF && gender != get_gender(PiD, ratio)));

    return PiD;
}

void sp228_setattr2() //pokeID, nature, gender, shininess
{
    struct pokemon* poke = &party_player[var_8004];
    u32 PiD = get_attributes(poke, ATTR_PID, 0);

    u8 wanted_nature = var_8005;
    if (wanted_nature >= 25) {wanted_nature = get_nature(PiD);}

    u16 species = get_attributes(poke, ATTR_SPECIES, 0);
    u8 ratio = (*basestat_table)[species].gender_ratio;
    u8 wanted_gender = get_gender(PiD, ratio);
    if (ratio != 0 && ratio != 0xFE && ratio != 0xFF && (var_8006 == 0 || var_8006 == 0xFE))
    {
        wanted_gender = var_8006;
    }

    u32 TiD = get_full_trainerID();

    //just change gender and nature, easy
    if (var_8007 != 1)
    {
        do
        {
            PiD = rng() | (rng() << 0x10);
        } while((wanted_nature <= 25 && wanted_nature != get_nature(PiD)) || (wanted_gender != 0xFF && wanted_gender != get_gender(PiD, ratio)));
    }
    //this one may take more time
    else
    {
        PiD = create_shiny_pid(wanted_nature, wanted_gender, ratio, TiD);
    }

    //since pid completely changes pokemon structure we need to copy all the data
    struct pokemon* changed = malloc(sizeof(struct pokemon));
    pokemon_slot_purge(changed);
    pokemon_make_wrapper(changed, species, get_attributes(poke, ATTR_LEVEL, 0), 1, 1, PiD, 1, TiD);

    for (u8 i = 0xA; i <= 0x50; i++)
    {
        u32 attr = 0;
        attr = get_attributes(poke, i, 0);
        set_attributes(changed, i, &attr);
    }
    //set font lang and marks
    u32 attr = 0;
    attr = get_attributes(poke, ATTR_FONT_LANG, 0);
    set_attributes(changed, ATTR_FONT_LANG, &attr);

    attr = 0;
    attr = get_attributes(poke, ATTR_MARKS, 0);
    set_attributes(changed, ATTR_MARKS, &attr);

    //set nick and ot name
    u8 nick[12];
    get_attributes(poke, ATTR_NAME, nick);
    set_attributes(changed, ATTR_NAME, nick);

    get_attributes(poke, ATTR_OT_NAME, nick);
    set_attributes(changed, ATTR_OT_NAME, nick);

    //set correct ball in case of changing origin struct
    /* todo */

    memcpy(poke, changed, sizeof(struct pokemon));
    free(changed);
}

void sp229_createpokemon() //species, lvl, nature, shininess
{
    pokemon_purge_opponent();
    struct pokemon* poke = &party_opponent[0];
    u32 PiD;
    u8 nature = var_8006;
    if (var_8007 == 1) //shininess
    {
        PiD = create_shiny_pid(nature, 0xFF, 0xFF, get_full_trainerID());
    }
    else
    {
        do
        {
            PiD = rng() | (rng() << 0x10);
        } while(nature <= 25 && nature != get_nature(PiD));
    }
    pokemon_make_wrapper(poke, var_8004, var_8005, 32, 1, PiD, 0, 0);
    damage_loc = (u32) poke;
}

void sp22A_custompoke_setattr() //attributeID, value
{
    struct pokemon* poke = &party_opponent[0];
    set_attributes(poke, var_8004, &var_8005);
}

u8 sp22B_givecustompoke()
{
    struct pokemon* poke = &party_opponent[0];
    u8 ret = generate_pokemon_data_for_player(poke);
    if (ret != 2) //poke was given
    {
        u16 natNO = species_to_dex_number(get_attributes(poke, ATTR_SPECIES, 0));
        get_or_set_pokedex_flags(natNO, 2);
        get_or_set_pokedex_flags(natNO, 3);
    }
    return ret;
}

void* new_specials_table[] = {sp20F_check_move_comp, sp210_get_attr, sp211_set_attr, sp212_ability_switcher, sp213_add_safariballs, sp214_add_safaristeps, sp215_set_player_nick,
sp216_set_timer, sp217_get_timer, sp218_buffer_timer, sp219_stop_timer, sp21A_continue_timer, sp21B_new_multichoice,
sp21C_prepare_multichoice, sp21D_add_to_multichoice, sp21E_display_multichoice, sp21F_text_moving, sp220_give_pokeblock, sp221_create_rbox,
sp222_puttext_on_rbox, sp223_delete_rbox, sp224_delete_pokemon, sp225_set_lvl, sp226_setword, sp227_getattr2, sp228_setattr2,
sp229_createpokemon, sp22A_custompoke_setattr, sp22B_givecustompoke};

bool s25_special(struct ov_script* script)
{
    u16 special_id = script_read_halfword(script);
    void (*function)();
    if (special_id >= 0x20F) {function = new_specials_table[special_id - 0x20F];}
    else {function = old_special_table[special_id];}
    function(script);
    return 0;
}

bool s26_special2(struct ov_script* script)
{
    u16* var_address = get_var_address(script_read_halfword(script));
    u16 special_id = script_read_halfword(script);
    u16 (*function)();
    if (special_id >= 0x20F) {function = new_specials_table[special_id - 0x20F];}
    else {function = old_special_table[special_id];}
    *var_address = function(script);
    return 0;
}
