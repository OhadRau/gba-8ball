.text           @ aka .section .text
.code 16        @ aka .thumb

/*
This file includes BIOS procedures to use in the C code

SWIs (software interrupts) are really annoying to call
with all of the warning flags we have to enable, so
instead of that I'm just defining a _isqrt function to
call the 0x08 interrupt (integer square root).
*/

.align 2        @ aka .balign 4
.global _isqrt
.thumb_func
_isqrt:
  swi     0x08
  bx      lr
