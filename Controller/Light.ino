
// initialize lights 
Light emptyLight()
{
   Light light;
   light.red = 0;
   light.green = 0;
   light.blue = 0;
   light.brightness = 0;  
   light.power = 0;
   return light;
}

// change the colors of the lights
Light changeColor(Light light, int newRed, int newGreen, int newBlue)
{
    light.red = newRed;
    light.green = newGreen;
    light.blue = newBlue;   
    return light;
}

// invert the colors
Light invertColors(Light light)
{
  light.red = 255 - light.red;
  light.green = 255 - light.green;
  light.blue = 255 - light.blue;
  return light;
}
// calculate how bright the light will be
Light colorBrightness(Light light)
{
    double brightnessPercentage = ((double)light.brightness / 100);
    light.red = ((double)light.red * brightnessPercentage);
    light.green = ((double)light.green * brightnessPercentage);
    light.blue = ((double)light.blue * brightnessPercentage);
    return light;
}

// read from the serial port
int readSerial()
{
    while(Serial.available() == 0);
    return Serial.read();
}

// Dim lights slowly
Light dim(Light tempLight, int deltaRed, int deltaGreen, int deltaBlue)
{
    int red = tempLight.red - deltaRed;
    if(red > 0)
    {
       tempLight.red = red;
    }
    else 
    {
        tempLight.red = 0;
    }
          
    int green = tempLight.green - deltaGreen;
    if(green > 0)
    {
       tempLight.green = green;
    }
    else
    {
        tempLight.green = 0;
    }
      
    int blue = tempLight.blue - deltaBlue;
    if(blue > 0)
    {
       tempLight.blue = blue;
    }
    else
    {
        tempLight.blue= 0;
    }
    return tempLight;
}

// Brighten lights slowly
Light brighten(Light tempLight, Light light, int deltaRed, int deltaGreen, int deltaBlue)
{
      int red = tempLight.red + deltaRed;
      if(red < light.red)
      {
         tempLight.red = red;
      }
      else
      {
          tempLight.red = light.red;
      }
      
      int green = tempLight.green + deltaGreen;
      if(green < 0)
      {
         tempLight.green = green;
      }
      else
      {
          tempLight.green = light.green;
      }
      
      int blue = tempLight.blue + deltaBlue;
      if(blue < light.blue)
      {
         tempLight.blue = blue;
      }
      else 
      {
          tempLight.blue= light.blue;
      }
      return tempLight;
}

void getInfoFromSerial()
{
  // read light id
  int lightID = readSerial();
  if(lightID > 1)
  {
    lightID = lightID - 2;
  }
  Light light = lights[lightID];

  //Edit color
  unsigned int colorEdit = readSerial();
  if(colorEdit == 1)
  {
    int r = readSerial();
    int g = readSerial();
    int b = readSerial();
    light = changeColor(light, r, g, b);
  }

  //Edit brightness
  int brightnessEdit = readSerial();
  if(brightnessEdit == 1)
  {
    int brightness = readSerial();
    light.brightness = brightness;
  }

  // Edit Power
  int powerEdit = readSerial();
  if(powerEdit == 1)
  {
    int power = readSerial();
    light.power = power;
  }
  lights[lightID] = light;
}

//turn on and off the light
void onOff(int id, int red, int green, int blue)
{
   if(id == 0)
   {
       analogWrite(redPin0, red);  // set how much red is needed in redPin0
       analogWrite(greenPin0, green); // set how much green is needed in greenPin0
       analogWrite(bluePin0, blue); //set how much blue is needed in bluePin0
   } 
   else if(id == 1)
   {
       analogWrite(redPin1, red);  // set how much red is needed in redPin1
       analogWrite(greenPin1, green); // set how much green is needed in greenPin1
       analogWrite(bluePin1, blue); //set how much blue is needed in bluePin1
   }
} 
