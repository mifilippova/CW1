; Variant 4.
format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable
               strVecSize db 'Enter number of array elements ', 0
               strIncorSize db 'Incorrect size of vector = %d', 10, 0
               strVecElemI db '[%d]? ', 0
               strScanInt db '%d', 0
               strVecElemOut db '[%d] = %d', 10, 0
               strVectA db 'Old vector: ', 0
               strVectB db 'New vector: ', 0

               vec_size dd 0
               i  dd ?
               tmp dd ?
               tmpStack dd ?
               vec_a rd 100
               vec_b rd 100

               NULL = 0

section '.code' code readable executable
start:
; Input array A.
        call VectorInput

; Creating the array B according to rule: 1 - for positive elements in A,
;                                         0 - for 0 in A
;                                        -1 - for negative elements in A.
        call VectorChange

; Output array A.
        push strVectA
        call [printf]
        call VectorOutA

; Output array B.
        push strVectB
        call [printf]
        call VectorOutB

finish:
         call [getch]

         push NULL
         call [ExitProcess]

; Input the number of elements.
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size]
        ; Checking that the entered value is correct.
        ; The number must be between 0 and 100 (not including).
        cmp eax, 0
        jle failSize
        cmp eax, 100
        jge failSize

        cmp eax, 100
        jl getVectorA


;Warns about incorrect value,
failSize:
        push [vec_size]
        push strIncorSize
        call [printf]
        jmp finish


; Input array A.
getVectorA:

        xor ecx, ecx
        mov ebx, vec_a

getVectorLoop:
        mov [tmp], ebx
        ; Compare the number of item with the number of elements.
        ; If values are equal, stop the input of array A.
        cmp ecx, [vec_size]
        jge endInputVector

        ; Input of item.
        mov [i], ecx
        push ecx
        push strVecElemI
        call [printf]
        add esp, 8
        push ebx
        push strScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVectorLoop

; End of input items of array A.
endInputVector:
        ret

; Forming array B.
VectorChange:
        xor ecx, ecx
        mov ebx, vec_a
        mov edx, vec_b

;Loop for array B formation.
formNewVecByRuleLoop:

        cmp ecx, [vec_size]
        je endFormNewVec

        mov eax, [ebx]
        ; If the item in A is positive
        ; the corresponding item in B is 1.
        cmp eax, 0
        jg rule1

        ;If the item in A is negative
        ;the corresponding item in B is -1.
        cmp eax, 0
        jl rule2

        ;If the item in A is 0,
        ;the corresponding item in B is 0.
        cmp eax, 0
        je rule3


rule1:
            mov eax, 1
            mov [edx], eax
            inc ecx
            add ebx, 4
            add edx, 4
            jmp formNewVecByRuleLoop


rule2:
            mov eax, -1
            mov [edx], eax
            inc ecx
            add ebx, 4
            add edx, 4
            jmp formNewVecByRuleLoop

rule3:
           mov eax, 0
           mov [edx], eax
           inc ecx
           add ebx, 4
           add edx, 4
           jmp formNewVecByRuleLoop


;Exit from B formation loop.
endFormNewVec:
        ret


; Array A output.
VectorOutA:

        mov [tmpStack], esp
        xor ecx, ecx
        mov ebx, vec_a

; Loop for array output.
putVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push strVecElemOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop

; Exit from the loop of output.
endOutputVector:
        mov esp, [tmpStack]
        ret

; Array B output.
VectorOutB:

        mov [tmpStack], esp
        xor ecx, ecx
        mov ebx, vec_b

        call putVecLoop
        ret

section '.idata' import data readable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll',\
                user32, 'USER32.DLL'

;include 'api\user32.inc'
;include 'api\kernel32.inc'

        import kernel,\
               ExitProcess, 'ExitProcess'



        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'