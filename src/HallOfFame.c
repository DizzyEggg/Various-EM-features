#include "defines.h"
#include <string.h>
#include <stdlib.h>

#define TRAINER_PLAYER 0xFF

//declarations of tasks that get referenced multiple times
void fame_hall_registering_poke_display(u8 taskID);
void set_fame_hall_pokes_from_party(u8 taskID);
void fame_hall_pc_display_poke_sprites(u8 taskID);

void fame_hall_print_poke_data(struct fame_hall_pokes_new* hall_poke)
{
    rbox_fill(0, 0);
    rbox_800378C(0);
    u16 species = hall_poke->species;
    u8 text[14];
    //print poke dex number
    if (species != POKE_EGG)
    {
        u8* string = text;
        string = strcpy_xFF_terminated_0(string, text_No_dot);
        u16 dex_num = species_to_dex_number(species);
        if (dex_num == 0xFFFF) //unknown dex number
        {
            *string = QuestionMark;
            string++;
        }
        else
        {
            string = unsigned_int_to_string(string, dex_num, 2, 3);
        }
        *string = 0xFF;
        fbox_print_sth(0, 1, 0x10, 1, (void*)(0x85E5388), 0xFF, text);
    }
    //print nick
    u8 nick[12];
    memcpy(nick, hall_poke->nick, 10);
    nick[10] = 0xFF;
    if (species == POKE_EGG)
    {
        u8 width = str_get_half_width(1, nick, 0xD0);
        fbox_print_sth(0, 1, width, 1, (void*)(0x85E5388), 0xFF, nick);
    }
    else
    {
        u8 width = str_get_width(1, nick, 0x80);
        fbox_print_sth(0, 1, width, 1, (void*)(0x85E5388), 0xFF, nick);
        text[0] = 0xBA;
        u8* string = &text[1];
        string = strcpy_xFF_terminated_0(string, (*poke_name_table)[species]);
        //append gender
        u8 gender = gender_from_pid(species, hall_poke->PiD);
        if (species != POKE_NIDORANF && species != POKE_NIDORANM && gender != 0xFF)
        {
            u8 gender_sign = 0x0;
            if (gender == 0x0) {gender_sign = 0xB5;}
            else if (gender == 0xFE) {gender_sign = 0xB6;}
            *string = gender_sign;
            string++;
        }
        *string = 0xFF;
        fbox_print_sth(0, 1, 0x80, 1, (void*)(0x85E5388), 0xFF, text);
        //print level
        string = text;
        string = strcpy_xFF_terminated_0(string, text_Lv_dot);
        unsigned_int_to_string(string, hall_poke->lvl, 0, 3);
        fbox_print_sth(0, 1, 0x24, 0x11, (void*)(0x85E5388), 0xFF, text);
        //print IDNO
        string = text;
        string = strcpy_xFF_terminated_0(string, text_IDNo_dot);
        unsigned_int_to_string(string, (u16)(hall_poke->TiD), 2, 5);
        fbox_print_sth(0, 1, 0x68, 0x11, (void*)(0x85E5388), 0xFF, text);
    }
    rbox_to_vram(0, 3);
}

void fame_hall_registering_display_another_poke(u8 taskID)
{
    struct task* curr_task = &tasks[taskID];
    //check timer
    if (curr_task->private[3])
        curr_task->private[3]--;
    else
    {
        u8* pok_id = (u8*) &curr_task->private[1];
        struct object* obj = &objects[curr_task->private[*pok_id + 5]];
        u32 to_fade = 0x10000 << (obj->final_oam.attr2 >> 4);
        fame_hall_fading_related |= to_fade;
        if (*pok_id > 4 || !fame_hall_pokes->pokes[*pok_id + 1].species) //all pokes displayed
        {
            curr_task->function = (void*)(0x08173D24 | 1);
        }
        else
        {
            *pok_id += 1;
            fadescreen_related(to_fade, 0, 0xC, 0xC, 0x63B0);
            obj = &objects[curr_task->private[*pok_id + 5]];
            obj->final_oam.attr2 = NEG_AND(obj->final_oam.attr2, 0xD) | 4;
            curr_task->function = fame_hall_registering_poke_display;
        }
    }
}

void fame_hall_registering_print_poke_data_when_sprite_done(u8 taskID)
{
    struct task* curr_task = &tasks[taskID];
    u8 pok_id = curr_task->private[1];
    struct object* obj = &objects[curr_task->private[pok_id + 5]];
    if (obj->callback == nullsub_1485)
    {
        obj->final_oam.flags1 &= 0xFB; //Clear bit 4
        fame_hall_print_poke_data(&fame_hall_pokes->pokes[pok_id]);
        curr_task->private[3] = 0x78; //time to wait between showing another poke
        curr_task->function = fame_hall_registering_display_another_poke;
    }
}

