using System;
using UFABC.AccessController.Server.Repository;

namespace UFABC.AccessController.Server.Client
{
    public class SetCommand : ICommand<AccessRepository>
    {
        private string _Id;
        private string _Message;
        private bool _AllowAccess;

        public bool IsMalformed
        {
            get
            {
                return string.IsNullOrEmpty(_Id) || string.IsNullOrEmpty(_Message);
            }
        }

        public SetCommand(string id, string message, bool allowAccess)
        {
            _Id = id;
            _Message = message;
            _AllowAccess = allowAccess;
        }

        public void Execute(AccessRepository context)
        {
            var response = new Response(_Message, _AllowAccess);

            context.SetResponse(_Id, response);

            GetAllCommand.WriteItem(_Id, response);
        }
    }
}