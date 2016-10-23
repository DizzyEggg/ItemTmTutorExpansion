#!/usr/bin/env python3

import shutil
import os

#Lines to edit by an user
##############################
NEW_ITEMS = X + 378 #X is how many new items you want to add
TMS_NO = 100 #Number of all TMs
HMS_NO = 8 #Number of all HMs
POKE_NUM = 412 #Number of Pokemon present in your game
CLEAN_REPOINTED_DATA = True #cleans repointed data if true; leaves it if false
FREE_SPACE = 0xF00000 #Location to start looking for free space
TM_EXPANSION = True #expands TMs if True
EXPANDED_BAG_OFFSET = 0x0203CF64 #RAM Location to store expanded bag items
INFINITE_TMS = 1 #Set to 0 if you want TMs to disappear after use
BUILD_CODE = True #Set to False if you want to modify and run the build/insert scripts manually
MOVETUTOR_EXPANSION = True #expands tutor moves if True
TUTOR_MOVES_NO = 64 #Amount of available tutor moves
ITEM_POCKET_MAX = 30 #Max number of items in pocket items 
KEY_POCKET_MAX = 30 #Max number of items in pocket key items
BALL_POCKET_MAX = 16 #Max number of items in pocket balls
BERRY_POCKET_MAX = 46 #Max number of items in pocket berries
##############################

def align_offset(offset):
	while (offset % 4) != 0:
		offset += 1
	return offset

def find_offset_to_put(rom, needed_bytes, start_loc):
	offset = start_loc
	found_bytes = 0
	while (found_bytes < needed_bytes):
		for i in range (0, needed_bytes):
			rom.seek(offset + i)
			byte = rom.read(1)
			if (byte):
				if (byte != b'\xFF'):
					offset += i + 1
					align_offset(offset)
					found_bytes = 0
					break
				found_bytes += 1
			else:
				return 0
	return offset

def get_pointer(rom, loc):
	rom.seek(loc)
	return int.from_bytes(rom.read(4), byteorder = 'little') - 0x08000000
	
def clear_space(rom, loc, bytes):
	if (CLEAN_REPOINTED_DATA == True):
		rom.seek(loc)
		for i in range(0, bytes):
			rom.write(b'\xFF')
			
def save_table_ptrs(name, offset, clear):
	ini = open("offsets.ini", 'a')
	if clear == 1:
		ini.seek(0x0)
		ini.truncate()
	else:
		ini.write(name)
		ini.write(": ")
		ini.write(hex(offset))
		ini.write("\n")
	ini.close()
	
def update_ptrs(rom, oldptr, newptr):
	rom.seek(0x0)
	copy = rom.read(0x1E8000)
	copy = copy.replace((oldptr + 0x08000000).to_bytes(4, byteorder = 'little'), (newptr + 0x08000000).to_bytes(4, byteorder = 'little'))
	rom.seek(0x0)
	rom.write(copy)

def repoint_table(rom, table_address, bytes, offset, name):
	rom.seek(table_address)
	to_copy = rom.read(bytes)
	clear_space(rom, table_address, bytes)
	rom.seek(offset)
	rom.write(to_copy)
	save_table_ptrs(name, offset, 0)
	update_ptrs(rom, table_address, offset)
	offset += bytes
	return offset
	
def append_new_entries(rom, offset, copy, sizeof, entries):
	rom.seek(offset)
	for i in range(0, entries):
		rom.write(copy)
		offset += sizeof
	return offset

def write_new_tm_data(rom, offset, num):
	rom.seek(offset)
	rom.write((0xCEC7).to_bytes(2, byteorder = 'big')) #TM prefix
	if num > 99:
		rom.write((0xACAC).to_bytes(2, byteorder = 'big')) #question marks
	else:
		div_10 = num // 10
		rom.write((0xA1 + div_10).to_bytes(1, byteorder = 'little')) #first digit
		rom.write((0xA1 + (num - div_10 * 10)).to_bytes(1, byteorder = 'little')) #second digit
	rom.write((0xFF000000000000000000).to_bytes(10, byteorder = 'big')) #name
	rom.write((0x00B80B0000A0FE57080000030149DF0F08000000000000000000000000).to_bytes(30, byteorder = 'big'))
	
