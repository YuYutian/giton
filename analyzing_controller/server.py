import asyncio
import json
import config
import manager
from logger import setup_logging
log = setup_logging(__name__)

message_queue = None
mission_dict = None
dict_lock = None

class Connection(asyncio.Protocol):

    NOT_AUTH = 0
    AUTHED = 1
    WRONG_ADDRESS = -1
    func_dict = {""}

    def __init__(self):
        self.__remoteHost = None
        self.__transport = None
        self.__status = Connection.NOT_AUTH

    def connection_made(self, transport):
        self.__remoteHost = transport.get_extra_info('peername')
        self.__transport = transport
        log.debug("Connection made from {0}.".format(self.__remoteHost))
        #if not security.identifyAddress(self.__remoteHost[0]):
        #     self.__status = Connection.WRONG_ADDRESS
        # else:
        #     self.__status = Connection.AUTHED
        # self.__status = Connection.AUTHED ######################

    def connection_lost(self, exc):
        log.debug("Connection {0} lost.".format(self.__remoteHost))

    def data_received(self, raw_data):
        try:
            data = json.loads(raw_data.decode())
        except json.JSONDecodeError:
            log.debug("Non-JSON message.")
            return
        if data["message_type"] == "start":
            self.__do_start(data)
        elif data["message_type"] == "stop":
            self.__do_stop(data)
        elif data["message_type"] == "query":
            self.__do_query(data)
        elif data["message_type"] == "delete":
            self.__do_delete(data)

    def eof_received(self):
        log.debug("Connection {0} lost.".format(self.__remoteHost))

    def write(self, raw_data):
        self.__transport.write(raw_data.encode())

    def __del__(self):
        self.__transport.close()

    def __do_start(self, data):
        global message_queue
        global mission_dict
        global dict_lock

        user_ID = data["user_ID"]
        timestamp = data["timestamp"]
        check_ID = data["check_ID"]
        detection_module_name = data["detection_module_name"]
        #{"message_type": "start", "detection_module_name": "e", "check_ID":"1", "timestamp":"1", "user_ID":"23"}
        for item in detection_module_name:
            # TODO: item
            m = shared.Mission(user_ID, timestamp, check_ID, None, test_mission, (), ())
            m.start()
            import multiprocessing
            log.debug(multiprocessing.active_children())
            # dict_lock.acquire()
            # mission_dict[m.mission_ID] = m
            # dict_lock.release()

    def __do_stop(self, data):
        pass

    def __do_query(self, data):
        pass

    def __do_delete(self, data):
        # TODO: if mission in queue: mark in dict, else terminate process
        pass


def start_server(IP, port, queue, dict, lock):
    global message_queue
    global mission_dict
    global dict_lock
    message_queue = queue
    mission_dict = dict
    dict_lock = lock
    loop = asyncio.get_event_loop()
    server = loop.create_server(Connection, IP, port)
    serverloop = loop.run_until_complete(server)

    # Start listen for concurrent connections
    try:
        loop.run_forever()
    except KeyboardInterrupt:
        log.debug("Server exiting!")
    finally:
        serverloop.close()
        loop.close()

mission_pool = None

def init():
    log.debug('start')
    config.init("config.json")
    global  mission_pool
    mission_pool = manager.Mission_Pool(config.MaxProcessNum)

def start(user_ID, detection_module, check_ID, reserved):
    global mission_pool
    if not mission_pool == None:
        return mission_pool.startTask(user_ID, detection_module, check_ID, reserved)
    else:
        import manager
        return manager.POOL_NOT_STARTED

def stop(_task_ID, _user_ID = None):
    import manager
    return manager.CANNOT_STOP_TASK

def query(_task_IDs = None, _user_ID = None):
    global mission_pool
    if not mission_pool == None:
        return mission_pool.queryTask(_task_IDs, _user_ID)
    else:
        return []

def delete(_task_ID, _user_ID = None):
    global mission_pool
    if not mission_pool == None:
        return mission_pool.deleteTask(_task_ID, _user_ID)
    else:
        import manager
        return manager.POOL_NOT_STARTED

def free():
    global mission_pool
    mission_pool.stop_pool()


