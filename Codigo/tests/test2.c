int maxFrames = 60;
int currentFrame = 0;
bool engineReady = false;
int shipVelocity;

// Prueba 1: Funcion VOID con I/O y return vacio
void initGraphicsEngine()
{
    engineReady = true;
    print(engineReady);
    return;
}

// Prueba 2: Funcion con tipo de retorno (INT), lectura de datos y return con operacion
int getShipSpeed()
{
    int inputSpeed;
    
    read(inputSpeed); // Simulamos leer el input del teclado
    
    if (inputSpeed < 0)
    {
        return 0; // Return con valor estatico
    }
    
    return inputSpeed * 2; // Return con operacion matematica
}

// Prueba 3: Llamadas a funciones dentro de estatutos e interrupciones
void updateScene()
{
    if (!engineReady)
    {
        return; // Rompemos la ejecucion si el motor no esta listo
    }
    
    // Asignacion de una variable utilizando el valor de retorno de una funcion
    shipVelocity = getShipSpeed();
    
    currentFrame = currentFrame + 1;
    print(currentFrame);
}

// Prueba 4: Estructura de control llamando a funciones aisladas
void mainRenderLoop()
{
    initGraphicsEngine(); // Llamada a funcion como instruccion solitaria
    
    while (currentFrame < maxFrames && engineReady == true)
    {
        updateScene();
    }
}