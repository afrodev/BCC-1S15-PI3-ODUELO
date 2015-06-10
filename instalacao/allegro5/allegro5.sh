echo "Iniciando instalador Allegro5"
arch=$(uname -m)
if [ "$arch" == "i686" -o "$arch" == "i386" -o "$arch" == "i486" -o "$arch" == "i586" ]; then
flag=1
else
flag=0
fi
echo "Baixando Allegro 5 ..."
git clone git://git.code.sf.net/p/alleg/allegro
echo "Instalando Allegro 5..."
cd allegro
git checkout 5.0
cmake -DCMAKE_INSTALL_PREFIX=/usr .
make
sudo make install
cd ..
echo "Allegro 5 Instalado"