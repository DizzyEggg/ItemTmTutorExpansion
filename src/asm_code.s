.text
.thumb
.thumb_func
.align 2

.equ INFINITE_TMS, 1

fix_TM_swapping:
	lsl r4, r5, #2
	cmp r0, #0x0
	beq sort
	ldr r0, [r6]
	add r0, r0, r7
	ldrh r0, [r0]
	cmp r0, #0
	beq sort
	ldr r0, [r6]
	add r0, r0, r4
	add r0, #2
	ldr r1, =(get_item_quantity)
	bl r1_caller
	cmp r0, #0x0
	beq dont_sort
	ldr r0, [r6]
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, #0x0
	beq dont_sort
	ldr r1, =(0x080D6FF0 | 1)
	b r1_caller
	
sort:
	ldr r1, =(0x080D6FFE | 1)
	b r1_caller
	
dont_sort:
	ldr r1, =(0x080D7008 | 1)
	
r1_caller:
	bx r1
	
r2_caller:
	bx r2
	
delete_tm_after_use:
	mov r0, r4
	bl get_TM_move
	bl is_move_a_HM
	cmp r0, #0x1
	beq return_loc
	mov r1, #INFINITE_TMS
	cmp r1, #0x1
	beq return_loc
	mov r0, r4
	mov r1, #1
	ldr r2, =(bag_remove_item)
	bl r2_caller
	
return_loc:
	ldr r1, =(0x081B6EF0 | 1)
	bx r1

