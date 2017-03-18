.text
.thumb
.thumb_func
.align 1

chain_fishing_move_check:
	push {r4-r7, lr}
	mov r4, r0
	mov r7, r4
	ldr r2, =(sav1)
	ldr r1, [r2]
	ldr r1, [r1]
	ldr r2, =(last_fishing_loc)
	ldr r0, [r2]
	cmp r1, r0
	bne reset_fishing_tries
chain_fishing_move_check_olffunction_part:
	ldr r6, =(0x020375D4)
	ldrb r0, [r6]
	ldr r1, =(0x0809CBF4 | 1)
	bx r1
reset_fishing_tries:
	mov r0, #0
	strb r0, [r2, #4]
	b chain_fishing_move_check_olffunction_part
	