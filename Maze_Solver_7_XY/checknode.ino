void checknode() {
  l = 0;
  r = 0;
  s = 0;
  u = 0;
  e = 0;
  paths = 0;
  FT = 60;

  // checks whethere bot is on node and the number of exits possible

  //digitalWrite(13, HIGH);
  if (analogRead(6) < threshold[6] && analogRead(5) > threshold[5]) {
    y = 1;
    FT = 160;
  }
  if (analogRead(6) < threshold[6] && analogRead(5) < threshold[5]) {
    r = 1;
  }
  if (analogRead(0) < threshold[0] && analogRead(1) > threshold[1]) {
    x = 1;
    FT = 160;
  }
  if (analogRead(0) < threshold[0] && analogRead(1) > threshold[1]) {
    l = 1;
  }

  if ((analogRead(0) > threshold[0] && (analogRead(6) > threshold[6]) && (analogRead(3) > threshold[3]))) {
    u = 1;
  }
  if ((analogRead(3) < threshold[3]) && (analogRead(5) < threshold[5]) && (analogRead(6) < threshold[6])) {
    e = 1;
  }

  if (u == 0) {
    for (int i = 0; i < FT; i++) {
      PID();

      if (analogRead(6) < threshold[6] && analogRead(5) > threshold[5]) y = 1;
      if (analogRead(6) < threshold[6] && analogRead(5) < threshold[5]) r = 1;
      if (analogRead(0) < threshold[0] && analogRead(1) > threshold[1]) x = 1;
      if (analogRead(0) < threshold[0] && analogRead(1) > threshold[1]) l = 1;
    }
    //digitalWrite(13, LOW);
    
    for (int i = 0; i < 40; i++) {
      PID();
      if (analogRead(3) < threshold[3]) s = 1;
      if ((e == 1) && (analogRead(5) < threshold[5]) && (analogRead(6) < threshold[6]) && (analogRead(3) < threshold[3])) e = 2;
    }
    if (e == 2) {
      for (int i = 0; i < FT; i++) {
        PID();
      }
      if ((analogRead(5) < threshold[5]) && (analogRead(6) < threshold[6]) && (analogRead(3) < threshold[3])) e = 3;
    }
  }
  if (u == 1) {
    for (int i = 0; i < 5; i++) {
      botinchforward();
    }
  }

  paths = l + s + r;
}
