#!/bin/bash

################### Lendo video ###############################################
img=0
if [ $# -ge 1 ]; then
    video=$1
    if [ ! -e "$video" ] ; then # confirmando se o video existe
        echo "o arquivo não existe, escolha um arquivo válido"
        exit 1
    fi
    echo "O vídeo escolhido foi $video"
    img=1
fi
###############################################################################





################# Gerando imagens a partir do video ##########################
if [ $img -eq 1 ] ; then
    criar=""

    if [ -e rgb ] ; then # verificando se a pasta rgb ja foi criada
        echo "Imagens já geradas, deseja gerar as imagens outra vez [S/n]?"
        read criar 
    fi
    if [ ! "$criar" == "n" ] && [ ! "$criar" == "N" ] ; then
        rm -rf rgb
        echo "Criando pasta rgb para colocar as imagens geradas"
        mkdir rgb # criando pasta rgb caso ela nao ja esteja criada
        ffmpeg -i "$video" -vf fps=30 rgb/%06d.png # gerando as imagens a partir do video
        criar="S"
    fi
fi
############################################################################






################ Gerando arquivo rgb.txt a partir das imagens ################

if [ -e rgb.txt ] && [ ! "$criar" == "S" ] ; then # verificando se o arquivo rgb.txt ja foi criado
    echo "Arquivo rgb.txt ja existe, deseja atualiza-lo [S/n]?"
    read criar 
fi

if [ ! "$criar" == "n" ] && [ ! "$criar" == "N" ] ; then
    python3 generatetimestamp.py # criando o arquivo rgb.txt com o timestamp e o endereço das imagens
fi

############################################################################