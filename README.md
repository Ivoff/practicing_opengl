## Requerimentos
* cmake
* xorg-dev

## Setup
```
sudo chmod u+x ./setup.sh
./setup.sh
export LD_LIBRARY_PATH=./lib
```
## Executar
```
make && make run
```

## Variáveis de ambiente
Todas já tem valor default mas podem ser alteradas
```
GLAD_GL_VERSION
GLAD_GL_PROFILE
GLFW_VERSION
PYTHON_PATH
```