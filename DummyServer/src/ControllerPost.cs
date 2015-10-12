using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UFABC.AccessController.Server
{
    public enum FlowMode
    {
        Entering = 'E',
        Exiting = 'S'
    }

    public class ControllerPost
    {
        public int ControllerID { get; private set; }
        public int ServerID { get; private set; }
        public string Card { get; private set; }
        public FlowMode FlowMode { get; private set; }

        private string _Headers;

        private ControllerPost() { }

        public static ControllerPost Parse(string value)
        {
            var fields = value.Split(';');

            return new ControllerPost
            {
                ControllerID = int.Parse(fields[0]),
                ServerID = int.Parse(fields[1]),
                Card = fields[2],
                FlowMode = fields[3] == "E" ? FlowMode.Entering : FlowMode.Exiting,
                _Headers = value
            };
        }

        public override string ToString()
        {
            return _Headers;
        }
    }
}