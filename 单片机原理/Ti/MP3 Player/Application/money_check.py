import requests

def main():
    res = requests.get('https://application.xiaofubao.com/app/electric/queryISIMSRoomSurplus?schoolCode=10337&areaId=1908211437059099&buildingCode=24&floorCode=2404&roomCode=240420&shiroJID=').json()
    if(res):
        print(res.get('data').get('surplusList')[0].get('amount'))

main()

