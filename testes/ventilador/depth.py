import numpy as np
import cv2
from matplotlib import pyplot as plt
import glob
import datetime
import os, shutil

tempo = datetime.datetime.now().timestamp() # pega o tempo atual e coloca ele no formato timestamp

print ("lendo imagens")

fotos = glob.glob("rgb/*.png") # lendo fotos da pasta rgb
print ("Existe " + str(len(fotos)) + " imagens na pasta rgb")
fotos.sort() # ordenando as fotos para coloca-las em ordem crescente no arquivo rgb.txt
timestamp = 0.033333 # tempo entre cada frame

directory = "depth"

if os.path.exists(directory):
    shutil.rmtree(directory)

os.makedirs(directory)

a = fotos[0]

for i, f in enumerate(fotos[1:]):
    imgR = cv2.imread(a,0)
    imgL = cv2.imread(f,0)


    stereo = cv2.StereoBM_create(numDisparities=16, blockSize=15)
    disparity = stereo.compute(imgL,imgR)
    # fig,ax = plt.subplots(1)
    # fig.subplots_adjust(left=0,right=1,bottom=0,top=1)
    # Display the image
    # ax.imshow(disparity,'gray')
    # Turn off axes and set axes limits
    # plt.axis('tight')
    # ax.axis('off')
    
    plt.axis("off")
    plt.imshow(disparity,'gray')
    # plt.show()

    plt.savefig(directory + "/" + "{0:06}".format(i + 1) + ".png",bbox_inches='tight', pad_inches=0, transparent = True)

f = open("depth.txt", "w") # abrindo o arquivo rgb.txt ou criando-o caso ele nao exista

f.write("# color images\n")
f.write("# timestamp filename\n")

depths = glob.glob("depth/*.png") # lendo fotos da pasta depth
depths.sort()

for i, foto in enumerate(depths): # colocando o timestamp e o endereco das imagens no arquivo rgb.txt
    f.write("{0:.6f}".format(tempo + i*timestamp) + " " + foto + "\n")

f.close() # fechando o arquivo

print ("operação finalizada")