; Выполнила: Филиппова Мария, БПИ-196.
; Задание.
; Разработать программу реализации 4-х действий
; арифметики комплексных чисел, представленных дробями
; (использовать целые со знаком, обеспечить максимальную
; простоту результата).
format PE console
entry start

include 'win32a.inc'
include 'Microproject.inc'

;--------------------------------------------------------------------------
section '.data' data readable writable
        ; подсказки пользователю при вводе данных
        strInputA db 'Input complex number A', 10, 0
        strInputB db 'Input complex number B', 10, 0
        strInputRe db 'Input real part of the complex number',10, 0
        strInputIm db 'Input imaginary part of the complex number',10, 0
        strInputNumerator db 'Input numerator from [-1000000, 1000000]',10, 0
        strInputDenominator db 'Input denominator from [-1000000, 1000000]', 10, 0
        strIncorrectVal db 'Attempt to divide by zero.',10, 0
        strIncorrectOp db 'Undefined operation.', 10, 0
        strOutOfRange db 'The value is out of allowed range.', 10, 0
        strScanInt   db '%d', 0
        strEmpty db ' ', 0
        strOp db 'Input operation', 10, 0

        d dd ? ; НОД
        ; Зранение числителей, знаменателей дробей,
        ; входящих в состав комплексных чисел.
        ; А:
        a_re_num dd ?
        a_re_den dd ?
        a_im_num dd ?
        a_im_den dd ?
        ; В :
        b_re_num dd ?
        b_re_den dd ?
        b_im_num dd ?
        b_im_den dd ?
        ; C, здесь же хранится результат операций:
        c_re_num dd ?
        c_re_den dd ?
        c_im_num dd ?
        c_im_den dd ?
        ;
        ; Резервирование места для хранения кода операциию
        op rd 1
        ; Строки оформления результата.
        strRes db 'Result: ', 10, 0
        strOpAdd db ' + ', 0
        strOpSub db ' - ', 0
        strOpMul db ' * ', 0
        strOpDiv db ' / ', 0
        strOpEq db ' = ', 0
        strCNPosIm db '(%d/%d + %d/%d * i)', 0
        strCNNegIm db '(%d/%d - %d/%d * i)', 0

        ; Промежуточные значения при операциях умножения/деления.
        x_re_num dd ?
        x_re_den dd ?
        x_im_num dd ?
        x_im_den dd ?
        y_re_num dd ?
        y_re_den dd ?
        y_im_num dd ?
        y_im_den dd ?

;--------------------------------------------------------------------------
section '.code' code readable executable

start:
  ;Ввод комплексных чисел А и В.
  CNInputMacro strInputA, a_re_num, a_re_den, a_im_num, a_im_den
  CNInputMacro strInputB, b_re_num, b_re_den, b_im_num, b_im_den

  ;Ввод действия.
  Print strOp

  call [getch]
  mov [op], eax
  ; 43 = '+', 45 = '-', 42 = '*', 47 = '/'
  ; Выполнить действие в зависимости от введенного действия.
  cmp [op], 43 ; +
  jne @notAdd
      CNAddMacro
      jmp printResult
 @notAdd:
      cmp [op], 45 ;-
      jne @notSub
          CNSubMacro
          jmp printResult
 @notSub:
      cmp [op], 42 ; *
      jne @notMul
          CNMulMacro
          jmp printResult
 @notMul:
      cmp [op], 47 ; /
      jne @notDiv
          CNDivMacro
          jmp printResult
 @notDiv:
     ; В случае нкорректно введенного оператора, выводится ссобщение об ошибке.
     Print strIncorrectOp
     jmp finish

; Вывод результата.
printResult:

     ; проверить, не появился ли 0 в дробях знаменателя результата
     cmp [c_re_den], 0
     je DivByZero

     cmp [c_im_den], 0
     je DivByZero

     ; Приводим дроби к формату, где знаменатель положителен, а
     ; знак числителя отображает знак всей дроби.
     formatFractions a_re_num, a_re_den, a_im_num, a_im_den
     formatFractions b_re_num, b_re_den, b_im_num, b_im_den
     formatFractions c_re_num, c_re_den, c_im_num, c_im_den

     ; Вывод результата.
     Print strRes
     OutputResult
     jmp finish

; Уведомление о попытке деления на 0.
DivByZero:
     Print strIncorrectVal

finish:
        call [getch]
        push 0
        call [ExitProcess]

;-------------------------------------------------------------------------
section '.idata' import data readable
        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
           ExitProcess, 'ExitProcess'

        import msvcrt,\
           printf, 'printf',\
           scanf, 'scanf',\
           getch, '_getch'

