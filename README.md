# BigInteger
http://sorokin.github.io/cpp-year2013/task-2.html

<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <title>Задание №2</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
  
  </head>
  <body>
    <h2>Задание №2</h2>
    <p>В данном задании необходимо написать класс длинное знаковое число. Требования к решению следующие:</p>
    <p>Реализация должна содержаться в классе <span class="codein">big_integer</span>
    и находиться в файле <span class="codein">big_integer.h</span>. Класс должен содержать:</p>
    <ul>
      <li>Конструктор по умолчанию, инициализирующий число нулем.</li>
      <li>Конструктор копирования, после которого исходный объект и копию можно изменять независимо.</li>
      <li>Конструктор от <span class="codein">int</span>.</li>
      <li>Explicit конструктор от <span class="codein">std::string</span>.</li>
      <li>Оператор присваивания.</li>
      <li>Операторы сравнения.</li>
      <li>Арифметические операции: сложение, вычитание, изменение знака (унарный минус), унарный плюс.</li>
      <li>Умножение работающее за время O(nm), где n,m &mdash; длины множителей в битах.</li>
      <li>Деление и остаток от деления работающие за время O(nm), где n &mdash; длина делимого в битах, а m &mdash; длина делителя в битах.</li>
      <li>Префиксный/постфиксный инкремент/декремент. (опционально)</li>
      <li>Битовые операции: и, или, исключающее или, не.</li>
      <li>Битовые сдвиги.</li>
    </ul>
    <p>Должна существовать глобальная функция <span class="codein">std::string to_string(big_integer const&amp;)</span> возвращающая
    строковое представление числа.</p>
<ul>
    <p>Реализация функций класса должна быть расположена в файле <span class="codein">big_integer.cpp</span>.</p>
    <p>Пользоваться сторонними библиотеками длинных чисел при выполнении этого задания нельзя.</p>

    <p>По ссылке <a href="https://github.com/sorokin/cpp-year2013/tree/master/bigint">https://github.com/sorokin/cpp-year2013/tree/master/bigint</a>
    можно найти:</p>
    <ul>
      <li><span class="codein"><a href="https://github.com/sorokin/cpp-year2013/blob/master/bigint/big_integer_testing.cpp">big_integer_testing.cpp</a></span> &mdash; набор модульных тестов для проверки корректности вашей программы. Прохождение этого набора тестов является необходимым,
      но не достаточным условием сдачи данного задания. При необходимости, этот набор тестов будет расширяться и дополняться. Тесты реализованы с помощью библиотеки gtest (Google Test).</li>
      <li><span class="codein"><a href="https://github.com/sorokin/cpp-year2013/tree/master/bigint/gtest">gtest/</a></span> &mdash; библиотека <a href="https://code.google.com/p/googletest/">gtest</a>.</li>
      <li><span class="codein"><a href="https://github.com/sorokin/cpp-year2013/blob/master/bigint/big_integer.h">big_integer.</a>{<a href="https://github.com/sorokin/cpp-year2013/blob/master/bigint/big_integer.h">h</a>, <a href="https://github.com/sorokin/cpp-year2013/blob/master/bigint/big_integer.cpp">cpp</a>}</span> &mdash; реализация класса длинного числа с использованием библиотеки <a href="https://gmplib.org/">GNU Multi-Precision</a>, которая приходит приведенные тесты.</li>
    

    <p>Перед тем как писать код, возможно, стоит ознакомиться с книгой <a href="http://www.loria.fr/~zimmerma/mca/mca-0.5.pdf">Richard Brent, Paul Zimmermann &mdash; Modern Computer Arithmetic</a>.</p>

    </ul>
   
  </body>
</html>


