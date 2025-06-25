using System.Net;
using System.Net.Sockets;

namespace GemServer
{
    static internal class NetworkHandler
    {
        // Client to Server
        public const byte MOVED = 1;

        // Possible Moves
        public const byte UP = 1;
        public const byte DOWN = 2;
        public const byte LEFT = 3;
        public const byte RIGHT = 4;
        public const byte ACTION = 5;

        // Server to Client (first byte)
        public const byte UPDATE = 128;

        public static readonly TimeSpan UPDATE_PERIOD = TimeSpan.FromMilliseconds(1000);

        public static void Start()
        {
            TcpListener listener = new(IPAddress.Any, 8000);
            TcpClient client;
            listener.Start();

            ThreadPool.SetMinThreads(10000, 10000);
            ThreadPool.SetMaxThreads(10000, 10000);

            while (true)
            {
                client = listener.AcceptTcpClient(); // Blocks
                _ = ThreadPool.QueueUserWorkItem(ThreadProc, client);
            }
        }

        private static void ThreadProc(object? obj)
        {
            if (obj == null) return;           

            byte[] buffer = new byte[10];

            TcpClient client = (TcpClient)obj;
            NetworkStream stream = client.GetStream();

            Console.WriteLine("Client connected: [" + client.Client.RemoteEndPoint + "] on thread [" + Environment.CurrentManagedThreadId + "]");
            Thread.CurrentThread.Name = "Player Thread [" + client.Client.RemoteEndPoint + "]";

            Player player = new(stream);
            player.SetPosition (10,10);  // TODO Random

            while (client.Connected)
            {
                if (stream.DataAvailable)  // To not block
                {
                    int bytes = stream.Read(buffer, 0, buffer.Length);

                    if (bytes == 0) break;  // Socket shut down 

                    switch (buffer[0])
                    {
                        case MOVED:
                            ProcessMove(ref player, buffer[1]);
                            break;

                        default:
                            Console.WriteLine("Unknown player command " + (int)buffer[0]);
                            break;
                    }
                }                

                // Check if periodic update needed
                TimeSpan delta = DateTime.UtcNow - player.LastUpdateTime;
                if (delta >= UPDATE_PERIOD)
                {
                    SendUpdate(player);
                }

                // Snooze
                Thread.Sleep(10);  // milliseconds
            }

            client.Close();
            Console.WriteLine("Client Disconnected: [" + client.Client.RemoteEndPoint + "]");
        }

        private static void ProcessMove(ref Player player, byte command)
        {
            player.LastMoveTime = DateTime.Now;
        }

        private static void SendUpdate(Player player)
        {
            byte[] send_buffer = new byte[Maze.MAZE_WINDOW_SIZE + 1];

            send_buffer[0] = UPDATE;
            byte[] window_buffer = Maze.GetWindow(player.x, player.y);
            Array.Copy(window_buffer, 0, send_buffer, 1, Maze.MAZE_WINDOW_SIZE);

            player.stream.Write(send_buffer);

            player.LastUpdateTime = DateTime.UtcNow;
        }
    }
}
