using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UFABC.AccessController.Server
{
    public class ServerPost
    {
        private ControllerPost c;
        private bool allowAccess;
        private string message;

        private ServerPost() { }

        public static ServerPost FromClientMessage(ControllerPost clientMessage, string message, bool allowAccess)
        {
            return new ServerPost
            {
                c = clientMessage,
                allowAccess = allowAccess,
                message = message
            };
        }

        public override string ToString()
        {
            string[] result = {
                c.ServerID.ToString(),
                c.ControllerID.ToString(),
                "0;51;E",
                allowAccess ? "L" : "B",
                c.Card,
                message,
                "3"
            };

            return String.Join(";", result) + '\x0D';
        }
    }
}