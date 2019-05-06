public class Global {

    String name;
    private static Global instance;

    private Global(String _name)
    {
        this.name = _name;
    };

    public static synchronized Global getInstance(String _name)
    {
        if( instance == null)
        {
            instance = new Global(_name);
        }
        return instance;
    }

}
