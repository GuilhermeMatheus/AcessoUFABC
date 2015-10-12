using System;

namespace UFABC.AccessController.Server.Repository
{
    public struct Response
    {
        public string Message { get; }
        public bool AllowAccess { get; }

        public Response(string message = null, bool allowAccess = false)
        {
            Message = message;
            AllowAccess = allowAccess;
        }
        
    }
}