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
        public int ControllerID { get; set; }
        public int ServerID { get; set; }
        public string Card { get; set; }
        public FlowMode FlowMode { get; set; }

        ControllerPost() { }

        public static ControllerPost Parse(string value)
        {
            var fields = value.Split(';');

            return new ControllerPost
            {
                ControllerID = int.Parse(fields[0]),
                ServerID = int.Parse(fields[1]),
                Card = fields[2],
                FlowMode = fields[3] == "E" ? FlowMode.Entering : FlowMode.Exiting
            };
        }
    }
}
