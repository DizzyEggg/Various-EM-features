#include "defines.h"

const u8 repel_text[] = {W_, o_, u_, l_, d_, Space, y_, o_, u_, Space, l_, i_, k_, e_, Space, t_, o_, Space, u_, s_, e_, JumpLine, a_, n_, o_, t_, h_, e_, r_, Space, o_, n_, e_, QuestionMark, 0xFF};
const u8 player_used_item_text[] = {0xFD, 1, Space, u_, s_, e_, d_, Space, t_, h_, e_, JumpLine, 0xFD, 2, Exclam, 0xFF};

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

u8 add_to_multichoice_option(struct multichoice_option Options[], u16 itemID, u8 optionID)
{
    if (checkitem(itemID, 1))
    {
        u16* var_ptr = &var_8004;
        var_ptr[optionID] = itemID;
        Options[optionID].text_ptr = get_item_desc(itemID);
        Options[optionID].no_of_options = 0;
        optionID++;
    }
    return optionID;
}

void multichoice_repel_box(void)
{
    struct multichoice_option RepelTexts[3];
    register u8 no_of_options = 0;
    no_of_options = add_to_multichoice_option(RepelTexts, ITEM_MAXREPEL, no_of_options);
    no_of_options = add_to_multichoice_option(RepelTexts, ITEM_SUPERREPEL, no_of_options);
    no_of_options = add_to_multichoice_option(RepelTexts, ITEM_REPEL, no_of_options);
    if (no_of_options > 1)
    {
        multichoice_rbox_create(no_of_options, REPEL_X, REPEL_Y, RepelTexts);
    }
    var_800D_lastresult = no_of_options;
}

void s_repel_getitemquality(void)
{
    u16* var_ptr = &var_8004;
    u16 itemID = var_ptr[var_800D_lastresult];
    var_8004 = get_item_quality(itemID);
}
