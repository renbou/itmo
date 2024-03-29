# Архитектура ЭВМ

В ходе курса рассматриваются общие принципы работы современных процессоров ЭВМ используя "Базовую ЭВМ", простую гипотетическую ЭВМ, разработанную специально для курсов ИТМО, для рассмотрения примеров и использования её в домашних и лабораторных заданиях.

## Домашние задания

1. _[Выполнение арифметических операций с двоичными числами](./homeworks/hwk-1/README.md)_  
   Домашнее задание на отработку различных трудностей, возникающих при работе с числами в различных системах счисления и формах записи.
2. _[Программирование циклических алгоритмов](./homeworks/hwk-2/README.md)_  
   Домашнее задание на практику написания программ на ассемблере базовой ЭВМ с циклами.
3. _[Программирование обмена данными с внешними устройствами](./homeworks/hwk-3/README.md)_  
   Домашнее задание на исследование различных способов взаимодействия со внешними устройствами из ЭВМ, включая асинхронный и через прерывания программы.
4. _[Расширение системы команд ЭВМ](./homeworks/hwk-4/README.md)_
   Домашнее задание на создание собственных команд при помощи микрокоманд (аналог микрокода современных процессоров), поддерживаемых базовой ЭВМ.

## Лабораторные работы

1. _[Исследование работы ЭВМ при выполнении линейных программ](./labs/lab-1/README.md)_  
   Лабораторная работа на использование базовых команд ассемблера БЭВМ.
2. _[Исследование работы ЭВМ при выполнении разветвляющихся программ](./labs/lab-2/README.md)_  
   Лабораторная работа на использование команд ветвления ассемблера БЭВМ.
3. _[Исследование работы ЭВМ при выполнении циклических программ](./labs/lab-3/README.md)_  
   Лабораторная работа на построение циклов при помощи команд ветвления ассемблера БЭВМ.
4. _[Исследование работы ЭВМ при выполнении комплекса программ](./labs/lab-4/README.md)_  
   Лабораторная работа на использование команд вызова подпрограмм ассемблера БЭВМ.
5. _[Исследование работы ЭВМ при асинхронном обмене данными с ВУ](./labs/lab-5/README.md)_  
   Вводная лабораторная работа в работу со внешними устройствами из базовой ЭВМ.
6. _[Исследование работы ЭВМ при обмене данными с ВУ в режиме прерывания программы](./labs/lab-6/README.md)_  
   Лабораторная работа на исследование программы, полученной в ДЗ №3.
7. _[Исследование микропрограммного устройства управления](./labs/lab-7/README.md)_  
   Вводная лабораторная работа в написание собственных команд в БЭВМ.
8. _[Синтез команд базовой ЭВМ](./labs/lab-8/README.md)_  
   Лабораторная работа на исследование и тестирования собственных команд, написанных в ДЗ №4.

## Материалы

- _[Лабораторный практикум по Архитектуре ЭВМ](./materials/%D0%9B%D0%B0%D0%B1%D0%BE%D1%80%D0%B0%D1%82%D0%BE%D1%80%D0%BD%D1%8B%D0%B9%20%D0%BF%D1%80%D0%B0%D0%BA%D1%82%D0%B8%D0%BA%D1%83%D0%BC%20%D0%BF%D0%BE%20%D0%90%D1%80%D1%85%D0%B8%D1%82%D0%B5%D0%BA%D1%82%D1%83%D1%80%D0%B5%20%D0%AD%D0%92%D0%9C.pdf)_  
  Описание инструкций, ассемблера и структуры базовой ЭВМ, а также условия домашних заданий и лабораторных работ.
- _[Инструкции ассемблера БЭВМ](./materials/instructions.png)_  
  Список доступных в БЭВМ инструкций с описаниями.
- _[Эмулятор БЭВМ](./materials/bcomp.jar)_  
  JAR-файл, предоставляющий функционал эмуляции базовой ЭВМ.
- _[Расширенный эмулятор БЭВМ](./materials/bcomp-e.jar)_  
  JAR-файл, предоставляющий расширенный функционал эмуляции базовой ЭВМ. Репозиторий создателя: [github.com/AppLoidx/bcomp-extended](https://github.com/AppLoidx/bcomp-extended).
