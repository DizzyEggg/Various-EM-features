#!/usr/bin/env python3

#To Edit
BWRepel = 				False
#next feature
WildPokeBattleMusic = 	False
TrainerMusicVar = 0x0	#a var of your choice, if it's 0 music plays normally, other value will make that music play
#next feature
EVTrainers = 			False
#next feature
MoreMoney = 			False
#next feature
NewEvolutionMethods = 	False
EvosPerPoke =			5		#Number of evolutions per pokemon in your rom hack
EeveeTable =			False	#will create an additional table for eevee evolution, so that you dont have to expand evolutions to 8 slots
#next feature
BallsExpansion = 		False
FirstNewBallID = 		226		#item ID of your first new ball, you need 15 free consecutive item slots
FishingFlag = 	0x0 	#a flag of your choice, works as a way to make game know that you're fighting against a fished pokemon, only for lure ball
#next feature
NatureStatColor = 		False
#next feature
MoreLevels =			False
MaxLevel = 				250		#Maximal level available
#next feature
NewSpecials = False #new special commands
Tutors_Number = 32 						#number of move tutors in your game
TmsHms_Number = 58 					#number of tms + hms in your game
TutorMovesTable = 0x08616040	 	#pointer to move tutor table
TmsMovesTable = 0x0832937C 		#pointer to tms moves table
ExpandedLearnsetTable = False 	#set to true if you're using battle engine's upgrade
LearnsetsTable = 0x08F4A6A8 		#pointer to learnsets table in your game
#next feature
MenuOptionsControl = False #enables a menu option only if a flag is set, if flag is 0, the option will be always shown
FlagPokedex = 0x861 #default one game uses
FlagPokemon = 0x860 #default one game uses
FlagBag = 0x0
FlagPokenav = 0x862 #default one game uses
FlagSave = 0x0
FlagExit = 0x0
FlagTrainerCard = 0x0
FlagOptions = 0x0
#next feature
FireRedFishing = False #no biting, you either encounter on the first try or not
ChainFishing = False #XY feature, the more tries you fish on the first try, the more likely you'll encounter a shiny
FishingFlag = 0x0 	#a flag of your choice, works as a way to make game know that you're fighting against a fished pokemon, make sure it's the same as the one you set in the balls expansion(if you expand balls)
#next feature
ShinyCharm = 0x0 #set to item ID if you want to have that item in your hack
#next feature
HallofFameFix = False #set to true if you want to be able to have expanded pokemon properly displayed in the hall of fame
#next feature

insert_offset = 0xFF0000 	#Offset as to where insert data
rom_name = "BPEE0.gba"		#Name of your rom
copy_name = "test.gba"		#Name of the created rom
#To Edit ends

from glob import glob
import os
import itertools
import hashlib
import subprocess
import sys
import time
import shutil
import binascii
import textwrap

PathVar = os.environ.get('Path')
Paths = PathVar.split(';')
PATH = ""
for candidatePath in Paths:
    if "devkitARM" in candidatePath:
        PATH = candidatePath
        break
if PATH == "":
	print('DevKit does not exist in your Path variable.\nChecking default location.')
	PATH = 'C://devkitPro//devkitARM//bin'
	if os.path.isdir(PATH) == False:
		print("...\nDevkit not found.")
		sys.exit(1)
	else:
		print("Devkit found.")
		
PREFIX = '/arm-none-eabi-'
PREFIX2 = 'arm-none-eabi-'
NM = os.path.join(PATH, PREFIX2 + 'nm')
AS = (PATH + PREFIX + 'as')
CC = (PATH + PREFIX + 'gcc')
CXX = os.path.join(PATH, PREFIX + 'g++')
LD = (PATH + PREFIX + 'ld')
OBJCOPY = (PATH + PREFIX + 'objcopy')
OBJDUMP = os.path.join(PATH, PREFIX2 + 'objdump')
SRC = './src'
BUILD = './build'
ASFLAGS = ['-mthumb', '-I', SRC]
LDFLAGS = ['BPEE.ld', '-T', 'linker.ld']
CFLAGS = ['-mthumb', '-mno-thumb-interwork', '-mcpu=arm7tdmi',
		'-fno-inline', '-mlong-calls', '-march=armv4t', '-Wall', '-O2']

def run_command(cmd):
	try:
		subprocess.check_output(cmd)
	except subprocess.CalledProcessError as e:
		print(e.output.decode(), file = sys.stderr)
		sys.exit(1)
		
