import json

IP = None
Port = None
MaxProcessNum = None
AcceptedIPList = None
QueueMaxSize = None

def init(path):
    global IP
    global Port
    global MaxProcessNum
    global AcceptedIPList
    global QueueMaxSize
    with open(path, 'r') as f:
        result = json.load(f)
    IP = result["IP"]
    Port = result["Port"]
    MaxProcessNum = result["MaxProcessNum"]
    AcceptedIPList = result["AcceptedIPList"]
    QueueMaxSize = result["QueueMaxSize"]