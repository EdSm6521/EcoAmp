
using System;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using Android.Graphics.Drawables.Shapes;
using System.Threading.Tasks;

namespace EcoAmp.Droid
{
    
	[Activity (Label = "EcoAmp.Droid", MainLauncher = true, Icon = "@drawable/icon")]
	public class MainActivity : Activity
	{

		int count = 1;
        static string resultClientData = "";
        static string lastResultClientData = "";
        static cHTTPClient client = new cHTTPClient();
        static MyOvalShape mos;
        protected override void OnCreate (Bundle bundle)
		{

			base.OnCreate (bundle);

			// Set our view from the "main" layout resource
			SetContentView (Resource.Layout.Main);

			// Get our button from the layout resource,
			// and attach an event to it
			Button button = FindViewById<Button> (Resource.Id.myButton);
			
			button.Click += delegate
            {
				button.Text = string.Format ("{0} clicks!", count++);
			};

            mos = new MyOvalShape(this);
            SetContentView(mos);

            clientThread();

        }

        async void clientThread()
        {
            try
            {
                await Task.Run(async () => { await ThreadProc(); });
            }
            catch (Exception _e)
            {
                Console.WriteLine(_e.ToString());
            }
        }

        async Task<int> ThreadProc()
        {

            while (true)
            {
                try
                {
                    // mos.setAmpValue((float)new Random().NextDouble());
                    if (resultClientData == "")
                    {
                        resultClientData = client.sendCommand("[LOG][TAG,[BAD][TIMEREQUEST]|AMP1|", "40.76.45.86", 443);
                    }

                    if (resultClientData != "")
                    {
                        Console.WriteLine("getting data " + resultClientData);
                        if (resultClientData == lastResultClientData) { resultClientData = ""; continue; }
                        mos.clearAmpValues();
                        string [] lines = resultClientData.Split('\r');
                        for (int r = 0; r < lines.Length; r++)
                        {
                            
                            if (!lines[r].Contains("AMP1")) { continue; }
                            string [] items = lines[r].Split('|');
                            
                            if (items.Length >= 2)
                            {
                                
                                float value = 0.0f;
                                try
                                {
                                    value = Convert.ToSingle(items[2].Replace('.',(char)0));
                                    mos.setAmpValue(value);
                                }
                                catch (Exception _e)
                                {

                                }
                            }
                            
                        }
                        lastResultClientData = resultClientData;
                        resultClientData = "";
                    }
                    await Task.Delay(100);
                }
                catch (Exception _e)
                {
                    Console.WriteLine(_e.ToString());
                }

            }
            return 0;
        }

    }

}


