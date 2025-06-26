using System.Net.Sockets;

namespace GemServer
{
    internal class Player
    {
        public NetworkStream stream { get; set; }
        public uint x { get; set; }
        public uint y { get; set; }

        public DateTime LastUpdateTime { get; set; }
        public DateTime LastMoveTime { get; set; }

        public string ID { get; private set; }

        public bool Disconnected { get; set; }

        public Player(NetworkStream stream, string _ID)
        {
            this.stream = stream;
            ID = _ID;
        }

        public void SetPosition(uint _x, uint _y)
        {
           x = _x;
           y = _y;
        }     
    }
}
