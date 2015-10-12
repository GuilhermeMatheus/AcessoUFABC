using System;
using System.Reflection;
using System.Text.RegularExpressions;
using UFABC.AccessController.Server.Repository;

namespace UFABC.AccessController.Server.Client
{
    public static class CommandParser
    {
        public static ICommand<AccessRepository> Parse(string command)
        {
            var toLower = command.ToLower();

            if (toLower.StartsWith("set-response"))
                return ParseSetResponse(command);

            if (toLower.StartsWith("get-all"))
                return new GetAllCommand();

            if (toLower.StartsWith("drop-response"))
                return ParseDropResponse(command);

            if (toLower == "clear")
                return new ClearCommand<AccessRepository>();

            return new DumbCommand<AccessRepository>();
        }
        private static ICommand<AccessRepository> ParseSetResponse(string command)
        {
            var regex = @"(?:\s*)(?<=[-|/])(?<name>\w*)[:|=](""((?<value>.*?)(?<!\\)"")|(?<value>[\w]*))";
            var matchs = Regex.Match(command, regex);
            
            var id = String.Empty;
            var message = String.Empty;
            var allow = String.Empty;

            while (matchs.Success) { 
                //TODO: Use 'name' and 'value' fields instead doing this dirty job
                foreach (Capture item in matchs.Captures) {
                    var value = item.Value;
                    var str = value.ToLower();

                    if (str.StartsWith("id:"))
                        id = value.Substring(3);
                    else if (str.StartsWith("message:"))
                        message = value.Substring(8);
                    else if (str.StartsWith("allow:"))
                        allow = str.Substring(6);
                }
                matchs = matchs.NextMatch();
            }

            return new SetCommand(id, message, allow == "y");
        }

        private static ICommand<AccessRepository> ParseDropResponse(string command)
        {
            var id = command.ToLower().Replace("drop-response", "").Replace(" ", "");
            return new DropCommand(id);
        }
    }
}