struct FlightControl
{
    int speed;
    int altitude;
    bool enabled;
};

struct PayloadBay
{
    int mass;
    char status;
    bool sealed;
};

int globalCounter = 0;
int targetAltitude = 120;
bool missionActive = true;
bool payloadReady = false;
char missionPhase = 'B';

int computeEnergy()
{
    int base = 8;
    int multiplier = 3;
    int energy = base * multiplier + 4;

    print("Calculando energia de vuelo");

    if (energy >= 28)
    {
        energy = energy - 2;
    }
    else
    {
        energy = energy + 2;
    }

    return energy;
}

void manageFlight()
{
    int cycle = 0;
    int flightEnergy = computeEnergy();

    print("Gestionando secuencia de vuelo");

    while (cycle < 3 && missionActive == true)
    {
        cycle = cycle + 1;
        globalCounter = globalCounter + cycle;

        if (cycle == 1)
        {
            payloadReady = true;
        }
        else if (cycle == 2)
        {
            payloadReady = false;
        }
        else
        {
            payloadReady = true;
        }
    }

    do
    {
        flightEnergy = flightEnergy - 1;
        targetAltitude = targetAltitude + 1;
    }
    while (flightEnergy > 20);

    print("Ajustando modo de mision");

    switch (missionPhase)
    {
        case 'A':
            print(globalCounter);
            break;
        case 'B':
            if (missionActive && payloadReady == false)
            {
                targetAltitude = targetAltitude + 5;
            }
            else
            {
                targetAltitude = targetAltitude + 10;
            }
            break;
        default:
            while (globalCounter < 3)
            {
                globalCounter = globalCounter + 1;
            }
            break;
    }

    return;
}

int missionControl()
{
    int result = 0;
    int localValue = 5;

    print("Iniciando control de mision");
    result = computeEnergy();
    result = result + localValue;

    if (result > targetAltitude && payloadReady == false || missionActive)
    {
        result = result + 1;
    }

    for (int step = 0; step < 5; step++)
    {
        if (step == 4)
        {
            break;
        }

        result = result + step;
    }

    manageFlight();
    print(result);
    print("Control de mision completado");

    return result;
}