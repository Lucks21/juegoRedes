# Compilador
CXX = g++

# Compilación de servidor
servidor: servidor.cpp
	g++ -o servidor servidor.cpp -pthread

# Limpiar ejecutables
clean:
	rm -f servidor

# Obtener Ip de tu PC
Si está usando una distribucion basada en Debian/Ubuntu primero debera ejecutar estos comandos:

sudo apt update
sudo apt install net-tools

Si está usando una distribucion basada en Red Hat/Fedora primero debera ejecutar este comando:

sudo yum install net-tools

Luego ejecuta: 

ifconfig

Busca la primera dirección IP que aparece en la salida del comando

# Ejecutar el servidor 
./servidor <puerto>


