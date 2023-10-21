Compilação e execução do montador e simulador em Linux.

## Montador

1. Navegue até a pasta do montador:\
`cd montador`
2. Execute o comando `make` para compilar
3. execute o montador passando o programa assembly:\
`./montador path/to/program.asm (ou program.mcr)`

Isso criará um arquivo `program.obj` (no caso de macros `program.pre` também) na mesma pasta em que o programa de entrada está localizado.
## Simulador

1. Navegue até a pasta do simulador:\
`cd simulador`
2. Execute o comando `make` para compilar
3. execute o simulador passando o arquivo objeto:\
`./simulador path/to/program.obj`