def make_output_file(filename):
	'''Return hash of filename to use as object filename'''
	m = hashlib.md5()
	m.update(filename.encode())
	return os.path.join(BUILD, m.hexdigest() + '.o')
	
def process_assembly(in_file):
	'''Assemble'''
	out_file = make_output_file(in_file)
	print ('Assembling %s' % in_file)
	cmd = [AS] + ASFLAGS + ['-c', in_file, '-o', out_file]
	run_command(cmd)
	return out_file
	
def process_c(in_file):
	'''Compile C'''
	out_file = make_output_file(in_file)
	print ('Compiling %s' % in_file)
	cmd = [CC] + CFLAGS + ['-c', in_file, '-o', out_file]
	run_command(cmd)
	return out_file
	
def link(objects):
	'''Link objects into one binary'''
	linked = 'build/linked.o'
	cmd = [LD] + LDFLAGS + ['-o', linked] + list(objects)
	run_command(cmd)
	return linked
	
def objcopy(binary):
	cmd = [OBJCOPY, '-O', 'binary', binary, 'build/output.bin']
	run_command(cmd)
	
def run_glob(globstr, fn):
	'''Glob recursively and run the processor function on each file in result'''
	files = glob(os.path.join(SRC, globstr), recursive = True)
	return map(fn, files)
	
def bool_to_string(s):
	return "true" if s == True else "false"
	
def edit_defines():
	defines = open(SRC + "/defines.h", 'r+')
	copy = defines.read()
	defines.seek(0x0)
	for line in defines:
		try:
			directive, name, val = line.split()
		except:
			continue
		newval = ""
		if name == "MAX_LEVEL" and MoreLevels == True:
			newval =  str(MaxLevel)
		elif name == "MoreLevels":
			newval = bool_to_string(MoreLevels)
		elif NewEvolutionMethods == True and name == "EVO_PER_POKE":
			newval = str(EvosPerPoke)
		elif name == "COLORED_STATS":
			newval = bool_to_string(NatureStatColor)
		elif BallsExpansion == True and name == "FIRST_NEW_BALL_ID":
			newval = str(FirstNewBallID)
		elif NewSpecials == True and name == "NO_OF_TUTORS":
			newval = str(Tutors_Number)
		elif NewSpecials == True and name == "NO_OF_TMS_HMS":
			newval = str(TmsHms_Number)
		elif NewSpecials == True and name == "EXPANDED_LEARNSETS":
			newval = bool_to_string(ExpandedLearnsetTable)
		elif name == "BW_REPEL":
			newval = bool_to_string(BWRepel)
		elif name == "FIRERED_FISHING":
			newval = bool_to_string(FireRedFishing)
		elif name == "CHAIN_FISHING":
			newval = bool_to_string(ChainFishing)
		elif name == "FISHING_FLAG":
			newval = hex(FishingFlag)
		elif name == "SHINY_CHARM":
			newval = hex(ShinyCharm)
		elif name == "EEVEE_TABLE":
			newval = bool_to_string(EeveeTable)
		elif name == "TRAINER_BATTLE_MUSIC_VAR":
			newval = hex(TrainerMusicVar)
		elif name == "CUSTOM_WILD_POKE_MUSIC":
			newval = hex(WildPokeBattleMusic)
		elif MenuOptionsControl == True:
			if name == "DEX_MENU_FLAG":
				newval = hex(FlagPokedex)
			elif name == "POKE_MENU_FLAG":
				newval = hex(FlagPokemon)
			elif name == "POKENAV_MENU_FLAG":
				newval = hex(FlagPokenav)
			elif name == "BAG_MENU_FLAG":
				newval = hex(FlagBag)
			elif name == "SAVE_MENU_FLAG":
				newval = hex(FlagSave)
			elif name == "EXIT_MENU_FLAG":
				newval = hex(FlagExit)
			elif name == "TRAINERCARD_MENU_FLAG":
				newval = hex(FlagTrainerCard)
			elif name == "OPTIONS_MENU_FLAG":
				newval = hex(FlagOptions)
		if newval != "":
			copy = copy.replace(line, directive + ' ' + name + '\t\t' + newval + '\n')
	defines.seek(0x0)
	defines.write(copy)
	defines.close()
	
