# TP1 Algoritmos y Estructuras de Datos

## Integrantes

- Holm, Federico
- Incutti, Mateo
- Almada, Tomas
- Barcala, Santiago
- Cejas, Facundo


---

## Aclaraciones:

- Este archivo no va a estar en la entrega final
- Cualquier duda, le pasan esto a `ChatGPT` o me preguntan
- Si se les hace dificil utilizar GitHub, pueden mandarme por whatsapp las funciones que definan junto con otros cambios. No manden todo el archivo, solo los cambios
- No hagan commits directo en la branch `main`
- Todas las funciones deben ser declaradas arriba de la función `main` y definidas abajo del mismo
- Cada uno debe trabajar en una branch aparte. Esto es porque si trabajamos 10 personas sobre un solo archivo, vamos a generar muchos conflictos
- Cuando terminen de hacer su parte, avisen por el grupo. Si no detectamos ningún error, se fusiona con la branch `main`

---

## VS Code

Si quieren utilizar algo más cómodo, moderno y con mensajes de error más claros que `Code::Blocks` con `Borland`:

[Visual Studio code](https://code.visualstudio.com/download)

Si escriben el siguiente comando en la terminal (`cmd` / `powershell`), se instalará un compilador moderno para `C/C++`

```bash
winget install --id LLVM.LLVM --source winget
```

La versión final la vamos a compilar con `Borland 5.5` en `Code::Blocks`


### Extensiones recomendadas

- `C/C++ Extension Pack`
- `vscode-pdf`
- `clangd`

---

## Scripts

Estos scripts utilizan `clang++` y `clang-format` que se instalan con el comando de arriba

- `c.bat` se encarga de compilar `main.cpp` con el compilador clang++ que es más moderno y tiene mensajes de error más claros
- `f.bat` se encarga de darle un formato estándar a `main.cpp` con `clang-format`
- `l.bat` se encarga de eliminar todos los archivos temporales

---

## Comandos de git

Comandos útilies de git:

```bash
git clone https://github.com/F-Holm/TP1_Algoritmos.git
```
```bash
git add .
```
```bash
git commit -m "<nombre_commit>"
```
```bash
git push origin <nombre_branch>
```
```bash
git reset --hard
```
```bash
git status
```
```bash
git branch <nombre_branch>
```
```bash
git checkout <nombre_branch>
```