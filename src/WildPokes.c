#include "defines.h"
#include <string.h>

#define rod private[15]
#define tracker private[0]
#define dots_frames_to_wait private[1]
#define dots_printed private[2]
#define dots_no private[3]
#define bites_no private[12]

struct fish_loc{
    s16 x_pos;
    s16 y_pos;
    u8 tries;
};

extern struct fish_loc last_fishing_loc;

enum fish_rods{
    OLD_ROD, //0
    GOOD_ROD, //1
    SUPER_ROD, //2
};

typedef void (*task)(u8 taskID);
typedef bool (*fish_task)(struct task*);
void run_fishing_functions(u8 taskID);

void fishing_start(enum fish_rods fish_rod)
{
    task fish_loop = run_fishing_functions;
    u8 taskID = task_add(fish_loop, 0xFF);
    tasks[taskID].rod = fish_rod;
    fish_loop(taskID);
}

bool fish0_lock(struct task* curr_task)
{
    scripts_disabled = 1;
    walkrun.lock = 1;
    curr_task->tracker++;
    return 0;
}

bool fish1_npc_lock(struct task*);
bool fish2_rodthrow_anim(struct task*);

bool fish3_prepare_dots(struct task* curr_task)
{
    prepare_ov_textbox(0, 1);
    u8 dots;
    if (FIRERED_FISHING)
        dots = 10;
    else
    {
        dots = __umodsi3(rng(), 10) + 1;
        if (curr_task->bites_no == 0) {dots += 4;}
        if (dots > 10) {dots = 10;}
    }
    curr_task->dots_no = dots;
    curr_task->dots_frames_to_wait = 0;
    curr_task->dots_printed = 0;
    curr_task->tracker++;
    return 1;
}

bool fish4_print_dots(struct task* curr_task)
{
    fish_obj_hero_anim();
    curr_task->dots_frames_to_wait++;
    if (!FIRERED_FISHING && super.pressed_buttons.A) //too early pal, you're fucked
    {
        if (curr_task->bites_no) {curr_task->tracker = 0xC;} //it got away
        else {curr_task->tracker = 0xB;} //not even a nibble
        return 1;
    }
    else
    {
        if (curr_task->dots_frames_to_wait >= 19)
        {
            curr_task->dots_frames_to_wait = 0;
            if (curr_task->dots_printed < curr_task->dots_no) //print dots
            {
                u8 dot_text[] = {0xAF, 0xFF};
                put_text_on_rbox(0, 1, dot_text, curr_task->dots_printed * 8, 1, 0, 0);
                curr_task->dots_printed++;
            }
            else //all dots printed
            {
                if (curr_task->bites_no) {curr_task->tracker = 6;} //print oh a bite
                else {curr_task->tracker = 5;} //decide whether encounter something and eventually print oh a bite
                curr_task->bites_no++;
            }
        }
        return 0;
    }
}

bool fish5_encounter_odds(struct task* curr_task)
{
    fish_obj_hero_anim();
    bool encounter = 0;
    if (check_fishing_data_for_map())
    {
        u8 ability = get_ability_of_poke(&party_player[0]);
        if ((ability == ABILITY_SUCTION_CUPS || ability == ABILITY_STICKY_HOLD) && percent_chance(85))
            encounter = 1;
        else if (rng() & 1) //50 percent chance
            encounter = 1;
    }
    if (encounter)
    {
        if (!FIRERED_FISHING) {curr_task->tracker = 6;} //print oh a bite
        else {curr_task->tracker = 9;} //print poke on hook
        obj_change_img(&objects[walkrun.oamID], get_biting_fishing_frame(sp121_get_player_direction()));
    }
    else
        curr_task->tracker = 0xB; //not even a nibble
    return 1;
}

bool fish6_print_oh_a_bite(struct task*);
bool fish7_fast_A_press_or_fail(struct task*);
bool fish8_decide_if_bites_more(struct task*);
bool fish9_print_poke_on_hook(struct task*);
bool fishA_wait_for_a_pressed(struct task*);

void fish_fail(struct task* curr_task, const u8* text_ptr)
{
    fish_obj_hero_anim();
    obj_change_img(&objects[walkrun.oamID], get_fishing_direction_frame(sp121_get_player_direction()));
    rbox_fill(0, 0x11);
    box_related_one(0, 1, text_ptr, 1, 0, 2, 1, 3);
    curr_task->tracker = 0xD;
}

