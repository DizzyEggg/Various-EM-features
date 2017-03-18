#!/usr/bin/env python3

rom_name = "BPEE0.gba"		#Name of your rom
copy_name = "test.gba"		#Name of the created rom

PERSON_EVENTS = True		#All text said by NPCs
SIGNPOSTS = True			#All in-game signs
TRIGGER_SCRIPTS = True		#All text in scripts
LEVEL_SCRIPTS = True		#Decap all text in level scripts
PC_OPTIONS = True			#Decap Box PC Options
MENU_OPTIONS = True			#Decap Menu Options
NATURES = True				#Decap Nature Names
BAG_OPTIONS = True			#Decap Bag Options
DEFAULT_NAMES = True     	#Male and Female default names
STAT_NAMES = True			#Decap Stat names
POKEBLOCK_NAMES = True		#Decap Pokeblock names
POKEMART_OPTIONS = True		#Decap Mart Options
POKEMON_TEXT = True			#'POKEMON' text
ITEM_NAMES = True			#Various item names, not item table
BERRY_DESCRIPTIONS = True	#Desciprtions of Berries in 'check tag'
POKE_MENU = True			#Things related to pokemon menu
CANCEL = True				#Various instances of word 'cancel'
OPTIONS = True				#Text Speed, Buttons, etc.
YESNO = True				#Yes, No text
SAVE_MENU = True			#Text that appears when you save the game
TRAINERCARD = True			#Various texts in the trainer card
CONTEST_DESC = True			#Move's contest descriptions
NEWGAME_OPTIONS = True		#New game, Continue, etc.
BOYGIRL = True				#Boy/Girl text
MULTICHOICES = True			#All Multichoice boxes
STD_TABLE = True			#scripting std table
TV_REPORTS = True			#Decap Tv Reports
GABBY_TV_REPORTS = True		#TV Reports brought to you by Gabby lol
DEX_DESCRIPTIONS = True		#Decap Pokemon descriptions in dex
BATTLE_STRINGS = True		#Decap all battle strings
ITEMS = True				#Decap item names and item descriptions
TRAINER_CLASSES = True		#Decap all trainer classes
BIRCH_INTRO = True			#Decap Birch Intro
MAP_NAMES = True			#Decap Map Names
TRAINER_NAMES = True		#Decap All Trainer Names
FD_OVERWORLD = True			#Special character FD in overworld
DEX_MENU = True				#Decap stuff in pokedex
BEHAVIOURAL_SCRIPTS = True	#Run through all tiles' behavioural scripts to decap messages
STANDART_SCRIPTS = True		#Run through standart scripts
PLAYERS_PC = True			#Decap players pc text
BIRCH_TROUBLES = True		#Text when you choose your starter
BATTLE_OPTIONS = True		#Decap Fight/Run/Menu/Pokemon and TYPE
POKENAV = True				#Various things in the pokenav

import shutil
import sys

def read_ptr(rom, offset):
	rom.seek(offset)
	ptr = int.from_bytes(rom.read(4), 'little')
	if ptr > 0x08000000 and ptr < 0x09000000:
		return ptr - 0x08000000
	else:
		return 0
		
def decap_letter(rom, offset, letter):
	if letter >= 0xBB and letter <= 0xD4: #between A-Z
		rom.seek(offset)
		rom.write((letter + 0x1A).to_bytes(1, 'little'))
		return True
	return False
	