void fame_hall_registering_poke_display(u8 taskID)
{
    struct task* curr_task = &tasks[taskID];
    u8 pok_id = curr_task->private[1];
    struct fame_hall_pokes_new* hall_poke = &fame_hall_pokes->pokes[pok_id];
    struct fame_hall_poke_pos* pos_poke;
    if (curr_task->private[2] > 3)
        pos_poke = &fame_hall_pokes_pos1[pok_id];
    else
        pos_poke = &fame_hall_pokes_pos2[pok_id];
    u16 species = hall_poke->species;
    u16 objectID = make_poke_sprite_appear_as_object(species, hall_poke->TiD, hall_poke->PiD, 1, pos_poke->x_jump, pos_poke->y_jump, pok_id, 0xFFFF);
    struct object* obj = &objects[objectID];
    obj->private[0] = 0;
    obj->private[1] = pos_poke->field4;
    u32 sth = pos_poke->field6;
    if (species == POKE_EGG)
        sth = ((sth << 0x10) + 0xA0000) >> 0x10;
    obj->private[2] = sth;
    obj->private[7] = species;
    obj->callback = fame_hall_sprite_animate;
    curr_task->private[pok_id + 5] = objectID;
    rbox_delete1(0, 1);
    curr_task->function = fame_hall_registering_print_poke_data_when_sprite_done;
}

void prepare_fame_hall_registering(bool no_save)
{
    if (!hall_of_fame_build_registering_graphics())
    {
        u8 taskID = task_add(set_fame_hall_pokes_from_party, 0);
        tasks[taskID].private[0] = no_save;
        fame_hall_pokes = malloc_and_clear(sizeof(struct fame_hall_team_new));
    }
}

void prepare_fame_hall_registering1(void)
{
    prepare_fame_hall_registering(0);
}

void prepare_fame_hall_registering2(void)
{
    prepare_fame_hall_registering(1);
}

void fame_hall_registering_poke_display_after_savesound(u8 taskID)
{
    struct task* curr_task = &tasks[taskID];
    if (!(--curr_task->private[3]))
    {
        curr_task->function = fame_hall_registering_poke_display;
    }
}

void hall_of_fame_pokes_to_flash(u8 taskID)
{
    sav_function_sth = prepare_fame_hall_registering2;
    //check if save failed
    if (save_game_if_memory_present(3) == 0xFF && sav_ret_state)
    {
        bg_nullify_tilemap(1);
        bg_nullify_tilemap(3);
        rboxes_free();
        if (fame_hall_pokes) {free(fame_hall_pokes);}
        if (fame_hall_data) {free(fame_hall_data);}
        task_delete(taskID);
    }
    //save worked
    else
    {
        play_sound(0x37);
        struct task* curr_task = &tasks[taskID];
        curr_task->private[3] = 0x21; //time to wait before displaying pokemon
        curr_task->function = fame_hall_registering_poke_display_after_savesound;
    }
}

void prepare_savbuffer_for_new_fame_hall_pokes(u8 taskID)
{
    //clear fame data if save broken or first time registering
    if (!has_fame_hall_records || get_sav_data(3) != 1)
    {
        memset(decompression_buffer.saved_HOF_pokes, 0, 0x2000);
    }
    u8 free_id = 0;
    while (free_id < FAMEHALL_MAX && decompression_buffer.saved_HOF_pokes[free_id].pokes[0].species) {free_id++;}
    //check if all max amount of is stored and we have to clear the oldest one
    if (free_id >= FAMEHALL_MAX)
    {
        for (u8 i = 0; i < FAMEHALL_MAX - 1; i++)
        {
            memcpy(&decompression_buffer.saved_HOF_pokes[i], &decompression_buffer.saved_HOF_pokes[i + 1], sizeof(struct fame_hall_team_new));
        }
        free_id = FAMEHALL_MAX - 1;
    }
    memcpy(&decompression_buffer.saved_HOF_pokes[free_id], fame_hall_pokes, sizeof(struct fame_hall_team_new));
    rbox_prepare_textbox(0, 0);
    box_related_one(0, 1, text_Saving_dont_turn_off, 0, 0, 2, 1, 3);
    rbox_to_vram(0, 3);
    tasks[taskID].function = hall_of_fame_pokes_to_flash;
}

void set_fame_hall_pokes_from_party(u8 taskID)
{
    struct task* curr_task = &tasks[taskID];
    u16* pok_counter = &curr_task->private[2];
    *pok_counter = 0;
    //save poke's data
    for (u8 i = 0; i < 6; i++)
    {
        struct fame_hall_pokes_new* hall_poke = &fame_hall_pokes->pokes[i];
        struct pokemon* poke = &party_player[i];
        u16 species = get_attributes(poke, ATTR_SPECIES, 0);
        hall_poke->species = species;
        if (species)
        {
            hall_poke->lvl = get_attributes(poke, ATTR_LEVEL, 0);
            u8 nick[12];
            get_attributes(poke, ATTR_NAME, nick);
            for (u8 i2 = 0; i2 < 10; i2++)
            {
                hall_poke->nick[i2] = nick[i2];
            }
            hall_poke->TiD = get_attributes(poke, ATTR_TID, 0);
            hall_poke->PiD = get_attributes(poke, ATTR_PID, 0);
            *pok_counter += 1;
        }
    }
    fame_hall_pokes->trainerID = TRAINER_PLAYER;

    fame_hall_fading_related = 0;
    curr_task->private[1] = 0; //currently displayed poke's id
    curr_task->private[4] = 0xFF;
    //clear soon to be poke objects ID
    for (u8 i = 0; i < 6; i++)
    {
        curr_task->private[5 + i] = 0xFF;
    }
    //set function to run
    if (curr_task->private[0])
        curr_task->function = fame_hall_registering_poke_display;
    else
        curr_task->function = prepare_savbuffer_for_new_fame_hall_pokes;
}

