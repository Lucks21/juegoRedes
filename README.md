# juegoRedes
1ero compila: 
g++ -o servidor servidor.cpp -pthread
g++ -o cliente cliente.cpp
2do: ve que ip tiene tu pc (linux)
     ifconfig
3ero: ejecuta el servidor 
    ./servidor 2020 <-- aqui colocas el numero que quieras entre 1024 y 65535
4to: ejecutas el cliente en otra consola
    ./cliente 192.168.5.5 (ahi va la primera ip que aparece en el ifconfig) 2020 (el puerto)
