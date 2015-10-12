using System;

namespace UFABC.AccessController.Server.Client
{
    public interface ICommand<TContext>
    {
        void Execute(TContext context);

        bool IsMalformed { get; }
    }

    public class DumbCommand<TContext> : ICommand<TContext>
    {
        public bool IsMalformed
        {
            get { return true;}
        }

        public void Execute(TContext context)
        {
            throw new InvalidOperationException();
        }
    }

    public class ClearCommand<TDumbContext> : ICommand<TDumbContext>
    {
        public bool IsMalformed
        {
            get { return false;}
        }

        public void Execute(TDumbContext context)
        {
            Console.Clear();
        }
    }
}