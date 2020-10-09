; Вариант 4.
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
; Ввод массива А.
        call VectorInput

; Формирование массива В по правилу: 1 - положительные, 0 - 0, -1 - отрицательные.
        call VectorChange

; Вывод массива А.
        push strVectA
        call [printf]
        call VectorOutA

; Вывод массива В.
        push strVectB
        call [printf]
        call VectorOutB

finish:
         call [getch]

         push NULL
         call [ExitProcess]

; Ввод числа элементов.
VectorInput:
        push strVecSize
        call [printf]
        add esp, 4

        push vec_size
        push strScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size]
        ; Проверяем, корректно ли введеное значение.
        ; Число должно принадлежать от 1 до 100.
        cmp eax, 0
        jle failSize
        cmp eax, 100
        jge failSize

        cmp eax, 100
        jl getVectorA


; Оповещает пользователя о некооректном значении введенного числа.
failSize:
        push [vec_size]
        push strIncorSize
        call [printf]
        jmp finish


; Ввод массива А.
getVectorA:

        xor ecx, ecx
        mov ebx, vec_a

getVectorLoop:
        mov [tmp], ebx
        ; Сравниваем порядок текущего элемента, с числом элементов.
        ; Есди величины равны, то заканчиваем ввод.
        cmp ecx, [vec_size]
        jge endInputVector

        ; Ввод элемента.
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

; Окончание ввода массива А.
endInputVector:
        ret

; Формирование массива В.
VectorChange:
        xor ecx, ecx
        mov ebx, vec_a
        mov edx, vec_b

; Цикл для формирвоания массива В.
formNewVecByRuleLoop:

        cmp ecx, [vec_size]
        je endFormNewVec

        mov eax, [ebx]
        ; Если элемент массива А положителен,
        ; то соответственный элемент массива В равен 1.
        cmp eax, 0
        jg rule1

        ;Если элемент иассива А отрицателен,
        ;то соответствующий элемент массива B равен -1.
        cmp eax, 0
        jl rule2

        ;Если элемент массива A равен 0,
        ;то соответствующий элемент массива В равен 0.
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


; Выход из цикла формироания массива В.
endFormNewVec:
        ret


; Вывод массива А.
VectorOutA:

        mov [tmpStack], esp
        xor ecx, ecx
        mov ebx, vec_a

; Цикл для вывода массива.
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

; Выход из цикла вывода массива.
endOutputVector:
        mov esp, [tmpStack]
        ret

; Вывод массива В.
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