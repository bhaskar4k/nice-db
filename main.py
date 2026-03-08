from app.app import create_app
from app.logger.logger import logger
from app.utils.config import load_config
from worker_service_job.worker import start_scheduler
import uvicorn
import threading

# Load configuration
try:
    CONFIG = load_config()
    logger.info("Configuration loaded successfully")
except Exception as e:
    logger.exception("Failed to load configuration")
    raise

try:
    app = create_app()
    logger.info("Application created successfully")
except Exception as e:
    logger.exception("Failed to create application")
    raise

if __name__ == "__main__":
    try:
        # Start worker scheduler in background thread
        print("Starting worker scheduler in background")
        logger.info("Starting worker scheduler in background")

        scheduler = start_scheduler()

        print("Worker scheduler started in background")
        logger.info("Worker scheduler started in background")
        

        # Start API server
        HOST = CONFIG["server"]["host"]
        PORT = CONFIG["server"]["port"]
        DEBUG = CONFIG["app"]["debug"]
        
        logger.info(f"Starting server on {HOST}:{PORT} (Debug: {DEBUG})")
        
        uvicorn.run(
            app, 
            host=HOST, 
            port=PORT,
            log_level="info" if not DEBUG else "debug"
        )
    except Exception as e:
        logger.exception("Failed to start server")
        raise