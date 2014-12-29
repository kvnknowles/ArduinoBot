#include <ArduinoUnit.h>
TestSuite suite;

test(correct)
{
  int x=1;
  assertEqual(x,1);
}

test(incorrect)
{
  int x=1;
  assertNotEqual(x,1);
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  suite.run();
}
