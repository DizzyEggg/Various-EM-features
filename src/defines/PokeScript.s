.text
.thumb

.macro s_nop
.byte 0x00
.endm

.macro s_nop_
.byte 0x01
.endm

.macro s_end
.byte 0x02
.endm

.macro s_return
.byte 0x03
.endm

.macro s_call pointer
.byte 0x04
.word \pointer
.endm

.macro s_jump pointer
.byte 0x05
.word \pointer
.endm

.macro s_if cond, action, sub
.ifeqs "\action", "jump"
.byte 0x06
.else
.ifeqs "\action", "call"
.byte 0x07
.else
.ifeqs "\action", "jumpstd"
.byte 0x0A
.else
.ifeqs "\action", "callstd"
.byte 0x0B
.else
.ifeqs "\action", "vjumpstd"
.byte 0xBB
.else
.ifeqs "\action", "vcallstd"
.byte 0xBC
.endif
.endif
.endif
.endif
.endif
.endif

.byte \cond
.ifeqs "\action", "jumpstd"
.byte \sub
.else
.ifeqs "\action", "callstd"
.byte \sub
.else
.ifeqs "\action", "vjumpstd"
.byte \sub
.else
.ifeqs "\action", "vcallstd"
.byte \sub
.else
.word \sub
.endif
.endif
.endif
.endif
.endm

.macro s_jumpif cond ptr
.byte 0x6
.byte \cond
.word \ptr
.endm

.macro s_jumpstd func
.byte 0x08
.byte \func
.endm

.macro s_callstd func
.byte 0x09
.byte \func
.endm

.macro s_jumpram
.byte 0x0C
.endm

.macro s_killscript
.byte 0x0D
.endm

.macro s_setbyte val
.byte 0x0E
.byte \val
.endm

.macro s_loadptr bank, pointer
.byte 0x0F
.byte \bank
.word \pointer
.endm

.macro s_setbytebank bank, val
.byte 0x10
.byte \bank
.byte \val
.endm

.macro s_writeptrbyte val, pointer
.byte 0x11
.byte \val
.word \pointer
.endm

.macro s_loadptrbyte bank, pointer
.byte 0x12
.byte \bank
.word \pointer
.endm

.macro s_writebankbyte bank, pointer
.byte 0x13
.byte \bank
.word \pointer
.endm

.macro s_copybank dest, src
.byte 0x14
.byte \dest
.byte \src
.endm

.macro s_copybyte dest, src
.byte 0x15
.word \dest
.word \src
.endm

.macro s_setvar var, value
.byte 0x16
.hword \var
.hword \value
.endm

.macro s_addvar var, value
.byte 0x17
.hword \var
.hword \value
.endm

.macro s_subvar var, value
.byte 0x18
.hword \var
.hword \value
.endm

.macro s_copyvar dest, src
.byte 0x19
.hword \dest
.hword \src
.endm

.macro s_setorcopyvar dest, srcval
.byte 0x1A
.hword \dest
.hword \srcval
.endm

.macro s_comparebanks bank_a, bank_b
.byte 0x1B
.byte \bank_a
.byte \bank_b
.endm

.macro s_comparebankbyte bank, val
.byte 0x1C
.byte \bank
.byte \val
.endm

.macro s_comparebankptrbyte bank, pointer
.byte 0x1D
.byte \bank
.word \pointer
.endm

.macro s_compareptrbytebank pointer, bank
.byte 0x1E
.word \pointer
.byte \bank
.endm

.macro s_compareptrbytebyte pointer, value
.byte 0x1F
.word \pointer
.byte \value
.endm

.macro s_compareptrbytes pointer_a, pointer_b
.byte 0x20
.word \pointer_a
.word \pointer_b
.endm

.macro s_comparevar var, value
.byte 0x21
.hword \var
.hword \value
.endm

.macro s_comparevars var_a, var_b
.byte 0x22
.hword \var_a
.hword \var_b
.endm

.macro s_callasm pointer
.byte 0x23
.word \pointer
.endm

.macro s_jumpasm pointer
.byte 0x24
.word \pointer
.endm

