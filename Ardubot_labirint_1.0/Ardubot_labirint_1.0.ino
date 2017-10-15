// подключаем библиотеку Servo для работы с сервоприводом
#include <Servo.h>

// пины input на плате драйвера
#define IN_1    7
#define IN_2    3
#define IN_3    4
#define IN_4    2

// регулировка скорости ШИМ
#define EN_A    6
#define EN_B    5

#define TRIG_PIN  8
#define ECHO_PIN  9

/* объявляем переменную минимальной
   дистанции до препятствия */
long minDist = 18;

int delayStopPeriod = 180;

// объявляем переменные дистанций
long leftDist     = 0;
long rightDist    = 0;
long forwardDist  = 0;

// объявляем переменные угола поворота сервопривода
int servoLeft    =  170;
int servoRight   =  10;
int servoForward =  85;

// объявляем переменные скороти моторов
int motorLeftMaxSpeed  = 155;
int motorRightMaxSpeed = 155;
int motorMinSpeed      = 0;

// создаем объектs ServoX и ServoY
Servo  ServoX;
Servo  ServoY;

void forward();
void backward();
void turn_left();
void turn_right();
void motors_stop();

long look_left();
long look_right();
long look_right();
long look_forward();

long getEchoTiming();

// функция настройки
void setup()
{
  // настраиваем пины драйвера на выход
  pinMode(IN_1,  OUTPUT);
  pinMode(IN_2,  OUTPUT);
  pinMode(IN_3,  OUTPUT);
  pinMode(IN_4,  OUTPUT);

  // настраиваем пины ШИМ на выход
  pinMode(EN_A,  OUTPUT);
  pinMode(EN_B,  OUTPUT);

  /* настраиваем контакт trig
    ультра-звукового дальномера на выход */
  pinMode(TRIG_PIN,  OUTPUT);

  /* настраиваем контакт echo
    ультра-звукового дальномера на вход */
  pinMode(ECHO_PIN,  INPUT);

  // настраиваем объекты ServoX и ServoY
  ServoX.attach(12);
  ServoY.attach(11);

  /* поворачиваем сервопривод по осям Y и X
    чтобы робот смотрел прямо */
  ServoY.write(45);
  ServoX.write(servoForward);
  delay(500);

  Serial.begin(9600);
}

void loop()
{
  forward();
  forwardDist = look_forward();

  // если впереди препятствие
  if (forwardDist <= minDist)
  {
    // останавливаем моторы
    motors_stop();

    // узнаем дистанцию справа и слева
    rightDist   = look_right();
    leftDist    = look_left();

    // если справа расстояние меньше чем слева
    if (rightDist < leftDist)
    {
      // повернуть направо
      turn_right();
      delay(delayStopPeriod);
      motors_stop();
    }
    // если справа расстояние меньше чем слева
    if (rightDist > leftDist)
    {
      // повернуть налево
      turn_left();
      delay(delayStopPeriod);
      motors_stop();
    }
    // повораиваем сервопривод вперед
    ServoX.write(servoForward);
    /* делаем задержку чтобы сервопривод
       успел выполнить команду */
    delay(500);
  }

  //Serial.println(forwardDist);
}



// функция вперед
void forward()
{
  // заставляем первый мотор вращаться вперед
  digitalWrite(IN_1,  LOW);
  digitalWrite(IN_2,  HIGH);

  // скорость ШИМ
  analogWrite (EN_A, motorLeftMaxSpeed);

  // заставляем второй мотор вращаться вперед
  digitalWrite(IN_3,  HIGH);
  digitalWrite(IN_4,  LOW);

  // скорость ШИМ
  analogWrite (EN_B, motorRightMaxSpeed);
}

// функция назад
void backward()
{
  // заставляем первый мотор вращаться назад
  digitalWrite(IN_1,  HIGH);
  digitalWrite(IN_2,  LOW);

  // скорость ШИМ
  analogWrite (EN_A, motorLeftMaxSpeed);

  // заставляем второй мотор вращаться назад
  digitalWrite(IN_3,  LOW);
  digitalWrite(IN_4,  HIGH);

  // скорость ШИМ
  analogWrite (EN_B, motorRightMaxSpeed);
}

// функция влево
void turn_left()
{
  // заставляем первый мотор вращаться вперед
  digitalWrite(IN_1,  LOW);
  digitalWrite(IN_2,  HIGH);

  // скорость ШИМ
  analogWrite (EN_A, motorLeftMaxSpeed);

  // заставляем второй мотор вращаться назад
  digitalWrite(IN_3,  LOW);
  digitalWrite(IN_4,  HIGH);

  // скорость ШИМ
  analogWrite (EN_B, motorRightMaxSpeed);
}

// функция вправо
void turn_right()
{
  // заставляем первый мотор вращаться назад
  digitalWrite(IN_1,  HIGH);
  digitalWrite(IN_2,  LOW);

  // скорость ШИМ
  analogWrite (EN_A, motorLeftMaxSpeed);

  // заставляем второй мотор вращаться вперед
  digitalWrite(IN_3,  HIGH);
  digitalWrite(IN_4,  LOW);

  // скорость ШИМ
  analogWrite (EN_B, motorRightMaxSpeed);
}

// функция стоп
void motors_stop()
{
  // останавливаем первый мотор
  digitalWrite(IN_1,  HIGH);
  digitalWrite(IN_2,  LOW);

  // скорость ШИМ
  analogWrite (EN_A, motorMinSpeed);

  // останавливаем второй мотор
  digitalWrite(IN_3,  HIGH);
  digitalWrite(IN_4,  LOW);

  // скорость ШИМ
  analogWrite (EN_B, motorMinSpeed);
}

// функция смотреть влево
long look_left()
{
  long functionLeftDist = 0;
  // повораиваем сервопривод влево
  ServoX.write(servoLeft);
  /* делаем задержку чтобы сервопривод
     успел выполнить команду */
  delay(500);
  // считаем дистанцию
  functionLeftDist = getEchoTiming() * 1.7 * 0.01;
  // возвратить значение дистанции
  return functionLeftDist;
}

// функция смотреть вправо
long look_right()
{
  long functionRightDist = 0;
  // повораиваем сервопривод вправо
  ServoX.write(servoRight);
  /* делаем задержку чтобы сервопривод
     успел выполнить команду */
  delay(500);
  // считаем дистанцию
  functionRightDist = getEchoTiming() * 1.7 * 0.01;
  // возвратить значение дистанции
  return functionRightDist;
}

// функция смотреть вперед
long look_forward()
{
  long functionForwardDist = 0;
  // считаем дистанцию
  functionForwardDist = getEchoTiming() * 1.7 * 0.01;
  // возвратить значение дистанции
  return functionForwardDist;
}

// функция определениея времени задержки
long getEchoTiming()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); // генерируем импульс запуска
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // определение на пине echoPin длительности уровня HIGH, мксек:
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration;
}

