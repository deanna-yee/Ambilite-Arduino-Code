#include "Light.h";

// define pins of lights
#define bluePin0 11
#define greenPin0 10
#define redPin0 9
#define bluePin1 6
#define greenPin1 5
#define redPin1 3

//store Lights
Light lights[2];
Light usedLights[2];

const int off0 = 0;
const int off1 = 255;
const int delayTimeDim = 5;
const int delayTimeBrighten = 100;

void setup()
{
  // open serial port and set sample rate to 9600
  Serial.begin(9600); 

  // set pins to output
  pinMode(bluePin0, OUTPUT);
  pinMode(greenPin0, OUTPUT);
  pinMode(redPin0, OUTPUT);
  pinMode(bluePin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(redPin1, OUTPUT);

  // initalize lights in array
  lights[0] = emptyLight();
  lights[1] = emptyLight();
  usedLights[0] = emptyLight();
  usedLights[1] = emptyLight();

  //start with lights off
  onOff(0,off0,off0,off0);
  onOff(1,off1,off1,off1);
  
}

void loop()
{
  // get old lights from array
  Light oldLight0 = usedLights[0];
  oldLight0 = colorBrightness(oldLight0);
  Light oldLight1 = usedLights[1];
  oldLight1 = colorBrightness(oldLight1);
  
  while(Serial.available() == 0);
  int numOfLights = Serial.read();
  
  //read Serial info
  for(int i = 0 ; i < numOfLights ; i++)
  {
    getInfoFromSerial();
  }
  
  // get new lights from array 
  Light light0 = lights[0];
  Light light1 = lights[1];
  light0 = colorBrightness(light0);
  light1 = colorBrightness(light1);
  
  // calculate the difference between new values of red, green, and blue and old values of red, green, blue
  int deltaRed0 = abs((oldLight0.red - light0.red)/10)+1;
  int deltaRed1 = abs((oldLight1.red - light1.red)/10)+1;
  int deltaGreen0 = abs((oldLight0.green - light0.green)/10)+1;
  int deltaGreen1 = abs((oldLight1.green - light1.green)/10)+1;
  int deltaBlue0 = abs((oldLight0.blue - light0.blue)/10)+1;
  int deltaBlue1 = abs((oldLight1.blue - light1.blue)/10)+1;
  
  // create a temp variable
  Light tempLight0 = oldLight0;
  Light tempLight1 = oldLight1;
  
  // if both lights are on
  if(light0.power == 1 && light1.power == 1)
  {
    if((tempLight0.red != light0.red) || (tempLight0.green != light0.green) || (tempLight0.blue != light0.blue) || (tempLight1.red != light1.red) || (tempLight1.green != light1.green) || (tempLight1.blue != light1.blue))
    {
      // dim both lights until they are off
      while((tempLight0.red > 0) || (tempLight0.green > 0) || (tempLight0.blue > 0) || (tempLight1.red > 0) || (tempLight1.green > 0) || (tempLight1.blue > 0))
      {
        if((tempLight0.red != light0.red) || (tempLight0.green != light0.green) || (tempLight0.blue != light0.blue))
        {
          tempLight0 = dim(tempLight0, deltaRed0, deltaGreen0, deltaBlue0);
        }
        if((tempLight1.red != light1.red) || (tempLight1.green != light1.green) || (tempLight1.blue != light1.blue))
        {
          tempLight1 = dim(tempLight1, deltaRed1, deltaGreen1, deltaBlue1);
        }
        Light tl1 = invertColors(tempLight1);
        onOff(0, tempLight0.red, tempLight0.green, tempLight0.blue);
        onOff(1, tl1.red, tl1.green, tl1.blue);
        delay(delayTimeDim);
      }
      
      // brighten both lights until they are fully on
      while((tempLight0.red < light0.red) || (tempLight0.green < light0.green) || (tempLight0.blue < light0.blue) || (tempLight1.red < light1.red) || (tempLight1.green < light1.green) || (tempLight1.blue < light1.blue))
      {
        tempLight0 = brighten(tempLight0, light0, deltaRed0, deltaGreen0, deltaBlue0);
        tempLight1 = brighten(tempLight1, light1, deltaRed1, deltaGreen1, deltaBlue1);
        Light tl1 = invertColors(tempLight1);
        onOff(0, tempLight0.red, tempLight0.green, tempLight0.blue);
        onOff(1, tl1.red, tl1.green, tl1.blue);
        if((tempLight0.red != light0.red) || (tempLight0.green != light0.green) || (tempLight0.blue != light0.blue) || (tl1.red != light1.red) || (tl1.green != light1.green) || (tl1.blue != light1.blue))
        {
          delay(delayTimeBrighten);
        }
      }
      usedLights[0] = light0;
      usedLights[1] = light1;
    }
  }
  else if(light0.power == 1 && light1.power == 0) // if the first light is on and the second light is off
  {
    if((tempLight0.red != light0.red) || (tempLight0.green != light0.green) || (tempLight0.blue != light0.blue))
    {
      // dim both lights until they are off
      while((tempLight0.red > 0) || (tempLight0.green > 0) || (tempLight0.blue > 0) || (tempLight1.red > 0) || (tempLight1.green > 0) || (tempLight1.blue > 0))
      {
        tempLight0 = dim(tempLight0, deltaRed0, deltaGreen0, deltaBlue0);
        tempLight1 = dim(tempLight1, deltaRed1, deltaGreen1, deltaBlue1);
        Light tl1 = invertColors(tempLight1);
        onOff(0, tempLight0.red, tempLight0.green, tempLight0.blue);
        onOff(1, tl1.red, tl1.green, tl1.blue);
        delay(delayTimeDim);
      }
      
      // brighten first light until it is fully on
      while((tempLight0.red < light0.red) || (tempLight0.green < light0.green) || (tempLight0.blue < light0.blue))
      {
        tempLight0 = brighten(tempLight0, light0, deltaRed0, deltaGreen0, deltaBlue0);
        onOff(0, tempLight0.red, tempLight0.green, tempLight0.blue);
        if((tempLight0.red != light0.red) || (tempLight0.green != light0.green) || (tempLight0.blue != light0.blue))
        {
          delay(delayTimeBrighten);
        }
      }
      usedLights[0] = light0;
    }
  }
  else if(light0.power == 0 && light1.power == 1) // if the first light is off and the second light is on
  {
    // dim both lights until they are off
    if((tempLight1.red != light1.red) || (tempLight1.green != light1.green) || (tempLight1.blue != light1.blue))
    {
      while((tempLight0.red > 0) || (tempLight0.green > 0) || (tempLight0.blue > 0) || (tempLight1.red > 0) || (tempLight1.green > 0) || (tempLight1.blue > 0))
      {
        tempLight0 = dim(tempLight0, deltaRed0, deltaGreen0, deltaBlue0);
        tempLight1 = dim(tempLight1, deltaRed1, deltaGreen1, deltaBlue1);
        Light tl1 = invertColors(tempLight1);
        onOff(0, tempLight0.red, tempLight0.green, tempLight0.blue);
        onOff(1, tl1.red, tl1.green, tl1.blue);
        delay(delayTimeDim);
      }
      
      // brighten second light until it is fully on
      while((tempLight1.red < light1.red) || (tempLight1.green < light1.green) || (tempLight1.blue < light1.blue))
      {
        tempLight1 = brighten(tempLight1, light1, deltaRed1, deltaGreen1, deltaBlue1);
        Light tl1 = invertColors(tempLight1);
        onOff(1, tl1.red, tl1.green, tl1.blue);
        if((tempLight1.red != light1.red) || (tempLight1.green != light1.green) || (tempLight1.blue != light1.blue))
        {
          delay(delayTimeBrighten);
        }
      }
      usedLights[1] = light1;
    }
  }
  else // if both lights are off
  {
    // dim both lights until they are fully off
    while((tempLight0.red > 0) || (tempLight0.green > 0) || (tempLight0.blue > 0) || (tempLight1.red > 0) || (tempLight1.green > 0) || (tempLight1.blue > 0))
    {
      tempLight0 = dim(tempLight0, deltaRed0, deltaGreen0, deltaBlue0);
      tempLight1 = dim(tempLight1, deltaRed1, deltaGreen1, deltaBlue1);
      Light tl1 = invertColors(tempLight1);
      onOff(0, tempLight0.red, tempLight0.green, tempLight0.blue);
      onOff(1, tl1.red, tl1.green, tl1.blue);
      delay(delayTimeDim);
    }
    usedLights[0] = changeColor(usedLights[0], 0, 0, 0);
    usedLights[1] = changeColor(usedLights[1], 0, 0, 0);
  } 
}

