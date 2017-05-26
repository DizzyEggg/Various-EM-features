.text
.align 1
.thumb
.thumb_func

c2_overworld:
	push {r4, lr}
	ldr r0, =(fadescreen_info)
	ldrb r4, [r0, #7]
	ldr r0, =(c2_ov_basic)
	lsr r4, r4, #7
	cmp r4, #0x0
	beq c2_just_do_basic
@here we do ov basic, but also set the handler set
	ldr r4, =(0x30022CC) @vblank handler ptr
	mov r1, #0
	str r1, [r4]
	bl bx_r0
	ldr r0, =(0x08086390 | 1) @vblank handler for overworld
	str r0, [r4]
	pop {r4, pc}
	
c2_just_do_basic:
	bl bx_r0
	bl ov_consider_palchange
	pop {r4, pc}
	
bx_r0:
	bx r0
	