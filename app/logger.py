import logging
import logging.handlers
import json
from pathlib import Path
from datetime import datetime

# Load configuration from JSON
BASE_DIR = Path(__file__).resolve().parent.parent
CONFIG_FILE = BASE_DIR / "config.json"

with open(CONFIG_FILE, 'r') as f:
    CONFIG = json.load(f)

# Create logs directory if it doesn't exist
LOGS_DIR = BASE_DIR / CONFIG["logging"]["log_dir"]
LOGS_DIR.mkdir(exist_ok=True)

# Logger configuration
LOG_FILE = LOGS_DIR / "app.txt"
EXCEPTION_LOG_FILE = LOGS_DIR / "exceptions.txt"

# Get logging config values
LOG_LEVEL = CONFIG["logging"]["level"]
MAX_BYTES = CONFIG["logging"]["max_bytes"]
BACKUP_COUNT = CONFIG["logging"]["backup_count"]

# Create formatters
detailed_formatter = logging.Formatter(
    '%(asctime)s - %(name)s - %(levelname)s - [%(filename)s:%(lineno)d] - %(message)s',
    datefmt='%Y-%m-%d %H:%M:%S'
)

exception_formatter = logging.Formatter(
    '%(asctime)s - EXCEPTION - %(name)s\n'
    'Level: %(levelname)s\n'
    'File: %(filename)s\n'
    'Function: %(funcName)s\n'
    'Line: %(lineno)d\n'
    'Message: %(message)s\n'
    'Exception: %(exc_info)s\n'
    '=' * 80,
    datefmt='%Y-%m-%d %H:%M:%S'
)

# Setup main logger
logger = logging.getLogger("NICE-DB")
logger.setLevel(LOG_LEVEL)

# File handler for general logs
file_handler = logging.handlers.RotatingFileHandler(
    LOG_FILE,
    maxBytes=MAX_BYTES,
    backupCount=BACKUP_COUNT
)
file_handler.setLevel(LOG_LEVEL)
file_handler.setFormatter(detailed_formatter)

# File handler for exceptions
exception_handler = logging.handlers.RotatingFileHandler(
    EXCEPTION_LOG_FILE,
    maxBytes=MAX_BYTES,
    backupCount=BACKUP_COUNT
)
exception_handler.setLevel(logging.ERROR)
exception_handler.setFormatter(exception_formatter)

# Add handlers to logger
logger.addHandler(file_handler)
logger.addHandler(exception_handler)