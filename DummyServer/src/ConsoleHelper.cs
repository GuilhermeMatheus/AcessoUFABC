using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UFABC.AccessController.Server
{
    public static class ConsoleHelper
    {
        public static void WriteLine(string text, ConsoleColor background = ConsoleColor.Black, ConsoleColor foreground = ConsoleColor.White)
        {
            using (new _ConsoleStateSaver(background, foreground))
                Console.WriteLine(text);
        }

        public static void Write(string text, ConsoleColor background = ConsoleColor.Black, ConsoleColor foreground = ConsoleColor.White)
        {
            using (new _ConsoleStateSaver(background, foreground))
                Console.Write(text);
        }

        private class _ConsoleStateSaver : IDisposable
        {
            private ConsoleColor _background, _foreground;
            public _ConsoleStateSaver(ConsoleColor newBackground, ConsoleColor newForeground)
            {
                _background = Console.BackgroundColor;
                _foreground = Console.ForegroundColor;

                Console.BackgroundColor = newBackground;
                Console.ForegroundColor = newForeground;
            }
            public void Dispose()
            {
                Console.BackgroundColor = _background;
                Console.ForegroundColor = _foreground;
            }
        }
    }
}
