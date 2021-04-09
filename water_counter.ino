#include <Bounce2.h>                  // Для обработки замыкания линий со счетчиков
#include <EEPROMex.h>                 // Для работы с EEPROM, расширенная библиотека

//////////////////////////////////////////////////////////////////////////////////////////////
// Модули, включенные в состав блока

#define DEBUG            0   // Выдача отладочной информации в COM-порт


//////////////////////////////////////////////////////////////////////////////////////////////
// Пины для подключения устройств
#define HOT_COUNTER_PIN  4    //Пин счетчика горячей воды
#define COLD_COUNTER_PIN 5    //Пин счетчика холодной воды


//////////////////////////////////////////////////////////////////////////////////////////////
// Конфигурация счетчиков 
#define COUNTERS         2                          	//Количество счетчиков
#define COUNTER_DELAY    1000                       	//Задержка в 1 сек, пусть будет. Мы уверены, 
							//что два раза в секунду счетчик не может сработать 
							//ни при каких  обстоятельствах
unsigned long CounterHighBase[COUNTERS] = {0,0};   	// Если значение отлично от нуля - то пишем его в качестве базового     
unsigned int CounterLowBase[COUNTERS]  = {0,0};     	// Если значение отлично от нуля - то пишем его в качестве базового
int counterReadDelay  = 0;                          	// Текущая задержка считывания счетчика (нужна для уверенной отработки переключения счетчика)

int CounterPin[COUNTERS]         = {COLD_COUNTER_PIN, HOT_COUNTER_PIN};  // Пины 
int CounterHighAddress[COUNTERS] = {0x40, 0x48};     	//Адреса EEPROM для старшего слова (кубометры) - 4 байта для long, 2 байта для int
int CounterLowAddress[COUNTERS]  = {0x44, 0x4C};     	//Адреса EEPROM для младшего слова (литры)
char *CounterName[COUNTERS]      = {"Hot: ", "Cold:"};   // Названия счетчиков для вывода на экран 
Bounce CounterBouncer[COUNTERS]  = {};               	// Формируем для счетчиков Bounce объекты

boolean setFlag = 0;					//Флаг о том, что будут вводится данные счетчиков
const int MaxCountBuff = 32;				// Максимальное количество принимаемых байт
char InputDataBuffer[MaxCountBuff];			// Массив для принимаемых байт


void setup()
{
  Serial.begin(57600);
//  analogReference(DEFAULT) ;
  countersInit();                                   	// Инициализация начальных показаний счетчиков                 

  #if DEBUG 
     Serial.println( "Debug is ON...");
     echoCounters();
  #endif

} 

//////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  counterReadDelay++;
  readCounter();                                    	// Читаем и обрабатываем значения счетчиков
  if (Serial.available() ) { 			 	// Проверяем, а есть ли данные
    int InputCount = Serial.readBytesUntil(0x0D,(char*)&InputDataBuffer[0],MaxCountBuff); // Складываем байты в массив и заодно подсчитываем количество
    ParseInputData(InputDataBuffer, InputCount - 1);					// полученные данные вместе с количеством байт передаем на обработку
    memset(InputDataBuffer, 0, sizeof(InputDataBuffer));				// Очищаем массив
   }
} 

