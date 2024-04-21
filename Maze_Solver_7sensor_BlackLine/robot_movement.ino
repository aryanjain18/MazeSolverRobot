void botleft ()
{
  motor1.drive(-1 * turnspeed);
  motor2.drive(turnspeed);
  delay(150);
  while (analogRead(3) < threshold[3])
  {
    motor1.drive(-1 * turnspeed);
    motor2.drive(turnspeed);
  }
  motor1.drive(0);
  motor2.drive(0);
  delay(50);
}

void botright ()
{
  motor1.drive(turnspeed);
  motor2.drive(-1 * turnspeed);
  delay(150);
  while (analogRead(3) < threshold[3])
  {
    motor1.drive(turnspeed);
    motor2.drive(-1 * turnspeed);
  }
  motor1.drive(0);
  motor2.drive(0);
  delay(50);
}

void botstraight ()
{
  motor1.drive(lfspeed);
  motor2.drive(lfspeed);
}

void botinchforward ()
{
  motor1.drive(turnspeed);
  motor2.drive(turnspeed);
  delay(10);
}
void botstop ()
{
  motor1.drive(0);
  motor2.drive(0);
}
void botuturn ()
{
  motor1.drive(-1 * turnspeed);
  motor2.drive(turnspeed);
  delay(300);
  while (analogRead(3) < threshold[3])
  {
    motor1.drive(-1 * turnspeed);
    motor2.drive(turnspeed);
  }
  motor1.drive(0);
  motor2.drive(0);
  delay(50);
}
void forwardstep()
{
  int battV = analogRead(7)- 770;
}
