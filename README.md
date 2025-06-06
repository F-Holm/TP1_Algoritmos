# TP1 Algoritmos y Estructuras de Datos

## Integrantes

- Holm, Federico
- Incutti, Mateo

---

## Aclaraciones:

- Cualquier duda le pasan esto a `ChatGPT` o me preguntan
- Si se les hace dificil utilizar GitHub, pueden mandar por whatsapp las funciones que definan. No manden todo el archivo, solo los cambios
- Todas las funciones deben ser declaradas arriba del main y definida abajo del mismo
- Cada uno debe trabajar en una branch aparte. Esto es porque trabajamos sobre un solo archivo y puede generar muchos conflictos
- cuando terminen de hacer su parte, avisen por el grupo. Si no detectamos ningún error se fusiona con la branch `main`

---

## VS Code

Si quieren utilizar algo más cómodo y moderno que Code::Blocks y Borland y con mensajes de error más claros:

### Si escriben el siguiente comando en la terminal (cmd / powershell) se instalará un compilador moderno para C/C++

```bash
winget install --id LLVM.LLVM --source winget
```

### Extensiones recomendadas

- `C/C++ Extension Pack`
- `vscode-pdf`
- `clangd`

---

## Scripts

Estos scripts utilizan `clang++` y `clang-format` que se instalan con el comando de arriba

- `c.bat` se encarga de compilar `main.cpp` con el compilador clang++ que es más moderno y tiene mensajes de error más claros
- `f.bat` se encarga de darle un formato estándar a `main.cpp`. Suele estar incluido con el compilador clang++
- `l.bat` se encarga de eliminar todos los archivos temporales