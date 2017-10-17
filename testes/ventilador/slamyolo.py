import glob
import datetime
tempo = datetime.datetime.now().timestamp() # pega o tempo atual e coloca ele no formato timestamp

print ("lendo imagens")

# fotos = glob.glob("rgb/*.png") # lendo fotos da pasta rgb
# print ("Existe " + str(len(fotos)) + " imagens na pasta rgb")
# fotos.sort() # ordenando as fotos para coloca-las em ordem crescente no arquivo rgb.txt
timestamp = 0.003037 # tempo entre cada frame

r = open("rgb.txt", "r") # abrindo o arquivo rgb.txt ou criando-o caso ele nao exista
saida = open("saida.txt", "r") # abrindo o arquivo rgb.txt ou criando-o caso ele nao exista
f = open("rgbm.txt", "w")
f.write("# color images\n")
f.write("# color images\n")
f.write("# timestamp filename\n")

yolo = saida.readlines()
index = 0

fotos = r.readlines()
for foto in fotos[2:]: # colocando o timestamp e o endereco das imagens no arquivo rgb.txt
    line = yolo[index]
    # print (line)
    # print (foto)
    if (int(line.split(' ')[0]) == int(foto.split('/')[1].split('.')[0])):
        f.write(foto.split('\n')[0] + " " + ' '.join(line.split(' ')[1:]))
        index += 1
    else:
        f.write(foto.split('\n')[0] + " nada 0 0 0 0" + "\n")


f.close()
saida.close()
r.close() # fechando o arquivo

print ("operação finalizada")