//////////////////////////////////////////////////////////////////////////////////////////////
void countersInit()
{
  // инит пинов счетчиков
  for (int i=0; i<COUNTERS; i++)
  {
    // Если значение отлично от нуля - пишем его в EEPROM
    if (CounterLowBase[i])
      EEPROM.writeInt(CounterLowAddress[i], CounterLowBase[i]);
    if (CounterHighBase[i])
      EEPROM.writeLong(CounterHighAddress[i], CounterHighBase[i]);
    
      
    pinMode(CounterPin[i], INPUT);                              // Инициализируем пин
    digitalWrite(CounterPin[i], HIGH);                          // Включаем подтягивающий резистор
    CounterBouncer[i].attach(CounterPin[i]);                    // Настраиваем Bouncer
    CounterBouncer[i].interval(10);                             // и прописываем ему интервал дребезга    
    CounterHighBase[i] = EEPROM.readLong(CounterHighAddress[i]);// Читаем начальные значения из EEPROM
    CounterLowBase[i]  = EEPROM.readInt(CounterLowAddress[i]);  // Читаем начальные значения из EEPROM
    #if DEBUG
      Serial.print("Read form EEPROM "); Serial.print(i,DEC ); Serial.print(" counter. Name "); Serial.println(CounterName[i]); 
      Serial.print(CounterHighAddress[i] ,HEX); Serial.print(" => "); Serial.println(CounterHighBase[i]);
      Serial.print(CounterLowAddress[i]  ,HEX); Serial.print(" => "); Serial.println(CounterLowBase[i] );
    #endif
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Обрабатываются счетчики
void readCounter()
{
  if (counterReadDelay >= COUNTER_DELAY) // Если подошло время обработки
  {
    counterReadDelay = 0;
    for (int i=0; i<COUNTERS; i++) 
    {
      boolean changed = CounterBouncer[i].update();
      if ( changed ) 
      {
        int value = CounterBouncer[i].read();
        if ( value == LOW) // Если значение датчика стало ЗАМКНУТО, т.е сработал счетчик
        {
          #if DEBUG
            Serial.print("Counter [ "); Serial.print(CounterName[i]);
          #endif
          if (CounterLowBase[i]<999) // если не произошло перехода на кубометры - увеличиваем счетчик литров на 1
          {
            CounterLowBase[i]+=1;  
            #if DEBUG
              Serial.print(CounterLowBase[i]); Serial.println("]");
            #endif            
//            EEPROM.writeInt (CounterLowAddress[i],CounterLowBase[i]); // писать каждый литр в память не будем, экономим EEPROM
          }
          else  // иначе, если произошел переход - обнуляем счетчик литров и увеличиваем счетчик кубометров на 1
          {
            CounterLowBase[i] = 0;
            CounterHighBase[i]++;
            
//            writeCounter(i); // записываем показатели в память  (убираем запись в память - теперь будет команда SAVE)
          }  
        }
      }
    }
  }
  else //Если время обработки еще не истекло
  {    
    counterReadDelay++;
  } 
}

void writeCounter(byte nCounter) {
    EEPROM.writeInt( CounterLowAddress[nCounter],  CounterLowBase[nCounter]);
    EEPROM.writeLong( CounterHighAddress[nCounter], CounterHighBase[nCounter]);
}

void echoCounters() {

char buf[6];

   Serial.println("Current counters state:");
    for (int i=0; i<COUNTERS; i++)                  // Выводим на экран значения счетчиков   
    {
        Serial.print(CounterName[i]);
	sprintf(buf, "%05lu", CounterHighBase[i]);
        Serial.print(buf);
        Serial.print(",");
	sprintf(buf, "%03d", CounterLowBase[i]);
        Serial.println(buf);
    }
}


void ParseInputData(char InputData[], int Count) {

char * istr;
byte num = 0;
char * strNum[2];
unsigned long  Number[2];
String strError = "Input data error!";  

if (!setFlag) { 								// Если не установлен флаг, принимаем команды
   if (String(InputData) == "READ") {						// Введена команда READ  - считываем показания
      echoCounters();
      //echoVoltage();								
   } else if (String(InputData) == "SAVE") {                                    // Введена команда SAVE  - записываем показания в EEPROM
      writeCounter(0);
      writeCounter(1);
      Serial.println("OK");                                                     // Конец данных
   } else if (String(InputData) == "SET") {					// Флаг для приема данных установки значений счетчиков 
      setFlag = 1;
   } else {
      Serial.print("Command not found: ");						// в противном случае сообщаем об ошибке
      Serial.println(InputData);
      }
} else {

if (String(InputData).startsWith("H:")) {
	    istr = strtok (InputData,",");
	    while (istr != NULL)						// Находим и разделяем строки до и после ,
	    {	    
	    strNum[num] = istr;
	    num++;
	    istr = strtok (NULL,","); 						// Выделение очередной части строки
	    }
	    if (num == 2) {							// Проверяем, что получилось 2 числа
	    Number[0] = String(strNum[0]).substring(2).toInt();			// Отсекаем H:    
	    Number[1] = String(strNum[1]).toInt();
#if DEBUG
	    Serial.println (Number[0]);
	    Serial.println (Number[1]);	    
	    Serial.print("Set "); Serial.print(CounterName[0]); Serial.println(" counter");
#endif
	    CounterHighBase[0] = Number[0];
	    CounterLowBase[0]  = Number[1];
            writeCounter(0);
	    Serial.println("OK");	    
	    } else {Serial.println(strError);}
}

if (String(InputData).startsWith("C:")) {
	    istr = strtok (InputData,",");
	    while (istr != NULL)						// Находим и разделяем строки до и после ,
	    {
	    strNum[num] = istr;
	    num++;
	    istr = strtok (NULL,","); 						// Выделение очередной части строки
	    }
	    if (num == 2) {							// Проверяем, что получилось 2 числа
	    Number[0] = String(strNum[0]).substring(2).toInt();			// Отсекаем C:    
	    Number[1] = String(strNum[1]).toInt();
#if DEBUG
	    Serial.println (Number[0]);
	    Serial.println (Number[1]);	    
	    Serial.print("Set "); Serial.print(CounterName[1]); Serial.println(" counter");
#endif
	    CounterHighBase[1] = Number[0];
	    CounterLowBase[1]  = Number[1];
            writeCounter(1);
	    Serial.println("OK");	    
	    } else {Serial.println(strError);}
}
   if (num != 2) {Serial.println(strError);}
   setFlag = 0;      		 
}
}
