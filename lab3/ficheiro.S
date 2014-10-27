.global _kbd_handler_ass //function that handles the interrupts when in Assembly
.global _scan_key //variable that receives what is on input buffer

.text

_kbd_handler_ass:
inb $0x60, %al //function that passes to %al what is on input buffer
movb %al, _scan_key //copying the content of %al to the variable _scan_key
ret
