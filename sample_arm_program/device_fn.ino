#include <Arduino.h>

///Functions to operate the device
//////////////////////////////////////////////////////////////////////////////////////
//web_parser
//the first argument determines the function that is called next.
//The arguments are loaded into the web_buffer array
//if the first argument is > 100 then the following arguments are Strings
//The array length is fixed at 10
void web_parser() {
  String message = "";
  Serial.println("Entered parser");
  int next_fun = webServer.arg(0).toInt();
  for (int i = 1; i < webServer.args(); i++) {
    if(next_fun < 100){
      web_buffer[i - 1] = webServer.arg(i).toInt();
    }else{
      web_string_buffer[i - 1] = webServer.arg(i);
    }
  }
  Serial.println(next_fun);
  switch (next_fun) {
    case 0://stop arm and set status to 0
      arm.stop_arm();
      arm.status = 0;
    break;
    case 1: //move the arm
       Serial.println("Entered move arm");
       Serial.print(float(web_buffer[0])/10.0f);
       Serial.print(" ");
       Serial.println(float(web_buffer[1])/10.0f);
       arm.move_to(float(web_buffer[0])/10.0f,float(web_buffer[1])/10.0f);
       arm.status = 1;  //this resets the sample arm and makes it active again.
       break;
     case 2://Run the uploaded program
      arm.run_program();
      break;
    case 101: //upload new program!!
      arm.write_program(web_string_buffer[0]);
      message = "program uploaded";
      break;
     case 102: //rename the unit!
      arm.set_name(web_string_buffer[0]);
      message = "Name changed to " + web_string_buffer[0];
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
  webServer.send(200, "text/html",message);
}

//update the arm
void arm_update(){
  arm.calc_motor_settings();
}

void settings_report(){
//  Serial.println(arm.report('l'));
  Serial.println(arm.report('d'));
 // Serial.print(arm.step);
 // Serial.print(" ");
 // Serial.println(arm.steps);
  Serial.println(arm.report('p'));
}

