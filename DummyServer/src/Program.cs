using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace UFABC.AccessController.Server
{
	class Program
	{
		static void Main(string[] args)
		{
			var server = new ControllerServer(2050);

            while (true)
                server.Listen();
		}
	}

    public class ControllerServer
    {
        readonly IPEndPoint _endPoint;
        Socket _socket;

        public ControllerServer(int port)
        {
            _endPoint = new IPEndPoint(new IPAddress(new byte[] { 0, 0, 0, 0 }), port); //broadcast
        }

        public void Listen()
        {
            _socket = new Socket(SocketType.Stream, ProtocolType.Tcp);
            _socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            _socket.Bind(_endPoint);

            _socket.Listen(5);

            while (listen());
        }

        bool listen()
		{
			_socket.Blocking = true;
			var client = _socket.Accept();
			_socket.Blocking = false;

			var data = getData(client);

			if (data.Contains((byte)'\x0D'))
			{
				var str = Encoding.Default.GetString(data.ToArray());
				
                var message = ControllerPost.Parse(str);
                Console.WriteLine("Controller {0}: {1}", message.ControllerID, str);

				responseAndClose(client, message);

                return true;
			}
			else
            {
				Console.WriteLine("Connection closed");
                return false;
            }
		}

        byte[] getData(Socket socket)
		{
			var data = new List<byte>();
			var buffer = new byte[1024];
			
            while (!data.Contains((byte)'\x0D'))
			{
				if (socket.Poll(5000, SelectMode.SelectRead))
					socket.Receive(buffer);
				
				data.AddRange(buffer.Where(b => b != '\0'));
			}

			return data.ToArray();
		}

        void responseAndClose(Socket client, ControllerPost clientMessage)
		{
            var response = ServerPost.FromClientMessage(clientMessage, true).ToString();
            
            client.Send(Encoding.Default.GetBytes(response));

            Console.WriteLine("Server: {0}", response);
			client.Close();
		}
    }
}