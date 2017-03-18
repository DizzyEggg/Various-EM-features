#include "defines.h"

void add_suitable_menu_options()
{
    if (custom_flagcheck(DEX_MENU_FLAG)) //pokedex
        menu_option_add(MENU_DEX);

    if (custom_flagcheck(POKE_MENU_FLAG)) //pokemon
        menu_option_add(MENU_POKE);

    if (custom_flagcheck(BAG_MENU_FLAG)) //bag
        menu_option_add(MENU_BAG);

    if (custom_flagcheck(POKENAV_MENU_FLAG)) //pokenav
        menu_option_add(MENU_POKENAV);

    if (custom_flagcheck(TRAINERCARD_MENU_FLAG)) //trainercard
        menu_option_add(MENU_TRAINERCARD);

    if (custom_flagcheck(SAVE_MENU_FLAG)) //SAVE
        menu_option_add(MENU_SAVE);

    if (custom_flagcheck(OPTIONS_MENU_FLAG)) //OPTIONS
        menu_option_add(MENU_OPTION);

    if (custom_flagcheck(EXIT_MENU_FLAG)) //exit
        menu_option_add(MENU_EXIT);
}
