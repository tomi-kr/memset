.data

mybyte byte 35
myword word ?
myfloat real4 77.2

what qword 9

.code
ASM_Test proc	
	mov what, 999	

	lea r12, what

	mov qword ptr[r12], 2021

	mov rax, what
	mov rax, rax
	mov al, al
	nop 

	ret
ASM_Test endp
end