bool fishB_not_even_a_nibble(struct task* curr_task)
{
    fish_fail(curr_task, text_Not_even_a_nibble);
    return 1;
}

bool fishC_it_got_away(struct task* curr_task)
{
    fish_fail(curr_task, text_it_got_away);
    return 1;
}

bool fishD_obj_hero_anim(struct task*);
bool fishE_npc_object_magic(struct task*);

bool fishF_finish_fishing_failure(struct task* curr_task)
{
    if (!textbox_print_ret())
    {
        scripts_disabled = 0;
        walkrun.lock = 0;
        npc_release_all();
        rbox_delete1(0, 1);
        increment_fishing_tries_consider_tv_report(0);
        task_delete(task_find_id_by_funcptr(run_fishing_functions));
        if (CHAIN_FISHING == true)
            last_fishing_loc.tries = 0;
    }
    return 0;
}

const fish_task fish_tasks[0x10] = {fish0_lock, fish1_npc_lock, fish2_rodthrow_anim, fish3_prepare_dots, fish4_print_dots, fish5_encounter_odds, fish6_print_oh_a_bite,
fish7_fast_A_press_or_fail, fish8_decide_if_bites_more, fish9_print_poke_on_hook, fishA_wait_for_a_pressed, fishB_not_even_a_nibble,
fishC_it_got_away, fishD_obj_hero_anim, fishE_npc_object_magic, fishF_finish_fishing_failure};

void run_fishing_functions(u8 taskID)
{
    struct task* curr_task = &tasks[taskID];
    bool ret;
    do
    {
        u8 volatile track = curr_task->tracker;
        ret = fish_tasks[track](curr_task);
    } while(ret);
}

u8 get_wild_poke_gender(u16 species)
{
    u8 gender_ratio = (*basestat_table)[species].gender_ratio;
    if (gender_ratio != 0 && gender_ratio != 0xFE && gender_ratio != 0xFF && get_ability_of_poke(party_player) == ABILITY_CUTE_CHARM && __umodsi3(rng(), 3) == 0)
    {
        return poke_get_gender(party_player);
    }
    return 0; //random gender
}

void create_wild_poke_new(u16 species, u8 lvl)
{
    memset(party_opponent, 0, 600);
    u8 wanted_gender = get_wild_poke_gender(species);
    u8 wanted_nature = get_wild_poke_nature();
    s32 pid_tries = 0;
    if (CHAIN_FISHING == true && custom_flagcheck(FISHING_FLAG))
    {
        pid_tries += (1 + 2 * last_fishing_loc.tries);
        if (pid_tries > 41) {pid_tries = 41;}
    }
    if (SHINY_CHARM != 0x0 && checkitem(SHINY_CHARM, 1)) {pid_tries += 2;}
    u32 PiD;
    u32 TiD = get_full_trainerID();
    do
    {
        do
        {
            PiD = rng() | (rng() << 0x10);
        } while ((wanted_gender && wanted_gender != gender_from_pid(species, PiD)) || (wanted_nature != 30 && wanted_nature != __umodsi3(PiD, 25)));
        pid_tries--;
    } while(pid_tries >= 0 && !is_poke_shiny(TiD, PiD));
    pokemon_make_wrapper(party_opponent, species, lvl, 32, 1, PiD, 1, TiD);
}

void wild_fishing_battle(enum fish_rods fish_rod)
{
    struct wild_pokemon* wild_pok;
    if (check_fishing_for_feebas())
    {
        wild_pok = &wild_data_feebas;
    }
    else
    {
        u8 slot = fishing_rod_get_wild_slot(fish_rod);
        wild_pok = &(*wild_pokemon_data[get_wild_data_index_for_map()].fishing->poke)[slot];
    }
    last_fished_poke = wild_pok->species;
    custom_flagset(FISHING_FLAG);
    create_wild_poke_new(last_fished_poke, get_wild_pokemon_level(wild_pok));
    if (CHAIN_FISHING == true)
    {
        last_fishing_loc.x_pos = sav1->x_coords;
        last_fishing_loc.y_pos = sav1->y_coords;
        if (last_fishing_loc.tries < 99)
            last_fishing_loc.tries++;
    }
    increment_encrypted_counter(0xC);
    wild_battle_or_safari_battle();
}
