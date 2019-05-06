abstract class PizzaBuilder {

    Pizza pizza;

    public Pizza getPizza()
    {
        return pizza;
    }



    public abstract void setName();

    public abstract void createSauce();

    public abstract void createStuff();

    public void createNew()
    {
        pizza = new Pizza();
    }
}

class PizzaDerevnya extends PizzaBuilder{
    @Override
    public void setName()
    {
        pizza.name = "Babushkina";
    }
    @Override
    public void createSauce()
    {
        pizza.sauce = "kepchuk";
    }
    @Override
    public void createStuff()
    {
        pizza.stuff = "kalbasa+pamidora";
    }

}

class PizzaObshaga extends PizzaBuilder{
    @Override
    public void setName()
    {
        pizza.name = "Ugli";
    }
    @Override
    public void createSauce()
    {
        pizza.sauce = "Ugli";
    }
    @Override
    public void createStuff()
    {
        pizza.stuff = "vseo+sgorelo";
    }

}