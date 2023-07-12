// See https://aka.ms/new-console-template for more information
using System.Diagnostics;
using System.IO.Pipes;
using System.Runtime.InteropServices;

internal class Program
{
    [DllImport("CppDll.dll", EntryPoint = "AddTwoNum", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    static extern int AddTwoNum(int a, int b);

    private static void Main(string[] args)
    {
        Console.WriteLine("Hello, World!");

        Task.Run(() =>
        {
            try
            { 
                for (; ; )
                {
                    using (NamedPipeServerStream pipeStream = new NamedPipeServerStream("BvrPipe"))
                    {
                        Console.WriteLine("[Server] Pipe created {0}", pipeStream.GetHashCode());
                        // Wait for a connection
                        pipeStream.WaitForConnection();
                        Console.WriteLine("[Server] Pipe connection established");

                        using (StreamReader sr = new StreamReader(pipeStream))
                        {
                            string? temp;
                            // We read a line from the pipe and print it together with the current time
                            while ((temp = sr.ReadLine()) != null)
                            {
                                Console.WriteLine("{0}: {1}", DateTime.Now, temp);
                            }
                        }

                        Console.WriteLine("[Server] Pipe Disconnected");
                    }
                }
            }
            catch (Exception ex) 
            {
                Console.WriteLine($"ERROR: {ex.Message}");
            }
        });


        string? cmd = string.Empty;
        while (cmd != "q")
        {
            cmd = Console.ReadLine();
            Console.WriteLine("Conduct cpp function");
            AddTwoNum(1, 2);
            Console.WriteLine("Finish cpp function");
        }

    }
}