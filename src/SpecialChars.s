.text
.thumb
.thumb_func
.align 1

FC_switch_hook: @hook at 080058E0 via r0
	ldr r0, [r6]
	ldrb r3, [r0]
	add r0, #1
	str r0, [r6]
	cmp r3, #0x90
	beq FC_pal_change
	sub r0, r3, #1
	cmp r0, #23
	bls FC_switch_jump
	ldr r0, =(0x08005B90 | 1)
	bx r0
FC_switch_jump:
	ldr r1, =(0x080058F0 | 1)
	bx r1
FC_pal_change:
	ldrb r3, [r0] @r0 contains pointer to current char; r3 ID to change
	ldrb r1, [r0, #1]
	ldrb r2, [r0, #2]
	lsl r2, r2, #8
	orr r1, r2
	add r0, #3
	str r0, [r6]
	ldr r2, =(0x02037CF4)
	lsl r3, r3, #1
	add r2, r2, r3
	strh r1, [r2]
	ldr r0, =(0x08005A0A | 1)
	bx r0
	