.macro s_special event
.byte 0x25
.hword \event
.endm

.macro s_specialval event, var
.byte 0x26
.hword \var
.hword \event
.endm

.macro s_waitstate
.byte 0x27
.endm

.macro s_pause delay
.byte 0x28
.hword \delay
.endm

.macro s_setflag flag
.byte 0x29
.hword \flag
.endm

.macro s_clearflag flag
.byte 0x2A
.hword \flag
.endm

.macro s_checkflag flag
.byte 0x2B
.hword \flag
.endm

.macro s_compareflags flag_a, flag_b
.byte 0x2C
.hword \flag_a
.hword \flag_b
.endm

.macro s_checkdailyflags
.byte 0x2D
.endm

.macro s_resetvars
.byte 0x2E
.endm

.macro s_playsound sound
.byte 0x2F
.hword \sound
.endm

.macro s_waitsfx
.byte 0x30
.endm

.macro s_fanfare sound
.byte 0x31
.hword \sound
.endm

.macro s_waitfanfare
.byte 0x32
.endm

.macro s_playsong song, continue_after_battle
.byte 0x33
.hword \song
.byte \continue_after_battle
.endm

.macro s_playsong_ song
.byte 0x34
.hword \song
.endm

.macro s_fadedefault
.byte 0x35
.endm

.macro s_fadesong song
.byte 0x36
.hword \song
.endm

.macro s_fadeout speed
.byte 0x37
.byte \speed
.endm

.macro s_fadein speed
.byte 0x38
.byte \speed
.endm

.macro s_warp banknum, mapnum, warpnum, x_pos, y_pos
.byte 0x39
.byte \banknum
.byte \mapnum
.byte \warpnum
.hword \x_pos
.hword \y_pos
.endm

.macro s_warpmuted banknum, mapnum, warpnum, x_pos, y_pos
.byte 0x3A
.byte \banknum
.byte \mapnum
.byte \warpnum
.hword \x_pos
.hword \y_pos
.endm

.macro s_warpwalk banknum, mapnum, warpnum, x_pos, y_pos
.byte 0x3B
.byte \banknum
.byte \mapnum
.byte \warpnum
.hword \x_pos
.hword \y_pos
.endm

.macro s_warphole banknum, mapnum
.byte 0x3C
.byte \banknum
.byte \mapnum
.endm

.macro s_warpteleport banknum, mapnum, warpnum, x_pos, y_pos
.byte 0x3D
.byte \banknum
.byte \mapnum
.byte \warpnum
.hword \x_pos
.hword \y_pos
.endm

.macro s_setwarpplace banknum, mapnum, warpnum, x_pos, y_pos
.byte 0x3F
.byte \banknum
.byte \mapnum
.byte \warpnum
.hword \x_pos
.hword \y_pos
.endm

.macro s_getplayerxy var_x, var_y
.byte 0x42
.hword \var_x
.hword \var_y
.endm

.macro s_countpokemon
.byte 0x43
.endm

.macro s_additem itemnum, quantity
.byte 0x44
.hword \itemnum
.hword \quantity
.endm

.macro s_removeitem itemnum, quantity
.byte 0x45
.hword \itemnum
.hword \quantity
.endm

.macro s_checkitemroom itemnum, quantity
.byte 0x46
.hword \itemnum
.hword \quantity
.endm

.macro s_checkitem itemnum, quantity
.byte 0x47
.hword \itemnum
.hword \quantity
.endm

.macro s_checkitemtype itemnum
.byte 0x48
.hword \itemnum
.endm

.macro s_addpcitem itemnum, quantity
.byte 0x49
.hword itemnum
.hword \quantity
.endm

.macro s_checkpcitem itemnum, quantity
.byte 0x4A
.hword itemnum
.hword \quantity
.endm

.macro s_adddecor decornum
.byte 0x4B
.hword \decornum
.endm

.macro s_removedecor decornum
.byte 0x4C
.hword \decornum
.endm

.macro s_checkdecorroom decornum
.byte 0x4D
.hword \decornum
.endm

