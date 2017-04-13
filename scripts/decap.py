#!/usr/bin/env python3

rom_name = "BPEE0.gba"		#Name of your rom
copy_name = "test.gba"		#Name of the created rom

PERSON_EVENTS = False		#All text said by NPCs
SIGNPOSTS = False			#All in-game signs
TRIGGER_SCRIPTS = False		#All text in scripts
LEVEL_SCRIPTS = False		#Decap all text in level scripts
ADDITIONAL_SCRIPTS = False		#Scripts that are called by the game functions and arent related to events
PC_OPTIONS = False			#Decap Box PC Options
MENU_OPTIONS = False			#Decap Menu Options
NATURES = False				#Decap Nature Names
BAG_OPTIONS = False			#Decap Bag Options
DEFAULT_NAMES = False     	#Male and Female default names
STAT_NAMES = False			#Decap Stat names
POKEBLOCK_NAMES = False		#Decap Pokeblock names
POKEMART_OPTIONS = False		#Decap Mart Options
POKEMON_TEXT = False			#'POKEMON' text
ITEM_NAMES = False			#Various item names, not item table
BERRY_DESCRIPTIONS = False	#Desciprtions of Berries in 'check tag'
POKE_MENU = False			#Things related to pokemon menu
CANCEL = False				#Various instances of word 'cancel'
OPTIONS = False				#Text Speed, Buttons, etc.
YESNO = False				#Yes, No text
SAVE_MENU = False			#Text that appears when you save the game
TRAINERCARD = False			#Various texts in the trainer card
CONTEST_DESC = False			#Move's contest descriptions
NEWGAME_OPTIONS = False		#New game, Continue, etc.
BOYGIRL = False				#Boy/Girl text
MULTICHOICES = False			#All Multichoice boxes
STD_TABLE = False			#scripting std table
TV_REPORTS = False			#Decap Tv Reports
GABBY_TV_REPORTS = False		#TV Reports brought to you by Gabby lol
DEX_DESCRIPTIONS = False		#Decap Pokemon descriptions in dex
BATTLE_STRINGS = False		#Decap all battle strings
ITEMS = False				#Decap item names and item descriptions
TRAINER_CLASSES = False		#Decap all trainer classes
BIRCH_INTRO = False			#Decap Birch Intro
MAP_NAMES = False			#Decap Map Names
TRAINER_NAMES = False		#Decap All Trainer Names
FD_OVERWORLD = False			#Special character FD in overworld
DEX_MENU = False				#Decap stuff in pokedex
BEHAVIOURAL_SCRIPTS = True	#Run through all tiles' behavioural scripts to decap messages
STANDART_SCRIPTS = True		#Run through standart scripts
PLAYERS_PC = False			#Decap players pc text
BIRCH_TROUBLES = False		#Text when you choose your starter
BATTLE_OPTIONS = False		#Decap Fight/Run/Menu/Pokemon and TYPE
POKENAV = False				#Various things in the pokenav

FLAGSINFO_FILE = False	#creates a file with flags/vars/specials info

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
	
def read_byte(rom, offset):
	rom.seek(offset)
	return int.from_bytes(rom.read(1), 'little')
	
def read_hword(rom, ptr):
	rom.seek(ptr)
	return int.from_bytes(rom.read(2), 'little')
	
def add_var(varID, script_ptr, to_write):
	if varID >= 0x4000 and varID <= 0x40FF:
		if script_ptr == 0 or (hex(script_ptr) not in vars_offsets[varID - 0x4000]):
			vars_offsets[varID - 0x4000] += to_write
			
