namespace GemServer
{
    internal class Program
    {
        static int Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.WriteLine("Need maze name");
                return -1;
            }

            Console.WriteLine("Loading Maze " + args[0]);
            Maze.LoadFile(args[0]);

            Console.WriteLine("Placing Items ");
            Maze.PlaceGems(200);
            Maze.PlaceTraps(50);

            Maze.SaveFile(@"C:\mazes\");  // For debug

            Console.WriteLine("Starting Server");
            NetworkHandler.StartTCP();

            return 0;
        }
    }
}