def can_decap_this_word(rom, ptr):
	letters = [0, 0, 0, 0]
	for i in range(0, 3):
		rom.seek(ptr + i)
		letters[i] = int.from_bytes(rom.read(1), 'little')
	rom.seek(ptr - 1)
	letter_before = int.from_bytes(rom.read(1), 'little')
	#don't decap PC
	if letters[0] == 0xCA and letters[1] == 0xBD:
		return 0
	#don't decap TV
	elif letters[0] == 0xCE and letters[1] == 0xD0:
		return 0
	#don't decap I
	elif letters[0] == 0xC3 and letters[1] == 0x0:
		#provided the letter before was not some capital letter like in CHERRI
		if not(letter_before >= 0xBB and letter_before <= 0xEE):
			return 0
	#don't decap TM
	elif letters[0] == 0xCE and letters[1] == 0xC7:
		#provided the letter before was not a character
		if not(letter_before >= 0xBB and letter_before <= 0xEE):
			return 0
	#don't decap HM
	elif letters[0] == 0xC2 and letters[1] == 0xC7:
		return 0
	#don't decap KO
	elif letters[0] == 0xC5 and letters[1] == 0xC9:
		#provided the letter before was not a character
		if not(letter_before >= 0xBB and letter_before <= 0xEE):
			return 0
	#don't decap HP
	elif letters[0] == 0xC2 and letters[1] == 0xCA:
		return 0
	#don't decap PP
	elif letters[0] == 0xCA and letters[1] == 0xCA:
		#provided the letter before was not a character
		if not(letter_before >= 0xBB and letter_before <= 0xEE):
			return 0
	#don't decap S. in S.S.
	elif letters[0] == 0xCD and letters[1] == 0xAD and (letters[2] == 0x00 or letters[2] == 0xFF) and letter_before == 0xAD:
		return 0
	return 1
		
def decap_word(rom, offset):
	word_decapped = False
	letter_in_word = 1
	counter = 0
	while True:
		ptr = offset + counter
		rom.seek(ptr)
		counter += 1
		byte = int.from_bytes(rom.read(1), 'little')
		if byte == 0xFD: #omitting special character
			counter += 1
			continue
		if byte == 0xFC: #omitting special character and properly changing Fight/Run/Menu/Pokemon Battle Menu
			byte = int.from_bytes(rom.read(1), 'little')
			counter += 1
			if byte == 0x13:
				byte = int.from_bytes(rom.read(1), 'little')
				counter += 1
				if byte == 0x38:
					letter_in_word = 1
			continue
		if byte == 0xFF:
			break
		if byte == 0x0 or byte > 0xF0 or byte < 0x10 or byte == 0xB1: #space, ... , qutoation marks
			letter_in_word = 1
			continue
		if can_decap_this_word(rom, ptr) == 0:
			continue
		if letter_in_word > 1:
			if byte == 0xA1 and ptr == 0x5EA3F9: #for some unkown reason gamefreak decided to have '0 - number' instead of 'O - letter' here...
				byte = 0xC9
			if decap_letter(rom, ptr, byte):
				word_decapped = True
		letter_in_word += 1
	if word_decapped == True:
		print("Decapitalized at " + hex(offset))

def decap_all_in_table(rom, offset, length):
	for i in range(0, length):
		ptr = read_ptr(rom, offset + i * 4)
		if ptr != 0:
			decap_word(rom, ptr)
						
def decap_all_in_struct(rom, offset, length):
	for i in range(0, length):
		ptr = read_ptr(rom, offset + i * 8)
		if ptr != 0:
			decap_word(rom, ptr)
			
def decap_in_list(rom, list):
	for i in range(0, len(list)):
		decap_word(rom, list[i])
		
def decap_in_berry_struct(rom):
	berry_struct = 0x58A670
	struct_sizeof = 0x1C
	for i in range(0, 43):
		ptr = berry_struct + (i * struct_sizeof)
		#decap berry name
		decap_word(rom, ptr)
		#decap descriptions
		desc1 = read_ptr(rom, ptr + 0xC)
		desc2 = read_ptr(rom, ptr + 0x10)
		if desc1 != 0 and desc2 != 0:
			decap_word(rom, desc1)
			decap_word(rom, desc2)
			
checked_scripts = [0] * 5000
			
