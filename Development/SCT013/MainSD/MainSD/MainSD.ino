
#include <cD.h>
#include <cTimer.h>
#include <cTime.h>
#include <SD/cSDCard.h>
#include <cWIFI.h>
#include <cWIFIClient.h>
#include <cSkipCapScreen.h>
#include <Sensors/cAnalogSensor.h>
#include <WiFiClient.h>
#include <cStringUtils.h>

static cTimer sdLogTimer;
static cAnalogSensor ampMeter;

static String AMP_NUMBER = "2";
static String WIFI_NAME = "servicios";
static String WIFI_PASS = "AJ12_ja99D*";

static String	remoteServerName = "40.76.45.86";
static int		remoteServerPort = 443;

void setup()
{

  ciD.setup();

  ciWIFI.setup(WIFI_NAME, WIFI_PASS, "ECOAMP000" + AMP_NUMBER);

  bool result = false;

  result = skipCapScreen.doSkipCaptiveScreen(remoteServerName,remoteServerPort);

  ampMeter.setup("AMP" + AMP_NUMBER, A0, 100);

  // ciSDCard.setup(4, "EcoAmp");

  sdLogTimer.setup(2000);

}

String fileName;
int connectRetryCount = 0;
void loop(void)
{

  ampMeter.loop();

  if (sdLogTimer.loop())
  {

    String data = "";

    data = ampMeter.getData(1);

	String answer = ciWIFIClient.sendAndReceive(remoteServerName, remoteServerPort, "[BAD][TIMEREQUEST]" + data);
 
	/*String time;

	cStringUtils strutils;

	int count = strutils.fieldsCount(answer,'|');
  
	if (count != 0)
	{
		time = strutils.getField(0,answer,'|');
		String serverRequest = strutils.getField(1, answer, '|');
		if (serverRequest.indexOf("[LOG]"))
		{
			
			ciWIFIClient.sendAndReceive(remoteServerName, remoteServerPort, "[BAD][LOG][SENDLOG NOT IMPLEMENTED]");

		}
	}
	else
	{
		time = answer;
	}
  */

  if(answer == "")
  {
    connectRetryCount++;
    if(connectRetryCount > 30)
    {
     // ESP.reset();
     abort();
     // ciSDCard.setup(4, "EcoAmp");
    }
  }
  
	ciTime.setTime(answer);

	ciD.println("Server result : ");

	ciD.println(answer);

  ciD.println(data);

  // ciSDCard.loop(data);

  }

  ciTime.loop();

  delay(1);

}

