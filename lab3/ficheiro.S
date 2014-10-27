.global _kbd_handler_ass

.text

_kbd_handler_ass:
inb $0x60,%al
ret
