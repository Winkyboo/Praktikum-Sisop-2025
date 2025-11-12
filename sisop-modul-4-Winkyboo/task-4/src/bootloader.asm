bits 16

KERNEL_SEGMENT equ 0x1000 ; kernel akan dimuat ke 0x1000:0x0000
KERNEL_SECTORS equ 15     ; kernel max 15 sektor
KERNEL_START   equ 1      ; kernel mulai dari sektor ke-2 (0 = sektor 1)

bootloader:
  ; memuat kernel ke memori
  mov ax, KERNEL_SEGMENT
  mov es, ax
  mov bx, 0x0000

  mov ah, 0x02           ; function: read sectors from disk
  mov al, KERNEL_SECTORS ; jumlah sektor yang dibaca
  mov ch, 0x00           ; cylinder
  mov cl, KERNEL_START + 1 ; sektor (sektor 2)
  mov dh, 0x00           ; head
  mov dl, 0x00           ; drive (A)
  int 0x13               ; panggil BIOS disk interrupt

  ; set segment register
  mov ax, KERNEL_SEGMENT
  mov ds, ax
  mov es, ax
  mov ss, ax

  ; inisialisasi stack
  mov ax, 0xFFF0
  mov sp, ax
  mov bp, ax

  ; lompat ke kernel
  jmp KERNEL_SEGMENT:0x0000

  ; padding dan signature bootable
  times 510 - ($ - $$) db 0
  dw 0xAA55
