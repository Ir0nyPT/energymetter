# energymetter
contador de energia

# Pre-requesitos
1. Gerar chave no computador para pedir acesso ao github. Após correr o comando, este irá perguntar o ficheiro onde guardar e pass a atribuir, clicar no "Enter" 3 vezes para avançar sem adicionar qualquer valor
> ssh-keygen -t rsa -b 4096

2. Imprimir no terminal a chave gerada
> cat ~/.ssh/id_rsa.pub 

3. Ir ao link abaixo e colar a chave imprimida no último comando no espaço "Key" (chave completa e sem qualquer alteração). Como Título é indiferente o que escreves
https://github.com/settings/ssh/new

4. Clonar o repositório
> git clone git@github.com:Ir0nyPT/energymetter.git

5. Instalar e inicializar 
> cd energymetter

> ./script.sh setup

> ./script.sh init

# Compilar e descarregar
> ./script.sh buildandcopy

OU 

> ./script.sh build

> ./script.sh copy


# Comando disponíveis no script

## Inicializar
> ./script.sh init

## Compilar
> ./script.sh build

## Apagar cache - util para recompilar
> ./script.sh clean

## Enviar para o Pico
> ./script.sh copy

## Obter um terminal de "Debug" (com a ferramenta Minicom)
### Iniciar terminal
> ./script.sh debug
### Terminar terminal
> Press CTRL+A X
### Dar comando para o rasp ir para modo de programação
Com o terminal de debug aberto escrever "reboot" e pressionar enter



# Como guardar algerações?
Antes de iniciar alguma alteração do programa cria um "ramo" do programa
> git checkout -b devop_mj

Após alterações feitas, para reter as alterações
> git add .

Para gravar as alterações. Alterar a "mensagem" por uma breve descrição do que fizes-te
> git commit -m "mensagem"

Para enviar para o github de forma a partilhar
> git push -u origin devop_mj

# Como atualizar com as alterações online?
Desta forma iras obter alterações que outros tenham feito
> git pull 




# Pico W LED
https://github.com/raspberrypi/pico-examples/blob/master/pico_w/wifi/blink/picow_blink.c
