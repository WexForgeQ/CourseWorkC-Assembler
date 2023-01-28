#include"Error.h"
namespace Error {
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101), ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(105), ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY_NODEF(113), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115),
		ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200, "[ LEXICAL ] Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(201, "[ LEXICAL ] Превышен размер таблицы лексем"),
		ERROR_ENTRY(202, "[ LEXICAL ] Переполнение таблицы лексем"),
		ERROR_ENTRY(203, "[ LEXICAL ] Превышен размер таблицы идентификаторов"),
		ERROR_ENTRY(204, "[ LEXICAL ] Переполнение таблицы идентификаторов"),
		ERROR_ENTRY(205, "[ LEXICAL ] Неизвестная последовательность символов"),
		ERROR_ENTRY_NODEF(206), ERROR_ENTRY_NODEF(207), ERROR_ENTRY_NODEF(208), ERROR_ENTRY_NODEF(209),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250),
		ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY(300, "[ SEMANTIC ] Имеется не закрытый строковый"),									// +	
		ERROR_ENTRY(301, "[ SEMANTIC ] Имеется более одной точки входа в main"),						// +	
		ERROR_ENTRY(302, "[ SEMANTIC ] Не имеется точки входа в main"),									// +	
		ERROR_ENTRY(303, "[ SEMANTIC ] Ошибка в объявлении типа переменной"),							// +	
		ERROR_ENTRY(304, "[ SEMANTIC ] Объявление переменной без ключевого слова var"),					// +	
		ERROR_ENTRY(305, "[ SEMANTIC ] Повторное объявление идентификатора"),									// +	
		ERROR_ENTRY(306, "[ SEMANTIC ] Объвление переменной без указания типа"),						// +											
		ERROR_ENTRY(307, "[ SEMANTIC ] Попытка реализовать существующую функцию"),						// +											
		ERROR_ENTRY(308, "[ SEMANTIC ] Объявление функции без указания типа"),							// +										
		ERROR_ENTRY(309, "[ SEMANTIC ] Несовпадение типов передаваемых параметров функции"),			// +					
		ERROR_ENTRY(310, "[ SEMANTIC ] Несоответствие арифметических операторов(их последовательность/типы операндов"),						// +											
		ERROR_ENTRY(311, "[ SEMANTIC ] Невозможно деление на ноль"),									// +								
		ERROR_ENTRY(312, "[ SEMANTIC ] Несоответсвие типов данных"),									// +							
		ERROR_ENTRY(313, "[ SEMANTIC ] Повторное объявление функции"),			// -								
		ERROR_ENTRY(314, "[ SEMANTIC ] Функция возвращает неверный тип данных"),						// +								
		ERROR_ENTRY(315, "[ SEMANTIC ] Превыщено допустимое значение типа snum"),						// +
		ERROR_ENTRY(316, "[ SEMANTIC ] Превышена длина символьного литерала"),							// +
		ERROR_ENTRY(317, "[ SEMANTIC ] Несовпадение количества передаваемых параметров функции"),		//+
		ERROR_ENTRY(600, "[SYN]# Неверная структура программы"),
		ERROR_ENTRY(601, "[SYN]# Не найден список параметров функции"),
		ERROR_ENTRY(602, "[SYN]# Ошибка в теле функции"),
		ERROR_ENTRY(603, "[SYN]# Ошибка в теле процедуры"),
		ERROR_ENTRY(604, "[SYN]# Ошибка в списке параметров функции"),
		ERROR_ENTRY(605, "[SYN]# Ошибка в вызове функции/выражении"),
		ERROR_ENTRY(606, "[SYN]# Ошибка в списке фактических параметров функции"),
		ERROR_ENTRY(607, "[SYN]# Ошибка при констуировании цикла/условного выражения"),
		ERROR_ENTRY(608, "[SYN]# Ошибка в теле цикла/условного выражения"),
		ERROR_ENTRY(609, "[SYN]# Ошибка в условии цикла/условного выражения"),
		ERROR_ENTRY(610, "[SYN]# Неверный условный оператор"),
		ERROR_ENTRY(611, "[SYN]# Неверный арифметический оператор"),
		ERROR_ENTRY(612, "[SYN]# Неверное выражение. Ожидаются только идентификаторы/литералы"),
		ERROR_ENTRY(613, "[SYN]# Ошибка в арифметическом выражении"),
		ERROR_ENTRY(614, "[SYN]# Недопустимая синтаксическая конструкция"),
		ERROR_ENTRY(615, "[SYN]# Недопустимая синтаксическая конструкция в теле цикла/условного выражения"),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
	ERROR geterror(int id) {
		if (id > 0 && id < ERROR_MAX_ENTRY) {
			return errors[id];
		}
		else {
			return errors[0];
		}
	}
	ERROR geterrorin(int id, int line = -1, int col = -1) {
		if (id > 0 && id < ERROR_MAX_ENTRY) {
			errors[id].inext.col = col;
			errors[id].inext.line = line;
			return errors[id];
		}
		else {
			return errors[0];
		}
	}
}