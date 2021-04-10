# water_counter
Water counter based on Arduino and HC-05 bluetooth module

Счетчик воды на Arduino и bluetooth модуле HC-05

## Описание
За основу взят [проект](https://pikabu.ru/story/podklyuchaem_arduino_k_schetchikam_vodyi_4258720) уважаемого **ansealk**.

- **Убран** ЖК-дисплей с I2C-модулем, RTC модуль.
- **Добавлен** HC-05 bluetooth модуль для удаленного снятия показаний (телефон, компьютер, роутер), модуль зарядки и переключения для автономного питания, команды управления.

## Схема

- [ ] Она здесь обязательно будет.

## Команды терминала

- ***READ*** - Чтение текущих показаний.
- ***SET*** - Установка показаний
    - ***H:*** - счетчика горячей воды (*SET H:00000,000*)
    - ***C:*** - счетчика холодной воды (*SET C:00000,000*)
- ***SAVE*** - Сохранение текущих показаний в EEPROM (например, для отключения устройства при длительном отсутствии).

Я использую [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=ru&gl=US) для чтения и изменения показаний счетчиков.

![](images/screenshots/read0.jpg) ![](images/screenshots/save0.jpg) ![](images/screenshots/set0.jpg)

В программе можно назначить макросы для быстрого доступа к командам.  
![](images/screenshots/read.jpg) ![](images/screenshots/set.jpg) ![](images/screenshots/hot.jpg) ![](images/screenshots/cold.jpg) ![](images/screenshots/save.jpg)  


## Файлы

[*water_counter.ino*](https://github.com/nmare99/water_counter/blob/main/water_counter.ino) - скетч arduino 

[*get_count.sh*](https://github.com/nmare99/water_counter/blob/main/get_count.sh) - пример shell скрипта для получения данных о счетчиках в файл