def edit_linker():
	linker = open("linker.ld", 'r+')
	copy = linker.read()
	linker.seek(0x0)
	line_no = 1
	for line in linker:
		if (line_no == 4):
			copy = copy.replace(line, "\t\trom     : ORIGIN = (0x08000000 + " + hex(insert_offset) + "), LENGTH = 32M\n")
			break
		line_no += 1
	linker.seek(0x0)
	linker.write(copy)
	linker.close()
	
def build_script():
	#erase build folder if it exists
	if os.path.isdir(BUILD):
		shutil.rmtree(BUILD)
	#get which files to compiler and assemble
	globs = {}
	if MoreMoney == True:
		globs['MoreMoney.c'] = process_c
	if EVTrainers == True:
		globs['EVTrainers.c'] = process_c
	if WildPokeBattleMusic == True or TrainerMusicVar != 0x0:
		globs['BattleMusic.c'] = process_c
	if BWRepel == True:
		globs['Repel_Code.c'] = process_c
		globs['Repel_Script.s'] = process_assembly
	if MoreLevels == True or NatureStatColor == True:
		globs['MoreLevels.c'] = process_c
	if MoreLevels == True:
		globs['MoreLevels.s'] = process_assembly
	if NewEvolutionMethods == True:
		globs['EvolutionMethods.c'] = process_c
	if BallsExpansion == True:
		globs['BallsExpansion.c'] = process_c
		globs['BallsExpansion.s'] = process_assembly
	if BallsExpansion == True or MoreLevels == True:
		globs['OriginsStruct.s'] = process_assembly
	if NewSpecials == True:
		globs['NewSpecials.c'] = process_c
	if MenuOptionsControl == True:
		globs['MenuControl.c'] = process_c
	if FireRedFishing == True or ChainFishing == True or FishingFlag != 0 or ShinyCharm != 0:
		globs['WildPokes.c'] = process_c
	if ChainFishing == True:
		globs["ChainFishMoving.s"] = process_assembly
	if HallofFameFix == True:
		globs["HallOfFame.c"] = process_c
		globs["HallOfFame.s"] = process_assembly
	#check if at least one file is being built
	if not globs:
		print("No feature chosen.")
		sys.exit(1)
	#edit defines in defines.h
	edit_defines()
	#change offset in linker file
	edit_linker()
	# Create output directory
	try:
		os.makedirs(BUILD)
	except FileExistsError:
		pass
	
	# Gather source files and process them
	objects = itertools.starmap(run_glob, globs.items())
	
	# Link and extract raw binary
	linked = link(itertools.chain.from_iterable(objects))
	objcopy(linked)
	
def get_text_section():
	# Dump sections
	out = subprocess.check_output([OBJDUMP, '-t', 'build/linked.o'])
	lines = out.decode().split('\n')
	
	# Find text section
	text = filter(lambda x: x.strip().endswith('.text'), lines)
	section = (list(text))[0]
	
	# Get the offset
	offset = int(section.split(' ')[0], 16)
	
	return offset
		
def symbols(subtract=0):
	out = subprocess.check_output([NM, 'build/linked.o'])
	lines = out.decode().split('\n')
	
	name = ''
	
	ret = {}
	for line in lines:
			parts = line.strip().split()
			
			if (len(parts) < 3):
					continue
					
			if (parts[1].lower() not in {'t','d'}):
					continue
					
			offset = int(parts[0], 16)
			ret[parts[2]] = offset - subtract
			
	return ret
					   
def hook(rom, space, hook_at, register=0):
	# Align 2
	if hook_at & 1:
		hook_at -= 1
		
	rom.seek(hook_at)
	
	register &= 7
	
	if hook_at % 4:
		data = bytes([0x01, 0x48 | register, 0x00 | (register << 3), 0x47, 0x0, 0x0])
	else:
		data = bytes([0x00, 0x48 | register, 0x00 | (register << 3), 0x47])
		
	space += 0x08000001
	data += (space.to_bytes(4, 'little'))
	rom.write(bytes(data))

