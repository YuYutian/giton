from logger import setup_logging
log = setup_logging(__name__)
import config
import interface

import multiprocessing
import time

task_list = None
list_lock = None
OK = 200
TASK_NOT_FOUND = 401
TASK_NOT_STARTED = 402
POOL_NOT_STARTED = 403
CANNOT_STOP_TASK = 404

class Task:
    RUNNING = 1
    ERROR = 2
    COMPLETED = 3

    CURRENT_TASK_ID = 0

    def __init__(self, user_ID, detection_module, check_ID, reserved, result):
        import time
        self.task_ID = Task.CURRENT_TASK_ID
        Task.CURRENT_TASK_ID = Task.CURRENT_TASK_ID + 1
        self.user_ID = user_ID
        self.timestamp = time.time()
        self.status = self.RUNNING
        self.check_ID = check_ID
        self.reserved = reserved
        self.async_result = result
        self.detection_module = detection_module

class Mission_Pool:

    def __init__(self, MaxProcessNum):
        self.__pool = multiprocessing.Pool(processes = MaxProcessNum)
        self.__task_dict = {}

    def startTask(self, user_ID, detection_module, check_ID, reserved):
        func, args, kwds = interface.getAnalyzingFunction(user_ID, detection_module, check_ID, reserved)
        try:
            result = self.__pool.apply_async(func=func, args=args, kwds=kwds)# callback=self.change_task_status, error_callback=self.change_task_status)
            t = Task(user_ID, detection_module, check_ID, reserved, result)
            self.__task_dict[t.task_ID] = t
            import manager
            return manager.OK
        except:
            log.debug('error')
            self.terminate_pool()
            import manager
            return manager.TASK_NOT_STARTED

    def queryTask(self, _task_IDs = None, _user_ID = None):
        ret_list = []
        for item in self.__task_dict:
            if ((_task_IDs == None) or (self.__task_dict[item].task_ID in _task_IDs)) and (
                    (self.__task_dict[item].user_ID == _user_ID) or (_user_ID == None)):
                if self.__task_dict[item].async_result.ready():
                    if self.__task_dict[item].async_result.successful():
                        self.__task_dict[item].status = Task.COMPLETED
                    else:
                        self.__task_dict[item].status = Task.ERROR
                else:
                    self.__task_dict[item].status = Task.RUNNING
                ret_list.append(
                    {
                        "task_ID": self.__task_dict[item].task_ID,
                        "user_ID": self.__task_dict[item].user_ID,
                        "timestamp": self.__task_dict[item].timestamp,
                        "status": self.__task_dict[item].status,
                        "check_ID": self.__task_dict[item].check_ID,
                        "detection_module": self.__task_dict[item].detection_module,
                    }
                )
        return ret_list

    def deleteTask(self, _task_ID, _user_ID = None):
        import manager
        index = 0
        flag = manager.TASK_NOT_FOUND
        for item in self.__task_dict:
            if (self.__task_dict[item].status != Task.RUNNING) and (self.__task_dict[item].task_ID == _task_ID) and ((self.__task_dict[item].user_ID == _user_ID) or (_user_ID == None)):
                del self.__task_dict[item]
                flag = manager.OK
                break
            index = index + 1
        return flag

    def stop_pool(self):
        self.__pool.close()
        self.__pool.join()

    def join_pool(self):
        self.__pool.join()

    def terminate_pool(self):
        self.__pool.terminate()



