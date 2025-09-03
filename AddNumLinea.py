# Script: numerar_lineas.py

def main():
    # Nombre del archivo .cpp a leer (cambiá este valor por el tuyo)
    input_file = "TP1V2_K1021G10_HOLM FEDERICO.cpp"
    output_file = "resultado.txt"

    with open(input_file, "r", encoding="utf-8") as f:
        lineas = f.readlines()

    with open(output_file, "w", encoding="utf-8") as f:
        for i, linea in enumerate(lineas, start=1):
            f.write(f"{i:03}: {linea}")

if __name__ == "__main__":
    main()