def add_special(specialID, script_ptr):
	if specialID < 0x20F and hex(script_ptr) not in special_offsets[specialID]:
		special_offsets[specialID] += ("Script at " + hex(script_ptr) + '\n')
																		
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
		elif cmd_id == 0x2B or cmd_id == cmd_id == 0x2A or cmd_id == 0x29: #checkflag, clearflag, setflag
			flagID = read_hword(rom, script_ptr + counter)
			counter += 2
			if flagID < len(flags_offsets):
				if hex(script_ptr) not in flags_offsets[flagID]:
					flags_offsets[flagID] += ("Script at " + hex(script_ptr) + '\n')
		elif cmd_id == 0x16: #setvar
			add_var(read_hword(rom, script_ptr + counter), script_ptr, "Script at " + hex(script_ptr) + '\n')
			counter += 4
		elif cmd_id == 0x21: #compare var to value
			add_var(read_hword(rom, script_ptr + counter), script_ptr, "Script at " + hex(script_ptr) + '\n')
			counter += 4
		elif cmd_id == 0x22: #compare var to var
			add_var(read_hword(rom, script_ptr + counter), script_ptr, "Script at " + hex(script_ptr) + '\n')
			counter += 2
			add_var(read_hword(rom, script_ptr + counter), script_ptr, "Script at " + hex(script_ptr) + '\n')
			counter += 2
		elif cmd_id == 0x25: #special
			specialID = read_hword(rom, script_ptr + counter)
			counter += 2
			add_special(specialID, script_ptr)
		elif cmd_id == 0x26: #special 2
			varID = read_hword(rom, script_ptr + counter)
			add_var(varID, script_ptr, "Script at " + hex(script_ptr) + '\n')
			counter += 2
			specialID = read_hword(rom, script_ptr + counter)
			counter += 2
			add_special(specialID, script_ptr)
		elif cmd_id == 0x19 or cmd_id == 0x1A: #copyvar, setvar2
			add_var(read_hword(rom, script_ptr + counter), script_ptr, "Script at " + hex(script_ptr) + '\n')
			counter += 2
			add_var(read_hword(rom, script_ptr + counter), script_ptr, "Script at " + hex(script_ptr) + '\n')
			counter += 2
		elif cmd_id == 2 or cmd_id == 3 or cmd_id == 8 or cmd_id == 0xC or cmd_id == 0xD: #end/return/gotostd/callstd/jumpram/killscript
			break
		elif cmd_id == 9: #callstd value check
			arg = read_byte(rom, script_ptr + counter)
			counter += 1
			if arg >= 10:
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
					
maps_in_banks = [56, 4, 4, 5, 6, 7, 8, 6, 6, 13, 7, 16, 9, 22, 12, 14, 14, 1, 1, 1, 2, 0, 0, 0, 107, 60, 88, 1, 0, 12, 0, 0, 2, 0]
					
def decap_sings_npcs(rom):
	#Loop through all map headers
	offset = 0x485D60
	map_counter = 0x0
	while offset <= 0x486574: #0x486574
		header_ptr = read_ptr(rom, offset)
		maps_sum = 0x0
		map_bank = 0xFF
		map_no = 0xFF
		for i in range(0, len(maps_in_banks)):
			maps_sum += maps_in_banks[i] + 1
			if map_counter < maps_sum:
				map_bank = i
				map_no = map_counter
				for j in range(0, map_bank):
					map_no -= (maps_in_banks[j] + 1)
				break
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
							add_var(varID, 0, "Level Script Map Bank = " + str(map_bank) + " Map No = " + str(map_no) + '\n')
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
							sign_type = read_byte(rom, sings_ptr + (i * 0xC) + 0x5)
							if sign_type <= 4: #0-4 script msg
								script_ptr = read_ptr(rom, sings_ptr + (i * 0xC) + 0x8)
								decap_msg_in_script(rom, script_ptr)
							elif sign_type <= 7: #5-7 hidden item
								flagID = 0x1F4 + (read_hword(rom, sings_ptr + (i * 0xC) + 0xA))
								flags_offsets[flagID] += ("Hidden Item " + hex(read_hword(rom, sings_ptr + (i * 0xC) + 0x8)) + " Map Bank =  " + str(map_bank) + " Map No = " + str(map_no) + '\n')
				if PERSON_EVENTS == True:
					rom.seek(events_ptr)
					npc_no = int.from_bytes(rom.read(1), 'little')
					npc_ptr = read_ptr(rom, events_ptr + 4)
					if npc_ptr != 0:
						for i in range(0, npc_no):
							script_ptr = read_ptr(rom, npc_ptr + (i * 0x18) + 0x10)
							flagID = read_hword(rom, npc_ptr + (i * 0x18) + 0x14)
							spriteID = read_byte(rom, npc_ptr + (i * 0x18) + 0x1)
							if flagID < len(flags_offsets) and flagID != 0:
								if spriteID == 59:
									flags_offsets[flagID] += ("Pokeball Map Bank =  " + str(map_bank) + " Map No = " + str(map_no) + " Local ID = " + str(read_byte(rom, npc_ptr + (i * 0x18) + 0x0)) + '\n')
								else:
									flags_offsets[flagID] += ("Event flag Map Bank =  " + str(map_bank) + " Map No = " + str(map_no) + " Local ID = " + str(read_byte(rom, npc_ptr + (i * 0x18) + 0x0)) + '\n')
							decap_msg_in_script(rom, script_ptr)
				if TRIGGER_SCRIPTS == True:
					rom.seek(events_ptr + 2)
					scripts_no = int.from_bytes(rom.read(1), 'little')
					trigger_scripts = read_ptr(rom, events_ptr + 0xC)
					if trigger_scripts != 0:
						for i in range(0, scripts_no):
							add_var(read_hword(rom, trigger_scripts + (i * 0x10) + 0x6), 0, "Trigger Script Map Bank = " + str(map_bank) + " Map No = " + str(map_no) + '\n')
							script_ptr = read_ptr(rom, trigger_scripts + (i * 0x10) + 0xC)
							decap_msg_in_script(rom, script_ptr)
								
		offset += 4
		map_counter += 1

