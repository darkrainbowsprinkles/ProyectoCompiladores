struct EngineCore
{
    int throttle;
    int temperature;
    bool armed;
};

struct NavigationPack
{
    float altitude;
    float velocity;
    char mode;
    bool locked;
};

int boostLevel = 3;
int safetyMargin = 7;
bool telemetryReady = true;
char autopilotMode = 'A';

int calibrateCore()
{
    int base = 2;
    int factor = 5;
    int total = base + factor * 2;

    if (total > 10 && telemetryReady == true)
    {
        total = total - 1;
    }
    else
    {
        total = total + 1;
    }

    while (total < 20)
    {
        total = total + 2;
        if (total == 16)
        {
            break;
        }
    }

    return total;
}

void runDiagnostics()
{
    int iteration = 0;
    int status = 0;
    int coreResult = calibrateCore();

    print(coreResult);

    for (int i = 0; i < 4; i++)
    {
        iteration = iteration + 1;
        status = status + i;

        if (i == 2)
        {
            status = status + 5;
        }
    }

    do
    {
        iteration = iteration - 1;
        status = status + 1;
    }
    while (iteration > 1);

    switch (status)
    {
        case 0:
            print(status);
            status = status + 1;
            break;
        case 3:
            if (status < 10)
            {
                status = status + 10;
            }
            else
            {
                status = status + 20;
            }
            break;
        default:
            while (status < 25)
            {
                status = status + 4;
            }
            break;
    }

    return;
}

int main()
{
    int initial = 1;
    int processed = 0;
    bool engineOn = true;

    read(initial);

    processed = calibrateCore();
    if (initial < processed || !engineOn)
    {
        processed = processed + boostLevel;
    }
    else if (telemetryReady && engineOn)
    {
        processed = processed + safetyMargin;
    }
    else
    {
        processed = processed - 1;
    }

    runDiagnostics();
    print(processed);

    return processed;
}