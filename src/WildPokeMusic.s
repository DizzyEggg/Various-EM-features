.text
.align 1
.thumb
.thumb_func

WILD_POKE_MUSIC:
	mov r0, #8
	and r0, r1
	cmp r0, #0x0
	bne POKE_MUSIC_TRAINER
	ldr r0, =(party_opponent)
	mov r1, #0x41	@speces
	ldr r3, =(get_attributes)
	bl R3_CALLER
	ldr r3, =(basestat_table)
	ldr r3, [r3]
	mov r1, #0x1C @sizeof of base stat table
	mul r0, r1
	add r0, r0, r3
	ldrh r1, [r0, #0x1A]
	mov r0, #0xED
	lsl r0, r0, #1
	cmp r1, #0x0
	beq WILD_POKE_MUSIC_RETURN
	mov r0, r1
	
WILD_POKE_MUSIC_RETURN:
	ldr r1, =(0x0806E646 | 1)
	bx r1
	
POKE_MUSIC_TRAINER:
	ldr r0, =(0x0806E470 | 1)
	bx r0
	
R3_CALLER:
	bx r3
