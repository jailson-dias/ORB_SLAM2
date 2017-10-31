import glob
import datetime
tempo = datetime.datetime.now().timestamp() # pega o tempo atual e coloca ele no formato timestamp

print ("lendo imagens")

fotos = glob.glob("rgb/*.png") # lendo fotos da pasta rgb
print ("Existe " + str(len(fotos)) + " imagens na pasta rgb")
fotos.sort() # ordenando as fotos para coloca-las em ordem crescente no arquivo rgb.txt
timestamp = 0.003037 # tempo entre cada frame

f = open("rgb.txt", "w") # abrindo o arquivo rgb.txt ou criando-o caso ele nao exista

f.write("# color images\n")
f.write("# timestamp filename\n")

for i, foto in enumerate(fotos): # colocando o timestamp e o endereco das imagens no arquivo rgb.txt
    f.write("{0:.6f}".format(tempo + i*timestamp) + " " + foto + "\n")

f.close() # fechando o arquivo

print ("operação finalizada")