def local_script_check(checked_scripts, ptr): #avoids infinite recursion
	for i in range(0, len(checked_scripts)):
		if ptr == checked_scripts[i]:
			return 0
	for i in range(0, len(checked_scripts)):
		if checked_scripts[i] == 0x0:
			checked_scripts[i] = ptr
			return 1
	print("NOT ENOUGH SPACE FOR CHECKED SCRIPTS!")
	sys.exit(1)
			
def handle_trainerbattle_cmd(rom, script_ptr, counter):
	rom.seek(script_ptr + counter)
	type = int.from_bytes(rom.read(1), 'little')
	counter += 1
	counter += 4 #skipping past trainer ID and reserved
	txt_ptr = read_ptr(rom, script_ptr + counter)
	counter += 4
	if txt_ptr != 0:
		decap_word(rom, txt_ptr)
	if type != 3: #type 3 has only one message, others have more
		txt_ptr = read_ptr(rom, script_ptr + counter)
		counter += 4
		if txt_ptr != 0:
			decap_word(rom, txt_ptr)
		if type != 0x0 and type != 0x9 and type != 5: #those two types have only two messages
			ptr = read_ptr(rom, script_ptr + counter)
			counter += 4
			if type == 0x1 or type == 0x2: #two msgs, one script
				decap_msg_in_script(rom, ptr)
			elif type == 0x7 or type == 0x4: #double battle, three msgs
				if ptr != 0:
					decap_word(rom, ptr)
			elif type == 0x6 or type == 0x8: #three msgs, one script
				if ptr != 0:
					decap_word(rom, ptr)
				ptr = read_ptr(rom, script_ptr + counter)
				counter += 4
				decap_msg_in_script(rom, ptr)
	return counter
						
def decap_msg_in_script(rom, script_ptr):
	if script_ptr == 0x0:
		return
	counter = 0
	args_as_bytes = [0, 0, 0, 0, 4, 4, 5, 5, 1, 1, 2, 2, 0, 0, 1, 5, 2, 5, 5, 5, 2, 8, 4, 4, 4, 4, 4, 4, 2, 5, 5, 5, 8, 4, 4, 4, 4, 2, 4, 0, 2, 2, 2, 2, 4, 0, 0, 2, 0, 2, 0, 3, 2, 0, 2, 1, 1, 7, 7, 7, 2, 7, 7, 7, 7, 7, 4, 0, 4, 4, 4, 4, 2, 4, 4, 2, 2, 2, 2, 6, 8, 2, 4, 2, 4, 2, 4, 6, 4, 4, 0, 3, 13, 0, 0, 0, 2, 2, 2, 6, 2, 3, 0, 4, 0, 0, 0, 0, 0, 0, 2, 4, 5, 5, 4, 4, 4, 4, 0, 1, 4, 14, 2, 4, 2, 3, 1, 3, 3, 3, 3, 3, 3, 5, 4, 4, 4, 2, 3, 0, 0, 0, 0, 2, 5, 5, 5, 3, 2, 3, 2, 1, 2, 2, 1, 4, 2, 3, 2, 2, 0, 4, 8, 0, 2, 0, 1, 2, 5, 4, 8, 2, 4, 4, 0, 4, 4, 6, 0, 2, 2, 2, 5, 0, 4, 4, 4, 5, 5, 4, 4, 5, 2, 2, 2, 1, 7, 0, 3, 1, 0, 0, 0, 0, 3, 2, 2, 0, 2, 7, 3, 2, 0, 2, 0, 7, 0, 0, 0, 4, 1, 3, 3, 4, 7, 3, 5]
	#0xE2 last in table
	while True:
		rom.seek(script_ptr + counter)
		cmd_id = int.from_bytes(rom.read(1), 'little')
		counter += 1
		if cmd_id >= len(args_as_bytes):
			print("Last in Table: " + hex(len(args_as_bytes) - 1))
			print("UNSUPPORTED COMMAND! " + hex(cmd_id) + " Offset: " + hex(script_ptr) + " Pos: " + hex(script_ptr + counter))
			sys.exit(1)
		elif cmd_id == 2 or cmd_id == 3 or cmd_id == 8 or cmd_id == 9 or cmd_id == 0xC or cmd_id == 0xD: #end/return/gotostd/callstd/jumpram/killscript
			break
		elif cmd_id == 5: #goto
			goto_script = read_ptr(rom, script_ptr + counter)
			if local_script_check(checked_scripts, goto_script):
				decap_msg_in_script(rom, goto_script)
			break
		elif cmd_id == 4: #call
			call_script = read_ptr(rom, script_ptr + counter)
			if local_script_check(checked_scripts, call_script):
				decap_msg_in_script(rom, call_script)
			counter += 4
		elif cmd_id == 0xF: #load msg pointer
			counter += 1
			msg_ptr = read_ptr(rom, script_ptr + counter)
			counter += 4
			rom.seek(script_ptr + counter)
			cmd_id = int.from_bytes(rom.read(1), 'little')
			counter += 1
			if cmd_id == 9: #call msg function
				counter += 1
				if msg_ptr != 0:
					decap_word(rom, msg_ptr)
			elif cmd_id == 8: #goto msg function
				counter += 1
				if msg_ptr != 0:
					decap_word(rom, msg_ptr)
				break
		elif cmd_id == 0x67 or cmd_id == 0x9B or cmd_id == 0xDF: #preparemsg, preparemsg2, pokenavcall
			msg_ptr = read_ptr(rom, script_ptr + counter)
			counter += 4
			if msg_ptr != 0:
				decap_word(rom, msg_ptr)
		elif cmd_id == 0x85: #bufferstring
			counter += 1
			msg_ptr = read_ptr(rom, script_ptr + counter)
			counter += 4
			if msg_ptr != 0:
				decap_word(rom, msg_ptr)
		elif cmd_id == 6 or cmd_id == 7: #if call, if goto
			counter += 1
			if_script = read_ptr(rom, script_ptr + counter)
			if local_script_check(checked_scripts, if_script):
				decap_msg_in_script(rom, if_script)
			counter += 4
		elif cmd_id == 0x5C: #damn trainerbattle having different number of arguments...
			counter = handle_trainerbattle_cmd(rom, script_ptr, counter)
		else:
			counter += args_as_bytes[cmd_id]
						
