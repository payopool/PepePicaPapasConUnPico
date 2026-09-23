/**
 * @file EngineAPI.h
 * @brief Define las macros de control para la exportación e importación de símbolos en bibliotecas dinámicas (DLL) bajo Windows.
 */

#pragma once

#if defined(_WIN32)
#if defined(ENGINE_BUILD_DLL)
 /**
  * @def ENGINE_API
  * @brief Macro para exportar símbolos (clases, funciones o variables) desde la DLL del motor.
  */
#define ENGINE_API __declspec(dllexport)
#else
 /**
  * @def ENGINE_API
  * @brief Macro para importar símbolos (clases, funciones o variables) hacia una aplicación consumidora de la DLL del motor.
  */
#define ENGINE_API __declspec(dllimport)
#endif
#else
 /**
  * @def ENGINE_API
  * @brief Definición vacía para sistemas operativos distintos de Windows donde no se requiere exportación explícita.
  */
#define ENGINE_API
#endif