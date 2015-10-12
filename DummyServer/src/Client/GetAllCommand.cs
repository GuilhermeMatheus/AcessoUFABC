using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using UFABC.AccessController.Server.Repository;

namespace UFABC.AccessController.Server.Client
{
    public class GetAllCommand : ICommand<AccessRepository>
    {
        public bool IsMalformed
        {
            get { return false; }
        }

        public void Execute(AccessRepository context)
        {
            var items = context.GetAll();

            //           "|1234567890123456|1234567|12345678901234567890123456789012|";
            var seprtr = "+----------------+-------+--------------------------------+";
            var header = "|       ID       | Allow |             Message            |";
            
            Console.WriteLine(seprtr);
            Console.WriteLine(header);
            Console.WriteLine(seprtr);

            foreach (var item in items)
                WriteItem(item.Key, item.Value);

            Console.WriteLine(seprtr);
        }

        //I know, 'internal static' for this is very, very ugly 
        internal static void WriteItem(string key, Response value)
        {
            var pipe = '|';

            var id = $"{key,16}";
            var allow = $"{(value.AllowAccess ? 'Y' : 'N'),4}   ";
            var message = $"{value.Message,-32}";

            Console.Write(pipe);
            ConsoleHelper.Write(id, ConsoleColor.Yellow, ConsoleColor.DarkBlue);
            Console.Write(pipe);

            if (value.AllowAccess)
                ConsoleHelper.Write(allow, foreground: ConsoleColor.Blue);
            else
                ConsoleHelper.Write(allow, foreground: ConsoleColor.Red);

            Console.Write(pipe);
            Console.Write(message);
            Console.Write(pipe + "\n");
        }
    }
}