# water_counter
Water counter based on Arduino and HC-05 bluetooth module

Счетчик воды на Arduino и bluetooth модуле HC-05

## Описание
За основу взят проект уважаемого ansealk (https://pikabu.ru/story/podklyuchaem_arduino_k_schetchikam_vodyi_4258720). 

**Убран** ЖК-дисплей с I2C-модулем, RTC модуль.

**Добавлен** HC-05 bluetooth модуль для удаленного снятия показаний (телефон, компьютер, роутер), модуль зарядки и переключения для автономного питания, команды управления.

##Файлы

*water_counter.ino* - скетч arduino 

*get_count.sh* - пример shell скрипта для получения данных о счетчиках в файл

