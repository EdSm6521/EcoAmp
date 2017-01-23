
#define SERVER_AP_PORT 80

#include <Arduino.h>

#include "cD.h"
#include "cServer.h"
#include "cTimer.h"
#include "cSkipCapScreen.h"
#include "Sensors/cAnalogSensor.h"
#include "cIOTClient.h"

cAnalogSensor amps;

cTimer updateTime;

static bool ap = false;
static bool cs = true;

void setup()
{
  cd.setup();

  amps.setup("AMPS",A0,100);

  updateTime.setup(5000);

  iotClient.setup("40.76.45.86", "443","BA-IOT-DEVICE-0001","YeKegJsztpa6XeVGUErmgX8ZB6Fc/+D5OKGRv1LCqug=");
  
  ap = Server.setup("BA WiFi","","BA-IOT-DEVICE-0001");

  if(!ap)
  {
    bool csresult = skipCapScreen.doSkipCaptiveScreen(iotClient.ip, iotClient.port.toInt());
    if (csresult)
    {
		cs = false;
    }
	else
	{
		cd.println("Captive Screen ByPass failed Rebooting ...");
		delay(10000);
		ESP.restart();
	}
    
  }

}

bool commandsEnabled = false;

void processCommands()
{
	if (cd.readInt() == '1')
	{
		commandsEnabled = true;
		cd.println("Commands are enabled");
	}
	if (commandsEnabled)
	{
		String result = cd.readString();
		if (result != "")
		{
			cd.println("command: " + result);
			Server.execCommands(result);
			if (result == "consoleoff")
			{
				commandsEnabled = false;
			}
		}
	}
}

void loop()
{

	amps.loop();

	if (updateTime.loop())
	{
		//cd.println(amps.getData());
		//delay(1);
		//return;
		processCommands();
		if(commandsEnabled){return;}
		if(!ap)
		{
  			String result = "";
  			result = amps.getData();
  			iotClient.loop(result);
		}
	}

  delay(1);
}
