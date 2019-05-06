import java.util.ArrayList;
import java.util.List;

public class Director {
    PizzaBuilder builder;

    public void setBuilder(PizzaBuilder pb){builder = pb;}

    public void craftPizza()
    {
        builder.createNew();
        builder.setName();
        builder.createStuff();
        builder.createSauce();
    }

    public Pizza getPizza()
    {
       return builder.getPizza();
    }
}
