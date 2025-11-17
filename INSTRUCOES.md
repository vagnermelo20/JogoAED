# Instruções para Compilar e Executar o Jogo UNO

## Pré-requisitos

### Linux
```bash
sudo apt-get update
sudo apt-get install build-essential git
sudo apt-get install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
```

### Windows (MinGW-W64)
- Baixe e instale o W64devkit de: https://github.com/skeeto/w64devkit/releases
- OU use o compilador que vem com o raylib installer

### MacOS
```bash
brew install gcc make
```

## Compilação

### Linux
```bash
cd build
./premake5 gmake
cd ..
make
```

### Windows (MinGW-W64)
```bash
# Execute o build-MinGW-W64.bat
build-MinGW-W64.bat

# OU manualmente:
cd build
premake5.exe gmake
cd ..
make
```

### Windows (Visual Studio 2022)
```bash
# Execute o build-VisualStudio2022.bat
build-VisualStudio2022.bat

# Depois abra o arquivo .sln gerado e compile no Visual Studio
```

### MacOS
```bash
cd build
./premake5.osx gmake
cd ..
make
```

## Executar o Jogo

Após a compilação, o executável estará na pasta `bin/Debug`:

### Linux/MacOS
```bash
./bin/Debug/JogoAED
```

### Windows
```bash
bin\Debug\JogoAED.exe
```

## Limpeza

Para limpar os arquivos compilados:

```bash
make clean
```

## Como Jogar

1. **Menu Inicial**: Clique em "Jogar" para começar uma partida
2. **Durante o Jogo**:
   - Clique em uma carta da sua mão para jogá-la (se for válida)
   - Clique no baralho ou pressione ESPAÇO para comprar uma carta
   - Quando jogar uma carta INCOLOR, escolha a cor:
     - 1 - Amarelo
     - 2 - Azul
     - 3 - Verde
     - 4 - Vermelho

3. **Objetivo**: Seja o primeiro jogador a ficar sem cartas na mão!

## Regras do UNO

- **Carta Válida**: Deve ter a mesma cor OU o mesmo valor da carta no topo da pilha
- **Cartas Especiais**:
  - **+2**: Próximo jogador compra 2 cartas
  - **+4**: Próximo jogador compra 4 cartas (apenas INCOLOR)
  - **Reverso**: Inverte a direção do jogo
  - **Bloquear**: Próximo jogador perde a vez
  - **INCOLOR**: Pode ser jogada a qualquer momento, você escolhe a cor
  - **Especiais Personalizadas**:
    - **2**: Puxa mais duas cartas
    - **3**: Joga novamente
    - **4**: Troca mão com jogador anterior
    - **8**: Randomiza cores de todas suas cartas

## Estrutura do Projeto

```
JogoAED/
├── assets/          # Imagens das cartas e recursos gráficos
├── bin/             # Executáveis compilados
├── build/           # Scripts de compilação (premake)
├── include/         # Arquivos header (.h)
├── src/             # Código fonte (.c)
└── README.md        # Documentação original
```

## Dependências

- **raylib**: Biblioteca gráfica (incluída no projeto)
- **C Compiler**: GCC, Clang, ou MSVC
- **Make**: Para automação de build
