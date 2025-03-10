global kSwitchContext
kSwitchContext:
    ; Load general registers
    ; Skip return address, and get the pointer regs
    mov ebp, [esp + 4]
    mov ecx, [ebp + 4]
    mov edx, [ebp + 8]
    mov ebx, [ebp + 12]
    mov esi, [ebp + 24]
    mov edi, [ebp + 28]
    ; load eflags
    ;mov eax, [ebp + 32]
    ;push eax
    ;popfd
    ; Right now, eax, ebp, esp are not restored yet

    ; Enter usermode from here(make sure the registers are restored correctly for the user process !)
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push 0x23
    ; Push user esp
    mov eax, [ebp + 16]
    push eax
    mov eax, [ebp + 32]
    push eax
    ;pushfd
    push 0x1b
    ; Push eip
    mov eax, [ebp + 40]
    push eax
    ; Enter usermode from here(make sure the registers are restored correctly for the user process !)

    ; Load eax here
    mov eax, [ebp + 0]
    ; Now, restore ebp
    mov ebp, [ebp + 20]
    ; sti
    iret