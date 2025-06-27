namespace GemServer
{
    internal class Maze
    {
        public const int MAZE_SIZE = 100;
        public const int MAZE_WINDOW_SIZE = 9 * 9;
        public const int MAZE_WINDOW_EDGES = 4;

        public const int WALLCOUNT_MAX = 999;  // Nonsense value for error

        private static byte[,] MazeCells = new byte[MAZE_SIZE, MAZE_SIZE];

        public static bool LoadFile(string path)
        {
            string[] file = File.ReadAllLines(path);

            for (int x = 0; x < MAZE_SIZE; x++)
            {
                for (int y = 0; y < MAZE_SIZE; y++)
                {
                    MazeCells[x, y] = Characters.ASCIItoScreenCode(file[y][x]);
                }
            }

            return true;
        }

        public static bool SaveFile(string path)
        {
            using (StreamWriter outputFile = new StreamWriter(Path.Combine(path, "maze_output.txt")))
            {
                for (int y = 0; y < MAZE_SIZE; y++)
                {
                    for (int x = 0; x < MAZE_SIZE; x++)
                    {
                        outputFile.Write(Characters.ScreenCodeToASCII(MazeCells[x, y]));
                    }
                    outputFile.WriteLine();
                }
            }

            return true;
        }

        public static byte[] GetWindow(int center_x, int center_y)
        {
            int offset = 0;
            byte[] window = new byte[MAZE_WINDOW_SIZE];

            for (int x = (int)(center_x - MAZE_WINDOW_EDGES); x <= center_x + MAZE_WINDOW_EDGES; x++)
            {
                for (int y = (int)(center_y - MAZE_WINDOW_EDGES); y <= center_y + MAZE_WINDOW_EDGES; y++)
                {
                    if ((x < 0) || (x >= MAZE_SIZE) || (y < 0) || (y >= MAZE_SIZE))
                    {
                        window[offset++] = Characters.OUTSIDE;
                    }
                    else
                    {
                        window[offset++] = MazeCells[y, x];
                    }
                }
            }

            return window;
        }

        public static uint GetNeighboringWallCount(uint center_x, uint center_y)
        {
            uint count = 0;

            try
            {
                if (MazeCells[center_x - 1, center_y - 1] == Characters.WALL) count++;
                if (MazeCells[center_x - 0, center_y - 1] == Characters.WALL) count++;
                if (MazeCells[center_x + 1, center_y - 1] == Characters.WALL) count++;
                if (MazeCells[center_x - 1, center_y - 0] == Characters.WALL) count++;
                if (MazeCells[center_x + 1, center_y + 0] == Characters.WALL) count++;
                if (MazeCells[center_x - 1, center_y + 1] == Characters.WALL) count++;
                if (MazeCells[center_x + 0, center_y + 1] == Characters.WALL) count++;
                if (MazeCells[center_x + 1, center_y + 1] == Characters.WALL) count++;

                return count;
            }
            catch (Exception)  // Out of bounds
            {
                return WALLCOUNT_MAX;
            }
        }

        public static bool PlaceTraps(uint count)
        {
            return PlaceItem(Characters.TRAP, count, 0);  // Traps must be fully surrounded by empty space
        }

        public static bool PlaceGems(uint count)
        {
            return PlaceItem(Characters.GEM, count, 4);   // Gems must be in a corner
        }

        private static bool PlaceItem(byte character, uint count, uint spaces_required)
        {
            for (int i = 0; i < count; i++)
            {
                while (true)  // TODO, put an upper limit
                {
                    uint item_x = (uint)Globals.random.NextInt64(MAZE_SIZE);
                    uint item_y = (uint)Globals.random.NextInt64(MAZE_SIZE);

                    // Only on empty cells
                    if (MazeCells[item_x,item_y] != Characters.EMPTY)
                    {
                        continue;
                    }
                   
                    if (GetNeighboringWallCount(item_x, item_y) == spaces_required)
                    {
                        MazeCells[item_x, item_y] = character;
                        break;
                    }
                }
            }

            return true;
        }
    }
}
