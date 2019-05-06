public class Main {

    public static void main(String[] args) {
        System.out.println("Zdrasti");

        Global gl = Global.getInstance("info");
        Global gl0 = Global.getInstance("fake_info");

        System.out.println(gl.name);
        System.out.println(gl0.name);

        gl.name = "updatedInfo";

        Global gl1 = Global.getInstance("tryToDestroy");

        System.out.println(gl1.name);







        PizzaBuilder pd = new PizzaDerevnya();
        PizzaBuilder po = new PizzaObshaga();

        Director d = new Director();
        d.setBuilder(pd);
        d.craftPizza();
        Pizza p = d.getPizza();
        System.out.println(p.getName());
        System.out.println(p.getSauce());
        System.out.println(p.getStuff());
        d.setBuilder(po);
        d.craftPizza();
        p = d.getPizza();
        System.out.println(p.getName());
        System.out.println(p.getSauce());
        System.out.println(p.getStuff());



    }
}
