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
    [DllImport("CppDll.dll", EntryPoint = "WriteErrorLog", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
    static extern void WriteErrorLog();

    private static void Main(string[] args)
    {
        Console.WriteLine("Hello, World!");

        CreateTaskReadCppSharedMemoryStream();

        string? cmd = string.Empty;
        string[]? nums = null;
        int n1 = 0, n2 = 0, result = 0;
        while (cmd != "q")
        {
            cmd = Console.ReadLine();
            if (cmd == "e")
            {
                WriteErrorLog();
            }
            else
            { 
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

            Console.WriteLine();
        }

    }

    const string _cppInfoLog = "CppInfoLog";
    const string _cppErrorLog ="CppInfoError";
    static void CreateTaskReadCppSharedMemoryStream()
    { 
        Task.Run(() =>
        {
            try
            { 
                for (; ; )
                {
                    using (NamedPipeServerStream pipeStream = new NamedPipeServerStream(_cppInfoLog))
                    {
                        _logger.Info($"[Server] {_cppInfoLog} Pipe created {pipeStream.GetHashCode()}");
                        // Wait for n1 connection
                        pipeStream.WaitForConnection();
                        _logger.Info($"[Server] {_cppInfoLog} Pipe connection established");

                        using (StreamReader sr = new StreamReader(pipeStream))
                        {
                            string? temp;
                            // We read n1 line from the pipe and print it together with the current time
                            while ((temp = sr.ReadLine()) != null)
                            {
                                _logger.Info($"{temp}");
                            }
                        }

                        _logger.Info($"[Server] {_cppInfoLog} Pipe Disconnected");
                    }
                }
            }
            catch (Exception ex) 
            {
                Console.WriteLine($"In read {_cppInfoLog} Task, ERROR: {ex.Message}");
            }
        });


        Task.Run(() =>
        {
            try
            { 
                for (; ; )
                {
                    using (NamedPipeServerStream pipeStream = new NamedPipeServerStream(_cppErrorLog))
                    {
                        _logger.Error($"[Server] {_cppErrorLog} Pipe created {pipeStream.GetHashCode()}");
                        // Wait for n1 connection
                        pipeStream.WaitForConnection();
                        _logger.Error($"[Server] {_cppErrorLog} Pipe connection established");

                        using (StreamReader sr = new StreamReader(pipeStream))
                        {
                            string? temp;
                            // We read n1 line from the pipe and print it together with the current time
                            while ((temp = sr.ReadLine()) != null)
                            {
                                _logger.Error($"{temp}");
                            }
                        }

                        _logger.Error($"[Server] {_cppErrorLog} Pipe Disconnected");
                    }
                }
            }
            catch (Exception ex) 
            {
                Console.WriteLine($"In read CppInfoError Task, ERROR: {ex.Message}");
            }
        });
    }
}