void fame_hall_read_pokes_from_sav(u8 taskID)
{
    fame_hall_pc_create_rbox(0, 0x1E, 0, 0xC, 0x226);
    struct task* curr_task = &tasks[taskID];
    //check if corrupted save
    if (get_sav_data(3) != 1)
    {
        curr_task->function = fame_hall_corrupted_data_rbox;
    }
    else
    {
        u16 teams_no = 0;
        while (teams_no < FAMEHALL_MAX && decompression_buffer.saved_HOF_pokes[teams_no].pokes[0].species) {teams_no += 1;}
        memcpy(fame_hall_teams, decompression_buffer.saved_HOF_pokes, sizeof(struct fame_hall_team_new) * teams_no);
        curr_task->private[0] = teams_no - 1;
        curr_task->private[1] = get_encrypted_counter(0xA);
        curr_task->function = fame_hall_pc_display_poke_sprites;
    }
}

void fame_hall_pc_poke_data_display(u8 taskID)
{
    struct task* curr_task = &tasks[taskID];
    //do object magic
    for (u8 i = 0; i < 6; i++)
    {
        u8 objID = curr_task->private[5 + i];
        if (objID != 0xFF)
        {
            u8* attr2 = &objects[objID].final_oam.attr2;
            *attr2 = NEG_AND(*attr2, 0xD) | 4;
        }
    }
    //get pok id
    u8 pok_id = curr_task->private[2];

    //do object magic for specific poke

    struct object* obj = &objects[curr_task->private[5 + pok_id]];
    u8 attr2 = NEG_AND(obj->final_oam.attr2, 0xD);
    obj->final_oam.attr2 = attr2;

    //set fading info
    fame_hall_fading_related = (0x10000 << (attr2 >> 4)) ^ 0xFFFF0000;
    fame_hall_pal_fade_sth(fame_hall_fading_related, 0xC, 0x63B0);

    //find poke pointer
    struct fame_hall_team_new* hall_team = &(*fame_hall_teams)[curr_task->private[0]];

    struct fame_hall_pokes_new* hall_poke = &hall_team->pokes[pok_id];

    //play pokecry if not egg
    u16 species = hall_poke->species;
    if (species != POKE_EGG)
    {
        pokecry_stop();
        pokecry_play_sth(species, 0);
    }
    fame_hall_print_poke_data(hall_poke);
    //store new task
    curr_task->function = (void*)(0x08174728 | 1);
}

void fame_hall_pc_display_poke_sprites(u8 taskID)
{
    struct task* curr_task = &tasks[taskID];
    fame_hall_fading_related = 0;
    u8 team_id = curr_task->private[0];

    struct fame_hall_team_new* hall_team = &(*fame_hall_teams)[team_id];

    curr_task->private[2] = 0;

    //count pokemon
    u16* pokes_amnt = &curr_task->private[4];
    *pokes_amnt = 0;
    for (u8 i = 0; i < 6; i++)
    {
        if (hall_team->pokes[i].species)
            *pokes_amnt += 1;
    }

    //spawn sprites as objects and store their IDs
    for (u8 i = 0; i < 6; i++)
    {
        struct fame_hall_pokes_new* hall_poke = &hall_team->pokes[i];
        u16 species = hall_poke->species;

        u8 objID = 0xFF;

        if (species)
        {
            struct fame_hall_poke_pos* pos_poke;
            if (*pokes_amnt > 3)
                pos_poke = &fame_hall_pokes_pos1[i];
            else
                pos_poke = &fame_hall_pokes_pos2[i];
            s16 field_6 = pos_poke->field6;
            if (species == POKE_EGG)
                field_6 = ((field_6 << 0x10) + 0xA0000) >> 0x10;
            objID = spawn_poke_sprite_as_object(species, hall_poke->TiD, hall_poke->PiD, 1, pos_poke->field4, field_6, i, 0xFFFF);
            u8* attr2 = &objects[objID].final_oam.attr2;
            *attr2 = NEG_AND(*attr2, 0xD) | 4;
        }

        curr_task->private[5 + i] = objID;
    }

    //fade all pokemon and print text above

    pal_fade_sth(0xFFFF0000, 0xC, 0x63B0);

    unsigned_int_to_string(script_text_buff1, curr_task->private[1], 1, 3);
    fdecode_and_copy(displayed_string, text_HALL_OF_FAME_NO);

    void* txt = text_PICK_NEXT_CANCEL;
    if (curr_task->private[0] == 0)
        txt = text_PICK_CANCEL;
    fame_hall_pc_put_text(displayed_string, txt, 0, 0, 1);

    //store function
    curr_task->function = fame_hall_pc_poke_data_display;
}
