/**
 * @file Cube.hlsl
 * @brief Sombreador (Shader) HLSL para el pase directo de vértices y color sin transformaciones de matriz.
 */

/**
 * @struct VSInput
 * @brief Estructura de entrada con los atributos originales de los vértices para el Vertex Shader.
 */
struct VSInput
{
    float3 position : POSITION; ///< Coordenadas espaciales locales del vértice (X, Y, Z).
    float4 color : COLOR; ///< Color RGBA asignado al vértice.
};

/**
 * @struct VSOutput
 * @brief Estructura que define los datos transferidos e interpolados desde el Vertex Shader hacia el Pixel Shader.
 */
struct VSOutput
{
    float4 position : SV_POSITION; ///< Posición espacial del vértice convertida a coordenadas homogéneas (SV_POSITION).
    float4 color : COLOR; ///< Color interpolado que se pasará a la etapa de rasterización.
};

/**
 * @brief Función principal del sombreador de vértices (Vertex Shader).
 * 
 * Convierte las coordenadas espaciales tridimensionales a un vector homogéneo de cuatro componentes 
 * y reenvía el color del vértice directamente hacia la estructura de salida.
 * 
 * @param input Datos estructurales del vértice de entrada (@ref VSInput).
 * @return VSOutput Datos transformados listos para la etapa de rasterización y el Pixel Shader (@ref VSOutput).
 */
VSOutput VSMain(VSInput input)
{
    VSOutput output;

    output.position = float4(input.position, 1.0f);
    output.color = input.color;

    return output;
}

/**
 * @brief Función principal del sombreador de píxeles (Pixel Shader).
 * 
 * Recibe el color interpolado para cada píxel de la primitiva geométrica 
 * y lo devuelve como el color final que se pintará en el objetivo de renderizado.
 * 
 * @param input Datos interpolados del píxel actual provenientes del Vertex Shader (@ref VSOutput).
 * @return float4 Color RGBA final asignado al píxel (SV_TARGET).
 */
float4 PSMain(VSOutput input) : SV_TARGET
{
    return input.color;
}