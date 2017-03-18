.text
.thumb

.include "defines/PokeScript.s"
.include "defines/items.s"

REPEL_SCRIPT:
	msgbox 0x082A4B33 3
	s_checkitem ITEM_REPEL 0x1
	s_comparevar 0x800D 0x1
	s_jumpif 1 REPEL_ASK
	s_checkitem ITEM_SUPERREPEL 0x1
	s_comparevar 0x800D 0x1
	s_jumpif 1 REPEL_ASK
	s_checkitem ITEM_MAXREPEL 0x1
	s_comparevar 0x800D 0x0
	s_jumpif 1 SCRIPT_END
REPEL_ASK:
	msgbox repel_text 5
	s_closeonkeypress
	s_comparevar 0x800D 0x0
	s_jumpif 1 SCRIPT_END
REPEL_CALLASM:
	s_callasm multichoice_repel_box + 1
	s_comparevar 0x800D 0x1
	s_jumpif 1 REPEL_OPTION0
	s_waitstate
	s_comparevar 0x800D 0x0
	s_jumpif 1 REPEL_OPTION0
	s_comparevar 0x800D 0x1
	s_jumpif 1 REPEL_OPTION1
	s_comparevar 0x800D 0x2
	s_jumpif 1 REPEL_OPTION2
	s_end
REPEL_OPTION0:
	s_bufferitem 0x0 0x8004
	s_removeitem 0x8004 1
	s_setvar 0x800D 0x0
	s_jump REPEL_USED
REPEL_OPTION1:
	s_bufferitem 0x0 0x8005
	s_removeitem 0x8005 1
	s_jump REPEL_USED
REPEL_OPTION2:
	s_bufferitem 0x0 0x8006
	s_removeitem 0x8006 1
REPEL_USED:
	s_callasm s_repel_getitemquality + 1
	s_setorcopyvar 0x4021 0x8004
	s_playsound 0x2F
	msgbox player_used_item_text 3
SCRIPT_END:
	s_end
