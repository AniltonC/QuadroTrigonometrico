from quadroT import Esp

lista = []

def getEsp(id, angle, sen, cos, tan):
    for esp in lista:
        if esp.id == id:
            print("update esp: " + esp.id)
            esp.updateAll(id, angle, sen, cos, tan)
            break
    else:
        print("new esp created")
        esp = Esp(id, angle, sen, cos, tan)
        lista.append(esp)
    return 200

def printAllHome():
    saida = ""
    for esp in lista:
        saida += esp.printHome()
    return saida