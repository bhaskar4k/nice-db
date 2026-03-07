import sqlite3
from app.config import DB_PATH


def get_connection():
    conn = sqlite3.connect(DB_PATH, check_same_thread=False)
    return conn


def init_db():
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