def funcwrap(rom, space, hook_at, nparams, isreturning):
	# Align 2
	if hook_at & 1:
		hook_at -= 1
		
	rom.seek(hook_at)
	nparams=nparams-1

	if nparams<4:
		data = bytes([0x10, 0xB5, 0x3, 0x4C, 0x0, 0xF0, 0x3, 0xF8, 0x10, 0xBC , (isreturning+1), 0xBC , (isreturning<<3), 0x47, 0x20, 0x47])
	else:
		k=nparams-3
		data = bytes([0x10, 0xB5, 0x82, 0xB0])
		for i in range(k+2):
				data += bytes([ i+2, 0x9C , i, 0x94])
		data += bytes([0x0, 0x9C , (nparams-1), 0x94, 0x1, 0x9C , nparams, 0x94, 0x2, 0xB0 , (k+8), 0x4C,
					   0x0, 0xF0 , ((k<<1)+13), 0xF8, 0x82, 0xB0 , nparams, 0x9C, 0x1, 0x94 , (nparams-1), 0x9C , 0x0, 0x94])
		for i in reversed(range(k+2)):
				data += bytes([ i, 0x9C , i+2, 0x94])
		data += bytes([0x2, 0xB0 , 0x10, 0xBC, (isreturning+1), 0xBC , (isreturning<<3), 0x47, 0x20, 0x47])
	
	space += 0x08000001
	data += (space.to_bytes(4, 'little'))
	rom.write(bytes(data))

def repoint(rom, space, repoint_at):
	rom.seek(repoint_at)
	space += (0x08000000)
	data = (space.to_bytes(4, 'little'))
	rom.write(bytes(data))
	
def repoint_list(rom, space, list, len):
	for i in range (0, len):
		repoint(rom, space, list[i])
		
def routine_repoint(rom, space, repoint_at):
	repoint(rom, space | 1, repoint_at)
		
def bytereplace(rom, offset, value, bytecount):
	rom.seek(offset)
	rom.write(value.to_bytes(bytecount, byteorder = 'little'))
		
def insert_ball_items(rom, table):
	print("Inserting item balls data.")
	balls = open("./scripts/new_balls.txt", 'r')
	rom.seek(0x1C8)
	rom.seek(int.from_bytes(rom.read(4), byteorder = 'little') - 0x08000000 + (FirstNewBallID * 0x2C)) #get offset of first new item
	counter = 0x0
	#write item table data
	for line in balls:
		name, price, descr = line.split()
		#write item name
		name = int(name, 16)
		rom.write(name.to_bytes(14, 'big'))
		#write index
		rom.write((FirstNewBallID + counter).to_bytes(2, 'little'))
		#write price
		price = int(price, 10)
		rom.write(price.to_bytes(2, 'little'))
		#write battle function and quality
		rom.write((0).to_bytes(2, 'little'))
		#write description
		rom.write((table[descr] + 0x08000000).to_bytes(4, 'little'))
		#write everything else
		bytes = 0x00000201000000000200000095E30F0800000000
		rom.write(bytes.to_bytes(20, 'big'))
		counter += 1
	balls.close()
	#write bag images and palettes
	ball_names = ["level", "lure", "moon", "friend", "love", "heavy", "fast", "sport", "dusk", "quick", "heal", "cherish", "park", "dream", "beast"]
	rom.seek(0x1B0034)
	rom.seek(int.from_bytes(rom.read(4), byteorder = 'little') - 0x08000000 + (FirstNewBallID * 0x8)) #get offset of first new item
	for i in range(0, len(ball_names)):
		try:
			offset_img = table["bag_" + ball_names[i] + "_img"] + 0x08000000
		except:
			offset_img = 0x8DAB45C
		try:
			offset_pal = table["bag_" + ball_names[i] + "_pal"] + 0x08000000
		except:
			offset_pal = 0x8DAB50C
		rom.write(offset_img.to_bytes(4, 'little'))
		rom.write(offset_pal.to_bytes(4, 'little'))
	
