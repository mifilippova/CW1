; Вывод значений.
macro Print [args] {
  reverse
    push args
  common
    call [printf]
}
; Ввод значений.
macro Scan [args] {
  reverse
    push args
  common
    call [scanf]
}

; Ввод дроби.
macro GetFraction num, den {
 local getFraction, outOfRange,endGetFraction
 getFraction:
      ; Ввод числителя.
      Print strInputNumerator
      Scan strScanInt, num

      ; Проверка чилителя на корректность.
      cmp [num], 1000000
      jg outOfRange

      cmp [num], -1000000
      jl outOfRange

      ; Ввод знаменателя.
      Print strInputDenominator
      Scan strScanInt, den

      ; Проверка знаменателя на корректность.
      cmp [den], 1000000
      jg outOfRange

      cmp [den], -1000000
      jl outOfRange

        ; Проверка на неравенство знаменателя 0.
        cmp [den], 0
        jne endGetFraction

        Print strIncorrectVal
        call [getch]
        push 0
        call [ExitProcess]

outOfRange:
        Print strOutOfRange
        call [getch]
        push 0
        call [ExitProcess]

 endGetFraction:

}

; Ввод одного комплексного числа.
macro CNInputMacro strInput, n_re_num, n_re_den, n_im_num, n_im_den {
      Print strInput
      Print strInputRe
      ; Ввод действительной части.
      GetFraction n_re_num, n_re_den

      Print strInputIm
      ; Ввод мнимой части.
      GetFraction n_im_num, n_im_den
}

;Сокращение дроби.
macro ReduceFraction first, second{
local startNOD,negB, negA, negBB, @greaterB, NODLoop, negDivA, negDivB, negDivBB, reduceFraction, redIfZeroNum, endNOD
startNOD:
        ; НОД - это целое натуральное число,
        ; поэтому проверяем числа на отрицательность и на равенство 0,
        ; чтобы искать НОД для 2-х положительных чисел.
        cmp [first],0
        je redIfZeroNum

        mov eax, [first]
        mov ebx, [second]
        cmp [first], 0
        jl negA
           cmp [second], 0
           jl negB
             jmp NODLoop
           negB:
              neg ebx
              jmp NODLoop
        negA:
          neg eax
          cmp [second], 0
          jl negBB
              jmp NODLoop
           negBB:
              neg ebx
              jmp NODLoop

; Алгоритм Евклида.
NODLoop:
        cmp eax, ebx
        je reduceFraction

        cmp eax, ebx
        jl @greaterB
           sub eax, ebx
           jmp NODLoop
        @greaterB:
           sub ebx, eax

        jmp NODLoop

; Сокращение числителя и знаменателя на их НОД.
reduceFraction:
        mov [d], eax
        Print strEmpty

        ;поделить на d оба числа
        ;положительное число делится адекватно,
        ;отрицательное нужно сделать положительным, а затем
        ;поменяь знакрезультата.
        cmp [first], 0
        jl negDivA
           mov eax, [first]
           div [d]
           mov [first], eax

           cmp [second], 0
           jl negDivB
              mov eax, [second]
              div [d]
              mov [second], eax
              jmp endNOD
           negDivB:
              mov eax, [second]
              neg eax
              div [d]
              neg eax
              mov [second], eax
              jmp endNOD
        negDivA:
           mov eax, [first]
           neg eax
           div [d]
           neg eax
           mov [first], eax

           cmp [second], 0
           jl negDivBB
              mov eax, [second]
              div [d]
              mov [second], eax
              jmp endNOD
           negDivBB:
              mov eax, [second]
              neg eax
              div [d]
              neg eax
              mov [second], eax
              jmp endNOD
; В случае равенства числителя дроби 0,
; присвоим знаменателю значение 0.
redIfZeroNum:
         mov [second], 1

endNOD:

}


; Сложение дробей.
macro AddFractions a, b, c, d, num, den{
     ; Расчет числителя.
     mov eax,[a]
     imul eax, [d]

     mov ebx, [b]
     imul ebx, [c]

     add eax, ebx
     mov [num], eax

     ; Расчет знаменателя.
     mov eax, [b]
     imul eax, [d]
     mov [den], eax
}

; Сложение комплексных чисел.
macro CNAddMacro{
     ; Сложение действительных частей.
     AddFractions a_re_num, a_re_den, b_re_num, b_re_den, c_re_num, c_re_den

     ReduceFraction c_re_num, c_re_den

     ; Сложение мнимых частей.
     AddFractions a_im_num, a_im_den, b_im_num, b_im_den, c_im_num, c_im_den

     ReduceFraction c_im_num, c_im_den

}
; Отрицание числа.
macro negNum num{
     mov eax, [num]
     neg eax
     mov [num], eax
}

; Вычитание комплексных чисел.
macro CNSubMacro{
     ; - это тот же плюс, только с отрицанием b_re_num и b_im_num
     negNum b_re_num
     negNum b_im_num

     CNAddMacro

     negNum b_re_num
     negNum b_im_num
}

; Присвоить secondValue значение firstValue.
macro changeValues firstValue, secondValue{
      mov eax, [firstValue]
      mov [secondValue], eax
}

;Присваиваем одному кумплексному числу значение другого.
macro modify s_re_num, s_re_den, s_im_num, s_im_den, d_re_num, d_re_den, d_im_num, d_im_den {
    changeValues s_re_num, d_re_num
    changeValues s_re_den, d_re_den
    changeValues s_im_num, d_im_num
    changeValues s_im_den, d_im_den
}

