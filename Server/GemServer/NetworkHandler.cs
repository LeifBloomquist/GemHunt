using System.Net;
using System.Net.Sockets;

namespace GemServer
{
    static internal class NetworkHandler
    {
        private const int PORT = 6420;

        // Client to Server
        public const byte PACKET_MOVED = 1;

        // Possible Moves
        public const byte UP = 1;
        public const byte DOWN = 2;
        public const byte LEFT = 3;
        public const byte RIGHT = 4;
        public const byte ACTION = 5;

        // Server to Client (first byte)
        public const byte UPDATE = 128;

        public static readonly TimeSpan UPDATE_PERIOD = TimeSpan.FromMilliseconds(1000);  // Every second

        public static void StartTCP()
        {
            TcpListener listener = new(IPAddress.Any, PORT);
            TcpClient client;
            listener.Start();

            ThreadPool.SetMinThreads(10000, 10000);
            ThreadPool.SetMaxThreads(10000, 10000);

            while (true)
            {
                client = listener.AcceptTcpClient(); // Blocks
                _ = ThreadPool.QueueUserWorkItem(ThreadProcTCP, client);
            }
        }

        private static void ThreadProcTCP(object? obj)
        {
            if (obj == null) return;           

            byte[] buffer = new byte[10];

            TcpClient client = (TcpClient)obj;
            NetworkStream stream = client.GetStream();

            string id = client.Client.RemoteEndPoint.ToString();

            Console.WriteLine("Client connected: [" + id + "] on thread [" + Environment.CurrentManagedThreadId + "]");
            Thread.CurrentThread.Name = "Player Thread [" + id + "]";

            Player player = new(stream, id);
            player.SetPosition(10, 10);

            while (client.Connected)
            {
                if (stream.DataAvailable)  // To not block
                {
                    int bytes = stream.Read(buffer, 0, buffer.Length);

                    if (bytes == 0) break;  // Socket shut down 

                    Console.WriteLine("Received [" +bytes + "] bytes from client");

                    switch (buffer[0])
                    {
                        case PACKET_MOVED:
                            ProcessMove(ref player, buffer[1], buffer[2]);
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

                if (player.Disconnected) break;
            }

            client.Close();
            Console.WriteLine("Client disconnected: [" + player.ID + "]");
        }

        private static void ProcessMove(ref Player player, byte move, byte action)
        {
            switch (move)
            {
                case 1: player.y--; player.x--; break;
                case 2:             player.x--; break;
                case 3: player.y++; player.x--; break;
                case 4: player.y--;             break;
                case 5: player.y++;             break;
                case 6: player.y--; player.x++; break;
                case 7:             player.x++; break;
                case 8: player.y++; player.x++; break;
                default:
                    Console.WriteLine("Unknown player move " + move);
                    break;
            }

            // Bound
            if (player.x < 0) player.x = 0;
            if (player.y < 0) player.y = 0;
            if (player.x >= Maze.MAZE_SIZE) player.x = Maze.MAZE_SIZE-1;
            if (player.y >= Maze.MAZE_SIZE) player.y = Maze.MAZE_SIZE-1;

            player.LastMoveTime = DateTime.Now;
            SendUpdate(player);  // For immediate feedback
        }

        private static void SendUpdate(Player player)
        {
            byte[] send_buffer = new byte[Maze.MAZE_WINDOW_SIZE + 1];

            send_buffer[0] = UPDATE;
            byte[] window_buffer = Maze.GetWindow(player.x, player.y);
            Array.Copy(window_buffer, 0, send_buffer, 1, Maze.MAZE_WINDOW_SIZE);

            try
            {
                player.stream.Write(send_buffer);
            }
                catch (System.IO.IOException)    // Client disconnected
            {
                player.Disconnected = true;
                return;
            }

            player.LastUpdateTime = DateTime.UtcNow;

            Console.WriteLine("Sent update to " + player.ID + " at " + player.LastUpdateTime.ToString());
        }
    }
}
