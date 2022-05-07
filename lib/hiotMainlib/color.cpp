#include <hiotMainlib.h>

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(74, 2);
// NeoPixelBus<NeoGrbFeature, NeoWs2812Method> strip(74, 2);
// NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma800KbpsMethod> strip(74, 2);


void ColorObj::setup(){

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    // leds[0] = CRGB::Red; 
    // FastLED.show(); 
    // strip.Begin();
    // strip.Show();
    // ICH BIN DDDDDDDDDDDDDD
}
void ColorObj::loop(){
    effect();
    animateColor();
    writeColor();
}

void ColorObj::setRGBPins(int R, int G, int B){

}

// funcconvert
bool ColorObj::convert()
{
    for(int i = 0; i < ledAmmount ; i++ ){
        RGB[i][0] = RGBW[i][0];
        RGB[i][1] = RGBW[i][1];
        RGB[i][2] = RGBW[i][2];
        if((RGB[i][0] <= RGB[i][1]) && (RGB[i][0] <= RGB[i][2])){
            RGBW[i][0] = RGB[i][0] - RGB[i][0];
            RGBW[i][1] = RGB[i][1] - RGB[i][0];
            RGBW[i][2] = RGB[i][2] - RGB[i][0];
            RGBW[i][3] = RGB[i][0];
            
        }
        else if ((RGB[i][1] <= RGB[i][0]) && (RGB[i][1] <= RGB[i][2])){
            RGBW[i][0] = RGB[i][0] - RGB[i][1];
            RGBW[i][1] = RGB[i][1] - RGB[i][1];
            RGBW[i][2] = RGB[i][2] - RGB[i][1];
            RGBW[i][3] = RGB[i][1];
        }
        else{
            RGBW[i][0] = RGB[i][0] - RGB[i][2];
            RGBW[i][1] = RGB[i][1] - RGB[i][2];
            RGBW[i][2] = RGB[i][2] - RGB[i][2];
            RGBW[i][3] = RGB[i][2];

        }
    }
    return 0;
}

int ColorObj::calculatems(int speed, int current_colors, int next_colors){
    if (next_colors != current_colors){
        if (speed == 0){
            // Serial.println("0cms");
            return 0;
        }
        else if((float)speed / (fabsf((float)next_colors - (float)current_colors)) < 1 ){
            // Serial.println("1cms");
            return 1;
        }
        else{
            // Serial.println((float)speed /(fabsf((float)next_colors - (float)current_colors)));
            return (float)speed /(fabsf((float)next_colors - (float)current_colors));
        }
    }
    return 0;
}

// funcsetColorString
void ColorObj::setColorString(String color){
    currentEffect = 0;
    int commaIndex = color.indexOf(',');
    int secondCommaIndex = color.indexOf(',', commaIndex + 1);
    for(int j = 0; j < ledAmmount ; j++ ){
        RGBW[j][0] = color.substring(0, commaIndex).toInt();
        RGBW[j][1] = color.substring(commaIndex + 1, secondCommaIndex).toInt();
        RGBW[j][2] = color.substring(secondCommaIndex + 1).toInt();

        if(conf.useRGBW){
            convert();
        }
        for(int i = 0;i<pinAmount;i++){
            fadems[j][i] = calculatems(1000000, RGBW[j][i], RGBW_write[j][i]);
        }
    }

}
String ColorObj::getColorStringRGB(int i){
    return String(RGB_write[i][0]) + String(",") + String(RGB_write[i][1]) + String(",") + String(RGB_write[i][2]);
}

String ColorObj::getColorStringRGBW(int i){
    return String(RGBW_write[i][0]) + String(",") + String(RGBW_write[i][1]) + String(",") + String(RGBW_write[i][2]) + String(",") + String(RGBW_write[i][3]);
}

void ColorObj::jump(){
    if(millis() - _millis_Effect >= fadespeed){
        // Serial.println("CHANGING COLOR");
        _millis_Effect = millis();
        for(int j = 0; j < ledAmmount ; j++ ){
            for (int i = 0; i < pinAmount;i++){
                RGBW[j][i] = jumpcolors[effectIndex][i];
                fadems[j][i] = 0;
            }
        }
        effectIndex++;
        if(effectIndex>=jumplen){
            effectIndex = 0;
        }
    }
}

