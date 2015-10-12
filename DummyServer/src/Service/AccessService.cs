using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using UFABC.AccessController.Server.Repository;

namespace UFABC.AccessController.Server.Service
{
    public class AccessService
    {
        private readonly Task _listener;
        private readonly IPEndPoint _endPoint;

        private Socket _socket;


        public event AccessRequestEventHandler OnAccessRequest = (s,e)=> { };
        public event AccessRespondEventHandler OnRespond = (s, e) => { };

        public AccessService(int port)
        {
            byte[] broadcastIp = { 0, 0, 0, 0 };
            _endPoint = new IPEndPoint(new IPAddress(broadcastIp), port);

            _listener = new Task(() => {
                while (true)
                    listen();
            });
        }

        public Task StartAsync()
        {
            if (_listener.Status == TaskStatus.Running)
                throw new InvalidOperationException();

            _socket = new Socket(SocketType.Stream, ProtocolType.Tcp);
            _socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            _socket.Bind(_endPoint);

            _socket.Listen(5);
            _listener.Start();

            ConsoleHelper.WriteLine("Service started", ConsoleColor.Blue);
            return _listener;
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

                var args = new AccessRequestEventArgs(message);
                OnAccessRequest(this, args);
                
                responseAndClose(client, message, args.Response);

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

        void responseAndClose(Socket client, ControllerPost clientMessage, Response response)
        {
            var serverResponse = ServerPost.FromClientMessage(clientMessage, response.Message, response.AllowAccess);

            client.Send(Encoding.Default.GetBytes(serverResponse.ToString()));
            client.Close();

            OnRespond(this, new AccessRespondEventArgs(serverResponse));
        }
    }
}
