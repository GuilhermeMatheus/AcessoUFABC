using System;
using UFABC.AccessController.Server.Repository;

namespace UFABC.AccessController.Server.Client
{
    public class DropCommand : ICommand<AccessRepository>
    {
        private string _Id;

        public DropCommand(string id)
        {
            _Id = id;
        }

        public bool IsMalformed
        {
            get
            {
                return string.IsNullOrEmpty(_Id);
            }
        }

        public void Execute(AccessRepository context)
        {
            Response dropped;

            if (context.TryGet(_Id, out dropped)) {
                context.Delete(_Id);
                ConsoleHelper.WriteLine("DROPPED: ", ConsoleColor.DarkRed, ConsoleColor.Black);
                GetAllCommand.WriteItem(_Id, dropped);
            }
            else {
                ConsoleHelper.WriteLine($"'{_Id}' not found", ConsoleColor.DarkRed, ConsoleColor.Black);
            }
        }
    }
}