void ColorObj::strobe(){

    if(millis() - _millis_Effect >= fadespeed){
        // Serial.println("CHANGING COLOR");
        _millis_Effect = millis();
        for(int j = 0; j < ledAmmount ; j++ ){
            for (int i = 0; i < pinAmount;i++){
                // RGBW[j][i] = strobecolors[effectIndex][i];
                // fadems[j][i] = 10000;
                fadems[j][i] = 0;
                // fadems[j][i] = calculatems(fadespeed*3000,255,0);
            }
        }

        // for(int j = 0; j < ledAmmount ; j++ ){
            // for (int i = 0; i < pinAmount;i++){
            //     RGBW[0][i] = jumpcolors[effectIndex][i];
            //     fadems[0][i] = 0;
            // }
            // RgbColor rgbcl = RgbColor(RGBW[0][0],RGBW[0][1],RGBW[0][2]);
            // HsbColor hsbcl =HsbColor(rgbcl);
            basecolor.H+=0.005;
            if(basecolor.H>=1){
                basecolor.H=0;
            }
            // Serial.println(basecolor.H);
            // RgbColor rgbcl = RgbColor(basecolor).;
            // Serial.println(RgbColor(basecolor).R);
            // Serial.println(RgbColor(basecolor).G);
            // Serial.println(RgbColor(basecolor).B);
            RGBW[0][0] = RgbColor(basecolor).R;
            RGBW[0][1] = RgbColor(basecolor).G;
            RGBW[0][2] = RgbColor(basecolor).B;
        // }

        // for (size_t i = 0; i < 3; i++)
        // {
        //     if(neopixeltest[i]==0){
        //         countdirection[i]= true;
        //     }else if(neopixeltest[i]==255){
        //         countdirection[i]= false;
        //     }
        // }
        
        // RGBW[0][0]=neopixeltest[0];
        // RGBW[0][1]=neopixeltest[1];
        // RGBW[0][2]=neopixeltest[2];

        // for (size_t i = 0; i < 3; i++)
        // {
        //     if(countdirection[i]==true){
        //         neopixeltest[i]++;
        //     }else{
        //         neopixeltest[i]--;

        //     }
        // }

        rotatearrleft(RGBW,1,ledAmmount);
        //   Serial.println();
        // for (size_t i = 0; i < 1; i++)
        // {
        //     for (size_t j = 0; j < 4; j++)
        //     {
        //         Serial.print(RGBW[i][j]);
        //         Serial.print(" ");
        //     }
        //     Serial.print(" ");
        // }
        // Serial.println();
        // RGBW[_StrobeIndex+1][1]=255;
        // RGBW[_StrobeIndex+1][2]=255;
        // RGBW[_StrobeIndex+2][0]=0;
        // RGBW[_StrobeIndex+2][1]=0;
        // RGBW[_StrobeIndex+2][2]=0;
        // RGBW[_StrobeIndex][0] = 255-RGBW[_StrobeIndex][0];
        // RGBW[_StrobeIndex][1] = 255-RGBW[_StrobeIndex][1];
        // RGBW[_StrobeIndex][2] = 255-RGBW[_StrobeIndex][2];
        // RGBW[_StrobeIndex-1][0]=0;
        // RGBW[_StrobeIndex-1][1]=0;
        // RGBW[_StrobeIndex-1][2]=0;

        effectIndex++;
        if(effectIndex>=strobelen){
            effectIndex = 0;
        }
        _StrobeIndex +=2;
        if(_StrobeIndex>255){
            _StrobeIndex = 0;
        }
    }
}


void ColorObj::fade(){

    if(millis() - _millis_Effect >= fadespeed){
        _millis_Effect = millis();
        for(int j = 0; j < ledAmmount ; j++ ){
            for (int i = 0; i < pinAmount;i++){
                RGBW[j][i] = jumpcolors[effectIndex][i];
                fadems[j][i] = calculatems(fadespeed*1000, RGBW[j][i], RGBW_write[j][i]);
            }
        }
        effectIndex++;
        if(effectIndex>=jumplen){
            effectIndex = 0;
        }
    }
}


void ColorObj::rainbow(){

    if(millis() - _millis_Effect >= fadespeed){
        _millis_Effect = millis();


        basecolor.H+=0.005;
        if(basecolor.H>=1){
            basecolor.H=0;
        }

        for(int j = 0; j < ledAmmount ; j++ ){
            RGBW[j][0] = RgbColor(basecolor).R;
            RGBW[j][1] = RgbColor(basecolor).G;
            RGBW[j][2] = RgbColor(basecolor).B;
            fadems[j][0] = calculatems(fadespeed*1000, RGBW[j][0], RGBW_write[j][0]);
            fadems[j][1] = calculatems(fadespeed*1000, RGBW[j][1], RGBW_write[j][1]);
            fadems[j][2] = calculatems(fadespeed*1000, RGBW[j][2], RGBW_write[j][2]);
        }

        rotatearrleft(RGBW,1,ledAmmount);

        effectIndex++;
        if(effectIndex>=jumplen){
            effectIndex = 0;
        }
    }
}

