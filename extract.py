import pandas as pd

# ======================================================================
# CONFIGURACIÓN SEGÚN TU SCRIPT BASH
# ======================================================================
sizes = [400, 800, 1200, 1600]
mpi_processes = [2, 4, 6, 8]
num_repetitions = 10

# ======================================================================
# LECTURA DE ARCHIVOS
# ======================================================================

def read_times(filename):
    """Lee un archivo .doc y devuelve una lista de tiempos (uno por línea)."""
    with open(filename, "r") as f:
        return [float(line.strip()) for line in f if line.strip()]


mpi_times = read_times("mpi/results_mpi.doc")
seq_times = read_times("mpi/results_seq.doc")

# ======================================================================
# PARSEAR RESULTADOS MPI
# ======================================================================
mpi_records = []
index = 0

for size in sizes:
    for n in mpi_processes:
        for rep in range(1, num_repetitions + 1):
            time = mpi_times[index]
            mpi_records.append({
                "size": size,
                "n_processes": n,
                "rep": rep,
                "time": time
            })
            index += 1

df_mpi = pd.DataFrame(mpi_records)

# ======================================================================
# PARSEAR RESULTADOS SECUENCIALES
# ======================================================================
seq_records = []
index = 0

for size in sizes:
    for rep in range(1, num_repetitions + 1):
        time = seq_times[index]
        seq_records.append({
            "size": size,
            "rep": rep,
            "time": time
        })
        index += 1

df_seq = pd.DataFrame(seq_records)

# ======================================================================
# GUARDAR EN EXCEL
# ======================================================================
output_file = "resultados.xlsx"
with pd.ExcelWriter(output_file) as writer:
    df_mpi.to_excel(writer, sheet_name="MPI", index=False)
    df_seq.to_excel(writer, sheet_name="SEQUENTIAL", index=False)

print("Archivo Excel generado:", output_file)
