import json
from pathlib import Path
from app.app import create_app
from app.logger import logger
import uvicorn

# Load configuration
BASE_DIR = Path(__file__).resolve().parent
CONFIG_FILE = BASE_DIR / "config.json"

try:
    with open(CONFIG_FILE, 'r') as f:
        CONFIG = json.load(f)
    logger.info("Configuration loaded successfully")
except FileNotFoundError:
    logger.exception(f"Configuration file not found at {CONFIG_FILE}")
    raise
except Exception as e:
    logger.exception(f"Failed to load configuration from {CONFIG_FILE}")
    raise

try:
    app = create_app()
    logger.info("Application created successfully")
except Exception as e:
    logger.exception("Failed to create application")
    raise

if __name__ == "__main__":
    try:
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