from repository.db import get_connection
import uuid
from app.models.job_model import IngestionRequest
from app.logger import logger


def create(payload: IngestionRequest):
    try:
        logger.info("create -> start")

        # Generate unique job ID
        job_id = str(uuid.uuid4())
        
        query = """
            INSERT INTO ingestion_jobs
            (job_id, table_name, file_path, status)
            VALUES (?, ?, ?, 'pending')
        """

        conn = get_connection()
        conn.execute(query, (job_id, payload.table, payload.path))
        conn.commit()
        conn.close()
        
        logger.info(f"create -> end")
        return job_id
        
    except Exception as e:
        logger.exception(f"create -> error: {str(e)}")
        raise



def get_all():
    try:
        logger.info("get_all -> start")
        
        query = "SELECT * FROM ingestion_jobs"
        
        conn = get_connection()
        jobs = conn.execute(query).fetchall()
        conn.close()
        
        logger.info(f"get_all -> end")
        return jobs
    
    except Exception as e:
        logger.exception(f"get_all -> error: {str(e)}")
        raise