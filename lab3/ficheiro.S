.global _kbd_handler_ass
.global _scan_key

.text

_kbd_handler_ass:
inb $0x60, %al
movb %al, _scan_key
ret