def insert_script(rom):
	print("Inserting code.")
	table = symbols(get_text_section())
	rom.seek(insert_offset)
	with open('build/output.bin', 'rb') as binary:
			data = binary.read()
	binary.close()
	rom.write(data)
	
	# Adjust symbol table
	for entry in table:
			table[entry] += insert_offset
	
	# Insert hooks
	if MoreMoney == True:		
		hook(rom, table["prepare_money_box"], 0x0E51F4, 3)
	if EVTrainers == True:
		hook(rom, table["create_trainer_pokemon"], 0x0385E8, 3)
	if WildPokeBattleMusic == True or TrainerMusicVar != 0x0:
		hook(rom, table["choose_song_for_battle"], 0x06E42C, 0)
	if MoreLevels == True:
		hook(rom, table["display_exp_instatssummary"], 0x1C38C0, 0)
		hook(rom, table["LEVELUP_MORE_DIGITS"], 0x1D37FA, 2)
		hook(rom, table["pokemenu_print_currHP"], 0x1B2D3C, 2)
		hook(rom, table["pokemenu_print_maxHP"], 0x1B2DDC, 2)
	if MoreLevels == True or NatureStatColor == True:
		hook(rom, table["display_stats_on_left"], 0x1C3710, 0)
		hook(rom, table["display_stats_on_right"], 0x1C3808, 0)
	if NewEvolutionMethods == True:
		hook(rom, table["try_evolving_poke"], 0x06D098, 3)
	if BallsExpansion == True:
		hook(rom, table["item_to_ball"], 0x170D84, 1)
		hook(rom, table["FT1_use_new_balls"], 0x03E952, 0)
		hook(rom, table["third_frame_ball_hook"], 0x076AA0, 0)
	if MenuOptionsControl == True:
		hook(rom, table["add_suitable_menu_options"], 0x09F4CC, 0)
	if FireRedFishing == True or ChainFishing == True or FishingFlag != 0 or ShinyCharm != 0x0:
		hook(rom, table["fishing_start"], 0x08C88C, 1)
		hook(rom, table["wild_fishing_battle"],  0x0B5734, 1)
		hook(rom, table["create_wild_poke_new"],  0x0B4E68, 2)
	if ChainFishing == True:
		hook(rom, table["chain_fishing_move_check"],  0x09CBE8, 2)
	if HallofFameFix == True:
		hook(rom, table["fame_hall_print_poke_data"], 0x174A88, 1)
		hook(rom, table["fame_hall_registering_poke_display"], 0x173AA8, 1)
		hook(rom, table["set_fame_hall_pokes_from_party"], 0x17371C, 1)
		hook(rom, table["prepare_fame_hall_registering1"], 0x173694, 0)
		hook(rom, table["fame_hall_read_pokes_from_sav"], 0x174324, 1)
		hook(rom, table["fame_hall_pc_poke_data_display"], 0x1745FC, 1)
		hook(rom, table["fame_hall_pc_display_poke_sprites"], 0x1743EC, 1)
		
	# Insert repoints
	if BWRepel == True:
		repoint(rom, table["REPEL_SCRIPT"], 0x0B58C0)
	if MoreLevels == True:
		exptable = table["exp_table"]
		to_repoint = [0x0690B8, 0x069124, 0x067DC0, 0x0592C4, 0x074A10, 0x0593F0, 0x059500, 0x1C3940, 0x1C2338, 0x06C2E4, 0x06DFF0]
		for i in range(0, len(to_repoint)):
			repoint(rom, exptable, to_repoint[i])
	if BallsExpansion == True:		
		to_repoint_tiles = 	[0x0001D0, 0x076510, 0x0767C0, 0x076AD4, 0x076B0C]
		repoint_list(rom, table["ball_tiles"], to_repoint_tiles, len(to_repoint_tiles))
		
		to_repoint_pals = 	[0x0001D4, 0x076514, 0x0767C4, 0x076ADC, 0x076B10]
		repoint_list(rom, table["ball_pals"], to_repoint_pals, len(to_repoint_pals))
		
		to_repoint_templates = [0x075538, 0x076518, 0x0767C8, 0x170ED0, 0x171010, 0x1C4A74]
		repoint_list(rom, table["ball_templates"], to_repoint_templates, len(to_repoint_templates))
		
		stars_tiles = [0x171D8C, 0x1729D4]
		repoint_list(rom, table["ball_stars_tiles"], stars_tiles, len(stars_tiles))
		
		stars_pals  = [0x171D94, 0x1729D8]
		repoint_list(rom, table["ball_stars_pals"], stars_pals, len(stars_pals))
		
		stars_IDs = [0x171AA8, 0x171F38, 0x1720B4, 0x1721B8, 0x1722B4, 0x1723C0, 0x1724EC, 0x172644, 0x17277C, 0x1728C8]
		repoint_list(rom, table["Ball_Stars"], stars_IDs, len(stars_IDs))
		
		ball_tasks_IDs = [0x171E18, 0x172990]
		repoint_list(rom, table["ball_anim_tasks"], ball_tasks_IDs, len(ball_tasks_IDs))
		
		stars_templates = [0x171F30, 0x1720B0, 0x1721B4, 0x1722B8, 0x1723BC, 0x1724F0, 0x172648, 0x172774, 0x1728CC]
		repoint_list(rom, table["ball_stars_templates"], stars_templates, len(stars_templates))
		
		ball_fade = [0x172A54, 0x172AA0, 0x172B04, 0x172BE0]
		repoint_list(rom, table["ball_fadepoke_table"], ball_fade, len(ball_fade))
	if BallsExpansion == True or MoreLevels == True:
		repoint(rom, table["getattributes_origingame"], 0x06A79C)
		repoint(rom, table["setattributes_origingame"], 0x06AEC8)
		repoint(rom, table["getattributes_lvl"], 0x06A798)
		repoint(rom, table["setattributes_lvl"], 0x06AEC4)
		repoint(rom, table["getattributes_ball"], 0x06A7A0)
		repoint(rom, table["setattributes_ball"], 0x06AECC)
	#Insert routine pointers
	if NewSpecials == True:
		routine_repoint(rom, table["s25_special"], 0x1DB710)
		routine_repoint(rom, table["s26_special2"], 0x1DB714)
	if FireRedFishing == True or ChainFishing == True or FishingFlag != 0 or ShinyCharm != 0x0:
		routine_repoint(rom, table["run_fishing_functions"], 0x08CD90)
	# Insert byte changes
	if MoreMoney == True:
		bytereplace(rom, 0x0E5188, 9999999, 4)
		bytereplace(rom, 0x0C36EE, 7, 1)
		bytereplace(rom, 0x0E5238, 7, 1)
	if MoreLevels == True:
		bytereplace(rom, 0x06D91C, 0, 4) #large stat display
		bytereplace(rom, 0x06FE68, 3, 1) #Daycare int three-digits
		#Max Level
		to_bytereplace = [0x1C3908, 0x1C22E2, 0x05C54A, 0x06DFD8, 0x06FD20, 0x06C17C, 0x1B743A, 0x04A88E, 0x04A5BE, 0x069096, 0x069102, 0x1B889C]
		for i in range(0, len(to_bytereplace)):
			if i > 2:
				bytereplace(rom, to_bytereplace[i], MaxLevel, 1)
			else:
				bytereplace(rom, to_bytereplace[i], MaxLevel - 1, 1)
		#Sizeof changing
		to_change = [0x069082, 0x0690EE, 0x067CF0, 0x1C3922, 0x059240, 0x07499C, 0x0593A0, 0x0594A8, 0x1C22FC, 0x06C1A8, 0x06DFB2]
		for i in range(0, len(to_change)):
			curr_offset = to_change[i]
			bytereplace(rom, curr_offset, 0x20, 1)
			bytereplace(rom, curr_offset + 3, 1, 1)
	if BallsExpansion == True:
		#Don't call item_to_ball function as argument is already ballID
		to_bytereplace = [0x0754CE, 0x07567C, 0x075D3A, 0x170C14, 0x1C4A14]
		for i in range(0, len(to_bytereplace)):
			bytereplace(rom, to_bytereplace[i], 0, 4)
		#correct luxury ball ID in happiness boost
		bytereplace(rom, 0x06DA64, 0xA, 1)
		#limiter
		bytereplace(rom, 0x1711A4, 31, 1)
		bytereplace(rom, 0x171B08, 31, 1)
	if FireRedFishing == True or ChainFishing == True or FishingFlag != 0 or ShinyCharm != 0x0:
		#modify the wild poke get nature function, so it returns 30 if wants a random nature
		bytereplace(rom, 0x0B4E4C, 0xE006201E, 4)
	#if HallofFameFix == True:
		
	width = max(map(len, table.keys())) + 1
	offset_file = open("offsets.ini", 'r+')
	offset_file.truncate()
	for key in sorted(table.keys()):
				fstr = ('{:' + str(width) + '} {:08X}')
				offset_file.write(fstr.format(key + ':', table[key] + 0x08000000) + '\n')
	offset_file.close()
	if BallsExpansion == True:
		insert_ball_items(rom, table)
		
def main():
	build_script()
	shutil.copyfile(rom_name, copy_name) #copy rom
	rom = open(copy_name, 'rb+')
	insert_script(rom)
	rom.close()
	
if __name__ == '__main__':
	main()
