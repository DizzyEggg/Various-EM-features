.text
.align 1
.thumb
.thumb_func

getattributes_ball:
	ldrb r0, [r5, #3]
	mov r4, #0x7C
	and r4, r0
	lsr r4, r4, #2
	b jump_getattrs
	
setattributes_ball:
	ldrh r0, [r4]
	bl item_to_ball
	ldrb r1, [r5, #3]
	mov r2, #0x7C
	bic r1, r2
	lsl r0, r0, #2
	and r0, r2
	orr r1, r0
	strb r1, [r5, #3]
	b jump_setattrs

getattributes_lvl: @jumptable at 0x0806A798
	ldrb r4, [r5, #2]
	b jump_getattrs
	
setattributes_lvl: @jumptable at 0x0806AEC4
	ldrb r1, [r4]
	strb r1, [r5, #2]
	b jump_setattrs

getattributes_origingame: @jumptable at 0x0806A79C
	ldrb r4, [r5, #3]
	mov r0, #3
	and r4, r0
jump_getattrs:
	ldr r0, =(0x0806AC8C | 1)
	bx r0
	
setattributes_origingame: @jumptable at 0x0806AEC8
	ldrb r1, [r4]
	mov r0, #3
	and r1, r0
	ldrb r2, [r5, #3]
	bic r2, r0
	orr r2, r1
	strb r2, [r5, #3]
jump_setattrs:
	ldr r0, =(0x806B3D8 | 1)
	bx r0
	