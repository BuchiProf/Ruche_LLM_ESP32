#import de la bibliothèque
from flask import Flask, render_template, request, jsonify, redirect, url_for
from thingspeak import Thingspeak
from config import Config
import datetime

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
    return render_template("ruche_tension.html", data = data)

@app.route('/masse')
def courbe_masse():
    data_brute = ts.get_24heures()
    data = []
    for dico in data_brute['feeds']:
        new_dico ={}
        new_dico['date']=dico['created_at']
        new_dico['masse']=dico['field2']
        data.append(new_dico)
    return render_template("ruche_masse.html", data = data)

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
        donnee.append(new_dico)

    return render_template("ruche_multi.html", data=donnee)


#lancement de l'application en mode debugage version Thonny
#app.run(threaded=False, use_reloader=False, debug=True)
#lancement de l'application en mode debugage version classique
app.run(debug=True)