def decap_sings_npcs(rom):
	#Loop through all map headers
	offset = 0x485D60
	while offset < 0x486574: #0x486574
		header_ptr = read_ptr(rom, offset)
		if header_ptr != 0:
			level_scripts = read_ptr(rom, header_ptr + 8)
			if LEVEL_SCRIPTS == True and level_scripts != 0:
				counter = 0x0
				while True:
					rom.seek(level_scripts + counter)
					type = int.from_bytes(rom.read(1), 'little')
					counter += 1
					if type == 0x0:
						break
					script_ptr = read_ptr(rom, level_scripts + counter)
					counter += 4
					if type == 0x2 or type == 0x4: #different structure
						#u16 VarID, u16 value, u32 ptr, if varID is 0, looping finished
						tag2_counter = 0x0
						while True:
							rom.seek(script_ptr + tag2_counter)
							varID = int.from_bytes(rom.read(2), 'little')
							tag2_counter += 2
							if varID == 0x0:
								break
							tag2_counter += 2
							tag2_script_ptr = read_ptr(rom, script_ptr + tag2_counter)
							decap_msg_in_script(rom, tag2_script_ptr)
							tag2_counter += 4
					else:
						decap_msg_in_script(rom, script_ptr)
			events_ptr = read_ptr(rom, header_ptr + 4)
			if events_ptr != 0:
				if SIGNPOSTS == True:
					rom.seek(events_ptr + 3)
					signs_no = int.from_bytes(rom.read(1), 'little')
					sings_ptr = read_ptr(rom, events_ptr + 0x10)
					if sings_ptr != 0:
						for i in range(0, signs_no):
							script_ptr = read_ptr(rom, sings_ptr + (i * 0xC) + 0x8)
							decap_msg_in_script(rom, script_ptr)
				if PERSON_EVENTS == True:
					rom.seek(events_ptr)
					npc_no = int.from_bytes(rom.read(1), 'little')
					npc_ptr = read_ptr(rom, events_ptr + 4)
					if npc_ptr != 0:
						for i in range(0, npc_no):
							script_ptr = read_ptr(rom, npc_ptr + (i * 0x18) + 0x10)
							decap_msg_in_script(rom, script_ptr)
				if TRIGGER_SCRIPTS == True:
					rom.seek(events_ptr + 2)
					scripts_no = int.from_bytes(rom.read(1), 'little')
					trigger_scripts = read_ptr(rom, events_ptr + 0xC)
					if trigger_scripts != 0:
						for i in range(0, scripts_no):
							script_ptr = read_ptr(rom, trigger_scripts + (i * 0x10) + 0xC)
							decap_msg_in_script(rom, script_ptr)
								
		offset += 4

