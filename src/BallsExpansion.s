.text
.align 1
.thumb
.thumb_func

FT1_use_new_balls: @hook at 0x0803E952, via r0, r3 is itemID Fighting tracker function
	push {r3}
	mov r0, r3
	ldr r1, =(get_item_pocket_id)
	bl r1_caller
	pop {r3}
	cmp r0, #2
	beq FT1_throw_ball
	ldr r0, =(0x0803E998 | 1)
	bx r0
FT1_throw_ball:
	ldr r1, =(0x82DBD68)
	ldr r2, =(battlescripts_curr_instruction)
	cmp r3, #0x5
	bne FT1_store_bs_throw
	add r1, r1, #0x16 @safari ball has different instructions
FT1_store_bs_throw:
	str r1, [r2]
	ldr r0, =(0x0803E964 | 1)
	bx r0
r1_caller:
	bx r1
	
third_frame_ball_hook: @hook at 0x08076AA0, via r0, r4 is ballID
	ldr r1, =(0x08076ACC | 1) @end location
	cmp r4, #6
	beq r1_caller
	cmp r4, #8
	bgt r1_caller
	ldr r0, =(0x08076AB0 | 1)
	bx r0
	
	