using System.Net.Sockets;

namespace GemServer
{
    internal class Player
    {
        public NetworkStream stream { get; set; }
        public uint x { get; private set; }
        public uint y { get; private set; }

        public DateTime LastUpdateTime { get; set; }
        public DateTime LastMoveTime { get; set; }


        public Player(NetworkStream stream)
        {
            this.stream = stream;
        }

        public void SetPosition(uint _x, uint _y)
        {
           x = _x;
           y = _y;
        }     
    }
}
