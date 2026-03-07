import sqlite3
import json
from pathlib import Path
from app.logger import logger


# Load config from JSON
BASE_DIR = Path(__file__).resolve().parent.parent
CONFIG_FILE = BASE_DIR / "config.json"

with open(CONFIG_FILE, 'r') as f:
    CONFIG = json.load(f)

DB_PATH = BASE_DIR / CONFIG["database"]["path"]


def get_connection():
    try:
        logger.debug(f"Connecting to database")

        conn = sqlite3.connect(str(DB_PATH), check_same_thread=False)

        logger.debug(f"Database connection established")
        return conn
    
    except Exception as e:
        logger.exception(f"Failed to connect to database")
        raise


def init_db():
    try:
        logger.info("Initializing database")

        conn = get_connection()

        conn.execute(
        """
            CREATE TABLE IF NOT EXISTS ingestion_jobs (
                job_id TEXT PRIMARY KEY,
                table_name TEXT,
                file_path TEXT,
                status TEXT,
                rows_processed INTEGER DEFAULT 0,
                created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            )
        """
        )

        conn.commit()
        conn.close()
        logger.info("Database initialized successfully")
        
    except Exception as e:
        logger.exception("Failed to initialize database")
        raise