.macro s_checkdecor decornum
.byte 0x4E
.hword \decornum
.endm

.macro s_move pid, movement
.byte 0x4F
.hword \pid
.word \movement
.endm

.macro s_movevar pidvar, movement
.byte 0x50
.hword \pidvar
.word \movement
.endm

.macro s_waitmove pid
.byte 0x51
.hword \pid
.endm

.macro s_waitmovevar pidvar
.byte 0x52
.hword \pidvar
.endm

.macro s_disappear pid
.byte 0x53
.hword \pid
.endm

.macro s_disappearvar pidvar
.byte 0x54
.hword \pidvar
.endm

.macro s_reappear pid
.byte 0x55
.hword \pid
.endm

.macro s_reappearvar pidvar
.byte 0x56
.hword \pidvar
.endm

.macro s_movesprite pid, x_pos, y_pos
.byte 0x57
.hword \pid
.hword \x_pos
.hword \y_pos
.endm

.macro s_faceplayer
.byte 0x5A
.endm

.macro s_spriteface pid, direction
.byte 0x5B
.hword \pid
.byte \direction
.endm

.macro s_tbattle type, tid, filler, intro, defeat, extra_a, extra_b
.byte 0x5C
.byte \type
.hword \tid
.hword \filler
.word \intro
.word \defeat
.word \extra_a
.word \extra_b
.endm

.macro s_checktflag flag
.byte 0x60
.hword \flag
.endm

.macro s_settflag flag
.byte 0x61
.hword \flag
.endm

.macro s_cleartflag flag
.byte 0x62
.hword \flag
.endm

.macro s_movespriteperm pid, x_pos, y_pos
.byte 0x63
.hword \pid
.hword \x_pos
.hword \y_pos
.endm

.macro s_moveoffscreen pid
.byte 0x64
.hword \pid
.endm

.macro s_spritebehave pid, behaviour
.byte 0x65
.hword \pid
.hword \behaviour
.endm

.macro s_showmsg
.byte 0x66
.endm

.macro s_message pointer
.byte 0x67
.word \pointer
.endm

.macro s_closeonkeypress
.byte 0x68
.endm

.macro s_lockall
.byte 0x69
.endm

.macro s_lock
.byte 0x6A
.endm

.macro s_releaseall
.byte 0x6B
.endm

.macro s_release
.byte 0x6C
.endm

.macro s_waitbutton
.byte 0x6D
.endm

.macro s_yesnobox x_pos, y_pos
.byte 0x6E
.byte \x_pos
.byte \y_pos
.endm

.macro s_multichoice x_pos, y_pos, list, disallow_b
.byte 0x6F
.byte \x_pos
.byte \y_pos
.byte \list
.byte \disallow_b
.endm

.macro s_multichoicedef x_pos, y_pos, list, default, disallow_b
.byte 0x70
.byte \x_pos
.byte \y_pos
.byte \list
.byte \default
.byte \disallow_b
.endm

.macro s_multichoicerow x_pos, y_pos, list, per_row, disallow_b
.byte 0x71
.byte \x_pos
.byte \y_pos
.byte \list
.byte \per_row
.byte \disallow_b
.endm

.macro s_showbox x_pos, y_pos, width, height
.byte 0x72
.byte \x_pos
.byte \y_pos
.byte \width
.byte \height
.endm

.macro s_hidebox x_pos, y_pos, width, height
.byte 0x73
.byte \x_pos
.byte \y_pos
.byte \width
.byte \height
.endm

.macro s_clearbox x_pos, y_pos, width, height
.byte 0x74
.byte \x_pos
.byte \y_pos
.byte \width
.byte \height
.endm

.macro s_showpokepic pokemon, x_pos, y_pos
.byte 0x75
.hword \pokemon
.byte \x_pos
.byte \y_pos
.endm

.macro s_hidepokepic
.byte 0x76
.endm

.macro s_showcontestwinner contest
.byte 0x77
.byte \contest
.endm

.macro s_braille text
.byte 0x78
.endm

