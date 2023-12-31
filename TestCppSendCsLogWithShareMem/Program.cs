﻿// See https://aka.ms/new-console-template for more information
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

    class PipeCategory
    { 
        public const string CppInfoLog = "CppInfoLog";
        public const string CppErrorLog ="CppInfoError";
    }

    private class PipeStreamReader
    {
        string _pipeName = "";
        TaskCompletionSource<bool> _isFinished;
        bool _isOpened = false;
        public PipeStreamReader(string pipeName)
        {
            _pipeName = pipeName;
            _isFinished = new TaskCompletionSource<bool>(false);
        }

        public async Task Open()
        {
            if (_isOpened)
                return;

            _ = Task.Run(() => BeginReadPipeStream(_pipeName));

            if (!_isFinished.Task.IsCompleted)
                await _isFinished.Task;

            _isOpened = true;
        }

        private void BeginReadPipeStream(string name)
        {
            if (System.IO.File.Exists($"\\\\.\\pipe\\{name}"))
            {
                Console.WriteLine($"pipe {name} already opened");
                _isFinished.TrySetResult(true);
                return;
            }

            try
            { 
                for (; ; )
                {
                    ReadPipeStream(name);
                }
            }
            catch (Exception ex) 
            {
                Console.WriteLine($"In read {name} Task, ERROR: {ex.Message}");
            }
        }

        private void ReadPipeStream(string name) 
        {
            using (NamedPipeServerStream pipeStream = new NamedPipeServerStream(name))
            {
                _logger.Info($"[Server] {name} Pipe created {pipeStream.GetHashCode()}");
                _isFinished.SetResult(true);
                // Wait for n1 connection
                pipeStream.WaitForConnection();
                _logger.Info($"[Server] {name} Pipe connection established");

                using (StreamReader sr = new StreamReader(pipeStream))
                {
                    string? temp;
                    // We read n1 line from the pipe and print it together with the current time
                    while ((temp = sr.ReadLine()) != null)
                    {
                        if (name == PipeCategory.CppErrorLog)
                            _logger.Error($"{temp}");
                        else
                            _logger.Info($"{temp}");
                    }
                }

                _logger.Info($"[Server] {name} Pipe Disconnected");
            }
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

        _ = CreateTaskReadCppSharedMemoryStream();

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

    static async Task CreateTaskReadCppSharedMemoryStream()
    {
        PipeStreamReader reader = new PipeStreamReader(PipeCategory.CppInfoLog);
        PipeStreamReader reader2 = new PipeStreamReader(PipeCategory.CppErrorLog);

        await reader.Open();
        await reader2.Open();
    }
}