ESTRUTURA DE ASSETS PARA O JOGO UNO
====================================

Coloque as imagens das cartas nesta pasta seguindo a nomenclatura:

CARTAS NUMERICAS:
-----------------
red_0.png    - Carta vermelha numero 0
red_1.png    - Carta vermelha numero 1
...
red_9.png    - Carta vermelha numero 9

green_0.png  - Carta verde numero 0
green_1.png  - Carta verde numero 1
...
green_9.png  - Carta verde numero 9

blue_0.png   - Carta azul numero 0
blue_1.png   - Carta azul numero 1
...
blue_9.png   - Carta azul numero 9

yellow_0.png - Carta amarela numero 0
yellow_1.png - Carta amarela numero 1
...
yellow_9.png - Carta amarela numero 9

CARTAS ESPECIAIS:
-----------------
wild.png     - Carta coringa

VERSO:
------
card_back.png - Verso da carta (para o baralho de compra)

OBSERVACOES:
-----------
- As imagens devem estar em formato PNG
- Resolucao recomendada: proporcional a 80x120 pixels (ou maior)
- Se alguma imagem estiver faltando, o jogo usara desenho procedural (formas coloridas)
- O jogo carrega automaticamente as texturas disponiveis ao iniciar

EXEMPLO DE ESTRUTURA:
--------------------
Assets/
  red_0.png
  red_1.png
  ...
  green_0.png
  ...
  blue_0.png
  ...
  yellow_0.png
  ...
  wild.png
  card_back.png
  README.txt (este arquivo)