.macro s_givepoke species, level, item, unk, unk_, unk__
.byte 0x79
.hword \species
.byte \level
.hword \item
.word \unk
.word \unk_
.byte \unk__
.endm

.macro s_giveegg species
.byte 0x7A
.hword \species
.endm

.macro s_setattack partyslot, attackslot, attackid
.byte 0x7B
.byte \partyslot
.byte \attackslot
.hword \attackid
.endm

.macro s_checkattack attackid
.byte 0x7C
.hword \attackid
.endm

.macro s_bufferpoke buffer, species
.byte 0x7D
.byte \buffer
.hword \species
.endm

.macro s_bufferfirstpoke buffer @ does not use nicknames
.byte 0x7E
.byte \buffer
.endm

.macro s_bufferpartypoke buffer, slot @ uses nicknames
.byte 0x7F
.byte \buffer
.hword \slot
.endm

.macro s_bufferitem buffer, itemid
.byte 0x80
.byte \buffer
.hword \itemid
.endm

.macro s_bufferdecor buffer, decorid
.byte 0x81
.byte \buffer
.hword \decorid
.endm

.macro s_bufferattack buffer, attackid
.byte 0x82
.byte \buffer
.hword \attackid
.endm

.macro s_buffernumber buffer, var
.byte 0x83
.byte \buffer
.hword \var
.endm

.macro s_buffertext buffer, pointer
.byte 0x85
.byte \buffer
.word \pointer
.endm

.macro s_pokemart products
.byte 0x86
.word \products
.endm

.macro s_pokemartdecor products
.byte 0x87
.word \products
.endm

.macro s_pokemartbp products
.byte 0x88
.word \products
.endm

.macro s_casino var
.byte 0x89
.hword \var
.endm

.macro s_choosecontestpoke
.byte 0x8B
.endm

.macro s_startcontest
.byte 0x8C
.endm

.macro s_showcontestresults
.byte 0x8D
.endm

.macro s_contestlink
.byte 0x8E
.endm

.macro s_random range @ number created will always be < range
.byte 0x8F
.hword \range
.endm

.macro s_givemoney amount, null
.byte 0x90
.dword \amount
.byte \null
.endm

.macro s_paymoney amount, null
.byte 0x91
.dword \amount
.byte \null
.endm

.macro s_checkmoney amount, null
.byte 0x92
.dword \amount
.byte \null
.endm

.macro s_showmoney x_pos, y_pos
.byte 0x93
.byte \x_pos
.byte \y_pos
.endm

.macro s_hidemoney x_pos, y_pos
.byte 0x94
.byte \x_pos
.byte \y_pos
.endm

.macro s_updatemoney x_pos, y_pos
.byte 0x95
.byte \x_pos
.byte \y_pos
.endm

.macro s_fadescreen effect
.byte 0x97
.byte \effect
.endm

.macro s_darken intensity
.byte 0x99
.byte \intensity
.endm

.macro s_lighten intensity
.byte 0x9A
.byte \intensity
.endm

.macro s_doanimation anim
.byte 0x9C
.hword \anim
.endm

.macro s_setanimation anim, slot
.byte 0x9D
.hword \anim
.hword \slot
.endm

.macro s_waitanimation anim
.byte 0x9E
.hword \anim
.endm

.macro s_sethealplace id
.byte 0x9F
.hword \id
.endm

.macro s_checkgender
.byte 0xA0
.endm

.macro s_cry species, effect
.byte 0xA1
.hword \species
.hword \effect
.endm

.macro s_setmaptile x_pos, y_pos, tilenum, attrib
.byte 0xA2
.hword \x_pos
.hword \y_pos
.hword \tilenum
.hword \attrib
.endm

.macro s_resetweather
.byte 0xA3
.endm

.macro s_setweather weather
.byte 0xA4
.hword \weather
.endm

.macro s_doweather
.byte 0xA5
.endm

.macro s_incrementnpcheight pid, bank, map, unk
.byte 0xA8
.hword \pid
.byte \bank
.byte \map
.byte \unk
.endm

.macro s_restorenpcheight pid, bank, map
.byte 0xA9
.hword \pid
.byte \bank
.byte \map
.endm

