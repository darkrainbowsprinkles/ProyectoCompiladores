int maxKeyframes = 100;
int currentFrame = 0;
float rotationSpeed = 2.5;

bool isShipActive = true;
bool hasCollided = false;
bool renderTorus = true;

// Prueba 1: Precedencia matematica estandar
// rotationSpeed * maxKeyframes debe evaluarse ANTES que la suma y resta
float newPosition = currentFrame + rotationSpeed * maxKeyframes - 10 / 2;

// Prueba 2: Precedencia forzada por parentesis
float offsetPosition = (currentFrame + rotationSpeed) * maxKeyframes;

print("Prueba de cadena en test1");
print("Iniciando simulacion de cuadros");

// Prueba 3: Operadores logicos, relacionales y NOT
// El AND (&&) debe agruparse antes que el OR (||)
// El NOT (!) debe resolverse de inmediato sobre renderTorus
if (currentFrame < maxKeyframes && isShipActive == true || !renderTorus) 
{
    currentFrame++;
} 
else if (hasCollided || currentFrame >= 10 && renderTorus) 
{
    isShipActive = false;
}

print("Estado de la nave actualizado");

// Prueba 4: Bucle while con multiples condiciones booleanas
while (isShipActive && !hasCollided) 
{
    currentFrame = currentFrame + 1;
    
    if (currentFrame == 50) 
    {
        hasCollided = true;
    }
}

print("Bucle principal finalizado");

// Prueba 5: For validando logica de interrupcion booleana
for (int i = 0; i < maxKeyframes; i++) 
{
    if (!isShipActive) 
    {
        break;
    }
}