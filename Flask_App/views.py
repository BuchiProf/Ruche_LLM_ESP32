#import de la bibliothèque
from flask import Flask, render_template
import datetime

#création de l'application
app = Flask(__name__)

#extrait de données du 1 mai 2018 au 15 mai 2018
data = [{'masse': 56659.87, 'date': datetime.datetime(2018, 5, 2, 20, 0), 'temp_ext': 15.03, 'temp2': 25.82, 'temp3': 31.45, 'humi': 62.21}, {'masse': 56638.4, 'date': datetime.datetime(2018, 5, 2, 21, 1), 'temp_ext': 14.28, 'temp2': 25.54, 'temp3': 31.67, 'humi': 66.1}, {'masse': 56575.87, 'date': datetime.datetime(2018, 5, 2, 22, 1), 'temp_ext': 13.08, 'temp2': 24.91, 'temp3': 31.79, 'humi': 62.95}, {'masse': 56556.93, 'date': datetime.datetime(2018, 5, 2, 23, 1), 'temp_ext': 12.18, 'temp2': 24.44, 'temp3': 31.92, 'humi': 66.23}, {'masse': 56538.62, 'date': datetime.datetime(2018, 5, 3, 0, 0), 'temp_ext': 11.36, 'temp2': 24.19, 'temp3': 31.84, 'humi': 64.8}, {'masse': 56534.4, 'date': datetime.datetime(2018, 5, 3, 1, 0), 'temp_ext': 10.8, 'temp2': 23.71, 'temp3': 31.47, 'humi': 66.35}, {'masse': 56539.73, 'date': datetime.datetime(2018, 5, 3, 2, 1), 'temp_ext': 10.57, 'temp2': 23.53, 'temp3': 31.0, 'humi': 65.1}, {'masse': 56541.86, 'date': datetime.datetime(2018, 5, 3, 3, 1), 'temp_ext': 10.73, 'temp2': 23.43, 'temp3': 30.64, 'humi': 66.71}, {'masse': 56519.43, 'date': datetime.datetime(2018, 5, 3, 10, 23), 'temp_ext': 10.72, 'temp2': 23.26, 'temp3': 30.44, 'humi': 70.36}, {'masse': 56255.5, 'date': datetime.datetime(2018, 5, 3, 15, 9), 'temp_ext': 11.71, 'temp2': 22.82, 'temp3': 30.17, 'humi': 59.73}, {'masse': 56401.33, 'date': datetime.datetime(2018, 5, 3, 18, 10), 'temp_ext': 13.75, 'temp2': 23.11, 'temp3': 29.37, 'humi': 58.37}, {'masse': 56437.33, 'date': datetime.datetime(2018, 5, 3, 20, 48), 'temp_ext': 13.02, 'temp2': 23.23, 'temp3': 29.03, 'humi': 58.07}, {'masse': 56412.0, 'date': datetime.datetime(2018, 5, 4, 7, 27), 'temp_ext': 12.5, 'temp2': 23.25, 'temp3': 29.0, 'humi': 57.8}, {'masse': 55118.0, 'date': datetime.datetime(2018, 5, 5, 14, 37), 'temp_ext': 15.19, 'temp2': 25.19, 'temp3': 32.2, 'humi': 75.3}, {'masse': 56879.2, 'date': datetime.datetime(2018, 5, 5, 15, 0), 'temp_ext': 30.93, 'temp2': 33.86, 'temp3': 34.72, 'humi': 73.26}, {'masse': 57019.07, 'date': datetime.datetime(2018, 5, 5, 16, 1), 'temp_ext': 26.58, 'temp2': 34.68, 'temp3': 34.52, 'humi': 74.48}, {'masse': 57263.6, 'date': datetime.datetime(2018, 5, 5, 17, 1), 'temp_ext': 22.06, 'temp2': 34.28, 'temp3': 34.71, 'humi': 70.02}, {'masse': 57584.28, 'date': datetime.datetime(2018, 5, 5, 18, 0), 'temp_ext': 20.53, 'temp2': 32.82, 'temp3': 34.29, 'humi': 69.45}, {'masse': 57921.2, 'date': datetime.datetime(2018, 5, 5, 19, 0), 'temp_ext': 19.8, 'temp2': 31.96, 'temp3': 34.16, 'humi': 68.08}, {'masse': 58351.87, 'date': datetime.datetime(2018, 5, 5, 20, 1), 'temp_ext': 19.17, 'temp2': 30.95, 'temp3': 34.03, 'humi': 66.37}, {'masse': 58811.73, 'date': datetime.datetime(2018, 5, 5, 21, 1), 'temp_ext': 18.34, 'temp2': 30.01, 'temp3': 33.58, 'humi': 64.37}, {'masse': 58282.0, 'date': datetime.datetime(2018, 5, 6, 8, 0), 'temp_ext': 13.31, 'temp2': 26.47, 'temp3': 33.2, 'humi': 67.65}, {'masse': 58214.53, 'date': datetime.datetime(2018, 5, 6, 9, 0), 'temp_ext': 13.81, 'temp2': 26.31, 'temp3': 33.08, 'humi': 68.53}, {'masse': 58005.6, 'date': datetime.datetime(2018, 5, 6, 10, 1), 'temp_ext': 15.05, 'temp2': 26.59, 'temp3': 32.97, 'humi': 68.34}, {'masse': 58052.13, 'date': datetime.datetime(2018, 5, 6, 11, 1), 'temp_ext': 16.58, 'temp2': 27.76, 'temp3': 33.15, 'humi': 77.24}, {'masse': 58387.31, 'date': datetime.datetime(2018, 5, 6, 12, 0), 'temp_ext': 18.62, 'temp2': 29.61, 'temp3': 33.64, 'humi': 74.69}, {'masse': 58781.87, 'date': datetime.datetime(2018, 5, 6, 13, 0), 'temp_ext': 22.23, 'temp2': 30.86, 'temp3': 34.18, 'humi': 69.93}, {'masse': 59318.13, 'date': datetime.datetime(2018, 5, 6, 14, 0), 'temp_ext': 32.74, 'temp2': 32.28, 'temp3': 34.21, 'humi': 66.21}, {'masse': 59657.6, 'date': datetime.datetime(2018, 5, 6, 15, 1), 'temp_ext': 36.88, 'temp2': 34.08, 'temp3': 34.31, 'humi': 66.27}, {'masse': 59603.47, 'date': datetime.datetime(2018, 5, 6, 16, 1), 'temp_ext': 30.7, 'temp2': 35.51, 'temp3': 34.53, 'humi': 67.66}, {'masse': 59665.33, 'date': datetime.datetime(2018, 5, 6, 17, 1), 'temp_ext': 25.57, 'temp2': 35.25, 'temp3': 34.75, 'humi': 65.38}, {'masse': 60060.27, 'date': datetime.datetime(2018, 5, 6, 18, 1), 'temp_ext': 23.81, 'temp2': 33.49, 'temp3': 34.58, 'humi': 63.39}]





@app.route('/')
def index():
    date = [dico['date'].strftime("%Y-%m-%d %H:%M") for dico in data]
    print(date[0:10])
    masse = [dico['masse'] for dico in data]
    t_ext = [dico['temp_ext'] for dico in data]
    t2 = [dico['temp2'] for dico in data]
    t3 = [dico['temp3'] for dico in data]
    return render_template("index.html", d=date,m=masse, t=t_ext, t2=t2,t3=t3)




#lancement de l'application en mode debugage version Thonny
app.run(threaded=False, use_reloader=False, debug=True)
#lancement de l'application en mode debugage version classique
#app.run(debug=True)
