from metadata.db import get_connection
import uuid
from app.logger import logger


def create_job(table_name, file_path):
    try:
        logger.info("create_job -> start")

        # Generate unique job ID
        job_id = str(uuid.uuid4())
        
        query = """
            INSERT INTO ingestion_jobs
            (job_id, table_name, file_path, status)
            VALUES (?, ?, ?, 'pending')
        """

        conn = get_connection()
        conn.execute(query, (job_id, table_name, file_path))
        conn.commit()
        conn.close()
        
        logger.info(f"create_job -> end")
        return job_id
        
    except Exception as e:
        logger.exception(f"create_job -> error: {str(e)}")
        raise