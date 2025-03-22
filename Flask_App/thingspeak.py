import requests

class Thingspeak:
    def __init__(self, channel:int):
        
        self.channel = str(channel)

    def get_json(self):
        url = "https://api.thingspeak.com/channels/"+self.channel+"/feeds.json"
        
        reponse = requests.get(url)
        data = reponse.json()
        return data
    
    def get_24heures(self):
        url = "https://api.thingspeak.com/channels/"+self.channel+"/feeds.json?days=1"
        
        reponse = requests.get(url)
        data = reponse.json()
        return data

    def get_json_debut_fin(self, date_debut : str, date_fin : str):
        """"renvoi le json entre deux dates
        
        Keyword arguments:
        @date_debut -- debut de la requete au format "AAAA-MM-JJ%20HH:NN:SS"
        @date_fin -- fin de la requete au format "AAAA-MM-JJ%20HH:NN:SS"
        Return: renvoie un json des données comprises entre deux dates
        """
        
        url = "https://api.thingspeak.com/channels/" + \
                self.channel + \
                "/feeds.json?start=" + \
                date_debut + \
                "&end=" + \
                date_fin
        
        reponse = requests.get(url)
        data = reponse.json()
        return data


