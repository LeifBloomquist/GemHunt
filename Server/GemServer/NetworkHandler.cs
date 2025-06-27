using System.Drawing;
using System.Net;
using System.Net.Sockets;

namespace GemServer
{
    static internal class NetworkHandler
    {
        private const int PORT = 6420;
        private const int SEND_BUFFER_SIZE=94;

        private static int player_count = 0;

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
            var (x, y) = Maze.FindRandomEmptySpace(0); // Surrounded by empty space
            player.SetPosition(y, x);

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
            player_count--;
            Console.WriteLine("Client disconnected: [" + player.ID + "]");
        }

        private static void ProcessMove(ref Player player, byte move, byte action)
        {
            int temp_x = player.x;
            int temp_y = player.y;

            switch (move)
            {
                case Globals.DIRECTION_NW:    temp_y--; temp_x--; break;
                case Globals.DIRECTION_NORTH:           temp_x--; break;
                case Globals.DIRECTION_NE:    temp_y++; temp_x--; break;
                case Globals.DIRECTION_WEST:  temp_y--;           break;
                case Globals.DIRECTION_EAST:  temp_y++;           break;
                case Globals.DIRECTION_SW:    temp_y--; temp_x++; break;
                case Globals.DIRECTION_SOUTH: temp_x++;           break;
                case Globals.DIRECTION_SE:    temp_y++; temp_x++; break;
                default:
                    Console.WriteLine("Unknown player move " + move);
                    return;
            }

            // Bound
            if (temp_x < 0) temp_x = 0;
            if (temp_y < 0) temp_y = 0;
            if (temp_x >= Maze.MAZE_SIZE) temp_x = Maze.MAZE_SIZE-1;
            if (temp_y >= Maze.MAZE_SIZE) temp_y = Maze.MAZE_SIZE-1;

            // Move allowed?
            if (!Maze.CanEnterCell(temp_y,temp_x))
            {
                return;
            }

            // Move was OK
            player.x = temp_x;
            player.y = temp_y;

            // Any points?
            int points = Maze.ScorePoints(temp_y, temp_x);
            if (points > 0)
            {
                player.score += points;
                // TODO sound
            }

            // Any ill effects?
            int damage = Maze.TakeDamage(temp_y, temp_x);
            if (damage > 0)
            {
                player.health -= damage;
                // TODO sound
            }

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
            send_buffer[83] = Int2Char(player.score, 0);
            send_buffer[84] = Int2Char(player.score, 1);
            send_buffer[85] = Int2Char(player.score, 2);
            send_buffer[86] = Int2Char(player.health, 0);
            send_buffer[87] = Int2Char(player.health, 1);
            send_buffer[88] = Int2Char(player.health, 2);
            send_buffer[89] = Int2Char(player_count, 0);
            send_buffer[90] = Int2Char(player_count, 1);
            send_buffer[91] = Int2Char(player_count, 2);
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

        private static byte Int2Char(int value, int position)
        {
            string myString = value.ToString().PadLeft(3, ' ');
            return (byte)myString[position];
        }
    }
}
