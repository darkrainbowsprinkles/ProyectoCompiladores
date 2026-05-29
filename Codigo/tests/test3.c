int helper()
{
    print("Calculando valor auxiliar");
    return 4;
}

int main()
{
    int a = 2;
    int b = 3;
    int c = a + b;
    int d = c * 4;
    bool flag = true;

    print("Inicio de test de expresiones");
    print(d);

    if (1 && 0 || !0)
    {
        d = d + 0;
        a = a * 1;
    }
    else
    {
        d = 99;
    }

    print("Bloque condicional evaluado");

    while (0)
    {
        a = a + 1;
        d = d + 2;
    };

    do
    {
        b = b + 1;
    }
    while (false);

    print("Bucle do-while completado");

    c = helper();
    print(c);

    print("Fin de test principal");

    return d;
    a = 100;
}
