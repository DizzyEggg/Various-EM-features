#!/usr/bin/env python3

rom_name = "BPEE0.gba"		#Name of your rom

with open ('./scripts/list.txt', 'r+') as replacelist:
	rom = open(rom_name, 'rb+')
	for line in replacelist:
		offset = int(line[:6],16)
		print("Changing data at " + hex(offset))
		rom.seek(offset)
		replacements = line[7:].split()
		for i in range(0, len(replacements)):
			rom.write(bytes(int(replacements[i],16).to_bytes(1, 'little')))
	rom.close()
	replacelist.close()
