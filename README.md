# water_counter
Water counter based on Arduino and HC-05 bluetooth module

За основу взят проект уважаемого ansealk (https://pikabu.ru/story/podklyuchaem_arduino_k_schetchikam_vodyi_4258720). 
Убран ЖК-дисплей с I2C-модулем, RTC модуль.
Добавлен HC-05 bluetooth модуль для удаленного снятия показаний (телефон, компьютер, роутер).

Файлы:
water_counter.ino - arduino скетч
get_count.sh - пример shell скрипта для получения данных о счетчиках в файл data.txt