; Умножение комплексных чисел.
macro CNMulMacro{
    ; Сохраним значения чисел a, b в x, y, чтобы потом можно было их восстановить
    ; В силу реализации сложения, мы можем оперировать только a и b
    ; Можно было, конечно, добавить параметры в сложение,
    ; но показалось неразумным нагромождать операции сложения и вычитания.

    ; Сохраняем a в x, b в y.
    modify a_re_num, a_re_den, a_im_num, a_im_den, x_re_num, x_re_den, x_im_num, x_im_den
    modify b_re_num, b_re_den, b_im_num, b_im_den, y_re_num, y_re_den, y_im_num, y_im_den

    ; Дествительная часть новых дробей.
    mov eax, [a_re_num]
    imul eax, [b_re_num]
    mov [a_re_num], eax

    mov eax, [a_re_den]
    imul eax, [b_re_den]
    mov [a_re_den], eax

    negNum a_im_num
    mov eax, [a_im_num]
    imul eax, [b_im_num]
    mov [b_re_num], eax
    negNum a_im_num

    mov eax, [a_im_den]
    imul eax, [b_im_den]
    mov [b_re_den], eax


    ; Мнимая часть новых дробей.
    mov eax, [x_re_num]
    imul eax, [y_im_num]
    mov [a_im_num], eax

    mov eax, [x_re_den]
    imul eax, [y_im_den]
    mov [a_im_den], eax

    mov eax, [x_im_num]
    imul eax, [y_re_num]
    mov [b_im_num], eax

    mov eax, [x_im_den]
    imul eax, [y_re_den]
    mov [b_im_den], eax

    CNAddMacro

   ; Возвращение первоначальных значений а и b.
   modify x_re_num, x_re_den, x_im_num, x_im_den, a_re_num, a_re_den, a_im_num, a_im_den
   modify y_re_num, y_re_den, y_im_num, y_im_den, b_re_num, b_re_den, b_im_num, b_im_den
}

; Деление комплексных чисел.
macro CNDivMacro{
      ; Числитель - умножение a и сопряженного к b.
      negNum b_im_num
      CNMulMacro
      negNum b_im_num

      ; Знаменатель сумма квадратов действительной и мнимой части b.
      ; Проверяем, не равен ли b 0, в таком случае, деление на 0 невозможно.
      cmp [b_re_num], 0
      je divByZero

      cmp [b_im_num], 0
      je divByZero

      mov eax, [b_re_num]
      imul eax, [b_re_num]
      mov [x_re_num], eax

      mov eax, [b_re_den]
      imul eax, [b_re_den]
      mov [x_re_den], eax

      mov eax, [b_im_num]
      imul eax, [b_im_num]
      mov [y_re_num], eax

      mov eax, [b_im_den]
      imul eax, [b_im_den]
      mov [y_re_den], eax

      AddFractions x_re_num, x_re_den, y_re_num, y_re_den, x_im_num, x_im_den

      ; Делим чиситель на знаменатель
      mov eax, [c_re_num]
      imul eax, [x_im_den]
      mov [c_re_num], eax

      mov eax, [c_re_den]
      imul eax, [x_im_num]
      mov [c_re_den], eax

      mov eax, [c_im_num]
      imul eax, [x_im_den]
      mov [c_im_num], eax

      mov eax, [c_im_den]
      imul eax, [x_im_num]
      mov [c_im_den], eax

      ; Сокращаем дроби результатов.
      ReduceFraction c_re_num, c_re_den
      ReduceFraction c_im_num, c_im_den
      jmp endDiv

divByZero:
        Print strIncorrectVal
        call [getch]
        push 0
        call [ExitProcess]

endDiv:
}

; Форматирование дроби по правилу sqn(num * den)*|num| / |den|
macro checkFraction num, den{
local startCheckFract,@denLess, endCheckFraction
startCheckFract:
             cmp [den], 0
             jl @denLess
                jmp endCheckFraction
            @denLess:
               mov eax, [num]
               neg eax
               mov [num], eax

               mov eax, [den]
               neg eax
               mov [den], eax
               jmp endCheckFraction


endCheckFraction:

}
; Форматирование дробей комплексного числа.
macro formatFractions re_num, re_den, im_num, im_den{
      ; Приводим дроби к виду sqn(num, den)*|num| / |den|
      checkFraction re_num, re_den
      checkFraction im_num, im_den
}

; Вывод комплексного числа.
macro outputCN re_num, re_den, im_num, im_den{

local startOutputCN, negIm, endOutputCN

startOutputCN:
       ; Выбор строки формата, в зависимость от знака мнимой части.
       cmp [im_num], 0
       jl negIm
           Print strCNPosIm, [re_num], [re_den], [im_num], [im_den]
           jmp endOutputCN
       negIm:
           mov eax, [im_num]
           neg eax
           mov [im_num], eax
           Print strCNNegIm, [re_num], [re_den], [im_num], [im_den]
           jmp endOutputCN

endOutputCN:
}

; Вывод результата в формате А op B = C, где A, B, C - комплексные числа,
; а op - введенный оператор.
macro OutputResult{
startOutput:
     ; Вывод первого введенного числа.
     outputCN a_re_num, a_re_den, a_im_num, a_im_den

     ; Выбор строкового представления оператора в зависимости от кода.
     ; 43 = '+', 45 = '-', 42 = '*', 47 = '/'
     cmp [op], 43
     jne notAddOp
         Print strOpAdd
         jmp endOutput
     notAddOp:
        cmp [op], 45
        jne notSubOp
            Print strOpSub
            jmp endOutput
        notSubOp:
            cmp [op], 42
            jne notMultOp
            Print strOpMul
            jmp endOutput
            notMultOp:
                Print strOpDiv
endOutput:
    ; Вывод второго введенного числа.
    outputCN b_re_num, b_re_den, b_im_num, b_im_den
    Print strOpEq
    ; Вывод результата.
    outputCN c_re_num, c_re_den, c_im_num, c_im_den
}