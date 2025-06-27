using System.Net.Sockets;

namespace GemServer
{
    internal class Player
    {
        public NetworkStream stream { get; set; }
        public int x { get; set; }
        public int y { get; set; }

        public DateTime LastUpdateTime { get; set; }
        public DateTime LastMoveTime { get; set; }

        public string ID { get; private set; }

        public bool Disconnected { get; set; }

        public Player(NetworkStream stream, string _ID)
        {
            this.stream = stream;
            ID = _ID;
        }

        public void SetPosition(int _x, int _y)
        {
           x = _x;
           y = _y;
        }     
    }
}
