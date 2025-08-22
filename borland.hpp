/*
  Este archivo contiene funciones exclusivas de borlans 5.5 que no están
  disponibles en C++ estándar. Este archivo está creado para poder compilar el
  código con cualquier compilador. No hace falta incluir este header si utilizas
  Borland 5.5 como compilador
*/
#pragma once

#include <cctype>   // std::tolower
#include <cstring>  // std::strlen

// Implementación propia de strlwr (similar a Borland)
// Convierte en minúsculas la cadena C recibida.
// Modifica la cadena original y devuelve un puntero a la misma.
inline char* strlwr(char* str) {
  if (!str)
    return nullptr;
  for (char* p = str; *p; ++p) {
    *p = static_cast<char>(std::tolower(static_cast<unsigned char>(*p)));
  }
  return str;
}

// Implementación estilo "TO_LOWER" para c_str de std::string
// Crea una copia en minúsculas y la devuelve como std::string
#include <string>
inline static std::string to_lower(const std::string& input) {
  std::string result = input;
  for (char& c : result) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }
  return result;
}