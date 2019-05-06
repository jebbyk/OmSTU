public class Main {

    public static void Main()
    {
        System.out.println("Zdrasti");

        Global gl = Global.getInstance("info");
        Global gl0 = Global.getInstance("fake_info");

        System.out.println(gl.name);
        System.out.println(gl0.name);

        gl.name = "updatedInfo";

        Global gl1 = Global.getInstance("tryToDestroy");

        System.out.println(gl1.name);
    }
}
