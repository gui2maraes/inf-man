# INF-MAN

Trabalho Final da cadeira de Algorítmos e Programação.
Feito de acordo com a especificação do professor Marcelo Walter.

Features:

- Usa a biblioteca raylib.
- Menu contendo as opções: Jogar, LeaderBoard e Sair.
- Jogador com tiros projéteis.
- Máximo de 10 inimigos.
- Padrão de ataque dos inimigos igual ao jogo original.
- Altura de pulo variável.
- Leitura de nível por arquivo.
- Persistência do leaderboard através de um arquivo binário.
- Obstáculos (espinhos).

## Autores:

- Guilherme Guimarães
- Felipe Salvo

## Instruções:

- Deixar a pasta `assets` na mesma pasta do executável.
- Colocar um arquivo chamado `level.world` na mesma pasta do executável.
  contendo a descrição textual do nível.
- Selecionar opções do menu com o mouse.
- Movimentação nas setas direcionais (←, →).
- Pule no `x` e atire no `z`.
- Você vence ao alcançar o final do nível.
- Se a sua pontuação for alta o suficiente, você poderá
  colocar seu nome no leaderboard.
- Cada inimigo morto vale 100 pontos, e cada vida restante
  vale 200 pontos.

## Compilação:

### Requisitos:

- CMake,
- Ninja ou Make no linux,
- Compilador compatível,

### Instruções:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./infman
```
