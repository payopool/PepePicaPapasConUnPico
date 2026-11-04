/**
 * @file MeshComponent.h
 * @brief Declaración de la clase MeshComponent para el almacenamiento de datos geométricos, vértices, índices y transformaciones locales.
 */

#pragma once

#include "Engine/Prerequisitos.h"

 /**
  * @class MeshComponent
  * @brief Representa un componente de malla 3D que contiene geometría, vértices, índices y una matriz de transformación local.
  */
class MeshComponent
{
public:
  /**
   * @brief Construye una nueva instancia de MeshComponent inicializando los contadores de vértices e índices a cero.
   */
  MeshComponent() :
    m_numVertex(0),
    m_numIndex(0)
  {}

public:
  std::string m_name; ///< Nombre identificador de la malla.

  DirectX::XMFLOAT4X4 m_localtransform = DirectX::XMFLOAT4X4(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  ); ///< Matriz de transformación local de la malla (por defecto, la matriz identidad).

  std::vector<Vertex> m_vertex;         ///< Lista de vértices que componen la geometría de la malla.
  std::vector<unsigned int> m_index;    ///< Lista de índices para el ensamblaje de primitivas (índices de triángulos).
  int m_numVertex;                      ///< Número total de vértices en la malla.
  int m_numIndex;                       ///< Número total de índices en la malla.
};