from logger import setup_logging
log = setup_logging(__name__)
from server import *


if __name__ == "__main__":
    import sys
    import time
    sys.path.append(".") ###### export PYTHONPATH = $PYTHONPATH:#
    init()
    log.debug(mission_pool.queryTask())
    time.sleep(1)
    start(1, 'module1', 2, None)
    start(3, 'module2', 4, None)
    time.sleep(1)
    log.debug(mission_pool.queryTask())
    time.sleep(1)
    log.debug(mission_pool.queryTask())
    time.sleep(1)
    log.debug(mission_pool.queryTask())
    time.sleep(1)
    log.debug(mission_pool.queryTask())
    time.sleep(1)
    log.debug(mission_pool.queryTask())
    mission_pool.deleteTask(0)
    time.sleep(1)
    log.debug(mission_pool.queryTask())
    time.sleep(60)