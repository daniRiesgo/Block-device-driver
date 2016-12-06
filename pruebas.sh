# CLEAN UP BEFORE start
rm -rf salidas
mkdir salidas

# TEST 1: Poner uno a dormir y otros no

# PARTE 1
rm -rf disk_driver.c
cp disk_driverF1.c disk_driver.c
make
./boot 1 > salidas/out_11.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 2
cp disk_driverF2.c disk_driver.c
make
./boot 1 > salidas/out_12.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 3
cp disk_driverF3.c disk_driver.c
make
./boot 1 > salidas/out_13.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# TEST 2: todo normal: tres procesos ejecutando,
# simulando una ejecución normal de un programa con periodos de procesamiento
# entre peticiones

# PARTE 1
cp disk_driverF1.c disk_driver.c
make
./boot 2 > salidas/out_21.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 2
cp disk_driverF2.c disk_driver.c
make
./boot 2 > salidas/out_22.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 3
cp disk_driverF3.c disk_driver.c
make
./boot 2 > salidas/out_23.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# TEST 3: Un solo proceso corriendo, con distintas peticiones

# PARTE 1
cp disk_driverF1.c disk_driver.c
make
./boot 3 > salidas/out_31.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 2
cp disk_driverF2.c disk_driver.c
make
./boot 3 > salidas/out_32.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 3
cp disk_driverF3.c disk_driver.c
make
./boot 3 > salidas/out_33.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# Test 4: Ver que en la parte dos se ordenan bien las peticiones (sólo partes 2 y 3)

# Se crean distintas peticiones en distintos procesos, hacia bloques desordenados
# se espera que, en la cola, todas las peticiones estén siempre ordenadas por índice de bloque

# PARTE 2
cp disk_driverF2.c disk_driver.c
make
./boot 4 > salidas/out_42.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 3
cp disk_driverF3.c disk_driver.c
make
./boot 4 > salidas/out_43.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# tests que sugiere el mail

# TEST 5 : Se escribe y lee bloques correctamente

    #*) Se escribe y lee bloques correctamente > Una posible prueba podría ser:
     # Si ejecutas un cliente que:
     # 1) Rellena un buffer en memoria con 'aaa...'
     # 2) Escribe el buffer en el bloque 10
     # 3) Rellena el bloque en memoria con 'xxx...'
     # 4) Lee el bloque 10 al buffer
     # Entonces el buffer deberá contener todo letras 'a'

# PARTE 1
cp disk_driverF1.c disk_driver.c
make
./boot 5 > salidas/out_51.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 2
cp disk_driverF2.c disk_driver.c
make
./boot 5 > salidas/out_52.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 3
cp disk_driverF3.c disk_driver.c
make
./boot 5 > salidas/out_53.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*



# TEST 6 : Se realizan las peticiones en orden de llegada al driver (y se encolan por tanto)


#   Si uno de los requisitos es:
#       *) Se realizan las peticiones en orden de llegada al driver (y se encolan por tanto)
#       Una posible prueba podría ser:
#
#      Si ejecutas dos clientes que:
#      C1 1) Rellena un buffer en memoria con 'aaa...'
#      C1 2) Escribe el buffer en el bloque 20
#      C1 3) Rellena el bloque en memoria con 'xxx...'
#      C1 4) Lee el bloque 20 al buffer
#
#      C2 1) Rellena un buffer en memoria con 'bbb...'
#      C2 2) El cliente 2 espera usleep(10000) segundos
#      C2 3) Escribe el buffer en el bloque 0
#      C1 4) Rellena el bloque en memoria con 'xxx...'
#      C2 5) El cliente 2 espera usleep(10000) segundos
#      C1 6) Lee el bloque 0 al buffer
#
#      Entonces el buffer del cliente 1 (C1) deberá contener todo letras 'a' y
#      Entonces el buffer del cliente 2 (C2) deberá contener todo letras 'b' y además
#      las peticiones se ordenan de forma que C1/2 -> C2/3 -> C1/4 -> C2/6


# PARTE 1
cp disk_driverF1.c disk_driver.c
make
./boot 6 > salidas/out_61.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 2
cp disk_driverF2.c disk_driver.c
make
./boot 6 > salidas/out_62.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*

# PARTE 3
cp disk_driverF3.c disk_driver.c
make
./boot 6 > salidas/out_63.txt
rm -rf disk_driver.c
make clean
rm -rf /mqueue/*
