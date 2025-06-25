namespace GemServer
{
    internal static class Characters
    {
        // VIC Screen Character Codes
        public const byte PLAYER  = 81;   // filled circle
        public const byte GEM     = 90;   // diamond
        public const byte SPIDER  = 94;   // pi
        public const byte GREMLIN = 88;   // club

        public const byte OUTSIDE = 127;  // big checkerboard
        public const byte WALL    = 160;  // block
        public const byte EMPTY   = 32;   // empty
        public const byte SMOKE   = 102;  // checkerboard
        public const byte TRAP    = 214;  // reverse x

        internal static byte ASCIItoScreenCode(char v)
        {
            switch (v)
            {
                case '#': return WALL;
                case ' ': return EMPTY;
                default: return EMPTY;
            }
        }

        internal static char ScreenCodeToASCII(byte b)
        {
            switch (b)
            {
                case WALL: return '#';
                case EMPTY: return ' ';
                case GEM: return 'O';
                case TRAP: return 'X';
                default: return '.';
            }
        }
    }
}
