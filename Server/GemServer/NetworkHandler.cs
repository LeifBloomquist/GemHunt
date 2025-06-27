using System.Net;
using System.Net.Sockets;

namespace GemServer
{
    static internal class NetworkHandler
    {
        private const int PORT = 6420;
        private const int SEND_BUFFER_SIZE=94;

        private static uint player_count = 0;

        // Client to Server
        public const byte PACKET_MOVED = 1;

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

            player_count++;

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
                    //SendUpdate(player);
                }

                // Snooze
                Thread.Sleep(10);  // milliseconds

                if (player.Disconnected) break;
            }

            client.Close();
            player_count--;
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
                    return;
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
            byte[] send_buffer = new byte[SEND_BUFFER_SIZE];

            // Packet Type
            send_buffer[0] = UPDATE;

            // Screen update
            byte[] window_buffer = Maze.GetWindow(player.x, player.y);
            Array.Copy(window_buffer, 0, send_buffer, 1, Maze.MAZE_WINDOW_SIZE);

            // Statistics
            send_buffer[83] = (byte)' ';   // Score
            send_buffer[84] = (byte)'1';   
            send_buffer[85] = (byte)'2';
            send_buffer[86] = (byte)'1';   // Health
            send_buffer[87] = (byte)'2';
            send_buffer[88] = (byte)'3';
            send_buffer[89] = (byte)' ';   // Players
            send_buffer[90] = (byte)' ';
            send_buffer[91] = (byte)(48+player_count);
            send_buffer[92] = 0;           // Sounds
            send_buffer[93] = 0;

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