def run_scripts_in_list(rom, script_list):
	for i in range(0, len(script_list)):
		decap_msg_in_script(rom, script_list[i])
		
def run_scripts_in_table(rom, table, length):
	for i in range(0, length):
		decap_msg_in_script(rom, read_ptr(rom, table + (i * 4)))
		
flags_offsets = [0] * 0x960
vars_offsets = [0] * 0x100
special_offsets = [0] * 0x20F
for i in range (0, len(flags_offsets)):
	flags_offsets[i] = "\n\nFlag " + hex(i) +':\n'
for i in range (0, len(vars_offsets)):
	vars_offsets[i] = "\n\nVar " + hex(i + 0x4000) +':\n'
for i in range(0, len(special_offsets)):
	special_offsets[i] = "\nSpecial " + hex(i) + ':\n'
		
shutil.copyfile(rom_name, copy_name) #copy rom
with open(copy_name, 'rb+') as rom:
	if MULTICHOICES == True:
		multi_ptr = 0x58B760
		for i in range(0, 114):
			ptr = read_ptr(rom, multi_ptr + (i * 8) + 0)
			rom.seek(multi_ptr + (i * 8) + 4)
			no_options = int.from_bytes(rom.read(1), 'little')
			decap_all_in_struct(rom, ptr, no_options)
	if STANDART_SCRIPTS == True:
		run_scripts_in_table(rom, 0x1DC2A0, 11)
	if ADDITIONAL_SCRIPTS == True:
		additional_scripts = [0x2715DE, 0x271857, 0x271862, 0x2A4B4C, 0x277513, 0x2774EF, 0x277509, 0x252BE8, 0x2736BC, 0x291FC0, 0x273D1F, 0x238EAF, 0x1DF7BA, 0x1F49EC, 0x1FA4D6, 0x21307B, 0x224175,  0x23C050, 0x2A8350, 0x252C88, 0x271354, 0x271354, 0x2A4B2A, 0x290705, 0x267EDB, 0x275BB7, 0x23B4E8, 0x23B5E9, 0x275A86, 0x275ADF, 0x275B38, 0x2A4B8A, 0x2A4B6F, 0x2A4B9B, 0x274482, 0x2744C0, 0x242CFC, 0x23B4E8, 0x275D1F, 0x275D0C, 0x275D2E, 0x275D0C, 0x252C88, 0x2907F0, 0x1F958F, 0x1F863F, 0x290CAE, 0x2908FD, 0x2926F8, 0x271362, 0x2713D1, 0x2713F8, 0x27138A,  0x2713C2]
		run_scripts_in_list(rom, additional_scripts)
	if BEHAVIOURAL_SCRIPTS == True:
		script_list = [0x27EE0B, 0x271D92, 0x1E615D, 0x2393F9, 0x2A4BAC, 0x26A22A, 0x27208F, 0x292DE5, 0x2725CE, 0x2725D7, 0x2725E9, 0x2725F2, 0x2725FB, 0x272604, 0x277B8A, 0x277365, 0x27381B, 0x2C8393, 0x23B4BB, 0x23B589, 0x23B684, 0x23B68C, 0x1F860D, 0x1F9553]
		#TV, PC, closed door, closed door 2, pokeblock feeder, trick house door, hoenn map, shoes instruction, books, books, vase, thrascan, shelves, blueprint, wireless results, cable results, questionare, ?, decorations PC, secret base PC, ?, toy pc, rival PC, rivalPC2, 
		run_scripts_in_list(rom, script_list)
	if SIGNPOSTS == True or PERSON_EVENTS == True or TRIGGER_SCRIPTS == True or LEVEL_SCRIPTS == True:
		various_flags = {0x896 : "National Dex flag\n", 0x8AB : "Lanette met flag\n", 0x862 : "has Pokenav flag\n", 0x861 : "has Pokedex flag\n", 0x860 : "has Pokemon flag\n", 0x8C0 : "has Running Shoes flag\n", 0x8AC : "Mystery Event flag\n",  0x8DB : "Mystery Gift flag\n", 0x53 : "Legends in Sotopolis flag\n", 0x8AE : "Black Flute on\n", 0x8AD : "White flute on\n", 0x88B : "Cant dismount bike flag\n", 0x88 : "first Wally call flag\n", 0x8A : "Scott call after fortree gym leader defeat\n",  0x72 : "Scott call on S.S Tidal\n",  0x80 : "Roxanne Call flag\n", 0x75 : "Rival Rayquaza call flag\n",  0x88C : "Safari Zone flag\n", 0x888 : "Flash Flag\n", 0x895 : "Clock Time Set\n", 0x88d : "is on Slateport ferry flag\n", 0x8C2 : "Related to RTC\n", 0x8d7 : "Used in PC boxes operations\n",  0x891 : "Lilycove City Hotel TV related\n", 0x894 : "Set after Record Mixing\n", 0x10c : "Used in Secret Bases\n", 0x922 : "Used in Secret Bases\n", 0x865 : "player profile phrase entered\n", 0x893 : "trendy phrase entered\n"}
		various_vars = {0x402e: "set to 0 at new game, appears unused\n", 0x4036 : "Used in PC boxes operations\n", 0x402A : "Happiness step counter\n", 0x402B : "Overworld poison timer\n",  0x402C : "Related to RTC\n",  0x4040 : "Related to RTC\n", 0x400E : "Reset upon battle\n", 0x4021 : "Repel steps counter\n", 0x4054 : "Active secret base ID\n", 0x4026 : "Secret base map name ID\n", 0x4048 : "Soot Sack counter\n", 0x404B : "Daily Lottery var1\n", 0x404C : "Daily Lottery Var2\n", 0x4024 : "Daily Mirage Island var1\n", 0x4025 : "Daily Mirage Island var2\n", 0x4030 : "Daily Battle Frontier Battle Points 'spotting'\n", 0x4031 : "Daily get to frontier facility var\n", 0x4049 : "Daily Birch position var\n",  0x402F : "Daily the hottest going place var\n", 0x4047 : "highest seedot value\n", 0x404F : "highest lotad value\n", 0x4028 : "best bike time var1\n", 0x4029 : "best bike time var2\n", 0x4027 : "best bike collisions\n", 0x40C2 : "Return/Frustration giver var\n", 0x4038 : "Weather patterns related\n", 0x403A : "Faraway Island step counter\n", 0x4046 : "National Dex Var\n", 0x4023 : "Chosen starter\n", 0x40F2 : "Wally first call step counter\n", 0x40F3 : "Fortree Scott Call step counter\n", 0x40F5 : "SS Tidal Scott Call step counter\n",  0x40F4 : "Roxanne Call step counter\n",  0x40F6 : "Rival Rayquaza call step counter\n", 0x404A : "Slatepot ferry step counter\n",  0x40B4 : "Slateport ferry course ID\n"}
		for key, value in various_flags.items():
			flags_offsets[key] += value
			
		for key, value in various_vars.items():
			vars_offsets[key - 0x4000] += value
			
		for i in range(0x500, 0x500 + 854):
			flags_offsets[i] += ("trainer " + hex(i - 0x500) + " flag")
		for i in range(0x867, 0x867 + 8):
			flags_offsets[i] += ("badge " + hex(i - 0x867 + 1) + " flag\n")
		for i in range(0x15C, 0x15C + 77):
			flags_offsets[i] += ("pokenav trainer " + hex(i - 0x15C + 1) + " flag")
		for i in range(0x1E4, 0x1F4):
			flags_offsets[i] += ("Mystery Gift related " + str(i - 0x1E4 + 1) + " flag")
		for i in range(0x920, len(flags_offsets)):
			flags_offsets[i] += ("Daily flag " + str(i - 0x920 + 1) + "\n")
			
		for i in range(0x40DD, 0x40E5):
			vars_offsets[i - 0x4000] += ("Mystery Gift related var" + str(i - 0x40DD + 1) + '\n')
		for i in [0x40EC, 0x40ED, 0x40EE, 0x40EF, 0x40F0]:
			vars_offsets[i - 0x4000] += ("Secret base related\n")
		for i in [0x40E6, 0x40EB, 0x40E7, 0x40E8, 0x40E9, 0x40EA, 0x40F1]:
			vars_offsets[i - 0x4000] += ("TV related\n")
			
		decap_sings_npcs(rom)
		if FLAGSINFO_FILE == True:
			flags = open('flags_info.txt',  'w')
			flags.truncate()
			for i in range(0, len(flags_offsets)):
				flags.write(flags_offsets[i])
			for i in range(0, len(vars_offsets)):
				flags.write(vars_offsets[i])
			for i in range(0, len(special_offsets)):
				flags.write(special_offsets[i])
			flags.close()
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
	if DEX_MENU == True:
		decap_all_in_table(rom, 0x56EE0C, 21) #one giant table that's actually a few tables put one below another omitting ABC, etc.
		decap_all_in_table(rom, 0x56EEB8, 60)
		dex_list = [0x5E8840, 0x5E887C, 0x5E88A6, 0x5E881F, 0x5E8806, 0x5E88C8, 0x5E87D6, 0x5E87EF, 0x5E87A5, 0x5E871B, 0x5E8723, 0x5E8785]
		decap_in_list(rom, dex_list)
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