// loop for effects
void ColorObj::effect(){
    switch (currentEffect){
        case 1:
            jump();
            break;
        case 2:
            fade();
            break;
        case 3:
            strobe();
            break;
        case 4:
            rainbow();
            break;
        
        default:
            break;
    }
}


//calculating time for each increment
void ColorObj::animateColor(){
    int i = _countAnimateColor;
    // int j = ledIndex;
    for(int j=0;j<ledAmmount;j++){

        unsigned long microsec = micros();
        if(fadems[j][i] > 0){

            if (RGBW_write[j][i] < RGBW[j][i]){
                        
                // if (micros() - _lastMillis_AnimateColors[j][i] >= fadems[j][i]){
                if (microsec - _lastMillis_AnimateColors[j][i] >= fadems[j][i]){

                    // _lastMillis_AnimateColors[j][i] = micros();
                    _lastMillis_AnimateColors[j][i] = microsec;
                    // animate_color_is_not_Blocking = false;
                    RGBW_write[j][i]++;
                }
            }else if(RGBW_write[j][i] > RGBW[j][i]){
                
                // if (micros() - _lastMillis_AnimateColors[j][i] >= fadems[j][i]){
                if (microsec - _lastMillis_AnimateColors[j][i] >= fadems[j][i]){

                    // _lastMillis_AnimateColors[j][i] = micros();
                    _lastMillis_AnimateColors[j][i] = microsec;
                    // animate_color_is_not_Blocking = false;
                    RGBW_write[j][i]--;
                }             
            }
            // else{
                // RGBW_write[j][i] = RGBW[j][i];
                //animate_color_is_not_Blocking = true;
            // }
        }else if(RGBW_write[j][i] != RGBW[j][i]){
            
            RGBW_write[j][i] = RGBW[j][i];
        }
    }

        // if (RGB_write[j][i] < RGB[j][i] && fadems[j][i] > 0){

                    
        //     if (micros() - _lastMillis_AnimateColors[j][i] >= fadems[j][i]){

        //         _lastMillis_AnimateColors[j][i] = micros();
        //         // animate_color_is_not_Blocking = false;
        //         RGB_write[j][i]++;
        //     }
        // }else if(RGB_write[j][i] > RGB[j][i] && fadems[j][i] > 0){
            
        //     if (micros() - _lastMillis_AnimateColors[j][i] >= fadems[j][i]){

        //         _lastMillis_AnimateColors[j][i] = micros();
        //         // animate_color_is_not_Blocking = false;
        //         RGB_write[j][i]--;
        //     }             
        // }else{
        //     RGB_write[j][i] = RGB[j][i];
        //     //animate_color_is_not_Blocking = true;
        // }
    // }
    i ++;
    if(i > pinAmount){
        i = 0;

        // j++;
        // if(j >= ledAmmount){
        //     j = 0;
        // }
        // ledIndex = j;
    }
    _countAnimateColor = i;

}


//write color to pins
void ColorObj::writeColor(){
    if(conf.useWS2812){
        if(millis() - _lastMillis_writeColor_neopixelbus > 1000/60){
            _lastMillis_writeColor_neopixelbus = millis();
            for(int j = 0; j < ledAmmount;j++){
                RgbColor red(RGBW_write[j][0], RGBW_write[j][1], RGBW_write[j][2]);
                strip.SetPixelColor(j,red);
                leds[j].setRGB(RGBW_write[j][0], RGBW_write[j][1], RGBW_write[j][2]);
            }
            // strip.Show();
            FastLED.show(); 
            // while(Serial.available() > 0) {  }
            // delayMicroseconds(30 * 75);
        }
    } else {
        analogWrite(RGBW_Pin[_countWriteColor] , RGBW_write[0][_countWriteColor] );
    }
    _countWriteColor ++;
    if(_countWriteColor > pinAmount){
        _countWriteColor = 0;
    }
    // if(millis() - _millis_printcolors >= 1*100){
    //     _millis_printcolors = millis();
    //     for(int i=0;i<3;i++)
    //     {
    //         Serial.print(RGB_write[0][i]);
    //         Serial.print("\t");
    //     }
    //     Serial.print("\t");
    //     Serial.print("\t");
    //     Serial.print("\t");
    //     for(int i=0;i<pinAmount;i++)
    //     {
    //         Serial.print(RGBW_write[0][i]);
    //         Serial.print("\t");
    //     }
    //     Serial.println();
    // }
}