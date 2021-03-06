#include <hiotMainlib.h>

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(74, 2);

void ColorObj::setup(){

    strip.Begin();
    strip.Show();
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
        RGB[j][0] = color.substring(0, commaIndex).toInt();
        RGB[j][1] = color.substring(commaIndex + 1, secondCommaIndex).toInt();
        RGB[j][2] = color.substring(secondCommaIndex + 1).toInt();

        // Serial.println(j);
        // Serial.println(j);
        // Serial.println(j);
        // Serial.println(j);

        // if(!conf.useWS2812){

        //     RGB[j][0] *= 4;
        //     RGB[j][1] *= 4;
        //     RGB[j][2] *= 4;
        // }
        for(int i = 0;i<3;i++){
            fadems[j][i] = calculatems(1000000, RGB[j][i], RGB_write[j][i]);
        }
        // Serial.print("Colors.RGB:");
        // for(int i=0;i<3;i++){

        //     Serial.print("\t");
        //     Serial.print(RGB[j][i]);
        // }
    }
    // Serial.println();
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
            for (int i = 0; i < 3;i++){
                RGB[j][i] = jumpcolors[effectIndex][i];
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
            for (int i = 0; i < 3;i++){
                RGB[j][i] = strobecolors[effectIndex][i];
                fadems[j][i] = 0;
            }
        }
        effectIndex++;
        if(effectIndex>=strobelen){
            effectIndex = 0;
        }
    }
}


void ColorObj::fade(){

    if(millis() - _millis_Effect >= fadespeed){
        _millis_Effect = millis();
        for(int j = 0; j < ledAmmount ; j++ ){
            for (int i = 0; i < 3;i++){
                RGB[j][i] = jumpcolors[effectIndex][i];
                fadems[j][i] = calculatems(fadespeed*1000, RGB[j][i], RGB_write[j][i]);
            }
        }
        effectIndex++;
        if(effectIndex>=jumplen){
            effectIndex = 0;
        }
    }
}

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
        
        default:
            break;
    }
}


void ColorObj::animateColor(){
    int i = _countAnimateColor;
    int j = ledIndex;

    // if (RGB_write[j][i] != RGB[j][i] && fadems[j][i] > 0){
    //     if(millis() - _millis_printcolors >= 50){
    //         _millis_printcolors = millis();
    //         for(int i=0;i<3;i++)
    //         {
    //             Serial.print(RGB_write[0][i]);
    //             Serial.print("\t");
    //         }
    //         Serial.println();
    //     }
    // }
    if (RGB_write[j][i] < RGB[j][i] && fadems[j][i] > 0){

                
        if (micros() - _lastMillis_AnimateColors[j][i] >= fadems[j][i]){

            _lastMillis_AnimateColors[j][i] = micros();
            // animate_color_is_not_Blocking = false;
            RGB_write[j][i]++;
        }
    }else if(RGB_write[j][i] > RGB[j][i] && fadems[j][i] > 0){
        
        if (micros() - _lastMillis_AnimateColors[j][i] >= fadems[j][i]){

            _lastMillis_AnimateColors[j][i] = micros();
            // animate_color_is_not_Blocking = false;
            RGB_write[j][i]--;
        }             
    }else{
        RGB_write[j][i] = RGB[j][i];
        //animate_color_is_not_Blocking = true;
    }
    i ++;
    if(i > 3){
        i = 0;
    }
    _countAnimateColor = i;
    j++;
    if(j >= ledAmmount){
        j = 0;
    }
    ledIndex = j;
}



void ColorObj::writeColor(){
    if(conf.useWS2812){
        if(millis() - _lastMillis_writeColor_neopixelbus > 30){
            _lastMillis_writeColor_neopixelbus = millis();
            RgbColor red(RGB_write[0][0], RGB_write[0][1], RGB_write[0][2]);
            // delay(1);
            for(int j = 0; j <= 74;j++){
                strip.SetPixelColor(j,red);
            }
            strip.Show();
        }
    }else{
        analogWrite(RGB_Pin[_countWriteColor] , RGB_write[0][_countWriteColor] );
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
    //     Serial.println();
    // }
}