def expand_items(rom, offset):
	#first goes the item table
	sizeof = 0x2C
	offset = repoint_table(rom, get_pointer(rom, 0x0001C8), 377 * sizeof, offset, "item_table")
	#append the bag arrow entry
	rom.seek(offset)
	rom.write((0xBCD5DB00BBE6E6E3EBFF00000000000000000000000000000000000000000000000000000000000000000000).to_bytes(sizeof, byteorder = 'big'))
	offset += sizeof
	rom.seek(offset)
	#expand TMs
	if TM_EXPANSION == True:
		#move TM Data to a new place in the table
		old_tm_data_ptr = get_pointer(rom, 0x0001C8) + sizeof * 0x121
		rom.seek(old_tm_data_ptr)
		old_tm_data = rom.read(sizeof * 50)
		rom.seek(get_pointer(rom, 0x0001C8) + sizeof * 378)
		rom.write(old_tm_data)
		offset += (sizeof * 50)
		#Add placeholder TMs
		ptr_new_tms_data = get_pointer(rom, 0x0001C8) + sizeof * 428
		for i in range(0, TMS_NO - 50):
			ptr_to_write = ptr_new_tms_data + sizeof * i
			write_new_tm_data(rom, ptr_to_write, i + 51)
			offset += sizeof
		#move HM Data to a new place
		old_tm_data_ptr = get_pointer(rom, 0x0001C8) + sizeof * 0x153
		rom.seek(old_tm_data_ptr)
		old_tm_data = rom.read(sizeof * 8)
		rom.seek(get_pointer(rom, 0x0001C8) + sizeof * (378 + TMS_NO))
		rom.write(old_tm_data)
		offset += (sizeof * 8)
		#clear old tms data
		for i in range(0x121, 0x15B):
			rom.seek(get_pointer(rom, 0x0001C8) + sizeof * i)
			rom.write((0xC8D9EB00C3E8D9E1FF0000000000).to_bytes(14, byteorder = 'big')) #name
			rom.write((0x0000000000A0FE57080000010421E80F08000000000000000000000000).to_bytes(30, byteorder = 'big'))
	#add placeholder data for all new items
	rom.seek(offset)
	to_search = NEW_ITEMS - 378
	if TM_EXPANSION == True:
		to_search = NEW_ITEMS - (TMS_NO - 50) - 378
	for i in range(0, to_search):
		rom.write((0xC8D9EB00C3E8D9E1FF0000000000).to_bytes(14, byteorder = 'big')) #name
		rom.write((0x0000000000A0FE57080000010421E80F08000000000000000000000000).to_bytes(30, byteorder = 'big'))
		offset += sizeof
	
	#fix all item indices
	ptr = get_pointer(rom, 0x0001C8) + 14
	for i in range(0, NEW_ITEMS):
		rom.seek(ptr + sizeof * i)
		rom.write(i.to_bytes(2, byteorder = 'little'))
		
	#now the image images
	sizeof = 0x8
	offset = repoint_table(rom, get_pointer(rom, 0x1B0034), 378 * sizeof, offset, "item_images")
	offset = append_new_entries(rom, offset, (0x58B0DA08D0B0DA08).to_bytes(sizeof, byteorder = 'big'), sizeof, NEW_ITEMS - 378)
	
	if TM_EXPANSION == True: #make their images appear
		ptr = get_pointer(rom, 0x1B0034) + sizeof * 0x121
		rom.seek(ptr)
		tm_imgs = rom.read(sizeof * 50)
		rom.seek(ptr)
		for i in range (0, 50):
			rom.write((0x58B0DA08D0B0DA08).to_bytes(sizeof, byteorder = 'big'))
		rom.seek(get_pointer(rom, 0x1B0034) + sizeof * 378)
		rom.write(tm_imgs)
		for i in range(0, TMS_NO - 50):
			rom.write((0xF85CDB08F45DDB08).to_bytes(sizeof, byteorder = 'big'))
	
	return offset

def expand_comp_table(rom, offset, sizeof_old, sizeof_new, ptr, name):
	table_ptr = get_pointer(rom, ptr)
	save_table_ptrs(name, offset, 0)
	update_ptrs(rom, table_ptr, offset)
	for i in range(0, POKE_NUM):
		old_ptr = table_ptr + i * sizeof_old
		rom.seek(old_ptr)
		copy = rom.read(sizeof_old)
		clear_space(rom, old_ptr, sizeof_old)
		rom.seek(offset)
		rom.write(copy)
		rom.write((0x0).to_bytes(sizeof_new - sizeof_old, byteorder = 'little'))
		offset += sizeof_new
	return offset

def expand_tms(rom, offset):
	#TM/HM compatibility table
	return expand_comp_table(rom, offset, 8, 16, 0x06E0B0, "TM_compatibility_table")
	
def bytechanges(rom):
	if TM_EXPANSION == True:
		all_tms_no = (TMS_NO + HMS_NO).to_bytes(1, byteorder = 'little')
		rom.seek(0x1398D0)
		rom.write(all_tms_no)
		rom.seek(0x0D6624)
		rom.write(all_tms_no)
		rom.seek(0x1AB5FC)
		rom.write(((378 + TMS_NO) * -1).to_bytes(4, byteorder = 'little', signed = 'true'))
		rom.seek(0x1B6EE0)

