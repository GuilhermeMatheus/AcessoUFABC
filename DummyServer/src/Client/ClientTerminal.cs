using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using UFABC.AccessController.Server.Repository;

namespace UFABC.AccessController.Server.Client
{
    public class ClientTerminal
    {
        private readonly AccessRepository _repository;
        private readonly Thread _worker;

        public ClientTerminal(AccessRepository repository)
        {
            _repository = repository;
            _worker = new Thread(() =>
            {
                while(true)
                    executeNextCommand();
            });
            _worker.IsBackground = false;
        }

        public void StartThread()
        {
            if (_worker.IsAlive)
                throw new InvalidOperationException();

            _worker.Start();
            ConsoleHelper.WriteLine("Client started", ConsoleColor.Blue);
        }

        private void executeNextCommand()
        {
            var command = CommandParser.Parse(Console.ReadLine());

            if (command.IsMalformed)
                ConsoleHelper.WriteLine("Malformed command", ConsoleColor.Red);
            else
                command.Execute(_repository);
        }
    }
}
