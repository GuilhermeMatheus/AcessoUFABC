using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UFABC.AccessController.Server.Repository;

namespace UFABC.AccessController.Server.Service
{
    public class AccessRequestEventArgs : EventArgs
    {
        public ControllerPost Message { get; private set; }

        public bool Handled { get; set; }

        public Response Response { get; set; }

        public AccessRequestEventArgs(ControllerPost message)
        {
            this.Message = message;
        }
    }

    public class AccessRespondEventArgs : EventArgs
    {
        public ServerPost ServerResponse { get; private set; }

        public AccessRespondEventArgs(ServerPost serverResponse)
        {
            ServerResponse = serverResponse;
        }
    }

    public delegate void AccessRequestEventHandler(object sender, AccessRequestEventArgs e);
    public delegate void AccessRespondEventHandler(object sender, AccessRespondEventArgs e);
}
