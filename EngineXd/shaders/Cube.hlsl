/**
 * @file Cube.hlsl
 * @brief Sombreador (Shader) HLSL para el renderizado de cubos 3D con transformaciones de matriz y color por vértice.
 */

/**
 * @brief Búfer constante que almacena la matriz de transformación global.
 */
cbuffer TransformBuffer : register(b0)
{
    float4x4 worldViewProjection; ///< Matriz combinada Mundo-Vista-Proyección (WVP) para transformar vértices.
};

/**
 * @struct VSInput
 * @brief Estructura de entrada con los atributos de los vértices para el Vertex Shader.
 */
struct VSInput
{
    float3 position : POSITION; ///< Coordenadas espaciales locales del vértice (X, Y, Z).
    float4 color : COLOR; ///< Color RGBA asignado al vértice.
};

/**
 * @struct PSInput
 * @brief Estructura que define los datos interpolados pasados del Vertex Shader al Pixel Shader.
 */
struct PSInput
{
    float4 position : SV_POSITION; ///< Posición del vértice transformada en el espacio de recorte (Clip Space).
    float4 color : COLOR; ///< Color interpolado resultante para la rasterización.
};

/**
 * @brief Función principal del sombreador de vértices (Vertex Shader).
 * 
 * Transforma la posición local del vértice al espacio de proyección usando la matriz WVP 
 * y transfiere el color hacia el etapa de rasterización.
 * 
 * @param input Datos estructurales del vértice de entrada (@ref VSInput).
 * @return PSInput Datos procesados y listos para el Pixel Shader (@ref PSInput).
 */
PSInput VSMain(VSInput input)
{
    PSInput output;

    output.position =
        mul(
            float4(input.position, 1.0f),
            worldViewProjection
        );

    output.color =
        input.color;

    return output;
}

/**
 * @brief Función principal del sombreador de píxeles (Pixel Shader).
 * 
 * Recibe el color interpolado para cada píxel cubierto por la primitiva geométrica 
 * y lo asigna como color final en el búfer de renderizado.
 * 
 * @param input Datos interpolados del píxel actual (@ref PSInput).
 * @return float4 Color RGBA final del píxel para el objetivo de renderizado (SV_TARGET).
 */
float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}