def run_scripts_in_list(rom, script_list):
	for i in range(0, len(script_list)):
		decap_msg_in_script(rom, script_list[i])
		
def run_scripts_in_table(rom, table, length):
	for i in range(0, length):
		decap_msg_in_script(rom, read_ptr(rom, table + (i * 4)))
		
shutil.copyfile(rom_name, copy_name) #copy rom
with open(copy_name, 'rb+') as rom:
	if MULTICHOICES == True:
		multi_ptr = 0x58B760
		for i in range(0, 114):
			ptr = read_ptr(rom, multi_ptr + (i * 8) + 0)
			rom.seek(multi_ptr + (i * 8) + 4)
			no_options = int.from_bytes(rom.read(1), 'little')
			decap_all_in_struct(rom, ptr, no_options)
	if SIGNPOSTS == True or PERSON_EVENTS == True or TRIGGER_SCRIPTS == True or LEVEL_SCRIPTS == True:
		decap_sings_npcs(rom)
	if PC_OPTIONS == True:
		decap_all_in_table(rom, 0x5716C0, 10) #'withdraw, deposit'
		decap_all_in_table(rom, 0x58BB70, 4)#'someone's pc, log off'
		decap_word(rom, 0x5EB18B) #'hall of fame'
		decap_word(rom, 0x5EB198) #'log off'
	if MENU_OPTIONS == True:
		decap_all_in_struct(rom, 0x510540, 13)
	if NATURES == True:
		decap_all_in_table(rom, 0x61CB50, 25)
	if BAG_OPTIONS == True:
		decap_all_in_struct(rom, 0x613FB4, 15) #bag item functions 'give, use, etc'
		decap_all_in_table(rom, 0x5E91FC, 5) #bag pockets
		decap_word(rom, 0x5E8DB4) #'Close Bag'
	if DEFAULT_NAMES == True:
		decap_all_in_table(rom, 0x2FF128, 40)
	if STAT_NAMES == True:
		decap_all_in_table(rom, 0x5CBE00, 8)
	if POKEBLOCK_NAMES == True:
		decap_all_in_table(rom, 0x5B262C, 15) #pokeblock names
		decap_all_in_struct(rom, 0x5B2668, 6) #pokeblock functions
		decap_word(rom, 0x5E9344) #'Stow Case'
	if POKEMART_OPTIONS == True:
		decap_all_in_struct(rom, 0x589A10, 3) #'buy, sell, quit'
	if POKEMON_TEXT == True:
		poke_texts = [0x5E8268, 0x5EB264, 0x5ED415, 0x5E86B8, 0x5E8D26, 0x5EA5DB, 0x5EA39F, 0x5EA3AB, 0x5EA3B4, 0x5EA394, 0x5EA38D, 0x5EA384, 0x5EA37E, 0x5EA398, 0x5EA35B, 0x5EA353, 0x5EA34B, 0x5EA343, 0x5EA361, 0x5EA33C]
		decap_in_list(rom, poke_texts)
	if ITEM_NAMES == True:
		item_texts = [0x5EFCD4, 0x5EFCDF, 0x5EFCE5, 0x5CC0A0, 0x5CC0AD, 0x5E9026, 0x5E9058, 0x5E8F31, 0x5E8F6E, 0x5EE903]
		#'pokeballs', 'berry', 'berries', 'enigma berry', 'berry', 'Your COINS', 'Teach tm to a POKEMON?', 'time and place for everything', 'cant dismount bike', 'poke on the hook'
		decap_in_list(rom, item_texts)
	if BERRY_DESCRIPTIONS == True:
		decap_in_berry_struct(rom)
		decap_word(rom, 0x5E9225) #Size
		decap_word(rom, 0x5E922C) #Firm
		decap_word(rom, 0x5E926B) #Berry Tag
	if POKE_MENU == True:
		decap_all_in_table(rom, 0x615AF4, 27) #bottom text while choosing poke
		decap_all_in_table(rom, 0x615B60, 13) #text while selecting poke 'able, not able, etc.'
		text_display = [0x5EA3C8, 0x5EA3CF, 0x5EA3DC, 0x5EA3EB, 0x5EA3F8, 0x5EA406, 0x5EA378]
		#'Switch', 'Poke Info', 'Poke Skills', 'Battle Moves' , 'Contest Moves' , 'INFO', 'TYPE/'
		decap_in_list(rom, text_display)
		decap_all_in_struct(rom, 0x615C08, 33) #menu options 'give, take, fly, etc.'
	if CANCEL == True:
		cancel_list = [0x5E8CF0, 0x5E8CF7, 0x5ED92A]
		decap_in_list(rom, cancel_list)
	if OPTIONS == True:
		decap_all_in_table(rom, 0x55C664, 7)
		options_decap = [0x5EE589, 0x5EE5D4, 0x5EE5DF, 0x5EE5E9, 0x5EE5F4, 0x5EE5FD, 0x5EE607, 0x5EE613, 0x5EE61D, 0x5EE628, 0x5EE635, 0x5EE647]
		#text 'OPTION', 'SLOW', 'MID' , 'FAST', '0N', '0FF', 'SHIFT, 'SET', 'MONO', 'STEREO', 'TYPE', 'NORMAL'
		decap_in_list(rom, options_decap)
	if YESNO == True:
		yesno_decap = [0x5EE491, 0x5CCABB] #one for overworld and one in battle
		decap_in_list(rom, yesno_decap)
	if SAVE_MENU == True:
		save_decap = [0x5EED26, 0x5EED2D, 0x5EED34, 0x5EED3C, 0x252CA7, 0x2C8810]
		#text 'PLAYER', 'BADGES', 'POKEDEX', 'TIME', saving text in battle pyramind, 'SAVING DON'T TURN OFF THE POWER'
		decap_in_list(rom, save_decap)
	if CONTEST_DESC == True:
		decap_all_in_table(rom, 0x587C50, 62)
	if NEWGAME_OPTIONS == True:
		newgame_decap = [0x5E827C, 0x5E8285, 0x5E828E, 0x5E8295, 0x5E82A2, 0x5E82AF, 0x5EDCCF, 0x5EDCC3, 0x5EDCD7, 0x5EED26, 0x5EED2D, 0x5EED34, 0x5EED3C, 0x5EEA00, 0x5EDCCA, 0x5ECF99]
		decap_in_list(rom, newgame_decap)
	if BOYGIRL == True:
		boygirl_decap = [0x5E858F, 0x5E8593]
		decap_in_list(rom, boygirl_decap)
	if TRAINERCARD == True:
		decap_all_in_table(rom, 0x56FB5C, 3)
		tcard_list = [0x5ECFB8, 0x5ED010, 0x5ED036, 0x5ED0B6, 0x5ED0D3, 0x5ED09F, 0x5ECF7E, 0x5ECF71, 0x5ECF86, 0x5ECF99, 0x5ECFA6, 0x5ED932, 0x5ED94D]
		#'HALL_DEBUT', 'POKE_TRADES', 'BERRY_CRUSH', 'BATTLE POINTS WON', 'BATTLE_TOWER', 'WON_CONTESTS_W/FRIENDS', 'MONEY', 'NAME', 'POKEDEX', 'TIME', 'HERO'S_TRAINER_CARD', 'Check BATTLE_FRONTIER map', 'CHECK TRAINERCARD'
		decap_in_list(rom, tcard_list)
	if STD_TABLE == True:
		decap_all_in_table(rom, 0x58BAF0, 30)
	if TV_REPORTS == True:
		decap_all_in_table(rom, 0x58D150, 327) #it's actually lots of tables, but they're all one below other
	if GABBY_TV_REPORTS == True:
		decap_all_in_table(rom, 0x58D66C, 9)
	if DEX_DESCRIPTIONS == True:
		dex_table = read_ptr(rom, 0x1DB48C)
		if dex_table != 0:
			for i in range(0, 387):
				#decap kind
				decap_word(rom, dex_table + (i * 0x20))
				#decap description
				dex_descr = read_ptr(rom, dex_table + (i * 0x20) + 0x10)
				if dex_descr != 0:
					decap_word(rom, dex_descr)
	if BATTLE_STRINGS == True:
		decap_all_in_table(rom, 0x5CC270, 369)
	if ITEMS == True:
		item_table = read_ptr(rom, 0x1C8)
		if item_table != 0:
			for i in range(0, 377):
				#decap item name
				decap_word(rom, item_table + (i * 0x2C))
				#decap description
				item_descr = read_ptr(rom, item_table + (i * 0x2C) + 0x14)
				if item_descr != 0:
					decap_word(rom, item_descr)
	if TRAINER_CLASSES == True:
		traierclasses = read_ptr(rom, 0x06F0AC)
		if traierclasses != 0:
			for i in range(0, 66):
				decap_word(rom, traierclasses + (i * 13))
	if BIRCH_INTRO == True:
		birch_decap = [0x2C897B, 0x2C8A1F, 0x2C8BD0, 0x5E8692, 0x2C8C2A, 0x2C8C7A]
		decap_in_list(rom, birch_decap)
	if MAP_NAMES == True:
		decap_all_in_struct(rom, 0x5A147C + 4, 212)
	if TRAINER_NAMES == True:
		trainer_table = read_ptr(rom, 0x06F0A8)
		if trainer_table != 0:
			for i in range(0, 854):
				decap_word(rom, trainer_table + (i * 0x28) + 4)
	if FD_OVERWORLD == True:
		FD_list = [0x5E821B, 0x5E8224, 0x5E8229, 0x5E8231, 0x5E8236, 0x5E823C, 0x5E8243, 0x5E8249, 0x5E8250, 0x5E8258, 0x5E8260, 0x5E8268, 0x5E8264]
		decap_in_list(rom, FD_list)
	if BEHAVIOURAL_SCRIPTS == True:
		script_list = [0x27EE0B, 0x271D92, 0x1E615D, 0x2393F9, 0x2A4BAC, 0x26A22A, 0x27208F, 0x292DE5, 0x2725CE, 0x2725D7, 0x2725E9, 0x2725F2, 0x2725FB, 0x272604, 0x277B8A, 0x277365, 0x27381B, 0x2C8393, 0x23B4BB, 0x23B589, 0x23B684, 0x23B68C, 0x1F860D, 0x1F9553]
		#TV, PC, closed door, closed door 2, pokeblock feeder, trick house door, hoenn map, shoes instruction, books, books, vase, thrascan, shelves, blueprint, wireless results, cable results, questionare, ?, decorations PC, secret base PC, ?, toy pc, rival PC, rivalPC2, 
		run_scripts_in_list(rom, script_list)
	if DEX_MENU == True:
		decap_all_in_table(rom, 0x56EE0C, 21) #one giant table that's actually a few tables put one below another omitting ABC, etc.
		decap_all_in_table(rom, 0x56EEB8, 60)
		dex_list = [0x5E8840, 0x5E887C, 0x5E88A6, 0x5E881F, 0x5E8806, 0x5E88C8, 0x5E87D6, 0x5E87EF, 0x5E87A5, 0x5E871B, 0x5E8723, 0x5E8785]
		decap_in_list(rom, dex_list)
	if STANDART_SCRIPTS == True:
		run_scripts_in_table(rom, 0x1DC2A0, 11)
	if PLAYERS_PC == True:
		decap_all_in_table(rom, 0x5DFEA4, 4)
		decap_all_in_struct(rom, 0x5DFEB4, 4)
		decap_all_in_struct(rom, 0x5DFEDC, 4)
		decap_all_in_struct(rom, 0x5DFF04, 4)
	if BIRCH_TROUBLES == True:
		birch_list = [0x5E8C53, 0x5E8C90]
		decap_in_list(rom, birch_list)
	if BATTLE_OPTIONS == True:
		battle_list = [0x5CCA3A, 0x5CCA73]
		decap_in_list(rom, battle_list)
	if POKENAV == True:
		#Match Call
		decap_all_in_struct(rom, 0x60E3B8, 15 + 14 + 14 + 14 + 14 + 64 + 14 + 14 + 14 + 14 + 14) #lots of tables of structs put one below another
		decap_all_in_struct(rom, 0x624D1C, 11)
		decap_all_in_struct(rom, 0x624D8C, 9)
		decap_all_in_struct(rom, 0x624DFC, 3)
		decap_all_in_struct(rom, 0x624E2C, 7)
		decap_all_in_struct(rom, 0x624E7C, 15)
		decap_all_in_struct(rom, 0x624F0C, 15)
		decap_all_in_struct(rom, 0x624F9C, 7)
		decap_all_in_struct(rom, 0x625000, 7)
		decap_all_in_struct(rom, 0x62508C, 4)
		decap_all_in_struct(rom, 0x625104, 4)
		decap_all_in_struct(rom, 0x625140, 5)
		decap_all_in_struct(rom, 0x62517C, 5)
		decap_all_in_struct(rom, 0x6251B8, 5)
		matchcall_list = [0x5EFAEF, 0x5EFAFA, 0x5EFB25, 0x5EFB47, 0x5EFBC9, 0x5E8270, 0x5EFB32, 0x5EFB4B, 0x2B5B95, 0x5EFB04, 0x5EFB11, 0x5EFB18, 0x5E8260, 0x5E8258, 0x5EFB3E, 0x5EFB4F, 0x5EFB5C, 0x2B2456, 0x2B250E, 0x2B25C1, 0x2B2607, 0x5EFB62, 0x5EFB6F, 0x2B2912, 0x2B2912, 0x2B29CA, 0x2B2AB6, 0x2B2B01, 0x5EFB7B, 0x5EFB87, 0x5EFB94, 0x5EFB9E, 0x5EFBA9, 0x2B34CC, 0x5EFBB5, 0x2B3561, 0x2B35E4, 0x2B368B, 0x2B3790, 0x5EFBC0, 0x5EC00F, 0x5ED453, 0x5EBE84]
		decap_in_list(rom, matchcall_list)
		decap_all_in_table(rom, 0x622028, 312)
		decap_all_in_table(rom, 0x61FBE8, 3)
		decap_all_in_table(rom, 0x625390, 4)
		decap_all_in_table(rom, 0x6253A8, 4)
		decap_all_in_table(rom, 0x6253C0, 4)
		decap_all_in_table(rom, 0x6253D8, 4)
		decap_all_in_table(rom, 0x6227E8, 3)
		#Bottom text in Pokenav
		decap_all_in_table(rom, 0x6202D4, 14)
	rom.close()
