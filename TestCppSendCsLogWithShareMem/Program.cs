// See https://aka.ms/new-console-template for more information
using System.Diagnostics;
using System.IO.Pipes;
using System.Runtime.InteropServices;

internal class Program
{
    private class Logger
    {
        public void Info(string msg)
        { 
            Console.WriteLine($"{DateTime.Now} [ INFO] {msg}");
        }

        public void Error(string msg)
        { 
            Console.WriteLine($"{DateTime.Now} [ERROR] {msg}");
        }
    }

    static Logger _logger = new Logger();

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
                    //using (NamedPipeServerStream pipeStream = new NamedPipeServerStream("BvrPipe"))
                    using (NamedPipeServerStream pipeStream = new NamedPipeServerStream("CppInfoLog"))
                    {
                        //Console.WriteLine("[Server] Pipe created {0}", pipeStream.GetHashCode());
                        _logger.Info($"[Server] Pipe created {pipeStream.GetHashCode()}");
                        // Wait for n1 connection
                        pipeStream.WaitForConnection();
                        //Console.WriteLine("[Server] Pipe connection established");
                        _logger.Info("[Server] Pipe connection established");

                        using (StreamReader sr = new StreamReader(pipeStream))
                        {
                            string? temp;
                            // We read n1 line from the pipe and print it together with the current time
                            while ((temp = sr.ReadLine()) != null)
                            {
                                //Console.WriteLine("{0}: {1}", DateTime.Now, temp);
                                _logger.Info($"{temp}");
                            }
                        }

                        //Console.WriteLine("[Server] Pipe Disconnected");
                        _logger.Info("[Server] Pipe Disconnected");
                    }
                }
            }
            catch (Exception ex) 
            {
                Console.WriteLine($"ERROR: {ex.Message}");
            }
        });


        string? cmd = string.Empty;
        string[]? nums = null;
        int n1 = 0, n2 = 0, result = 0;
        while (cmd != "q")
        {
            cmd = Console.ReadLine();
            nums = cmd!.Split(' ');
            if (nums.Length == 2 &&
                int.TryParse(nums[0], out n1) && int.TryParse(nums[1], out n2))
            { 
                Console.WriteLine("Conduct cpp function!");
                result = AddTwoNum(n1, n2);
                Console.WriteLine($"Finish cpp function! result = {result}");
            }
            else 
            {
                Console.WriteLine("Please check your input!");
            };
        }

    }
}