def replace_word(file, to_search, replacement):
	get = 0
	break_loop = 0
	to_replace = to_search + " "
	file.seek(0x0)
	for line in file:
		if (break_loop == 1):
			break
		for word in line.split():
			if (word == to_search):
				get = 1
			elif (get == 1):
				to_replace += word
				break_loop = 1
				break
	file.seek(0x0)
	copy = file.read()
	copy = copy.replace(to_replace, to_search + " " + replacement)
	file.seek(0x0)
	file.write(copy)
		
def build_insert_code(offset, only_build):
	if only_build == True:
		linker = open("linker.ld", 'r+')
		replace_word(linker, '+', hex(offset) + "),")
		linker.close()
		c_define = open("src//defines.h", 'r+')
		replace_word(c_define, "ALL_ITEMS", str(NEW_ITEMS))
		replace_word(c_define, "ALL_POKES", str(POKE_NUM))
		replace_word(c_define, "expanded_bag_items_ptr", str(EXPANDED_BAG_OFFSET))
		replace_word(c_define, "BALL_POCKET_MAX_NEW", str(BALL_POCKET_MAX))
		replace_word(c_define, "ITEM_POCKET_MAX_NEW", str(ITEM_POCKET_MAX))
		replace_word(c_define, "KEY_POCKET_MAX_NEW", str(KEY_POCKET_MAX))
		replace_word(c_define, "BERRY_POCKET_MAX_NEW", str(BERRY_POCKET_MAX))
		if TM_EXPANSION == True:
			replace_word(c_define, "ALL_TMS", str(TMS_NO))
			replace_word(c_define, "ALL_HMS", str(HMS_NO))
			replace_word(c_define, "FIRST_TM_INDEX", str(378))
		else:
			replace_word(c_define, "ALL_TMS", str(50))
			replace_word(c_define, "ALL_HMS", str(8))
			replace_word(c_define, "FIRST_TM_INDEX", str(289))
		if MOVETUTOR_EXPANSION == True:
			replace_word(c_define, "TUTOR_MOVES", str(TUTOR_MOVES_NO))
		else:
			replace_word(c_define, "TUTOR_MOVES", str(32))
		c_define.close()
		asm_define = open("src//asm_code.s", 'r+')
		replace_word(asm_define, "INFINITE_TMS,", str(INFINITE_TMS))
		asm_define.close()
		os.system("python scripts//build")
		return
	else:
		insert = open("scripts//insert", 'r+')
		replace_word(insert, "at',", "default=" + hex(offset) + ')')
		insert.close()
		os.system("python scripts//insert --debug>function_offsets.ini")
		return

def expand_tutors(rom, offset):
	#Tutor Compatibility
	sizeof_new = 8
	if TUTOR_MOVES_NO > 64 and TUTOR_MOVES_NO <= 96:
		sizeof_new = 12
	elif TUTOR_MOVES_NO > 96 and TUTOR_MOVES_NO <= 128:
		sizeof_new = 16
	return expand_comp_table(rom, offset, 4, sizeof_new, 0x1B2390, "MoveTutor_compatibility_table")
		
shutil.copyfile("rom.gba", "test.gba")
with open("test.gba", 'rb+') as rom:
	needed_bytes = NEW_ITEMS * 0x2C + NEW_ITEMS * 8
	if TM_EXPANSION == True:
		needed_bytes += 16 * POKE_NUM
	offset = find_offset_to_put(rom, needed_bytes, align_offset(FREE_SPACE))
	if offset == 0:
		print("Not enough free space")
	elif TM_EXPANSION == True and (TMS_NO + HMS_NO) > 128:
		print("TMs and HMs in total cannot be higher than 128")
	elif TM_EXPANSION == True and (378 + TMS_NO + HMS_NO) > NEW_ITEMS:
		print("Number of new TMs cannot extend the number of new items")
	elif TM_EXPANSION == True and ((TMS_NO + HMS_NO) < 64 or TMS_NO <= 50):
		print("TMs and HMs in total must be higher than 64")
	elif MOVETUTOR_EXPANSION == True and (TUTOR_MOVES_NO > 128 or TUTOR_MOVES_NO < 32):
		print("Amount of Move Tutor Moves must be between 32 and 128")
	else:
		save_table_ptrs(0, 0,  1)
		offset = expand_items(rom, offset)
		if TM_EXPANSION == True:
			offset = expand_tms(rom, offset)
		if MOVETUTOR_EXPANSION == True:
			offset = expand_tutors(rom, offset)
		bytechanges(rom)
		if BUILD_CODE == True:
			build_insert_code(offset, True)
			offset = find_offset_to_put(rom, os.stat("build//output.bin").st_size, align_offset(offset))
			if offset == 0:
				print("Not enough free space to insert code")
			else:
				build_insert_code(offset, False)
	rom.close()
