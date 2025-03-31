#import de la bibliothèque
from flask import Flask, render_template, request, jsonify, redirect, url_for
from thingspeak import Thingspeak
from config import Config
from datetime import datetime
from math import nan

#création de l'application
app = Flask(__name__)
app.config.from_object(Config)

api_key = app.config['API']
ts = Thingspeak(api_key)

@app.route('/')
def index_ruche():
    return render_template("base.html")

@app.route('/tension')
def courbe_tension():
    
    data_brute = ts.get_24heures()
    data = []
    for dico in data_brute['feeds']:
        new_dico ={}
        new_dico['date']=dico['created_at']
        new_dico['tension']=dico['field1']
        data.append(new_dico)
    tension = data[-1]['tension']
    
   
    return render_template("ruche_tension.html", data = data, tension=tension)

@app.route('/masse')
def courbe_masse():
    data_brute = ts.get_24heures()
    data = []
    for dico in data_brute['feeds']:
        new_dico ={}
        new_dico['date']=dico['created_at']
        
        new_dico['masse']=dico['field2']
        if dico['field3'] is not None:
            data.append(new_dico)
    masse = data[-1]['masse']
    #data = data[:len(data)//2]
    return render_template("ruche_masse.html", data = data, masse_actuelle = masse)

@app.route('/multi')
def multi_courbe():
    data_brute = ts.get_24heures()
    
    donnee = []
    for dico in data_brute['feeds']:
        new_dico ={}
        new_dico['date']=dico['created_at']
        new_dico['temp_int']=dico['field6']
        new_dico['temp_ext']=dico['field3']
        new_dico['humi_int']=dico['field7']
        new_dico['humi_ext']=dico['field4']
        #ne pas enregistrer les données qui sont vides
        if dico['field6'] is not None and dico['field4'] is not None and dico['field3'] is not None and dico['field7'] is not None :
            donnee.append(new_dico)
    dernier_dico = donnee[-1]
    ti = dernier_dico['temp_int']
    te = dernier_dico['temp_ext']
    hi = dernier_dico['humi_int']
    he = dernier_dico['humi_ext']
    return render_template("ruche_multi.html", data=donnee, dernier_ti = ti, dernier_te = te, dernier_hi = hi, dernier_he = he)

@app.route('/masse_temp')
def multi_masse_temp():
    data_brute = ts.get_24heures()
    
    donnee = []
    for dico in data_brute['feeds']:
        new_dico ={}
        new_dico['date']=dico['created_at']
        new_dico['temp_ext']=dico['field3']
        new_dico['masse']=dico['field2']
        #ne pas enregistrer les données qui sont vides
        if dico['field3'] is not None and dico['field2'] is not None :
            donnee.append(new_dico)
    dernier_dico = donnee[-1]
    te = dernier_dico['temp_ext']
    m = dernier_dico['masse']
    return render_template("ruche_temp_masse.html", data=donnee, dernier_te = te, masse_actuelle = m)


#lancement de l'application en mode debugage version Thonny
#app.run(threaded=False, use_reloader=False, debug=True)
#lancement de l'application en mode debugage version classique
app.run(debug=True)
