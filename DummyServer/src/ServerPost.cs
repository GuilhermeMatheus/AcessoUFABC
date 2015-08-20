using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UFABC.AccessController.Server
{
    public class ServerPost
    {
        ControllerPost c;
        bool allowAccess;

        ServerPost() { }

        public static ServerPost FromClientMessage(ControllerPost clientMessage, bool allowAccess)
        {
            return new ServerPost
            {
                c = clientMessage,
                allowAccess = allowAccess
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
               (allowAccess ? "ACESSO PERMITIDO" : "ACESSO NEGADO").PadLeft(32, '-'),
                "3"
            };

            return String.Join(";", result) + '\x0D';
        }
    }
}