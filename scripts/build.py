#!/usr/bin/env python3

#To Edit
BWRepel = 				False
WildMusic = 			False
EVTrainers = 			False
MoreMoney = 			False
NewEvolutionMethods = 	False
NatureStatColor = 		False
MoreLevels =			False
MaxLevel = 				250		#Maximal level avaiable
EvosPerPoke =			5		#Number of evolutions per pokemon in your rom hack

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
	
def edit_defines():
	defines = open(SRC + "/defines.h", 'r+')
	copy = defines.read()
	defines.seek(0x0)
	for line in defines:
		try:
			directive, name, val = line.split()
		except:
			continue
		if name == "MAX_LEVEL" and MoreLevels == True:
			newline = directive + ' ' + name + '\t\t' + str(MaxLevel) + '\n'
			copy = copy.replace(line, newline)
		elif name == "MoreLevels":
			to_write = "true"
			if MoreLevels == False:
				to_write = "false"
			newline = directive + ' ' + name + '\t\t' + to_write + '\n'
			copy = copy.replace(line, newline)
		elif NewEvolutionMethods == True and name == "EVO_PER_POKE":
			newline = directive + ' ' + name + '\t' + str(EvosPerPoke) + '\n'
			copy = copy.replace(line, newline)
		elif name == "COLORED_STATS":
			to_write = "true"
			if NatureStatColor == False:
				to_write = "false"
			newline = directive + ' ' + name + '\t' + to_write + '\n'
			copy = copy.replace(line, newline)
			break
			
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
	if WildMusic == True:
		globs['WildPokeMusic.s'] = process_assembly
	if BWRepel == True:
		globs['Repel_Code.c'] = process_c
		globs['Repel_Script.s'] = process_assembly
	if MoreLevels == True or NatureStatColor == True:
		globs['MoreLevels.c'] = process_c
	if MoreLevels == True:
		globs['MoreLevels.s'] = process_assembly
	if NewEvolutionMethods == True:
		globs['EvolutionMethods.c'] = process_c
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
		
def insert(rom):
	where = insert_offset
	rom.seek(where)
	with open('build/output.bin', 'rb') as binary:
			data = binary.read()
	rom.write(data)
	return where
					   
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
		
def routine_repoint(rom, space, repoint_at):
	repoint(rom, space | 1, repoint_at)
		
def bytereplace(rom, offset, value, bytecount):
	rom.seek(offset)
	rom.write(value.to_bytes(bytecount, byteorder = 'little'))

def insert_script():
	shutil.copyfile(rom_name, copy_name)
	print("Inserting data.")
	rom = open(copy_name, 'rb+')
	offset = get_text_section()
	table = symbols(offset)
	where = insert(rom)
	
	# Adjust symbol table
	for entry in table:
			table[entry] += where
	
	# Insert hooks
	if MoreMoney == True:		
		hook(rom, table["prepare_money_box"], 0x0E51F4, 3)
	if EVTrainers == True:
		hook(rom, table["create_trainer_pokemon"], 0x0385E8, 3)
	if WildMusic == True:
		hook(rom, table["WILD_POKE_MUSIC"], 0x06E466, 2)
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

	# Insert repoints
	if BWRepel == True:
		repoint(rom, table["REPEL_SCRIPT"], 0x0B58C0)
	if MoreLevels == True:
		exptable = table["exp_table"]
		to_repoint = [0x0690B8, 0x069124, 0x067DC0, 0x0592C4, 0x074A10, 0x0593F0, 0x059500, 0x1C3940, 0x1C2338, 0x06C2E4, 0x06DFF0]
		for i in range(0, len(to_repoint)):
			repoint(rom, exptable, to_repoint[i])

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
		
	width = max(map(len, table.keys())) + 1
	offset_file = open("offsets.ini", 'r+')
	offset_file.truncate()
	for key in sorted(table.keys()):
				fstr = ('{:' + str(width) + '} {:08X}')
				offset_file.write(fstr.format(key + ':', table[key] + 0x08000000) + '\n')
	offset_file.close()
	
def main():
	build_script()
	insert_script()
	
if __name__ == '__main__':
	main()
