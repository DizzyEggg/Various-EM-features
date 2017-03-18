.text
.align 1
.thumb
.thumb_func

LEVELUP_MORE_DIGITS: @hook at 0x081D37FA
	mov r4, #4
	ldr r2, =(999)
	cmp r0, r2
	bgt LEVELUP_MORE_DIGITS_RETURN
	mov r4, #3
	cmp r0, #99
	bgt LEVELUP_MORE_DIGITS_RETURN
	mov r4, #1
	cmp r0, #9
	ble LEVELUP_MORE_DIGITS_RETURN
	mov r4, #2
LEVELUP_MORE_DIGITS_RETURN:
	ldr r2, =(0x081D3808 | 1)
	bx r2
	