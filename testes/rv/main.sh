#!/bin/bash

################### Lendo video ###############################################
video=""
if [ $# -ge 1 ]; then
    video=$1
else
    echo "Digite o caminho para o video"
    read video # lendo o caminho do video
fi

echo $video
while [ ! -e "$video" ] ; do # confirmando se o video existe
    echo "o arquivo não existe, escolha um arquivo válido"
    read video
done
echo "O vídeo escolhido foi $video"

###############################################################################





################# Gerando imagens a partir do video ##########################
criar=""

if [ -e rgb ] ; then # verificando se a pasta rgb ja foi criada
    echo "Imagens já geradas, deseja gerar as imagens outra vez [S/n]?"
    read criar 
else
    echo "Criando pasta rgb para colocar as imagens geradas"
    mkdir rgb # criando pasta rgb caso ela nao ja esteja criada
fi
if [ ! "$criar" == "n" ] && [ ! "$criar" == "N" ] ; then
    ffmpeg -i "$video" -vf fps=30 rgb/%06d.png # gerando as imagens a partir do video
fi

############################################################################






################ Gerando arquivo rgb.txt a partir das imagens ################
criar=""

if [ -e rgb.txt ] ; then # verificando se o arquivo rgb.txt ja foi criado
    echo "Arquivo rgb.txt ja existe, deseja atualiza-lo [S/n]?"
    read criar 
fi

if [ ! "$criar" == "n" ] && [ ! "$criar" == "N" ] ; then
    python3 generatetimestamp.py # criando o arquivo rgb.txt com o timestamp e o endereço das imagens
fi

############################################################################