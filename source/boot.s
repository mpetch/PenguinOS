.set ALIGN,    1<<0             							# Align loaded modules on page boundaries
.set MEMINFO,  1<<1             							# Provide memory map
.set FLAGS,    ALIGN | MEMINFO  							# This is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       							# 'Magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) 							# Checksum of above, to prove we are multiboot

# Declare a multiboot header that marks the program as a kernel
.section .multiboot											# Declare multiboot header
.align 4													# Align to byte 4
.long MAGIC													# 4 bytes (8)
.long FLAGS													# 4 bytes (12)
.long CHECKSUM												# 4 bytes (16)

# Define
.section .stack												# Declare stack header
.align 16													# Align to byte 32
stack_bottom:												# Function to skip to the bottom of the stack (16384)
.skip 16384 												# 16 KiB
stack_top:

/*
The linker script specifies _start as the entry point to the kernel and the
bootloader will jump to this position once the kernel has been loaded. It
doesn't make sense to return from this function as the bootloader is gone.
*/
.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp									# Set the esp (stack pointer) register to point to the
															# top of the stack
	call kernel_main										# Enter the kernel

	# Loop
	cli														# Disable interrupts
1:	hlt														# Wait for the next interrupt to arrive
	jmp 1b													# Jump back to the 'hlt' instruction

/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start
