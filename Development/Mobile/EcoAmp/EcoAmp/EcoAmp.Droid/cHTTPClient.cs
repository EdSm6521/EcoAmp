using System;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Text;


class cHTTPClient
{
    public string sendCommand(string _command,string _ip,int _port)
    {

        string recStr = "";

        try
        {
            TcpClient tcpc = new TcpClient(_ip, _port);
            NetworkStream stream;

            stream = tcpc.GetStream();

            byte[] sendbuffer = cStringUtils.toByteArray(_command);

            stream.Write(sendbuffer, 0, sendbuffer.Length);

            stream = tcpc.GetStream();
            long timeOut = 0;
            long msecs = DateTime.Now.Second;

            while (!stream.DataAvailable)
            {
                // timeOut++;
                timeOut = (DateTime.Now.Second - msecs);
                // Console.WriteLine("DateTime.Now.Ticks  " + DateTime.Now.Second + " timeOut " + timeOut);
                if (Math.Abs(timeOut) > 4) { Console.WriteLine("connection timeout"); return ""; }
            }

            Console.WriteLine("getting stream");

            try
            {
                for (int r = 0; r < 8000; r++)
                {
                    if (!stream.DataAvailable) { break; }
                    if (!stream.CanRead) { break; }
                    byte ba = (byte)stream.ReadByte();
                    // Console.Write(ba);
                    if (ba == 255) { break; }
                    recStr += (char)ba;
                }
            }
            catch (Exception _e)
            {
                // Console.WriteLine("error : " + _e.ToString());
            }
        }
        catch (Exception _e)
        {
            Console.WriteLine("error : " + _e.ToString());
        }

       //  Console.WriteLine("result : " + recStr);

        return recStr;
    }

}