.macro s_createnpc sprite, vpid, x_pos, y_pos, behave, facing
.byte 0xAA
.byte \sprite
.byte \vpid
.hword \x_pos
.hword \y_pos
.byte \behave
.byte \facing
.endm

.macro s_vspriteface vpid, facing
.byte 0xAB
.byte \vpid
.byte \facing
.endm

.macro s_setdooropened x_pos, y_pos
.byte 0xAC
.hword \x_pos
.hword \y_pos
.endm

.macro s_setdoorclosed x_pos, y_pos
.byte 0xAD
.hword \x_pos
.hword \y_pos
.endm

.macro s_doorchange
.byte 0xAE
.endm

.macro s_setdooropened_ x_pos, y_pos
.byte 0xAF
.hword \x_pos
.hword \y_pos
.endm

.macro s_setdoorclosed_ x_pos, y_pos
.byte 0xB0
.hword \x_pos
.hword \y_pos
.endm

.macro s_checkcoins var
.byte 0xB3
.hword \var
.endm

.macro s_givecoins amount
.byte 0xB4
.hword \amount
.endm

.macro s_paycoins amount
.byte 0xB5
.hword \amount
.endm

.macro s_setwbattle species, level, item
.byte 0xB6
.hword \species
.byte \level
.hword \item
.endm

.macro s_dowbattle
.byte 0xB7
.endm

.macro s_vsetaddr pointer
.byte 0xB8
.word \pointer
.endm

.macro s_vjump pointer
.byte 0xB9
.word \pointer
.endm

.macro s_vcall pointer
.byte 0xBA
.endm

.macro s_vloadptr pointer
.byte 0xBD
.word \pointer
.endm

.macro s_vloadptr_ pointer
.byte 0xBE
.word \pointer
.endm

.macro s_vbuffertext buffer, pointer
.byte 0xBF
.byte \buffer
.word \pointer
.endm

.macro s_showcoins x_pos, y_pos
.byte 0xC0
.byte \x_pos
.byte \y_pos
.endm

.macro s_hidecoins x_pos, y_pos
.byte 0xC1
.byte \x_pos
.byte \y_pos
.endm

.macro s_updatecoins x_pos, y_pos
.byte 0xC2
.byte \x_pos
.byte \y_pos
.endm

.macro s_incrementhvar hvar
.byte 0xC3
.byte \hvar
.endm

.macro s_waitcry
.byte 0xC5
.endm

.macro s_bufferboxname buffer, boxid
.byte 0xC6
.byte \buffer
.hword \boxid
.endm

.macro s_signmsg
.byte 0xCA
.endm

.macro s_normalmsg
.byte 0xCB
.endm

.macro s_comparehvar hvar, value
.byte 0xCC
.byte \hvar
.word \value
.endm

.macro s_makeobedient slot
.byte 0xCD
.hword \slot
.endm

.macro s_setworldmapflag flag
.byte 0xD0
.hword \flag
.endm

.macro s_setcatchlocation slot, nameid
.byte 0xD2
.hword \slot
.byte \nameid
.endm

.macro s_braillecursor text
.byte 0xD3
.word \text
.endm

.macro s_bufferitemsfr buffer, item, quantity
.byte 0xD4
.byte \buffer
.hword \item
.hword \quantity
.endm

.macro s_fadescreenem type
.byte 0xDC
.byte \type
.endm

.macro s_buffertclass buffer, tclass
.byte 0xDD
.byte \buffer
.hword \tclass
.endm

.macro s_buffertname buffer, tid
.byte 0xDE
.byte \buffer
.hword \tid
.endm

.macro s_pokenavcall text
.byte 0xDF
.word \text
.endm

.macro s_buffercontesttype buffer, type
.byte 0xE1
.byte \buffer
.hword \type
.endm

.macro s_bufferitemsem buffer, itemid, quantity
.byte 0xE2
.byte \buffer
.hword \itemid
.hword \quantity
.endm

.macro msgbox msgboxText msgboxStd
s_loadptr 0 \msgboxText
s_callstd \msgboxStd
.endm
